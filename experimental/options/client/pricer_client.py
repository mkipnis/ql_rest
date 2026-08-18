# Copyright (c) Mike Kipnis

import argparse
import json
import sqlite3
import queue
import asyncio
import threading
from datetime import datetime
from pprint import pprint
from sqlite3 import Cursor

import httpx

from data_loader import populate_vols

timeout = httpx.Timeout(
    connect=5.0,
    read=10.0,
    write=10.0,
    pool=5.0,
)

limits = httpx.Limits(max_connections=100, max_keepalive_connections=20)


# =========================================================
# ASYNC POLLING LOGIC
# =========================================================
async def poll_single_token(server_url, token, client):
    while True:
        await asyncio.sleep(1)

        resp = await client.get(f"{server_url}/result/{token}")
        resp.raise_for_status()

        data = resp.json()
        status = data.get("status")

        print(f"[poll] {token} -> {status}")

        if status == "completed":
            print(f"[DONE] {token}")
            return token, data

        if status == "failed":
            print(f"[FAILED] {token}")
            return token, data


def create_database(output_file):
    #  Create a database to store the results
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    if output_file is None:
        db_name = f"{timestamp}.db"
    else:
        db_name = output_file
    conn = sqlite3.connect(db_name)
    cursor = conn.cursor()
    cursor.execute("CREATE TABLE IF NOT EXISTS pricing_stats "
                   "( token text PRIMARY KEY, status_queued json, status_processed json, "
                   "last_update_time timestamp default CURRENT_TIMESTAMP);")

    return conn, cursor


async def polling_engine(server_url, request_queue: queue.Queue, output_file):
    tasks = {}
    exit_flag = False

    conn, db_cursor = create_database(output_file)

    async with httpx.AsyncClient(
            timeout=timeout,
            limits=limits,
    ) as client:

        while True:

            # ---------------------------------
            # 1. ingest queue (non-blocking burst)
            # ---------------------------------
            while True:
                try:
                    req = request_queue.get_nowait()
                except queue.Empty:
                    break

                token = req["token"]

                if token == "EXIT_FLAG":
                    exit_flag = True
                    continue


                db_cursor.execute(
                    "INSERT INTO pricing_stats (token, status_queued) VALUES (?, ?)",
                    (token, json.dumps(req)))


                tasks[token] = asyncio.create_task(
                    poll_single_token(server_url, token, client)
                )

            # ---------------------------------
            # 2. process finished tasks
            # ---------------------------------
            finished = []

            for token, task in list(tasks.items()):
                if not task.done():
                    continue

                try:
                    t, data = await task
                    #print(data)
                    pprint(data, sort_dicts=False, width=1024)
                    data.pop("results", None)


                    db_cursor.execute(
                        """
                        UPDATE pricing_stats
                        SET status_processed = ?,
                            last_update_time = CURRENT_TIMESTAMP
                        WHERE token = ?
                        """,
                        (json.dumps(data), token)
                    )

                    finished.append(token)

                except Exception as e:
                    print(f"[ERROR] {token}: {e}")
                    finished.append(token)

            for t in finished:
                tasks.pop(t, None)

            # ---------------------------------
            # 3. exit condition
            # ---------------------------------
            if exit_flag and not tasks:
                break

            await asyncio.sleep(1)

    # final commit (safe now)
    conn.commit()
    conn.close()


def start_async_engine(server_url, request_queue: queue.Queue, output_file):
    def runner():
        asyncio.run(polling_engine(server_url, request_queue, output_file))

    thread = threading.Thread(target=runner)
    thread.start()
    return thread


# =========================================================
# WORKER THREAD (REQUEST SUBMISSION)
# =========================================================

class WorkerThread(threading.Thread):

    def __init__(self, server_url, request_queue, symbol, options_requests):
        super().__init__()
        self.server_url = server_url
        self.request_queue = request_queue
        self.symbol = symbol
        self.options_requests = options_requests

    async def submit_request(self, client, request):
        resp = await client.post(
            f"{self.server_url}/price",
            json=request
        )
        resp.raise_for_status()

        response_details = resp.json()
        response_details['request_key'] = request['underlying_symbol'] + "_" + request['exp_date']
        response_details['options_count'] = len(request['vols']['calls']) + len(request['vols']['puts'])

        return response_details

    async def process_requests(self):

        async with httpx.AsyncClient(
                timeout=timeout,
                limits=limits,
        ) as client:

            tasks = [
                self.submit_request(client, req)
                for req in self.options_requests
            ]

            queued_requests = await asyncio.gather(*tasks)

            print(f"[worker {self.symbol}] got {len(queued_requests)} tokens")

            for t in queued_requests:
                self.request_queue.put(t)

    def run(self):
        print(f"[worker START] {self.symbol}")
        asyncio.run(self.process_requests())
        print(f"[worker DONE] {self.symbol}")


# =========================================================
# MAIN
# =========================================================

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Options Pricing Client")

    parser.add_argument(
        "--server_url",
        default="http://127.0.0.1:8000",
        help="Destination Server Url"
    )

    parser.add_argument(
        "--output_file",
        help="Output File"
    )

    args = parser.parse_args()


    with open("data/rates.json") as f:
        rates = json.load(f)

    with open("data/underlying_symbols.json") as f:
        symbols = json.load(f)

    with open("data/vols.json") as f:
        vols = json.load(f)

    requests = populate_vols(rates, symbols, vols)

    request_queue = queue.Queue(maxsize=10000)

    # start async polling engine
    polling_thread = start_async_engine(args.server_url, request_queue, args.output_file)

    # start workers
    workers = []

    for symbol, symbol_requests in requests.items():
        w = WorkerThread(args.server_url, request_queue, symbol, symbol_requests)
        w.start()
        workers.append(w)

    # wait for workers only
    for w in workers:
        w.join()

    print("All workers finished submitting requests.")

    request_queue.put({"token":"EXIT_FLAG"})

    polling_thread.join()
