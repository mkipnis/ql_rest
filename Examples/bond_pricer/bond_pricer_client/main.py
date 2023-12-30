"""
   Copyright (C) 2022 Mike Kipnis

   This file is part of ql_rest, a free-software/open-source library
   for utilization of QuantLib over REST

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
"""

import json
import time
import requests
import USTBond
from json import JSONEncoder

class CurveEncoder(JSONEncoder):

    def default(self,o):
        return o.__dict__

def load_instruments( pricer_count ):

    f = open('us_treasuries.json')
    data = json.load(f)

    instruments_out = [{} for _ in range(pricer_count)]

    instrument_index = 0
    for reference_data in data:

        if reference_data["securityType"] == 'Note' or reference_data["securityType"] == 'Bond':
            ust_bond = USTBond.USTBond()

            if ust_bond.set_reference_data(reference_data) == True:
                cusip = reference_data["cusip"]
                instruments_out[instrument_index % pricer_count][cusip] = ust_bond
                instrument_index = instrument_index + 1

    f.close()

    return instruments_out

def publish_request( end_point, pricing_request):

    price_request_out_json = json.dumps(pricing_request, indent=4, cls=CurveEncoder)

    response = requests.get(end_point, data=price_request_out_json, headers={"Content-Type": "application/json"})
    #print(response.json())

    return response.json()

def check_request( end_point, json_request):

    check_request = {}
    check_request["request_id"] = json_request["request_id"];
    check_request_json = json.dumps(check_request)
    response = requests.get(end_point, data=check_request_json, headers={"Content-Type": "application/json"})

    response_dict = json.loads(response.text)

    return response_dict

if __name__ == '__main__':

    price_request_out = dict()

    price_request_out["business_date"] = "2021-02-01" 

    curve_out = dict()
    curve_out["curve_points"] = {}
    curve_out["curve_points"]["2Y"] = 1/100.0
    curve_out["curve_points"]["3Y"] = 1.1/100.0
    curve_out["curve_points"]["5Y"] = 1.2/100.0
    curve_out["curve_points"]["10Y"] = 1.3/100.0
    curve_out["curve_points"]["30Y"] = 1.4/100.0

    curve_out["bond_template"] = USTBond.USTBond()

    price_request_out["curve"] = curve_out

    number_of_instrument_groups = 2

    instrument_groups = load_instruments(number_of_instrument_groups)

    json_out = []

    price_request_out["instruments"] = instrument_groups[0]
    server_response = publish_request("http://localhost:15555/submit_request/", price_request_out)
    json_out.append(server_response)

    price_request_out["instruments"] = instrument_groups[1]
    server_response = publish_request("http://localhost:16666/submit_request/", price_request_out)
    json_out.append(server_response)

    time.sleep(3)

    response = [];

    pricer_reply = check_request("http://localhost:15555/check_request/", json_out[0])
    response.append(pricer_reply)

    pricer_reply = check_request("http://localhost:16666/check_request/", json_out[1])
    response.append(pricer_reply)

    for cusip in instrument_groups[0]:
        print("Pricer 0 : [" + cusip + "] - " + str(response[0][cusip]))

    for cusip in instrument_groups[1]:
        print("Pricer 1 : [" + cusip + "] - " + str(response[1][cusip]))
