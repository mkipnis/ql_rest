# Autogenerated from the following stub : ql_rest/gensrc/stubs/stub.qlrest.python_module

# garbagecollection_py
# garbagecollection_py


import json
from json import JSONEncoder

class ohRepositoryDeleteAllObjects(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.DeletePermanent = False;
        self.Trigger = False;

    def default(self, o):
        return o.__dict__

import json
from json import JSONEncoder

class ohRepositoryLogObject(JSONEncoder):

    def __init__(self):
        self.instanceID = ""
        
        self.ObjectId = "";
        self.Trigger = False;

    def default(self, o):
        return o.__dict__
