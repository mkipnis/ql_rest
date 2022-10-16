# Autogenerated from the following stub : gensrc/stubs/stub.qlrest.python_module

# bonds_py
# bonds_py


import json
from json import JSONEncoder

class qlBond(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.Description = "";
        self.Currency = "";
        self.SettlementDays = 0;
        self.Calendar = "";
        self.FaceAmount = 0.0;
        self.MaturityDate = "";
        self.IssueDate = "";
        self.LegID = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlCmsRateBond(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.Description = "";
        self.Currency = "";
        self.SettlementDays = 0;
        self.PaymentBDC = "";
        self.FaceAmount = 0.0;
        self.ScheduleID = "";
        self.FixingDays = 0;
        self.IsInArrears = False;
        self.DayCounter = "";
        self.Floors = [];
        self.Gearings = [];
        self.SwapIndex = "";
        self.Spreads = [];
        self.Caps = [];
        self.Redemption = 0.0;
        self.IssueDate = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlFixedRateBond(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.Description = "";
        self.Currency = "";
        self.SettlementDays = 0;
        self.FaceAmount = 0.0;
        self.ScheduleID = "";
        self.Coupons = [];
        self.DayCounter = "";
        self.PaymentBDC = "";
        self.Redemption = 0.0;
        self.IssueDate = "";
        self.PaymentCalendar = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlFixedRateBond2(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.Description = "";
        self.Currency = "";
        self.SettlementDays = 0;
        self.FaceAmount = 0.0;
        self.ScheduleID = "";
        self.Coupons = [];
        self.PaymentBDC = "";
        self.Redemption = 0.0;
        self.IssueDate = "";
        self.PaymentCalendar = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlFloatingRateBond(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.Description = "";
        self.Currency = "";
        self.SettlementDays = 0;
        self.PaymentBDC = "";
        self.FaceAmount = 0.0;
        self.ScheduleID = "";
        self.FixingDays = 0;
        self.IsInArrears = False;
        self.DayCounter = "";
        self.Floors = [];
        self.Gearings = [];
        self.IborIndex = "";
        self.Spreads = [];
        self.Caps = [];
        self.Redemption = 0.0;
        self.IssueDate = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlZeroCouponBond(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.Description = "";
        self.Currency = "";
        self.SettlementDays = 0;
        self.Calendar = "";
        self.FaceAmount = 0.0;
        self.Maturity = "";
        self.PaymentBDC = "";
        self.Redemption = 0.0;
        self.IssueDate = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
