from django.http import HttpResponse
import json
from django.views.decorators.csrf import csrf_exempt
import requests

@csrf_exempt
def submit_request(request):
    print('RequestBody' + str(request.body))

    url = 'http://localhost:17921/submit_request/'

    response = requests.get(url, data=request.body, headers={"Content-Type": "application/json"})
    print(response.json())

    response_text = response.json()
    response_json = json.dumps(response_text, indent=4)

    response = HttpResponse(response_json, content_type="application/json")

    return response

@csrf_exempt
def check_request(request):
    print('RequestBody' + str(request.body))

    url = 'http://localhost:17921/check_request/'

    response = requests.get(url, data=request.body, headers={"Content-Type": "application/json"})
    print(response.json())

    response_text = response.json()
    response_json = json.dumps(response_text, indent=4)

    response = HttpResponse(response_json, content_type="application/json")

    return response
