# Autogenerated from the following stub : git/ql_rest/gensrc/stubs/stub.qlrest.python_module

# index_py
# index_py


import json
from json import JSONEncoder

class qlBMAIndex(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlEonia(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlEuribor(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.Tenor = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlEuribor365(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.Tenor = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlEuriborSwap(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.FixingType = "";
        self.Tenor = "";
        self.FwdCurve = "";
        self.DiscCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlEuriborSwapIsdaFixA(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.Tenor = "";
        self.FwdCurve = "";
        self.DiscCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlIborIndex(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.FamilyName = "";
        self.Tenor = "";
        self.FixingDays = 0;
        self.Currency = "";
        self.Calendar = "";
        self.BDayConvention = "";
        self.EndOfMonth = False;
        self.DayCounter = "";
        self.FwdCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlIndexAddFixings(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.FixingDates = [];
        self.FixingValues = [];
        self.ForceOverwrite = False;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlLibor(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.Currency = "";
        self.Tenor = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlLiborSwap(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.Currency = "";
        self.FixingType = "";
        self.Tenor = "";
        self.FwdCurve = "";
        self.DiscCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlOvernightIndex(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.FamilyName = "";
        self.FixingDays = 0;
        self.Currency = "";
        self.Calendar = "";
        self.DayCounter = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlProxyIbor(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.FamilyName = "";
        self.Tenor = "";
        self.FixingDays = 0;
        self.Currency = "";
        self.Calendar = "";
        self.BDayConvention = "";
        self.EndOfMonth = False;
        self.DayCounter = "";
        self.Gearing = 0.0;
        self.IborIndex = "";
        self.Spread = 0.0;
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlSonia(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.YieldCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlSwapIndex(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.FamilyName = "";
        self.Tenor = "";
        self.FixingDays = 0;
        self.Currency = "";
        self.Calendar = "";
        self.FixedLegTenor = "";
        self.FixedLegBDC = "";
        self.FixedLegDayCounter = "";
        self.IborIndex = "";
        self.DiscCurve = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
