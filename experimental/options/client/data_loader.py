# Copyright (c) Mike Kipnis
 
import json
from datetime import timedelta, date

expiration_cycle = [0, 2, 4]

def next_future_weekday(start_date, target_weekday):
    days_ahead = (target_weekday - start_date.weekday()) % 7

    if days_ahead == 0:
        days_ahead = 7

    return start_date + timedelta(days=days_ahead)


def assigned_dates_to_indexed_vols(base_request, vols):

    current_date = date.today()
    requests_with_vols = []

    for vol_index, vols in vols.items():

        exp_date_index = int(vol_index)%3
        weekday = expiration_cycle[exp_date_index]
        current_date = next_future_weekday(current_date,weekday)

        base_request_out = base_request.copy()
        base_request_out["exp_date"] = current_date.strftime("%Y-%m-%d")
        base_request_out["vols"] = vols
        requests_with_vols.append(base_request_out)

    return requests_with_vols


def populate_vols(rates, underlying_symbols, indexed_vols ):

    requests = {}
    for underlying_symbol, symbol_data in underlying_symbols.items():

        if underlying_symbol in indexed_vols:

           base_request = {
               "underlying_symbol": symbol_data["symbol"],
               "price": float(symbol_data["price"]),
               "dividend_yield": round( float( symbol_data[ "dividend" ] ) / float( symbol_data[ "price" ] ), 6 ),
               "risk_free_rate": round( rates["1Y"], 6 ),
           }

           exp_date_vols = assigned_dates_to_indexed_vols(base_request, indexed_vols[underlying_symbol])

           requests[underlying_symbol] = exp_date_vols

    return requests
