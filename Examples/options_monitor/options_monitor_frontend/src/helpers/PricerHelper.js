/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

//const url = "https://ustreasuries.online";
//import { MarketData } from '../data/MarketData'

//const url = "http://localhost:8000";
const url = "https://options.ustreasuries.online";

const PricerHelper = {

  submit_request: function(request, callback)
  {
      var result_out = {}

      request["portal"] = "OPTION_PORTAL";

      const requestOptions = {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(request)
      };

      fetch(url + '/submit_request/', requestOptions)
        .then(res => res.json())
        .then(pricingToken => { callback( pricingToken )
        } );

      return result_out;
  },

  check_request: function(pricingToken, callback)
  {
    setTimeout(() => {

      var check_results = {};
      check_results["request_id"] = pricingToken.request_id;
      const requestOptionsResults = { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(check_results) };
      fetch(url + '/check_request/', requestOptionsResults) .then(res => res.json()) .then(check_result => callback(check_result));

    }, 1000);
  },

  get_data: function( service, request, callback )
  {
    var result_out = {}

    request["portal"] = "OPTIONS_PORTAL";

    const requestOptions = {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(request)
    };

    fetch(url + '/' + service +'/', requestOptions)
      .then(res => res.json())
      .then( tickers => callback(tickers));

    return result_out;
  },

  get_vols: function ( vols_in )
  {
    var call_vols = vols_in['calls']
    var put_vols = vols_in['puts']

    var vols = {};

    for ( var strike in call_vols) {
      var f_strike = parseFloat(strike);
      vols[f_strike] = {};
      vols[f_strike]['Strike'] = f_strike;
      vols[f_strike]['call_vol'] = call_vols[strike];
    }

    for (var strike in put_vols) {
      var f_strike = parseFloat(strike);
      if (!(f_strike in vols) )
      {
        vols[f_strike] = {};
        vols[f_strike]['Strike'] = f_strike;
      }

      vols[f_strike]['put_vol'] = put_vols[strike];
    }

    //var strikes = Object.keys(vols)
    var sorted_stikes = Object.keys(vols).sort((a,b) => a-b);

    var vols_out = [];

    for (var i = 0; i < sorted_stikes.length; i++)
    {
      var strike_str = sorted_stikes[i].toString();
      var call_put_vol = vols[sorted_stikes[i]];
      vols_out.push(call_put_vol);
    }

    return vols_out;
  },


  get_greeks: function ( pricer_greeks )
  {
    var call_greeks = pricer_greeks['call']
    var put_greeks = pricer_greeks['put']

    var greeks = {};

    for ( var strike in call_greeks) {
      var f_strike = parseFloat(call_greeks[strike]['Strike']);
      greeks[f_strike] = {};
      greeks[f_strike]['Strike'] = f_strike;
      greeks[f_strike]['call_greeks'] = call_greeks[strike];
    }

    for (var strike in put_greeks) {
      var f_strike = parseFloat(put_greeks[strike]['Strike']);
      if (!(f_strike in greeks) )
      {
        greeks[f_strike] = {};
        greeks[f_strike]['Strike'] = f_strike;
      }

      greeks[f_strike]['put_greeks'] = put_greeks[strike];
    }

    var sorted_stikes = Object.keys(greeks).sort((a,b) => a-b);

    var greeks_out = {};

    for (var i = 0; i < sorted_stikes.length; i++)
    {
      var strike_str = sorted_stikes[i].toString();
      var call_put_greeks = {}

      var call_greeks = greeks[sorted_stikes[i]]['call_greeks'];
      var put_greeks = greeks[sorted_stikes[i]]['put_greeks'];

      for (var key in call_greeks)
      {
          call_put_greeks[key] = call_greeks[key];
      }

      for (var key in put_greeks)
      {
          call_put_greeks[key] = put_greeks[key];
      }


      greeks_out[strike_str] = call_put_greeks;
    }

    return greeks_out;
  }


};

export default PricerHelper;
