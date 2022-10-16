/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

const helpers = {

  int_to_date: function(int_val)
  {
    var business_date = parseInt(int_val);
    var year = business_date/10000|0;
    var month = (business_date - (year*10000))/100|0;
    var date = business_date - ((year * 10000) + (month *100))|0;

    return month.toString().padStart(2,'0') + "/" + date.toString().padStart(2,'0')  + "/" + year.toString();
  },

  get_index_object: function(price_request_out, marketDataIndex)
  {
    marketDataIndex.ObjectId = price_request_out["request_id"] + ':' + marketDataIndex.currency + '/' +
                              marketDataIndex.index + '/' + marketDataIndex.tenor;
    marketDataIndex.YieldCurve = "";
    marketDataIndex.Permanent = true;
    marketDataIndex.Trigger = true;
    marketDataIndex.Overwrite = false;

    return marketDataIndex;
  },

  populate_curve_object: function(price_request_out, marketDataIndex, marketData)
  {
    var swap_indexes = [];
    var swap_rate_helpers = [];
    var swap_rate_helper_objects = [];

    for (const swap_index of marketData)
    {
        if ( swap_index.rate === undefined )
          continue;

        console.log("SwapIndex and Market Data : " + swap_index);
        swap_index.ObjectId = price_request_out["request_id"] + ':' + swap_index.ticker;
        swap_index.Currency = marketDataIndex.Currency;
        //swap_index.Tenor = marketDataIndex.Tenor;
        swap_index.FwdCurve = '';
        swap_index.DiscCurve = '';
        swap_index.Permanent = true;
        swap_index.Trigger = true;
        swap_index.Overwrite = false;
        swap_indexes.push(swap_index);

        var swap_rate_helper = {};
        swap_rate_helper.ObjectId = swap_index.ObjectId + ':' + 'RateHelper';
        swap_rate_helper.Rate = swap_index.rate/100.0;
        swap_rate_helper.SwapIndex = swap_index.ObjectId;
        swap_rate_helper.Spread = 0.0;
        swap_rate_helper.ForwardStart = '0D';
        swap_rate_helper.DiscountingCurve = '';
        swap_rate_helper.PillarDate = 'LastRelevantDate';
        swap_rate_helper.CustomPillarDate = '';
        swap_rate_helper.Permanent = true;
        swap_rate_helper.Trigger = true;
        swap_rate_helper.Overwrite = false;
        swap_rate_helpers.push(swap_rate_helper);

        swap_rate_helper_objects.push(swap_rate_helper.ObjectId);
    }

    var ql_piecewise_yieldcurve = {};
    ql_piecewise_yieldcurve.ObjectId = price_request_out["request_id"]+"/qlPiecewiseYieldCurve";
    ql_piecewise_yieldcurve.NDays = 0;
    ql_piecewise_yieldcurve.Calendar = "TARGET";
    ql_piecewise_yieldcurve.RateHelpers = swap_rate_helper_objects;
    ql_piecewise_yieldcurve.DayCounter = "30/360 (Bond Basis)";
    ql_piecewise_yieldcurve.Accuracy = 0.000001;
    ql_piecewise_yieldcurve.Jumps = [];
    ql_piecewise_yieldcurve.JumpDates = [];
    ql_piecewise_yieldcurve.TraitsID = "Discount";
    ql_piecewise_yieldcurve.InterpolatorID = "LOGLINEAR";
    ql_piecewise_yieldcurve.Permanent = true;
    ql_piecewise_yieldcurve.Trigger = true;
    ql_piecewise_yieldcurve.Overwrite = false

    var ql_discounting_swap_engine = {};
    ql_discounting_swap_engine.ObjectId = price_request_out["request_id"]+"/qlDiscountingSwapEngine";
    ql_discounting_swap_engine.YieldCurve = ql_piecewise_yieldcurve.ObjectId;
    ql_discounting_swap_engine.IncludeSettlDate = false;
    ql_discounting_swap_engine.SettlementDate = "";
    ql_discounting_swap_engine.NpvDate = "";
    ql_discounting_swap_engine.Permanent = true;
    ql_discounting_swap_engine.Trigger = true;
    ql_discounting_swap_engine.Overwrite = false


    price_request_out["swap_indexes"] = swap_indexes;
    price_request_out["swap_rate_helpers"] = swap_rate_helpers;
    price_request_out["piecewise_yield_curve"] = ql_piecewise_yieldcurve;
    price_request_out["discounting_swap_engine"] = ql_discounting_swap_engine;

    price_request_out["index_with_curve"] = JSON.parse(JSON.stringify( price_request_out["index"] ) );
    price_request_out["index_with_curve"].ObjectId = price_request_out["index"].ObjectId + ql_piecewise_yieldcurve.ObjectId;
    price_request_out["index_with_curve"].YieldCurve = ql_piecewise_yieldcurve.ObjectId;

    return price_request_out;
  },

  get_market_data: function(index, tenors, market_data )
  {
    var market_data_out = [];

    if (index === undefined || market_data === undefined )
      return;

    for (const [array_index, tenor] of Object.entries(tenors))
    {
        var swap_ticker = index.Name + "-" + tenor;
        var rate = market_data[swap_ticker];

        var market_data_entry = JSON.parse(JSON.stringify( index ));
        market_data_entry.ticker = swap_ticker;
        if (rate !== undefined)
          market_data_entry.rate = rate;
        market_data_entry.Tenor = tenor;

        market_data_out.push( market_data_entry );
    }

    return market_data_out;
  }
}

export default helpers;
