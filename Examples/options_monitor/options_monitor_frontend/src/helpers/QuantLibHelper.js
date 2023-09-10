/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

const QuantLibHelper = {

  get_option_termstructure: function(request_id, type, underlying_price, strike, vol, div_rate, risk_free_rate)
  {

    var option_ts = {}

    request_id = type + '/' + request_id

    option_ts['qlStrikedTypePayoff'] = {}
    option_ts['qlBlackConstantVol'] = {}
    option_ts['qlGeneralizedBlackScholesProcess'] = {}

    option_ts['qlStrikedTypePayoff']['Strike'] = strike
    option_ts['qlStrikedTypePayoff']['ObjectId'] = request_id + '/qlStrikedTypePayoff' + '/' + option_ts['qlStrikedTypePayoff']['Strike']
    option_ts['qlStrikedTypePayoff']['PayoffID'] = 'Vanilla'
    option_ts['qlStrikedTypePayoff']['OptionType'] = type
    option_ts['qlStrikedTypePayoff']['ThirdParameter'] = 0.0
    option_ts['qlStrikedTypePayoff']['Permanent'] = false
    option_ts['qlStrikedTypePayoff']['Trigger'] = false
    option_ts['qlStrikedTypePayoff']['Overwrite'] = false

    option_ts['qlBlackConstantVol']['DayCounter'] = 'Actual/365 (Fixed)'
    option_ts['qlBlackConstantVol']['Calendar'] = 'TARGET'
    option_ts['qlBlackConstantVol']['SettlementDate'] = ''
    option_ts['qlBlackConstantVol']['ThirdParameter'] = 0.0
    option_ts['qlBlackConstantVol']['Permanent'] = false
    option_ts['qlBlackConstantVol']['Trigger'] = false
    option_ts['qlBlackConstantVol']['Overwrite'] = false

    option_ts['qlGeneralizedBlackScholesProcess']['DayCounter'] = option_ts['qlBlackConstantVol']['DayCounter']
    option_ts['qlGeneralizedBlackScholesProcess']['SettlementDate'] = ''
    option_ts['qlGeneralizedBlackScholesProcess']['Underlying'] = underlying_price
    option_ts['qlGeneralizedBlackScholesProcess']['RiskFreeRate'] = risk_free_rate

    if (div_rate != undefined)
      option_ts['qlGeneralizedBlackScholesProcess']['DividendYield'] = div_rate/underlying_price;
    else
      option_ts['qlGeneralizedBlackScholesProcess']['DividendYield'] = 0.0

    option_ts['qlGeneralizedBlackScholesProcess']['Permanent'] = false
    option_ts['qlGeneralizedBlackScholesProcess']['Trigger'] = false
    option_ts['qlGeneralizedBlackScholesProcess']['Overwrite'] = false

    option_ts['qlBlackConstantVol']['ObjectId'] = request_id + '/qlBlackConstantVol/' + option_ts['qlStrikedTypePayoff']['Strike'] + '/' + option_ts['qlStrikedTypePayoff']['OptionType']
    option_ts['qlBlackConstantVol']['Volatility'] = vol/100.0

    option_ts['qlGeneralizedBlackScholesProcess']['ObjectId'] = request_id +'/qlGeneralizedBlackScholesProcess/' + option_ts['qlBlackConstantVol']['ObjectId']
    option_ts['qlGeneralizedBlackScholesProcess']['BlackVolID'] = option_ts['qlBlackConstantVol']['ObjectId']


    return option_ts

  },

};

export default QuantLibHelper;
