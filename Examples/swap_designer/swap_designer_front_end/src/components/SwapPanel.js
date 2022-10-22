/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import React from 'react'
import { useEffect, useState } from 'react';

import SwapLegPanel from './SwapLegPanel'

import uuid from 'react-uuid'

//import './SwapPanel.css';

import 'bootstrap/dist/css/bootstrap.min.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

import helpers from './helpers'
import PricerHelper from './PricerHelper'

//const url = "http://localhost:7921";

const SwapPanel = React.forwardRef ((props, ref) => {

  const [fixedLeg, setFixedLeg] = useState();
  const [floatingLeg, setFloatingLeg] = useState();
  const [pricingToken, setPricingToken] = useState();
  const [pricingResults, setPricingResults] = useState();

  const handleSetup = ( leg_name, elementName, new_value ) =>
  {

      if ( props.termStructure !== undefined &&
          props.termStructure[leg_name][elementName] !== undefined &&
          new_value !== undefined &&
          props.termStructure[leg_name][elementName] != new_value )
      {
        props.termStructure[leg_name][elementName] = new_value;

        var price_request_out = {};

        price_request_out["request_id"] = uuid();
        price_request_out["operation"] = "RE_PRICE_SWAP";
        price_request_out["business_date"] = props.curvePricingData.business_date;
        price_request_out["fixing_type"] =  props.curvePricingData.index.FixingType;
        price_request_out["swap"] = props.termStructure;

        price_request_out["index"] = helpers.get_index_object(price_request_out, props.curvePricingData.index);
        helpers.populate_curve_object(price_request_out, props.curvePricingData.index, props.curvePricingData.forward_curve)

        props.pricing_start();

        var results = PricerHelper.submit_request(price_request_out, (pricingResults) =>
        {
          if ( pricingResults["state"] == 2 )
          {
            props.Pricing_callback(pricingResults);
          }
        });
      };
  };

  useEffect(() => {

    if ( props.termStructure !== undefined &&
        Object.keys(props.termStructure.fixed_leg).length != 0 &&
        Object.keys(props.termStructure.floating_leg).length != 0 )
    {
      if (props.termStructure["operation"] === "PRICE_SWAP_AT_PAR" )
      {
        setFixedLeg(undefined);
        setFloatingLeg(undefined);
      }

      setFixedLeg(props.termStructure.fixed_leg);
      setFloatingLeg(props.termStructure.floating_leg);
    }

  },  [props.termStructure]);

  return (
      <Container>
      <Row>
        <Col>
          <SwapLegPanel name="Fixed Leg" leg_name='fixed_leg' schedule={fixedLeg} handleSetup={handleSetup}/>
        </Col>
        <Col>
          <SwapLegPanel name="Floating Leg" leg_name='floating_leg' schedule={floatingLeg} handleSetup={handleSetup}/>
        </Col>
      </Row>
     </Container>
    )
 });



export default SwapPanel;
