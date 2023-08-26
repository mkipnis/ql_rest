import time
import yfinance as yf
import json
import pandas as pd

# Get yahoo price
def get_yp_market_price(ticker):

    current_timestamp = int(time.time())
    stock = yf.Ticker(ticker)
    #print(stock.info)

    yp_price = {}
    if 'regularMarketPrice' in stock.info:
        yp_price['price'] = stock.info['regularMarketPrice']
    else:
        yp_price['price'] = stock.info['regularMarketPreviousClose']

    if yp_price['price'] == None:
        data = stock.history()
        yp_price['price'] = data['Close'].iloc[-1]

    yp_price['price'] = round(yp_price['price'], 2)
    yp_price['timestamp'] = current_timestamp

    return yp_price

def get_strike_to_vol_dict(opt):

    sr2 = pd.Series(opt['impliedVolatility']).apply(lambda vol: round(vol * 100))
    sr3 = pd.Series(opt['strike'])
    vols = list(sr2.to_dict().values())
    strike = list(sr3.to_dict().values())

    return pd.Series(vols, index=strike).to_dict()


def get_yp_vols(ticker):

    stock = yf.Ticker(ticker)
    exps = stock.options

    provided_exps = len(exps)
    if provided_exps > 9:
        provided_exps = 9

    options_data = {}

    for expiration_date_index in range(provided_exps):
        opt = stock.option_chain(exps[expiration_date_index])
        options_data[exps[expiration_date_index]] = {}
        options_data[exps[expiration_date_index]]['calls'] = get_strike_to_vol_dict(pd.DataFrame(opt.calls))
        options_data[exps[expiration_date_index]]['puts'] = get_strike_to_vol_dict(pd.DataFrame(opt.puts))

    return options_data


def store_prices(base_dir, stock_price_market_data):
    with open(base_dir+'/demo_data/prices.json', 'w') as px_file:
        json.dump(stock_price_market_data, px_file)
        px_file.close()

def store_vols(base_dir, vol_data):
    with open(base_dir+'/demo_data/vols.json', 'w') as vol_file:
        json.dump(vol_data, vol_file)
        vol_file.close()

if __name__ == '__main__':

    tickers = 'MMM,AXP,AMGN,AAPL,BA,CAT,CVX,CSCO,KO,DIS,DOW,GS,HD,HON,IBM,INTC,JNJ,JPM,MCD,MRK,MSFT,NKE,PG,CRM,TRV,UNH,VZ,V,WBA,WMT'

    price_data = {}
    vol_data = {}
    for ticker in tickers.split(','):
        print("Retrieving Prices and Vols:" + ticker)
        price_data[ticker] = get_yp_market_price(ticker)
        vol_data[ticker] = get_yp_vols(ticker)

    store_prices('../', price_data)
    store_vols('../', vol_data)