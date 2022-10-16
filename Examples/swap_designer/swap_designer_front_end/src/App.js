/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */
import logo from './logo.svg';

import 'bootstrap/dist/css/bootstrap.min.css';
import './App.css';

import { useEffect, useState, useRef } from 'react';

import BusinessDatePicker from "./components/BusinessDatePicker";

import CurveChart from './components/CurveChart';
import MarketDataPanel from './components/MarketDataPanel';
import IndexPanel from './components/IndexPanel';
import SwapPanel from './components/SwapPanel';
import QuickSetupPanel from './components/QuickSetupPanel';
import CashflowsPanel from './components/CashflowsPanel';
import Results from './components/Results';
import PricerHelper from './components/PricerHelper'

import 'react-datepicker/dist/react-datepicker.css';

import { Container, Row, Col } from 'react-bootstrap/';
import { compareAsc, format, parseISO } from 'date-fns'


import { SwapIndexes, SwapIndexTenors } from './data/SwapIndexes'

import helpers from './components/helpers'

function App() {

  const quickSetupRef = useRef();
  const marketDataRef = useRef();
  const swapPanelRef = useRef();

  const swap_defaults = {"notional":1000000, "fixed_leg":{}, "floating_leg":{}};

  const [swapCurve, setSwapCurve] = useState();
  const [swapPricingResults, setSwapPricingResults] = useState(swap_defaults);
  const [swapIndex, setSwapIndex] = useState();
  const [valuationDate, setValuationDate] = useState(new Date());
  const [tenor, setTenor] = useState('5Y');
  const [curvePricingData, setCurvePricingData] = useState();
  const [error, setError] = useState();
  const [pricingDisabled, setPricingDisabled] = useState();
  const [marketData, setMarketData] = useState();


  useEffect(() => {

    PricerHelper.get_market_data((swap_data) =>
    {
      setMarketData(swap_data);
    })

  }, []);


  useEffect( () => {

    setSwapPricingResults(undefined);
    setSwapCurve(undefined);

    var curve_pricing_data = {};
    curve_pricing_data['index'] = swapIndex;
    curve_pricing_data['business_date'] = valuationDate;
    curve_pricing_data['forward_curve'] = helpers.get_market_data(swapIndex,SwapIndexTenors, marketData);

    setCurvePricingData(curve_pricing_data);

    console.log(curve_pricing_data);

  },[swapIndex]);

  useEffect( () => {

        if ( marketData !== undefined )
        {
          setSwapPricingResults(undefined);

          var curve_pricing_data = {};
          curve_pricing_data['index'] = swapIndex;
          curve_pricing_data['business_date'] = valuationDate;
          curve_pricing_data['forward_curve'] = helpers.get_market_data(swapIndex,SwapIndexTenors, marketData);

          setCurvePricingData(curve_pricing_data);

          console.log(curve_pricing_data);
          setSwapIndex(SwapIndexes[0]);
        }

    },[marketData]);

  const marketDataPanelSetup = {

    pricingData:{curvePricingData},

    curve_price_callback : ( priced_curve ) => {

        if ( priced_curve.error_code == 0 )
        {
          setSwapCurve(priced_curve.curve_points);
          marketDataRef.current.reset_selected_tenor(tenor);
          setError("");
          setPricingDisabled(false)
        } else {
          console.log( "Error : " +  priced_curve.error );
          setError(priced_curve.error);
          setPricingDisabled(true)
        }
      },

    pricing_start : () =>
    {
      setPricingDisabled(true)
    },

    tenorReset : ( tenor ) => {
        setTenor(tenor);
        //setShowPricing(true);
        quickSetupRef.current.run_setup(tenor);
      },
  };

  return (

    <div className="ag-theme-balham-dark">
     <nav>
        <h3 className="nav--logo_text">Swap Designer</h3>
              <div>
                      <BusinessDatePicker label='Valuation Date' elementName='business_date' selected_date={valuationDate}
                          onValueChange={
                              (elementName, date) =>
                              {
                                console.log("Business Date : " + date );
                                setSwapPricingResults(undefined);

                                var curve_pricing_data = {};
                                curve_pricing_data['index'] = swapIndex;
                                curve_pricing_data['business_date'] = date;
                                curve_pricing_data['forward_curve'] = helpers.get_market_data(swapIndex,SwapIndexTenors, marketData);

                                setCurvePricingData(curve_pricing_data);

                                console.log(curve_pricing_data);
                              }
                              }
                            />
              </div>
      </nav>
    <Container fluid>
    <Row>
      <Col>
        <Row>
          <Col>
            <IndexPanel indexChangeCallback={ (swap_index) =>
            {
              setSwapIndex(swap_index);
            }}
            />
          </Col>
          <Col>
            <MarketDataPanel setup={marketDataPanelSetup} ref={marketDataRef}/>
          </Col>
        </Row>
      </Col>
      <Col>
        <div style={pricingDisabled ? {pointerEvents: "none", opacity: "0.4"} : {}}>
          <CurveChart swapIndex={swapIndex} swapCurve={swapCurve}/>
        </div>
      </Col>
    </Row>
    <Row>
      <Col>
        {error}
      </Col>
    </Row>
    <div style={pricingDisabled ? {pointerEvents: "none", opacity: "0.4"} : {}}>
    <Row style={{margin:'5px'}}>
      <Col>
      <Row>
        <Col>
          <QuickSetupPanel
            curvePricingData={curvePricingData}
            termStructure={swapPricingResults}

            Pricing_callback={ (pricing_results) => {
              setSwapPricingResults(pricing_results.my_swap);
              setPricingDisabled(false);
            }}

            pricing_start = { () => {
              setPricingDisabled(true);
            }}


            ref={quickSetupRef}/>

        </Col>
      </Row>
        <Row style={{margin:'5px'}} >
          <Col style={{backgroundColor:'#21222A', border:'1px solid #404040'}}>
            <SwapPanel

              curvePricingData={curvePricingData}
              termStructure={swapPricingResults}

              Pricing_callback={ (pricing_results) => {
                    setSwapPricingResults(pricing_results.my_swap);
                    setPricingDisabled(false);
              } }

              pricing_start = { () => {
                      setPricingDisabled(true);
                    }}

            ref={swapPanelRef}/>

          </Col>
        </Row>
        <Row style={{margin:'5px'}}>
          <Col style={{backgroundColor:'#21222A', border:'1px solid #404040'}}>
            <Results name="Results"
                          curvePricingData={curvePricingData}
                          termStructure={swapPricingResults}

                          Pricing_callback={ (pricing_results) => {
                              setSwapPricingResults(pricing_results.my_swap);
                              setPricingDisabled(false);
                                } }

                          pricing_start = { () => {
                                  setPricingDisabled(true);
                                }}

                    ref={swapPanelRef}/>
          </Col>
        </Row>
        <div style={{display: 'flex', justifyContent: 'center'}}>
                <a href="mailto:mike.kipnis@gmail.com">Support</a>
        </div>
      </Col>
      <Col>
        <CashflowsPanel results={swapPricingResults}/>
      </Col>
    </Row>
        </div>
      </Container>
    </div>

  );
}

export default App;
