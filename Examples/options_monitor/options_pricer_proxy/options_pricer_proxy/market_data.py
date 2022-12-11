from django.shortcuts import render

from django.http import HttpResponse
import json
from .market_data_cache import MarketDataCacheConfig
from django.apps import apps
from django.views.decorators.csrf import csrf_exempt
import yfinance as yf
import pandas as pd
import time

@csrf_exempt
def get_latest_prices(request):
    print('RequestBody' + str(request.body))

    body_unicode = request.body.decode('utf-8')
    body = json.loads(body_unicode)
    tickers = body['tickers']

    market_data = apps.get_app_config(MarketDataCacheConfig.name)

    current_timestamp = int(time.time())

    response = {}
    for ticker in tickers:

        #if ticker not in market_data.market_data_cache.market_data or (current_timestamp - market_data.market_data_cache.market_data[ticker]['timestamp']) > 600:
        #    stock = yf.Ticker(ticker)
        #    price = {}
        #    price['price'] = stock.info['regularMarketPrice']
        #    price['timestamp'] = current_timestamp
        #    market_data.market_data_cache.market_data[ticker] = price
        #    print(price)
        #else:
        #    price = market_data.market_data_cache.market_data[ticker]

        #market_data.market_data_cache.get_price(ticker)
        response[ticker] = market_data.market_data_cache.get_price(ticker)

    response_json = json.dumps(response, indent=4)

    response = HttpResponse(response_json, content_type="application/json")

    return response

def get_strike_to_vol_dict(opt):
    sr2 = pd.Series(opt['impliedVolatility'])
    sr3 = pd.Series(opt['strike'])
    vols = list(sr2.to_dict().values())
    strike = list(sr3.to_dict().values())
    return pd.Series(vols, index=strike).to_dict()


@csrf_exempt
def get_latest_vols(request):
    print('RequestBody' + str(request.body))

    body_unicode = request.body.decode('utf-8')
    body = json.loads(body_unicode)
    tickers = body['tickers']

    market_data = apps.get_app_config(MarketDataCacheConfig.name)

    response = {}
    for ticker in tickers:

        options_data = market_data.market_data_cache.vol_cache.get(ticker)

        if options_data == None:
            stock = yf.Ticker(ticker)
            exps = stock.options

            provided_exps = len(exps)
            if provided_exps > 3: # Near, Far, FarFar
                provided_exps = 3

            options_data = {}

            for expiration_date_index in range(provided_exps):
                opt = stock.option_chain(exps[expiration_date_index])
                options_data[exps[expiration_date_index]] = {}
                options_data[exps[expiration_date_index]]['calls'] = get_strike_to_vol_dict(pd.DataFrame(opt.calls))
                options_data[exps[expiration_date_index]]['puts'] = get_strike_to_vol_dict(pd.DataFrame(opt.puts))

            market_data.market_data_cache.vol_cache[ticker] = options_data

        response[ticker] = options_data;

    response_json = json.dumps(response, indent=4)

    response = HttpResponse(response_json, content_type="application/json")

    return response