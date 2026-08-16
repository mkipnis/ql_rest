# Copyright (c) Mike Kipnis

import sys
import sqlite3


QUERY_BY_PRICER = """
SELECT
    status_queued->>'pricer' AS pricer,

    SUM(
        CAST(status_queued->>'options_count' AS INTEGER)
    ) AS options_count,

    MIN(status_queued->>'timestamp') AS queued_timestamp,
    MAX(status_processed->>'timestamp') AS processed_timestamp,

    MIN(
        CAST(status_processed->>'pricing_latency' AS REAL) / 1000
    ) AS min_pricing_latency_ms,

    MAX(
        CAST(status_processed->>'pricing_latency' AS REAL) / 1000
    ) AS max_pricing_latency_ms,

    AVG(
        CAST(status_processed->>'pricing_latency' AS REAL) / 1000
    ) AS avg_pricing_latency_ms,

    COUNT(*) AS count,

    MAX(
        (
            julianday(status_processed->>'timestamp')
            - julianday(status_queued->>'timestamp')
        ) * 86400
    ) AS total_time

FROM pricing_stats ps
GROUP BY status_queued->>'pricer';
"""


QUERY_BY_PRICER_SYMBOL = """
SELECT
    status_queued->>'pricer' AS pricer,

    status_queued->>'underlying_symbol' AS underlying_symbol,

    SUM(
        CAST(status_queued->>'options_count' AS INTEGER)
    ) AS options_count,

    MIN(
        CAST(status_processed->>'pricing_latency' AS REAL) / 1000
    ) AS min_pricing_latency_ms,

    MAX(
        CAST(status_processed->>'pricing_latency' AS REAL) / 1000
    ) AS max_pricing_latency_ms,

    AVG(
        CAST(status_processed->>'pricing_latency' AS REAL) / 1000
    ) AS avg_pricing_latency_ms,

    COUNT(*) AS count

FROM pricing_stats ps

GROUP BY
    status_queued->>'pricer',
    status_queued->>'underlying_symbol'

ORDER BY
    status_queued->>'underlying_symbol';
"""


def format_value(value, column):
    """Format values for display."""

    if value is None:
        return ""

    # Integer values
    if column in ("options_count", "count"):
        return f"{int(value):,}"

    # Milliseconds
    if column in (
        "min_pricing_latency_ms",
        "max_pricing_latency_ms",
        "avg_pricing_latency_ms",
    ):
        return f"{float(value):,.2f}"

    # Total time in seconds
    if column == "total_time":
        return f"{float(value):,.2f}"

    return str(value)


def print_results(cursor):
    columns = [description[0] for description in cursor.description]
    rows = cursor.fetchall()

    if not rows:
        print("No results.")
        return

    data = [
        [
            format_value(value, columns[i])
            for i, value in enumerate(row)
        ]
        for row in rows
    ]

    # Calculate column widths
    widths = [
        max(
            len(columns[i]),
            max(len(row[i]) for row in data)
        )
        for i in range(len(columns))
    ]

    # Header
    header = " | ".join(
        columns[i].ljust(widths[i])
        for i in range(len(columns))
    )

    separator = "-+-".join(
        "-" * widths[i]
        for i in range(len(columns))
    )

    print(header)
    print(separator)

    # Rows
    for row in data:
        print(
            " | ".join(
                row[i].ljust(widths[i])
                for i in range(len(columns))
            )
        )

    print(f"\nRows: {len(rows)}")


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <sqlite_file>")
        sys.exit(1)

    db_file = sys.argv[1]

    try:
        connection = sqlite3.connect(db_file)
    except sqlite3.Error as e:
        print(f"Error opening database: {e}")
        sys.exit(1)

    try:
        cursor = connection.cursor()

        # ---------------------------------------------------------
        # Query 1: Statistics by pricer
        # ---------------------------------------------------------

        print("\n" + "=" * 120)
        print("PRICING STATISTICS BY PRICER")
        print("=" * 120)

        cursor.execute(QUERY_BY_PRICER)
        print_results(cursor)

        # ---------------------------------------------------------
        # Query 2: Statistics by pricer and underlying symbol
        # ---------------------------------------------------------

        print("\n" + "=" * 120)
        print("PRICING STATISTICS BY PRICER / UNDERLYING SYMBOL")
        print("=" * 120)

        cursor.execute(QUERY_BY_PRICER_SYMBOL)
        print_results(cursor)

    except sqlite3.Error as e:
        print(f"SQLite error: {e}")
        sys.exit(1)

    finally:
        connection.close()


if __name__ == "__main__":
    main()