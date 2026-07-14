# Copyright (c) Mike Kipnis

import os
from datetime import datetime

from fastapi import FastAPI
from concurrent.futures import ThreadPoolExecutor
import multiprocessing
import asyncio
import uuid
import json
from redis.asyncio import Redis

from pricer import american_option_with_greeks, process_price_request
from contextlib import asynccontextmanager


@asynccontextmanager
async def lifespan(app):
    dispatcher_task = asyncio.create_task(dispatcher())
    print("Dispatcher started")

    yield  # Runs the app

    dispatcher_task.cancel()
    try:
        await dispatcher_task
    except asyncio.CancelledError:
        pass

    print("Dispatcher stopped")

app = FastAPI(lifespan=lifespan)

redis_host = os.environ.get("REDIS_HOST", "localhost")

redis = Redis( host=redis_host, port=6379)

MAX_WORKERS = max(1, multiprocessing.cpu_count() - 1)
executor = ThreadPoolExecutor(max_workers=MAX_WORKERS)

request_queue = asyncio.Queue()

async def dispatcher():

    loop = asyncio.get_running_loop()

    while True:
        token, req = await request_queue.get()

        pricing_status = {
            "token": token,
            "status": "running",
            "timestamp": datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
        }

        try:
            await redis.set(token, json.dumps(pricing_status), ex=3600)
            result = await loop.run_in_executor( executor, process_price_request, req )

            pricing_status = {
                "token": token,
                "status": "completed",
                "results": result,
                "timestamp": datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
            }

            await redis.set( token, json.dumps(pricing_status), ex=3600)

        except Exception as e:
            pricing_status = {
                "token": token,
                "status": "failed",
                "error": str(e),
                "timestamp": datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
            }

            await redis.set( token, json.dumps(pricing_status), ex=3600 )

        finally:
            request_queue.task_done()


# ----------------------------
# Submit request (socket → queue)
# ----------------------------
@app.post("/price")
async def price_option(req: dict):

    token = str(uuid.uuid4())

    pricing_state = {
            "token": token,
            "status": "queued",
            "timestamp" : datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
        }

    await redis.set(token, json.dumps(pricing_state), ex=3600)

    # put it on the producer/consumer queue
    await request_queue.put((token, req))

    return pricing_state


# ----------------------------
# Read result from Redis
# ----------------------------
@app.get("/result/{token}")
async def get_result(token: str):

    value = await redis.get(token)

    if not value:
        return {"status": "not_found"}

    return json.loads(value)