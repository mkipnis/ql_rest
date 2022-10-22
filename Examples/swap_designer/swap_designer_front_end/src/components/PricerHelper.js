/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

//const url = "https://ustreasuries.online";
import { MarketData } from '../data/MarketData'

const url = "http://localhost:7922";

const PricerHelper = {

  submit_request: function(request, callback)
  {
      var result_out = {}

      request["portal"] = "SWAP_PORTAL";

      const requestOptions = {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(request)
      };

      fetch(url + '/submit_request/', requestOptions)
        .then(res => res.json())
        .then(pricingToken => {


          setTimeout(() => {

            var check_results = {};
            check_results["request_id"] = pricingToken.request_id;
            const requestOptionsResults = { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(check_results) };
            fetch(url + '/check_request/', requestOptionsResults) .then(res => res.json())
            .then(check_result => callback(check_result));

          }, 1000);


        } );

      return result_out;
  },

  get_market_data: function( callback )
  {
    callback(MarketData);
  },

};

export default PricerHelper;
