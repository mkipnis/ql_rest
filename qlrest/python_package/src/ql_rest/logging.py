# Autogenerated from the following stub : ql_rest/gensrc/stubs/stub.qlrest.python_module

# logging_py
# logging_py


import json
from json import JSONEncoder

class ohLogSetConsole(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.Console = 0;
        self.LogLevel = 0;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class ohLogSetFile(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.LogFileName = "";
        self.LogLevel = 0;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class ohLogWriteMessage(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.LogMessage = "";
        self.LogLevel = 0;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__
