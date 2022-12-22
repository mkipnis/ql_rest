/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */


import { useEffect, useState } from 'react'

import 'bootstrap/dist/css/bootstrap.min.css'
import Container from 'react-bootstrap/Container'

import Row from 'react-bootstrap/Row'
import Col from 'react-bootstrap/Col'

import React from 'react'
import Select from 'react-select'

const { forwardRef, useRef, useImperativeHandle } = React;

const Dropdown = React.forwardRef ((props, ref) => {

    const [selectedOption, setSelectedOption] = useState();

     useEffect(()=>
     {
        if ( props.onValueChange !== undefined && selectedOption !== undefined )
        {
          props.onValueChange(props.elementName, selectedOption);
        }

     }, [selectedOption]);

     const customStyles = {
         control: (provided, state) => ({
           ...provided,
           borderColor: '#9e9e9e',
           backgroundColor: '#404040',
           minHeight: '20px',
           height: '30px',
           padding:'0px',
           fontSize:'15px',
           border: 0,
           boxShadow: false,
         }),

         option: provided => ({
           ...provided,
           backgroundColor: '#404040',
           color: 'white',
           fontSize:'15px',
           border:'3px',
           padding:'5px'
         }),

         valueContainer: (provided, state) => ({
           ...provided,
           height: '20px',
           color: 'white',
           padding: '2px 6px'
         }),

         singleValue: (provided, state) => ({
           ...provided,
           height: '25px',
           color: 'white',
           padding: '0 6px'
         }),


         Input: (provided, state) => ({
           ...provided,
           color: 'white',
           margin: '2px',
         }),
         indicatorSeparator: state => ({
           display: 'none',
         }),
         indicatorsContainer: (provided, state) => ({
           ...provided,
           height: '20px',
         })
       };

  return (
    <Container style={{marginTop:'5px'}}>
      <Row>
      <Col style={{textAlign: 'right'}}>
      <h6>
      {props.label}
      </h6>
      </Col>
      <Col>
      <Select
        options={props.options}
        value={props.selected_value}
        autosize={true}
        onChange={setSelectedOption}
        styles={customStyles}
        ref={ref}
      />
      </Col>
      </Row>
      </Container>
  )
  });

export default Dropdown;
