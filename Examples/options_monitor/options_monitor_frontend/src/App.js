/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import BusinessDatePicker from './components/BusinessDatePicker'
import LabeledNumericInput from './components/LabeledNumericInput'

import StockPanel from './components/StockPanel'
import VolPanel from './components/VolPanel'
import SmilePanel from './components/SmilePanel'
import GreeksPanel from './components/GreeksPanel'
import SingleStrikePricer from './components/SingleStrikePricer'
import Dropdown from './components/Dropdown'


import PricerHelper from './helpers/PricerHelper'
import QuantLibHelper from './helpers/QuantLibHelper'

import Symbols from './components/Symbols'
import 'react-datepicker/dist/react-datepicker.css';


import { useEffect, useState, useRef } from 'react';
import { Container, Row, Col } from 'react-bootstrap/';
import { compareAsc, format, parseISO } from 'date-fns'
import uuid from 'react-uuid'

import './App.css';

function App() {

  const volPanelRef = useRef();
  const smileRef = useRef();
  const greeksPanelRef = useRef();

  const [valuationDate, setValuationDate] = useState(new Date());
  const [defaultTicker, setDefaultTicker] = useState('AAPL');
  const [exerciseDates, setExerciseDates] = useState();
  const [exerciseDate, setExerciseDate] = useState();

  const [exerciseTypes, setExerciseTypes] = useState();
  const [exerciseType, setExerciseType] = useState();

  const [pricingToken, setPricingToken] = useState();
  const [stockTicker, setStockTicker] = useState();
  const [workingPrice, setWorkingPrice] = useState();
  const [workingDividendRate, setWorkingDividendRate] = useState();
  const [volPricingData, setVolPricingData] = useState();
  const [pricingDisabled, setPricingDisabled] = useState();
  const [greeksDisabled, setGreeksDisabled] = useState();
  const [error, setError] = useState();
  const [stockPrices, setStockPrices] = useState([]);
  const [volUpdate, setVolUpdate] = useState();
  const [volData, setVolData] = useState();
  const [selectedStrike, setSelectedStrike] = useState();
  const [selectedStrikeWithGreeks, setSelectedStrikeWithGreeks] = useState();
  const [riskFreeRate, setRiskFreeRate] = useState(0.25);
  useEffect(() => {

    setStockPrices(Symbols);

    var exercises = [];

    var exercise_american = {};
    exercise_american['value'] = 'American';
    exercise_american['label'] = 'American';
    exercises.push(exercise_american);

    var exercise_european = {};
    exercise_european['value'] = 'European';
    exercise_european['label'] = 'European';
    exercises.push(exercise_european);

    setExerciseTypes(exercises);
    setExerciseType(exercises[0]);

  }, []);

 useEffect(() => {

   if ( stockTicker === undefined || volData === undefined || exerciseDate === undefined )
   {
     return
   }

   console.log(riskFreeRate)

   var price_request = {}

   var vols_and_payoffs = {}
   vols_and_payoffs['call'] = []
   vols_and_payoffs['put'] = []

   var request_id = uuid();

   for ( var vol in volData )
   {
     if ( volData[vol].call_vol != undefined )
     {
       var call = QuantLibHelper.get_option_termstructure(request_id, 'Call', stockTicker.Price,
        volData[vol].Strike, volData[vol].call_vol, stockTicker.dividendRate, riskFreeRate/100.0 )
       vols_and_payoffs['call'].push(call)
     }

     if ( volData[vol].put_vol  != undefined )
     {
       var put = QuantLibHelper.get_option_termstructure(request_id, 'Put', stockTicker.Price,
        volData[vol].Strike, volData[vol].put_vol, stockTicker.dividendRate, riskFreeRate/100.0 )
       vols_and_payoffs['put'].push(put)
     }
   }

   price_request["request_id"] = request_id
   price_request["portal"] = "OPTIONS_PORTAL"
   price_request["exercise_type"] = exerciseType.value;
   price_request["business_date"] = format(valuationDate, "yyyy-MM-dd")

   price_request['vols_and_payoffs'] = vols_and_payoffs
   price_request['exercise'] = {}
   price_request['exercise']['ObjectId'] = request_id + "/" + stockTicker.Symbol

   var exerciseDateStr = exerciseDate.value;
   var y = exerciseDateStr.substring(0,4),
       m = parseInt(exerciseDateStr.substring(5,7)-1),
       d = exerciseDateStr.substring(8,10);

   // Add one day(Inconsistency between yahoo and quantlib expirations)
   var expiry_date = new Date(y,m,d);
   expiry_date.setDate(expiry_date.getDate() + 1)

   var expiry_date_iso = price_request['exercise']['ExpiryDate'] =  expiry_date.toLocaleString("default", { year: "numeric" }) + "-" +
    expiry_date.toLocaleString("default", { month: "2-digit" }) + "-" +
    expiry_date.toLocaleString("default", { day: "2-digit" })

   if (exerciseType.value == 'American')
   {
      price_request['exercise']['EarliestDate'] = format(valuationDate, "yyyy-MM-dd");
      price_request['exercise']['LatestDate'] = expiry_date_iso;
      price_request['exercise']['PayoffAtExpiry'] = false;
   }
   else
   {
     price_request['exercise']['ExpiryDate'] = expiry_date_iso;
   }

   price_request['exercise']['Permanent'] = false
   price_request['exercise']['Trigger'] = false
   price_request['exercise']['Overwrite'] = false


   setPricingDisabled(true);
   setSelectedStrikeWithGreeks(undefined);
   PricerHelper.submit_request(price_request, (pricingToken) => { setPricingToken(pricingToken); });



 }, [exerciseDate, volData, valuationDate, stockTicker, riskFreeRate, workingDividendRate, exerciseType]);

 useEffect(() => {

   if (pricingToken == undefined)
      return;

   PricerHelper.check_request(pricingToken, (pricingResults) =>
   {
     if (pricingResults.state == 1 )
     {
       var pricer_re_request = {};
       pricer_re_request["request_id"] = pricingResults.request_id;
       setPricingToken(pricer_re_request);
       return;

     } else {
       var greeks = PricerHelper.get_greeks(pricingResults);
       greeksPanelRef.current.update_greeks(greeks);
       setError(pricingResults.error);
       setPricingDisabled(false);
       setGreeksDisabled(false);
       if (selectedStrike !== undefined )
       {
         greeksPanelRef.current.flash_stike(selectedStrike);
       }

       setPricingToken(undefined);

     }
   });


 }, [pricingToken, selectedStrike]);

  useEffect(() => {

    if ( volUpdate != undefined && volData != undefined)
    {
      var updated_vols = [];
      updated_vols = volData.slice();
      updated_vols[volUpdate.rowIndex][volUpdate.colDef.field] = parseFloat(volUpdate.newValue);
      setVolData(updated_vols);
      setVolUpdate(undefined);
    }

  }, [volData, volUpdate]);


  useEffect(() => {

    if ( volPricingData != undefined && exerciseDate != undefined )
    {
      var volData = PricerHelper.get_vols(volPricingData[exerciseDate.value]);

      setVolData(volData);
    }

  }, [volPricingData, exerciseDate]);


  const VolPanelSetup = {
   pricingData:{volPricingData},
   curve_price_callback : ( priced_curve ) => {},
   pricing_start : () => { setPricingDisabled(true) },
 };

  return (
    <div className="ag-theme-balham-dark">
         <nav>
            <h3 className="nav--logo_text">
              <div>
                Vanilla Options
              </div>
              <div className="nav--logo_text_sub">
               <a href="https://github.com/mkipnis/ql_rest/tree/master/Examples/options_monitor">Support and Source Code</a>
              </div>
            </h3>
            <h6>
            </h6>
          </nav>
          <Container fluid  style={pricingDisabled ? {pointerEvents: "none", opacity: "0.4"} : {}}>
          <Row>
            <Col  style={{marginTop:'10px', textAlign:'Right', marginLeft:'auto', marginRight:0 }}>
              <Row>
                <Col>
                {
                  <div style={{margin :'10px', display:'flex', alignItems: 'flex-end', justifyContent:'flex-end', marginRight:10, fontSize:'15px'}}>
                    <BusinessDatePicker label='Valuation Date:'
                      elementName='business_date'
                      selected_date={valuationDate}
                      onValueChange={ (elementName, date) => {
                        setValuationDate(date);
                      } }/>
                    </div>

              }
              <Row>
          <Col  style={{textAlign:'Left'}}>
            <StockPanel symbols={stockPrices} defaultTicker={defaultTicker}

              gridReadyCallback = { () =>
              {
                var ticker_request = {};
                ticker_request['tickers'] = [];
                Symbols.forEach(symbol => ticker_request['tickers'].push(symbol['Symbol']));
                ticker_request['tickers'].push('risk_free_rates');

                var results = PricerHelper.get_data('get_latest_prices', ticker_request, (tickers) => {
                      var updated_market_data = [];

                      Symbols.forEach(symbol => {
                        symbol['Price'] = tickers[symbol['Symbol']].price;
                        symbol['dividendRate'] = tickers[symbol['Symbol']].dividendRate;
                        updated_market_data.push(symbol);

                      } );

                      var risk_free_rate = tickers['risk_free_rates']['1Y'];

                      setRiskFreeRate(risk_free_rate);
                      setStockPrices(updated_market_data);

                    });
                }}

              priceChangeCallback = { (updatePrice) =>
              {
                var updated_stock_price =  Object.assign({}, updatePrice.data);
                setWorkingPrice(updated_stock_price.Price);
                setWorkingDividendRate(updated_stock_price.dividendRate);
                setStockTicker(updated_stock_price);
              } }

              getVols = { (ticker) =>
              {
                var ticker_request = {}
                ticker_request['tickers'] = [ticker.Symbol,]

                setWorkingPrice(ticker.Price);
                setVolData(undefined)
                setStockTicker(ticker)
                setExerciseDate(undefined)
                setVolPricingData(undefined)

                var results = PricerHelper.get_data('get_latest_vols', ticker_request, (vols) => {

                      var updated_market_data = [];
                      var exercises = [];

                      Object.keys(Object.values(vols)[0]).forEach((item, i) => {
                          var exercise = {};
                          exercise['value'] = item;
                          exercise['label'] = item;
                          exercises.push(exercise);
                      });


                      var exercise_and_vols = Object.values(vols)[0] // First ticker

                      setExerciseDates(exercises)
                      setExerciseDate(exercises[0])
                      setVolPricingData(exercise_and_vols)
                    });
              }}

            />
              </Col>
            </Row>
          </Col>
          <Col>
          <Row>
                  <Col style={{marginTop:'5px', textAlign:'Right'}}>
                    <Dropdown label='Exercise Date:' elementName='Exercise' options={exerciseDates} selected_value={exerciseDate} onValueChange={ ( element, selected_option ) =>{
                      setExerciseDate(selected_option);
                    } }/>
                  </Col>
                </Row>
                <Row>
                  <Col style={{marginTop:'8px', textAlign:'Right'}}>
                    <VolPanel volData={volData} setup={VolPanelSetup} marketPrice={stockTicker} workingPrice={workingPrice} ref={volPanelRef}
                      strikeSelectedCallback={ (strike) => {
                        setSelectedStrike(strike);
                      } }
                      volChangeCallback={ ( updated_vol ) =>{
                        setWorkingPrice(updated_vol.Strike);
                        setVolUpdate(updated_vol);
                      } }/>
                  </Col>
                </Row>
          </Col>
        </Row>
        <Row style={{textAlign:'center'}}>
          <div className="main--sub--title">
            (Double click to change the Price or Call/Put Volatility)
          </div>
        </Row>
      </Col>
      <Col>
      <Row>
      <Col style={{textAlign: 'center', marginTop:'5px'}}>
            <h6>
              {
                stockTicker != undefined && exerciseDate != undefined ? stockTicker.Name + " : " + exerciseDate.label : ""
              }
            </h6>

      </Col>
      </Row>
        <Row>
        <Col>
        <div style={pricingDisabled ? {pointerEvents: "none", opacity: "0.4"} : {}}>
          <SmilePanel stockTicker={stockTicker}
                      volData={volData}
                      strikeSelectedCallback={ (strike) => {
                        setWorkingPrice(strike.Strike);
                      } }
                      ref={smileRef} />
        </div>
        </Col>
        </Row>
      </Col>
    </Row>

    <Row>
      <Col>
        {error}
      </Col>
    </Row>
    <Row style={{textAlign: 'center', marginTop:'20px'}}>
      <Col>
        <Row>
        <Col>

        <Row>
          <Col>
          <Row>
          {
            <div style={{margin :'10px', display:'flex', alignItems: 'flex-end', justifyContent:'flex-end', marginRight:10, fontSize:'15px'}}>
            <LabeledNumericInput label="Risk Free Rate:" value={riskFreeRate} elementName="riskFreeRate"
              onChanged={(elementName, new_value)=>{
                setRiskFreeRate(new_value);
              }}
              onChange= {(elementName, new_value)=>{
                setGreeksDisabled(true);
              }}
              postfix='%'
              step={0.025}/>
            </div>
            }
          </Row>
          </Col>
          <Col style={{marginTop:'5px'}}>
          <Dropdown label='Exercise:' elementName='ExerciseType'
              options={exerciseTypes} selected_value={exerciseType}
              onValueChange={ ( element, selected_option ) =>
                {
                  setExerciseType(selected_option);
                } }/>
          </Col>
        </Row>

        </Col>
        </Row>
        <div style={greeksDisabled ? {pointerEvents: "none", opacity: "0.4"} : {}}>
        <Row>
          <Col>
            <GreeksPanel volData={volData}
                    workingPrice={workingPrice}
                    marketPrice={stockTicker}
                    selectedStrike={selectedStrike}
                    ref={greeksPanelRef}

                    strikeSelectedCallback={ (strikeWithGreeks) => {
                      setSelectedStrikeWithGreeks(strikeWithGreeks);
                    } }/>
            </Col>
          </Row>
        </div>
        </Col>
      <Col>
        <SingleStrikePricer strikeWithGreeks = {selectedStrikeWithGreeks} exerciseType = {exerciseType} />
      </Col>
    </Row>
  </Container>
  </div>
  );
}

export default App;
