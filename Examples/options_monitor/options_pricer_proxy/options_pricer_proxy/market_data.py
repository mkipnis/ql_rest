from django.shortcuts import render

from django.http import HttpResponse
import json
from .market_data_cache import MarketDataCacheConfig
from django.apps import apps
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
def get_latest_prices(request):
    print('RequestBody' + str(request.body))

    body_unicode = request.body.decode('utf-8')
    body = json.loads(body_unicode)
    tickers = body['tickers']

    market_data = apps.get_app_config(MarketDataCacheConfig.name)

    response = {}
    for ticker in tickers:
        response[ticker] = market_data.market_data_cache.get_price(ticker)

    response_json = json.dumps(response, indent=4)

    response = HttpResponse(response_json, content_type="application/json")

    return response

@csrf_exempt
def get_latest_vols(request):
    print('RequestBody' + str(request.body))

    body_unicode = request.body.decode('utf-8')
    body = json.loads(body_unicode)
    tickers = body['tickers']

    market_data = apps.get_app_config(MarketDataCacheConfig.name)

    response = {}
    for ticker in tickers:
        response[ticker] = market_data.market_data_cache.get_vols(ticker)

    response_json = json.dumps(response, indent=4)

    response = HttpResponse(response_json, content_type="application/json")

    return response