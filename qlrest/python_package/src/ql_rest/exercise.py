# Autogenerated from the following stub : git/ql_rest/gensrc/stubs/stub.qlrest.python_module

# exercise_py
# exercise_py


import json
from json import JSONEncoder

class qlAmericanExercise(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.EarliestDate = "";
        self.LatestDate = "";
        self.PayoffAtExpiry = False;
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlBermudanExercise(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.Dates = [];
        self.PayoffAtExpiry = False;
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class qlEuropeanExercise(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.ExpiryDate = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
