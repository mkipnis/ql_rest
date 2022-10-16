# Autogenerated from the following stub : gensrc/stubs/stub.qlrest.python_module

# swaption_py
# swaption_py


import json
from json import JSONEncoder

class qlMakeSwaption(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.SwapIndex = "";
        self.OptionTenor = "";
        self.Strike = 0.0;
        self.PricingEngineID = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlSwaption(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.VanillaSwap = "";
        self.Exercise = "";
        self.SettlementType = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__