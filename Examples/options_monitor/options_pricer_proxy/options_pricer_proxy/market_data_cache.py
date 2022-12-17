import threading
import time
import copy
import yfinance as yf
import pandas as pd
from datetime import datetime
from pytz import timezone

from django.apps import AppConfig

class MarketDataCache(threading.Thread):

    def __init__(self):
        super().__init__()

        self.thread_lock = threading.Lock()
        self.market_data = {}
        self.vol_cache = {}

    # Get yahoo price
    def get_yp_market_price(self, ticker):

        current_timestamp = int(time.time())

        stock = yf.Ticker(ticker)
        yp_price = {}
        yp_price['price'] = stock.info['regularMarketPrice']
        if yp_price['price'] == None:
            data = stock.history()
            yp_price['price'] = data['Close'].iloc[-1]

        yp_price['timestamp'] = current_timestamp

        return yp_price

    # transform vols to smile
    def get_strike_to_vol_dict(self, opt):

        sr2 = pd.Series(opt['impliedVolatility']).apply(lambda vol: round(vol * 100))
        sr3 = pd.Series(opt['strike'])
        vols = list(sr2.to_dict().values())
        strike = list(sr3.to_dict().values())

        return pd.Series(vols, index=strike).to_dict()

    def get_yp_vols(self, ticker):

        stock = yf.Ticker(ticker)
        exps = stock.options

        provided_exps = len(exps)
        if provided_exps > 3:  # Near, Far, FarFar
            provided_exps = 3

        options_data = {}

        for expiration_date_index in range(provided_exps):
            opt = stock.option_chain(exps[expiration_date_index])
            options_data[exps[expiration_date_index]] = {}
            options_data[exps[expiration_date_index]]['calls'] = self.get_strike_to_vol_dict(pd.DataFrame(opt.calls))
            options_data[exps[expiration_date_index]]['puts'] = self.get_strike_to_vol_dict(pd.DataFrame(opt.puts))

        return options_data

    def get_price(self, ticker, force_reload = False):
        self.thread_lock.acquire()

        try:
            if ticker not in self.market_data or force_reload == True:
                market_price = self.get_yp_market_price(ticker)
                self.market_data[ticker] = market_price

            yp_market_price = copy.deepcopy(self.market_data[ticker])
        finally:
            self.thread_lock.release()

        return yp_market_price

    def get_vols(self, ticker, force_reload = False):
        self.thread_lock.acquire()
        try:
            if ticker not in self.vol_cache or force_reload == True:
                vols = self.get_yp_vols(ticker)
                self.vol_cache[ticker] = vols

            yp_vols = copy.deepcopy(self.vol_cache[ticker])
        finally:
            self.thread_lock.release()

        return yp_vols

    def run(self):

        tz = timezone('EST')

        while (1):

            current_est_time = datetime.now(tz)

            if current_est_time.isoweekday() > 5:
                time.sleep(60*60)
                continue

            is_market_open_time = (((current_est_time.hour * 60 + current_est_time.minute) > 9 * 60 + 30) and
                                   ((current_est_time.hour * 60 + current_est_time.minute) < 16 * 60 + 30))

            if is_market_open_time == False:
                time.sleep(60)
                continue

            current_timestamp = int(time.time())

            self.thread_lock.acquire()
            try:
                current_market_data_snapshot = copy.deepcopy(self.market_data)
            finally:
                self.thread_lock.release()

            for ticker in current_market_data_snapshot:
                if (current_timestamp - current_market_data_snapshot[ticker]['timestamp']) > 86400:
                    print('Lets reload : ' + ticker)
                    self.get_price(ticker, True)
                    self.get_vols(ticker, True)

            time.sleep(60)


class MarketDataCacheConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'options_pricer_proxy'
    market_data_cache = MarketDataCache()

    def ready(self):
        print('OptionsPricerProxy')
        self.market_data_cache.start()
        print('OptionsPricerProxy - is ready')
