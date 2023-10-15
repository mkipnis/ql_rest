/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import DatePicker from "react-datepicker";

import 'bootstrap/dist/css/bootstrap.min.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

import React from 'react'
import { useEffect, useState } from 'react';
import { compareAsc, format, parseISO } from 'date-fns'

const BusinessDatePicker = React.forwardRef ((props, ref) => {

  const [selectedDate, setSelectedDate] = useState(0);

  useEffect(() =>
  {
    setSelectedDate(props.selected_date);
  }, [props.selected_date]);


  return (

    <div>
        <Row>
      <Col style={{textAlign: 'right', opacity: '0.7'}}>
        <div className="text-nowrap">
          {props.label}
        </div>
      </Col>

      <Col style={{display:'flex', alignItems: 'flex-start', justifyContent:'flex-start'}}>
        <DatePicker selected={selectedDate}
        onChange ={(date)=>{setSelectedDate(date);props.onValueChange(props.elementName, date);}}
        calendarClassName="red-border"
        />
      </Col>

      </Row>
      </div>
    )
 });

export default BusinessDatePicker;
