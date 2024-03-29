# Autogenerated from the following stub : git/ql_rest/gensrc/stubs/stub.qlrest.python_module

# forwardrateagreement_py
# forwardrateagreement_py


import json
from json import JSONEncoder

class qlFRA(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.ValueDate = "";
        self.MaturityDate = "";
        self.Position = "";
        self.Strike = 0.0;
        self.Notional = 0.0;
        self.IborIndex = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
