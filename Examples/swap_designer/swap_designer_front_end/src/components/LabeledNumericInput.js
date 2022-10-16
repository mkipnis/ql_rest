/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import Select from 'react-select';

import { useEffect, useState } from 'react';

import 'bootstrap/dist/css/bootstrap.min.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

import React from 'react'

import NumberFormat from "react-number-format";

const { forwardRef, useRef, useImperativeHandle } = React;

const LabeledNumericInput = React.forwardRef ((props, ref) => {

    const [currentValue, setCurrentValue] = useState(0);

    var input_ref = useRef();

    useEffect(() =>
    {
      setCurrentValue(props.value);
    }, [props.value]);

/*
    useEffect(() => {

        if ( props.onChange!== undefined && currentValue != undefined && props.value !== currentValue )
        {
            props.onChange(props.elementName, currentValue);
        }

    }, [currentValue]);
*/
  return (
    <Container style={{margin:'5px'}}>
      <Row>
        <Col xs={6} style={{textAlign: 'right'}}>
          {props.label}
        </Col>
      <Col xs={6}>

      {

        props.type !== undefined && props.type === "notional" ?

          <NumberFormat value={currentValue} style={{width: '100px'}} thousandSeparator={true}
                onValueChange={(e)=> { setCurrentValue(e.floatValue); }}

                onKeyDown={(e)=> {
                            if(e.key === 'Enter' || e.key === 'Tab')
                            {
                              props.onChange(props.elementName, currentValue);
                            }
                          }}

                onInput={ (event) => {} }
                />

                        :

              <input type="number" value={currentValue} style={{width: '100px'} } step={props.step}
                      onChange={(e)=> {
                                    setCurrentValue(e.target.value);
                        }}

                        onKeyDown={(e)=>
                                  {
                                    if(e.key === 'Enter' || e.key === 'Tab') {
                                        props.onChange(props.elementName, currentValue);
                                      }
                                  }}

                      ref = {input_ref}

              />
        }

      </Col>
      </Row>
      </Container>
    )
 });
export default LabeledNumericInput;
