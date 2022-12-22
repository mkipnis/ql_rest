# Autogenerated from the following stub : git/ql_rest/gensrc/stubs/stub.qlrest.python_module

# volatilities_py
# volatilities_py


import json
from json import JSONEncoder

class qlBlackConstantVol(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.SettlementDate = "";
        self.Calendar = "";
        self.Volatility = 0.0;
        self.DayCounter = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
