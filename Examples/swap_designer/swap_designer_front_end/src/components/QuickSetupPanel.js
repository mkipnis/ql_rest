/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import React from 'react'
import { useEffect, useState, useRef, useImperativeHandle } from 'react';
import Dropdown from "./Dropdown";
import BusinessDatePicker from "./BusinessDatePicker";
import { compareAsc, format, parseISO } from 'date-fns'
import LabeledNumericInput from "./LabeledNumericInput"

import { QuickSetupTenors, FixedFloat } from '../data/Constants'

import 'bootstrap/dist/css/bootstrap.min.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

import Button from 'react-bootstrap/Button';

import uuid from 'react-uuid'
import helpers from './helpers'
import PricerHelper from './PricerHelper'


const url = "http://localhost:7922";


const QuickSetupPanel = React.forwardRef ((props, ref) => {

  const tenorRef = React.useRef();
  const swapSideRef = React.useRef();

  const min_value = 100000;

  const [swapSide, setSwapSide] = useState(FixedFloat[0]);
  const [notional, setNotional] = useState(min_value);

  const [quickSetupSetting, setQuickSetupSetting] = useState({});
  const [pricingToken, setPricingToken] = useState();
  const [pricingResults, setPricingResults] = useState();

  useImperativeHandle(ref, () => ({
    run_setup(tenor)
    {
      tenorRef.current.setValue({label:tenor,value:tenor});
    },
  }));

  const handleChange = (elementName, value) =>
  {
    if ( value === undefined || props.curvePricingData === undefined )
      return;

    var price_request_out = {};

    price_request_out["request_id"] = uuid();
    price_request_out["business_date"] = props.curvePricingData.business_date;
    price_request_out["fixing_type"] =  props.curvePricingData.index.FixingType;

    price_request_out["index"] = helpers.get_index_object(price_request_out, props.curvePricingData.index);
    helpers.populate_curve_object(price_request_out, props.curvePricingData.index, props.curvePricingData.forward_curve);

    switch (elementName) {

      case "tenor":
          price_request_out["operation"] = "PRICE_SWAP_AT_PAR";
          price_request_out["tenor"] = value;
          price_request_out["pay_or_recieve"] = swapSide.value;
          price_request_out["notional"] = notional;
        break;

      case "notional":
          price_request_out["operation"] = "RE_PRICE_SWAP";
          price_request_out["swap"] = props.termStructure;
          price_request_out["swap"]["notional"] = value;
          setNotional(value);
        break;

      case "side":
          price_request_out["operation"] = "RE_PRICE_SWAP";
          price_request_out["swap"] = props.termStructure;
          price_request_out["swap"]["pay_or_recieve"] = value;
          setSwapSide(value == 'payer' ? FixedFloat[0] : FixedFloat[1] );
        break;
      default:
        return;

    }

    props.pricing_start();

    var results = PricerHelper.submit_request(price_request_out, (pricingResults) =>
    {
      if ( pricingResults["state"] == 2 )
      {
        props.Pricing_callback(pricingResults);
      }
    });
  }

  return (
     <Container style={{backgroundColor:'#21222A', border:'1px solid #404040'}}>
      <Row>
        <Col style={{fontSize: '18px', textAlign:'center', margin:'5px'}}>
            Quick Swap Setup
        </Col>
      </Row>
       <Row  style={{marginBottom:'10px'}}>
          <Col>
            <Dropdown label='Pay' elementName="side"
                  selected_value={swapSide}
                  options={FixedFloat}
                  onValueChange={handleChange}
                  ref={swapSideRef}/>
          </Col>
          <Col>
            <Dropdown label='Tenor' elementName="tenor"
              options={QuickSetupTenors}
              onValueChange={handleChange}
              ref={tenorRef}/>
          </Col>
          <Col>
            <LabeledNumericInput label="Notional" elementName="notional" type="notional"
              min_value={min_value}
              value={notional}
              step={min_value}
              onChange={handleChange}/>
          </Col>
         </Row>
      </Container>
    )
 });


export default QuickSetupPanel;
