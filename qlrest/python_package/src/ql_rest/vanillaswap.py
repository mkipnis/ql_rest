# Autogenerated from the following stub : git/ql_rest/gensrc/stubs/stub.qlrest.python_module

# vanillaswap_py
# vanillaswap_py


import json
from json import JSONEncoder

class qlMakeIMMSwap(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.SwapTenor = "";
        self.IborIndex = "";
        self.FixedRate = 0.0;
        self.FirstImmDate = "";
        self.FixDayCounter = "";
        self.Spread = 0.0;
        self.PricingEngineID = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlMakeVanillaSwap(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.SettlDays = 0;
        self.SwapTenor = "";
        self.IborIndex = "";
        self.FixedRate = 0.0;
        self.ForwardStart = "";
        self.FixDayCounter = "";
        self.Spread = 0.0;
        self.PricingEngineID = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlVanillaSwap(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.PayerReceiver = "";
        self.Nominal = 0.0;
        self.FixSchedule = "";
        self.FixedRate = 0.0;
        self.FixDayCounter = "";
        self.FloatingLegSchedule = "";
        self.IborIndex = "";
        self.Spread = 0.0;
        self.FloatingLegDayCounter = "";
        self.PaymentConvention = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlVanillaSwapFromSwapIndex(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.SwapIndex = "";
        self.FixingDate = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlVanillaSwapFromSwapRateHelper(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.SwapRateHelper = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
