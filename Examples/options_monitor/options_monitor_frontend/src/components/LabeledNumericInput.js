/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */
import 'bootstrap/dist/css/bootstrap.min.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

import React from 'react'
import { useEffect, useState, useRef } from 'react';

const LabeledNumericInput = React.forwardRef ((props, ref) => {

  const [currentValue, setCurrentValue] = useState(0);

    var input_ref = useRef();

    useEffect(() =>
    {
      setCurrentValue(props.value);
    }, [props.value]);

  return (
    <Container>
      <Row>
        <Col xs={6} style={{textAlign: 'right', opacity: '0.7'}}>
          {props.label}
        </Col>
      <Col xs={6}>


              <input type="number" value={currentValue} style={{width: '100px', paddingLeft:'5px'} } step={props.step}
                      onChange={(e)=> {
                                    setCurrentValue(e.target.value);
                                    props.onChange(props.elementName, currentValue);
                        }}

                        onKeyDown={(e)=>
                                  {
                                    if(e.key === 'Enter' || e.key === 'Tab') {
                                        props.onChanged(props.elementName, currentValue);
                                      }
                                  }}

                      ref = {input_ref}
              />


      </Col>
      </Row>
      </Container>
    )
 });
export default LabeledNumericInput;
