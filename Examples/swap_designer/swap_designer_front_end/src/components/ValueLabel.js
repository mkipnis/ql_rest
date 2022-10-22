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

const ValueLabel = React.forwardRef ((props) => {

  function eLocal(x){

    let n = Number(x);
    let str = n.toLocaleString("en-US");
    console.log(str);

    return str;
  }


  return (
    <Container style={{margin:'5px'}}>
      <Row>
        <Col xs={6} style={{textAlign: 'right', whiteSpace: 'nowrap'}}>
          {props.label}
        </Col>
      <Col xs={6}>
        <Row>
          {eLocal(props.value)}
        </Row>
      </Col>
      </Row>
      </Container>
    )
 });
export default ValueLabel;
