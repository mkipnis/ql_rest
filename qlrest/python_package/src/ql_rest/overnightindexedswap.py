# Autogenerated from the following stub : gensrc/stubs/stub.qlrest.python_module

# overnightindexedswap_py
# overnightindexedswap_py


import json
from json import JSONEncoder

class qlMakeDatedOIS(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.StartDate = "";
        self.EndDate = "";
        self.OvernightIndex = "";
        self.FixedRate = 0.0;
        self.FixDayCounter = "";
        self.Spread = 0.0;
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlMakeOIS(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.SettlDays = 0;
        self.SwapTenor = "";
        self.OvernightIndex = "";
        self.FixedRate = 0.0;
        self.ForwardStart = "";
        self.FixDayCounter = "";
        self.Spread = 0.0;
        self.PaymentLag = 0;
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlOvernightIndexedSwap(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.PayerReceiver = "";
        self.Nominal = [];
        self.Schedule = "";
        self.FixedRate = 0.0;
        self.FixDayCounter = "";
        self.OvernightIndex = "";
        self.Spread = 0.0;
        self.PaymentLag = 0;
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlOvernightIndexedSwapFromOISRateHelper(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.OISRateHelper = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__