# Autogenerated from the following stub : gensrc/stubs/stub.qlrest.python_module

# serialization_py
# serialization_py


import json
from json import JSONEncoder

class ohObjectLoad(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.Directory = "";
        self.Pattern = "";
        self.Recurse = False;
        self.Overwrite = False;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class ohObjectLoadString(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.Xml = "";
        self.Overwrite = False;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class ohObjectSave(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectList = [];
        self.Filename = "";
        self.Overwrite = False;
        self.IncludeGroups = False;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class ohObjectSaveString(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectList = [];
        self.Overwrite = False;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__
