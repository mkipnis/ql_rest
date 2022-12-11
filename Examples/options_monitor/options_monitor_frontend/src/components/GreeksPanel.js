/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import { useEffect, useState } from 'react';
import React from 'react';
import PricerHelper from '../helpers/PricerHelper'
//import { SwapIndexes } from '../data/SwapIndexes'

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import {AgGridColumn, AgGridReact} from 'ag-grid-react';

const { forwardRef, useRef, useImperativeHandle } = React;

const GreeksPanel = React.forwardRef ((props, ref) => {

  const greeksGridRef = React.useRef();


  const numberFormatter = (params) => {
      if (!isNaN(params.value)) return parseFloat(params.value).toFixed(2);
  };



  const greeksGridColumnDefs = [
    {field: 'Strike', headerName: 'Strike',  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'} },
    {
    headerName : 'Calls',
    children: [
    { field: 'call_vol', cellStyle: {'textAlign': 'right'}, headerName:'Volatility', sortable: true, sortable: true, flex: 1,
            cellRenderer: props => {
                    if (!isNaN(props.value)) return `${(props.value * 100)|0}%`;},  sortable: true, flex: 1},
    { field: 'call_npv', headerName:'Price' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'call_delta', headerName:'Delta' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'call_gamma', headerName:'Gamma' ,  sortable: true, flex: 1,  cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'call_vega',  headerName:'Vega' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'call_theta', headerName:'Theta' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'call_rho', headerName:'Rho' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
  ]
},
  {headerName : 'Puts', style:{'align':'center'},
  children: [
    { field: 'put_vol', headerName:'Volatility', cellStyle: {'textAlign': 'right'},  sortable: true, flex: 1, cellRenderer: props => {
                    if (!isNaN(props.value)) return `${(props.value * 100)|0}%`;}},
    { field: 'put_npv', headerName:'Price' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'put_delta', headerName:'Delta' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'put_gamma', headerName:'Gamma' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'put_vega', headerName:'Vega' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'put_theta', headerName:'Theta' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
    { field: 'put_rho', headerName:'Rho' ,  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'}, valueFormatter: numberFormatter},
]
  }
  ];



  const greeksGridOptions = {

    rowSelection: 'single',

    getRowHeight: (params) => 25,

  }

  useEffect(() => {

    if ( props.selectedStrike != null )
    {
      greeksGridRef.current.api.ensureIndexVisible(props.selectedStrike.rowIndex);
      greeksGridRef.current.api.forEachNode( node=> {
      if ( node.rowIndex === props.selectedStrike.rowIndex ) {
        node.setSelected(true);
        }
      })
    }


  }, [props.selectedStrike]);


  useEffect(() => {

    if ( props.volData != undefined )
    {
      var selected_node = undefined;

      greeksGridRef.current.api.forEachNode( node=>
        {
          if ( selected_node === undefined && parseFloat(node.data.Strike) >= parseFloat(props.workingPrice) )
          {
            selected_node = node;
            node.setSelected(true);
            greeksGridRef.current.api.ensureIndexVisible(node.rowIndex);

            return true;
          }
      })
    }

  }, [props.workingPrice, props.volData]);

  useImperativeHandle(ref, () => ({
    update_greeks(greeks) {

        greeksGridRef.current.api.forEachNode(node =>
        {
          if (node.data == null )
            return;

          var strike = node.data.Strike;

          var strike_greeks = greeks[strike];

          if ( strike_greeks != undefined )
          {
            node.setData(strike_greeks);
          }
      }
      );
  }
}));


   return (
      <div>
        <div className="App-Row">
            <div className="ag-theme-balham-dark" style={{verticalAlign:"top",height:"40vh", width: "100%", display: "inline-block", margin: "auto", padding:"10px"}}>
            <AgGridReact
                      rowData={props.volData}
                      columnDefs={greeksGridColumnDefs}
                      gridOptions={greeksGridOptions}
                      ref={greeksGridRef}>
            </AgGridReact>
            </div>
          </div>
      </div>
     )
  });

export default GreeksPanel;
