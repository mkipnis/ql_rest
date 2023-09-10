import requests
import json

RISK_FREE_DATA_URL = "https://ustreasuries.online/data/"


def save_risk_free_rates(base_dir, risk_free_rates):
    with open(base_dir + '/demo_data/risk_free_rates.json', 'w') as risk_free_rates_file:
        json.dump(risk_free_rates, risk_free_rates_file)
        risk_free_rates_file.close()


if __name__ == '__main__':
    risk_free_data_response = requests.get(url=RISK_FREE_DATA_URL)
    risk_free_data = risk_free_data_response.json()
    save_risk_free_rates('../', risk_free_data['data']['market_data'][0])
