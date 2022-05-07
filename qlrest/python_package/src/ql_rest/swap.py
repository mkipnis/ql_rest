# Autogenerated from the following stub : ql_rest/gensrc/stubs/stub.qlrest.python_module

# swap_py
# swap_py


import json
from json import JSONEncoder

class qlMakeCms(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.SwapTenor = "";
        self.SwapIndex = "";
        self.IborIndex = "";
        self.IborSpread = 0.0;
        self.ForwardStart = "";
        self.CmsCouponPricer = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlSwap(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.LegIDs = [];
        self.Payer = [];
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
