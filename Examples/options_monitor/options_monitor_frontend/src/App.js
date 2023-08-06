/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import BusinessDatePicker from './components/BusinessDatePicker'

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
  const [pricingToken, setPricingToken] = useState();
  const [stockTicker, setStockTicker] = useState();
  const [workingPrice, setWorkingPrice] = useState();
  const [volPricingData, setVolPricingData] = useState();
  const [pricingDisabled, setPricingDisabled] = useState();
  const [error, setError] = useState();
  const [stockPrices, setStockPrices] = useState([]);
  const [volUpdate, setVolUpdate] = useState();
  const [volData, setVolData] = useState();
  const [selectedStrike, setSelectedStrike] = useState();
  const [selectedStrikeWithGreeks, setSelectedStrikeWithGreeks] = useState();
  const [riskFreeRate, setRiskFreeRate] = useState(0.0025);
  const [ratePlaceHolder, setRatePlaceHolder] = useState(riskFreeRate * 100.0 );

  useEffect(() => {

    setStockPrices(Symbols);

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
       var call = QuantLibHelper.get_option_termstructure(request_id, 'Call', stockTicker.Price, volData[vol].Strike, volData[vol].call_vol, 0.0, riskFreeRate )
       vols_and_payoffs['call'].push(call)
     }

     if ( volData[vol].put_vol  != undefined )
     {
       var put = QuantLibHelper.get_option_termstructure(request_id, 'Put', stockTicker.Price, volData[vol].Strike, volData[vol].put_vol, 0.0, riskFreeRate )
       vols_and_payoffs['put'].push(put)
     }
   }

   price_request["request_id"] = request_id
   price_request["portal"] = "OPTIONS_PORTAL"
   price_request["business_date"] = format(valuationDate, "yyyy-MM-dd")

   price_request['vols_and_payoffs'] = vols_and_payoffs
   price_request['exercise'] = {}
   price_request['exercise']['ObjectId'] = request_id + "/" + stockTicker.Symbol

   var expiry_date = new Date(Date.parse(exerciseDate.value))
   expiry_date.setDate(expiry_date.getDate() + 1)

   price_request['exercise']['ExpiryDate'] =  expiry_date.toLocaleString("default", { year: "numeric" }) + "-" +
      expiry_date.toLocaleString("default", { month: "2-digit" }) + "-" +
      expiry_date.toLocaleString("default", { day: "2-digit" })

   price_request['exercise']['Permanent'] = false
   price_request['exercise']['Trigger'] = false
   price_request['exercise']['Overwrite'] = false


   setPricingDisabled(true);
   PricerHelper.submit_request(price_request, (pricingToken) => { setPricingToken(pricingToken); });

 }, [exerciseDate, volData, valuationDate, stockTicker, riskFreeRate]);

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
            <h3 className="nav--logo_text">Vanilla Options</h3>
            <h6>
            <div> <BusinessDatePicker label='Valuation Date' elementName='business_date' selected_date={valuationDate} onValueChange={ (elementName, date) => { setValuationDate(date); } }/> </div>
            </h6>
          </nav>

          <Container fluid  style={pricingDisabled ? {pointerEvents: "none", opacity: "0.4"} : {}}>
          <Row>
            <Col>
              <Row>
                <Col style={{marginTop:'10px', textAlign:'Right'}}>
                <Row>
                  <Col style={{marginLeft:'95px'}}> <h6>Risk Free Rate:</h6> </Col>
                  <Col style={{marginRight:'10px'}}> <h6>
                  <input type="number" value={ratePlaceHolder}
                  style={{width: '100px', height:'30px', textAlign:'right'}} step={0.25}
                  onChange={(e)=> { 
                    setRatePlaceHolder(e.target.value);
                  }}
                  onKeyDown={(e)=> {
                    if(e.key === 'Enter' || e.key === 'Tab')
                    { var risk_free_rate = Number(ratePlaceHolder)/100.0;
                      setRiskFreeRate(risk_free_rate); }
                    }} /> </h6> </Col>
                </Row>
              <Row>
          <Col  style={{textAlign:'Left'}}>
            <StockPanel symbols={stockPrices} defaultTicker={defaultTicker}

              gridReadyCallback = { () =>
              {
                var ticker_request = {};
                ticker_request['tickers'] = [];
                Symbols.forEach(symbol => ticker_request['tickers'].push(symbol['Symbol']));

                var results = PricerHelper.get_data('get_latest_prices', ticker_request, (tickers) => {
                      var updated_market_data = [];

                      Symbols.forEach(symbol => {
                        symbol['Price'] = tickers[symbol['Symbol']].price;
                        updated_market_data.push(symbol);

                      } );

                      setStockPrices(updated_market_data);

                    });
                }}

              priceChangeCallback = { (updatePrice) =>
              {
                var updated_stock_price =  Object.assign({}, updatePrice.data);
                setWorkingPrice(updated_stock_price.Price);
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
      <Col style={{textAlign: 'center'}}>
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
        <GreeksPanel volData={volData}
                    workingPrice={workingPrice}
                    marketPrice={stockTicker}
                    selectedStrike={selectedStrike}
                    ref={greeksPanelRef}

                    strikeSelectedCallback={ (strikeWithGreeks) => {
                      setSelectedStrikeWithGreeks(strikeWithGreeks);
                    } }/>
          </Col>
      <Col>
        <SingleStrikePricer strikeWithGreeks = {selectedStrikeWithGreeks}/>
      </Col>
    </Row>
  </Container>
  </div>
  );
}

export default App;
