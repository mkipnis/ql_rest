

import { useEffect, useState } from 'react';
import React from 'react';
import PricerHelper from '../helpers/PricerHelper'

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import {AgGridColumn, AgGridReact} from 'ag-grid-react';

import { Container, Row, Col } from 'react-bootstrap/';

import { compareAsc, format, parseISO } from 'date-fns'

import QuantLibHelper from '../helpers/QuantLibHelper'

import BusinessDatePicker from './BusinessDatePicker'
import LabeledNumericInput from './LabeledNumericInput'

import uuid from 'react-uuid'

import '../App.css';

const { forwardRef, useRef, useImperativeHandle } = React;

const SingleStrikePricer = React.forwardRef ((props, ref) => {

  const numberFormatter = (params) => {
      if (!isNaN(params.value)) return parseFloat(params.value).toFixed(2);
  };

  const greeksGridRef = React.useRef();
  const [greeksData, setGreeksData] = useState([]);
  const [valuationDate, setValuationDate] = useState();
  const [expirationDate, setExpirationDate] = useState();

  const [stockPrice, setStockPrice] = useState();
  const [strikePrice, setStrikePrice] = useState();
  const [riskFreeRate, setRiskFreeRate] = useState();
  const [dividendRate, setDividendRate] = useState();
  const [callVol, setCallVol] = useState();
  const [putVol, setPutVol] = useState();
  const [error, setError] = useState();

  const [pricingToken, setPricingToken] = useState();
  const [pricingDisabled, setPricingDisabled] = useState();


  const greeksGridColumnDefs = [
    { field: 'name',  sortable: true, flex: 1, valueFormatter: numberFormatter},
    { field: 'call', headerName:'Call' ,
      sortable: true, flex: 1, valueFormatter: numberFormatter,
      cellStyle: {'textAlign': 'right', paddingRight:'20px', color: 'rgba(75,192,192,1)'}},
    { field: 'put', headerName:'Put' ,
      sortable: true, flex: 1,  valueFormatter: numberFormatter,
      cellStyle: {'textAlign': 'right', paddingRight:'20px', color: 'rgba(255, 99, 132,1)'}},
  ];

  const greeksGridOptions = {
    rowSelection: 'single',
    getRowHeight: (params) => 25,
  }

  const handleChange = (elementName, new_value) =>
  {
  }

  const getGreekPanelData = (data) =>
  {
    if ( data === undefined )
      return;

    var greeks = [];

    var npv = {'name':'Price',
              'call':data.call_npv,
              'put':data.put_npv};
    greeks.push(npv);

    var deltas = {'name':'Delta',
              'call':data.call_delta,
              'put':data.put_delta};
    greeks.push(deltas);

    var gamma = {'name':'Gamma',
              'call':data.call_gamma,
              'put':data.put_gamma};
    greeks.push(gamma);

    var vega = {'name':'Vega',
              'call':data.call_vega,
              'put':data.put_vega};
    greeks.push(vega);

    var theta = {'name':'Theta',
              'call':data.call_theta,
              'put':data.put_theta};
    greeks.push(theta);

    var rho = {'name':'Rho',
              'call':data.call_rho,
              'put':data.put_rho};
    greeks.push(rho);

    var vol = {'name':'Vol',
              'call':data.call_vol*100.0,
              'put':data.put_vol*100.0};
    greeks.push(vol);

    return greeks;
  }

  useEffect(() => {

    if (valuationDate === undefined || expirationDate === undefined )
      return;

    var price_request = {}

    var vols_and_payoffs = {}
    vols_and_payoffs['call'] = []
    vols_and_payoffs['put'] = []

    var request_id = uuid();


    var call = QuantLibHelper.get_option_termstructure(request_id, 'Call', stockPrice, strikePrice, callVol, dividendRate, riskFreeRate/100.0 )
    vols_and_payoffs['call'].push(call)

    var put = QuantLibHelper.get_option_termstructure(request_id, 'Put', stockPrice, strikePrice, putVol, dividendRate, riskFreeRate/100.0 )
    vols_and_payoffs['put'].push(put)

    price_request["request_id"] = request_id
    price_request["portal"] = "OPTIONS_PORTAL"
    price_request["exercise_type"] = props.exerciseType.value;
    price_request["business_date"] = format(valuationDate, "yyyy-MM-dd")

    price_request['vols_and_payoffs'] = vols_and_payoffs
    price_request['exercise'] = {}
    price_request['exercise']['ObjectId'] = request_id // + "/" + stockTicker.Symbol

    if ( props.exerciseType.value == 'American' )
    {
      price_request['exercise']['EarliestDate'] = format(valuationDate, "yyyy-MM-dd")
      price_request['exercise']['LatestDate'] = format(expirationDate, "yyyy-MM-dd")
      price_request['exercise']['PayoffAtExpiry'] = false;
    }
    else
      price_request['exercise']['ExpiryDate'] = format(expirationDate, "yyyy-MM-dd")

    price_request['exercise']['Permanent'] = false
    price_request['exercise']['Trigger'] = false
    price_request['exercise']['Overwrite'] = false


    setPricingDisabled(true);
    PricerHelper.submit_request(price_request, (pricingToken) => { setPricingToken(pricingToken); });

  }, [valuationDate, expirationDate, stockPrice, strikePrice, riskFreeRate, callVol, putVol, dividendRate]);

  useEffect(() => {

    if ( props.strikeWithGreeks != undefined )
    {
      setStockPrice(parseFloat(props.strikeWithGreeks.data.Underlying).toFixed(2));
      setStrikePrice(props.strikeWithGreeks.data.Strike);
      setCallVol(parseFloat(props.strikeWithGreeks.data.call_vol*100.0).toFixed(1));
      setPutVol(parseFloat(props.strikeWithGreeks.data.put_vol*100.0).toFixed(1));
      setRiskFreeRate(parseFloat(props.strikeWithGreeks.data.RiskFreeRate*100.0).toFixed(2));
      setDividendRate(parseFloat(props.strikeWithGreeks.data.DividendYield*props.strikeWithGreeks.data.Underlying).toFixed(2));

      if ( props.strikeWithGreeks.data.valuation_date != undefined &&
        props.strikeWithGreeks.data.expiration_date != undefined )
      {
        var valuation_date = parseISO(props.strikeWithGreeks.data.valuation_date);
        setValuationDate(valuation_date);

        var expiration_date = parseISO(props.strikeWithGreeks.data.expiration_date);
        setExpirationDate(expiration_date);
      }

      var greeks = getGreekPanelData(props.strikeWithGreeks.data);
      setGreeksData(greeks);
    }

  }, [props.strikeWithGreeks]);


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
        var greekIn = PricerHelper.get_greeks(pricingResults);
        var greeks = getGreekPanelData(greekIn[strikePrice]);
        setGreeksData(greeks);
        setError(pricingResults.error);
        setPricingDisabled(false);
        setPricingToken(undefined);
      }
    });


  }, [pricingToken, strikePrice]);


return (
   <div>

     <Row>
      <Col style={{textAlign: 'center'}}>
      <h6>
        <b>Single Strike Pricer</b>
      </h6>
      </Col>
     </Row>
      <Row>
        <Col>

        <Row>
          <Col style={{textAlign: 'center'}}>
          <h6>
            Option Input
          </h6>
          </Col>
        </Row>

        <Col style={{padding:'20px',backgroundColor:'#21222A', border:'1px solid #404040'}}>
          <h6>
          <Row>
            <LabeledNumericInput label="Stock Price:" value={stockPrice} elementName="stockPrice"
            onChanged= {(elementName, new_value)=>{setStockPrice(new_value);}}
            onChange= {(elementName, new_value)=>{setPricingDisabled(true);}}
            step={1.0}/>
          </Row>

          <Row>
            <LabeledNumericInput label="Strike Price:" value={strikePrice} elementName="strikePrice"
              onChanged= {(elementName, new_value)=>{setStrikePrice(new_value);}}
              onChange= {(elementName, new_value)=>{setPricingDisabled(true);}}
            step={1.0}/>
          </Row>

          {
          ( props.exerciseType!== undefined && props.exerciseType.value == 'American' ) ?
          (
            <div>
                <Row>
                  <BusinessDatePicker label='Earliest Date:' elementName='valuation_date' selected_date={valuationDate}
                  onValueChange={(elementName, date) => {setValuationDate(date);} }/>
                </Row>
                <Row>
                  <BusinessDatePicker label='Latest Date:' elementName='expiration_date' selected_date={expirationDate}
                  onValueChange={(elementName, date) => { setExpirationDate(date);} }/>
                </Row>
              </div>
            ) : (
              <div>
                  <Row>
                    <BusinessDatePicker label='Settlement Date:' elementName='valuation_date' selected_date={valuationDate}
                    onValueChange={(elementName, date) => {setValuationDate(date);} }/>
                  </Row>
                  <Row>
                    <BusinessDatePicker label='Expiration Date:' elementName='expiration_date' selected_date={expirationDate}
                    onValueChange={(elementName, date) => { setExpirationDate(date);} }/>
                  </Row>
                </div>
            )
          }

          <Row>
            <LabeledNumericInput label="Risk Free Rate:" value={riskFreeRate} elementName="riskFreeRate"
              onChanged={(elementName, new_value)=>{setRiskFreeRate(new_value);}}
              onChange= {(elementName, new_value)=>{setPricingDisabled(true);}}
              postfix='%'
              step={0.025}/>
          </Row>

          <Row>
            <LabeledNumericInput label="Dividend:" value={dividendRate} elementName="dividendRate"
              onChanged={(elementName, new_value)=>{setDividendRate(new_value);}}
              onChange= {(elementName, new_value)=>{setPricingDisabled(true);}}
              step={0.25}/>
          </Row>

          <Row>
            <LabeledNumericInput label="Call Volatility:" value={callVol} elementName="callVol"
              onChanged={(elementName, new_value)=>{setCallVol(new_value);}}
              onChange= {(elementName, new_value)=>{setPricingDisabled(true);}}
              postfix='%'
            step={0.5}/>
          </Row>

          <Row>
            <LabeledNumericInput label="Put Volatility:" value={putVol} elementName="putVol"
              onChanged={(elementName, new_value)=>{setPutVol(new_value);}}
              onChange= {(elementName, new_value)=>{setPricingDisabled(true);}}
              postfix='%'
              step={0.5}/>
          </Row>
          </h6>

          </Col>

          <Row style={{textAlign:'center'}}>
            <div className="main--sub--title">
              (Change the term structure and press Tab or Enter to Re-Price)
            </div>
          </Row>
        </Col>
        <Col style={pricingDisabled ? {pointerEvents: "none", opacity: "0.5"} : {}}>
          <Row>
            <Col>
            <h6>
              <Row>
                <Col style={{textAlign: 'center'}}>
                  Results
                </Col>
              </Row>
            </h6>
            </Col>
          </Row>
          <Row>
          <div className="ag-theme-balham-dark" style={{verticalAlign:"top",height:"22vh", width: "80%", display: "inline-block", margin: "auto"}}>
          <AgGridReact
                  rowData={greeksData}
                  columnDefs={greeksGridColumnDefs}
                  gridOptions={greeksGridOptions}
                  ref={greeksGridRef}>
            </AgGridReact>
        </div>
          </Row>
          <Row style={{width:'80%', margin: 'auto', padding:'10px'}}>
            <Col>
              Spot: {stockPrice}
            </Col>
            <Col>
              Strike: {strikePrice}
            </Col>
            <Col>
              CallVol: {callVol}
            </Col>
            <Col>
              PutVol: {putVol}
            </Col>
          </Row>
        </Col>
      </Row>
   </div>
  )
});

export default SingleStrikePricer;
