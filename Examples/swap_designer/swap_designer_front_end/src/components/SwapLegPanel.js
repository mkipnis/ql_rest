/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import React from 'react'
import { DayCounter, BusinessDateConventions, Frequency, DateGen } from '../data/Constants'
import { useEffect, useState } from 'react';
import Dropdown from "./Dropdown";
import BusinessDatePicker from "./BusinessDatePicker";
import { compareAsc, format, parseISO } from 'date-fns'

import 'bootstrap/dist/css/bootstrap.min.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';


const SwapLegPanel = React.forwardRef ((props, ref) => {


  const [startDate, setStartDate] = useState();
  const [endDate, setEndDate] = useState();

  const [dayCounter, setDayCounter] = useState();
  const [busDayConv, setBusDayConv] = useState();
  const [frequency, setFrequency] = useState();
  const [dateGen, setDateGen] = useState();

  const handleSetup = (elementName, value) =>
  {
    if ( props.handleSetup !== undefined && props.schedule !== undefined )
      props.handleSetup(props.leg_name, elementName, value);
  };

  useEffect(() => {

    if ( props.schedule !== undefined && Object.keys(props.schedule).length > 0)
    {
      setStartDate( parseISO(props.schedule.start) );
      setEndDate( parseISO(props.schedule.end) );

      setDayCounter({ label:props.schedule.day_counter, value:props.schedule.day_counter});
      setBusDayConv({ label:props.schedule.bdc, value:props.schedule.bdc});
      setFrequency({ label:props.schedule.frequency, value:props.schedule.frequency});
      setDateGen({ label:props.schedule.gen_rule, value:props.schedule.gen_rule});
    }

  },  [props]);

  return (
     <Container>
        <Row>
          <Col style={{fontSize: '18px', textAlign:'center', margin:'5px'}}>
            {props.name}
          </Col>
        </Row>
       <Row  style={{marginBottom:'10px'}}>
        <Col style={{margin:'10px'}}>
            <BusinessDatePicker label='Start Date' elementName='start' selected_date={startDate} onValueChange={handleSetup}/>
            <BusinessDatePicker label='End Date' elementName='end' selected_date={endDate} onValueChange={handleSetup}/>
            <Dropdown label='DayCounter' elementName='day_counter' options={DayCounter} selected_value={dayCounter} onValueChange={handleSetup}/>
            <Dropdown label='Bus.Day.Conv.' elementName='bdc' options={BusinessDateConventions} selected_value={busDayConv} onValueChange={handleSetup}/>
            <Dropdown label='Frequency' elementName='frequency' options={Frequency} selected_value={frequency} onValueChange={handleSetup}/>
            <Dropdown label='DateGen' elementName='gen_rule' options={DateGen} selected_value={dateGen} onValueChange={handleSetup}/>
        </Col>
         </Row>
      </Container>
    )
 });



export default SwapLegPanel;
