/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import React from 'react'
import { useEffect, useState } from 'react';
import BusinessDatePicker from "./BusinessDatePicker";
import LabeledNumericInput from "./LabeledNumericInput"
import ValueLabel from "./ValueLabel"

import 'bootstrap/dist/css/bootstrap.min.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

import uuid from 'react-uuid'
import helpers from './helpers'
import PricerHelper from './PricerHelper'


const Results = React.forwardRef ((props, ref) => {

  const [fixedLegBPS, setFixedLegBPS] = useState(0.0);
  const [floatingLegBPS, setFloatingLegBPS] = useState(0.0);
  const [fairRate, setFairRate] = useState(0.0);
  const [fairSpread, setFairSpread] = useState(0.0);
  const [fixedRate, setFixedRate] = useState(0.0);
  const [spread, setSpread] = useState(0.0);
  const [fixedLegNPV, setFixedLegNPV] = useState(0.0);
  const [floatingLegNPV, setFloatingNPV] = useState(0.0);
  const [npv, setNPV] = useState(0.0);

  useEffect(() => {

    if ( props.termStructure !== undefined )
    {
      setFixedLegBPS( props.termStructure.fixed_leg_bps );
      setFloatingLegBPS( props.termStructure.floating_leg_bps );
      setFairRate( props.termStructure.par_rate * 100.0 );
      setFixedRate( props.termStructure.fixed_rate * 100.0 );
      setFairSpread( props.termStructure.par_spread * 10000.0);
      setSpread( props.termStructure.spread * 10000.0);
      setFixedLegNPV( props.termStructure.fixed_leg_npv );
      setFloatingNPV( props.termStructure.floating_leg_npv );
      setNPV( props.termStructure.npv );
    }

  },  [props.termStructure]);


  const handleChange = (elementName, new_value) =>
  {
      if ( props.termStructure === undefined )
        return;

    var price_request_out = {};

    price_request_out["request_id"] = uuid();
    price_request_out["business_date"] = props.curvePricingData.business_date;
    price_request_out["fixing_type"] =  props.curvePricingData.index.FixingType;
    price_request_out["swap"] = props.termStructure;
    price_request_out["operation"] = "RE_PRICE_SWAP";

    price_request_out["index"] = helpers.get_index_object(price_request_out, props.curvePricingData.index);
    helpers.populate_curve_object(price_request_out,  props.curvePricingData.index, props.curvePricingData.forward_curve);

    switch (elementName) {

      case "spread":
        price_request_out["swap"][elementName] = new_value/10000.0;
        setSpread(new_value);
      break;
      case "fixed_rate":
        price_request_out["swap"][elementName] = new_value/100.0;
        setFixedRate(new_value);
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
     <Container style={{backgroundColor:'#21222A',flexShrink:1}}>
        <Row>
          <Col style={{fontSize: '18px', textAlign:'center', margin:'5px'}}>
            {props.name}
          </Col>
        </Row>

       <Row  style={{marginBottom:'10px',flexShrink:1}}>

        <Col>
        <Row>
            <LabeledNumericInput label="Fixed Rate:" value={fixedRate} elementName="fixed_rate" onChange={handleChange} step={0.01}/>
        </Row>
        <Row>
            <ValueLabel label="Fair Rate:" value={0}/>
        </Row>
        <Row>
          <ValueLabel label="Fixed Leg BPS:" value={fixedLegBPS}/>
        </Row>
        </Col>

        <Col>
          <Row>
            <ValueLabel label="Fixed Leg NPV:" value={fixedLegNPV}/>
          </Row>
          <Row>
            <ValueLabel label="Floating Leg NPV:" value={floatingLegNPV}/>
          </Row>
          <Row>
            <ValueLabel label="NPV:" value={npv}/>
          </Row>
        </Col>

        <Col>
          <Row>
            <LabeledNumericInput label="Spread(BPS):" value={spread} elementName="spread" onChange={handleChange} step={1.0}/>
          </Row>
          <Row>
            <ValueLabel label="Fair Spread(BPS):" value={fairSpread}/>
          </Row>
          <Row>
            <ValueLabel label="Floating Leg BPS:" value={floatingLegBPS}/>
          </Row>
        </Col>
      </Row>
      </Container>
    )
 });



export default Results;
