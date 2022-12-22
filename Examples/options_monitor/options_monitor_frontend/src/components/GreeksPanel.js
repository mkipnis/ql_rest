/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import { useEffect, useState } from 'react';
import React from 'react';
import PricerHelper from '../helpers/PricerHelper'

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import {AgGridColumn, AgGridReact} from 'ag-grid-react';

const { forwardRef, useRef, useImperativeHandle } = React;

const GreeksPanel = React.forwardRef ((props, ref) => {

  const greeksGridRef = React.useRef();


  const numberFormatter = (params) => {
      if (!isNaN(params.value)) return parseFloat(params.value).toFixed(2);
  };

  function compareCall(params) {
    if (params.data.Strike <= props.marketPrice.Price ){
      return {'textAlign': 'right', padding:'0px', color: 'rgba(75,192,192,1)'};
    } else {
      return {'textAlign': 'right', padding:'0px', color: 'rgba(75,192,192,0.7)'};
    }
  }

  function comparePut(params) {
    if (params.data.Strike >= props.marketPrice.Price ){
      return {'textAlign': 'right', padding:'0px', color: 'rgba(255, 99, 132,1)'};
    } else {
      return {'textAlign': 'right', padding:'0px', color: 'rgba(255, 99, 132,0.7)'};
    }
  }

  function compareCallGreeks(params) {
    if (params.data.Strike <= props.marketPrice.Price ){
      return {'textAlign': 'right', padding:'0px', opacity: '1'};
    } else {
      return {'textAlign': 'right', padding:'0px', opacity: '0.7'};
    }
  }

  function comparePutGreeks(params) {
    if (params.data.Strike >= props.marketPrice.Price ){
      return {'textAlign': 'right', padding:'0px', opacity: '1'};
    } else {
      return {'textAlign': 'right', padding:'0px', opacity: '0.7'};
    }
  }



  const greeksGridColumnDefs = [
    {field: 'Strike', headerName: 'Strike',  sortable: true, flex: 1, cellStyle: {'textAlign': 'right'} },
    {
    headerName : 'Calls',
    children: [
    { field: 'call_vol', cellStyle: compareCall, headerName:'Volatility', sortable: true, sortable: true, flex: 1,
            cellRenderer: props => {
                    if (!isNaN(props.value) && !isNaN(props.data.call_npv)) return `${(props.value * 100)|0}%`;},  sortable: true, flex: 1},
    { field: 'call_npv', headerName:'Price' ,  sortable: true, flex: 1, cellStyle: compareCall, valueFormatter: numberFormatter},
    { field: 'call_delta', headerName:'Delta' ,  sortable: true, flex: 1, cellStyle: compareCallGreeks, valueFormatter: numberFormatter},
    { field: 'call_gamma', headerName:'Gamma' ,  sortable: true, flex: 1,  cellStyle: compareCallGreeks, valueFormatter: numberFormatter},
    { field: 'call_vega',  headerName:'Vega' ,  sortable: true, flex: 1, cellStyle: compareCallGreeks, valueFormatter: numberFormatter},
    { field: 'call_theta', headerName:'Theta' ,  sortable: true, flex: 1, cellStyle: compareCallGreeks, valueFormatter: numberFormatter},
    { field: 'call_rho', headerName:'Rho' ,  sortable: true, flex: 1, cellStyle: compareCallGreeks, valueFormatter: numberFormatter},
  ]
},
  {headerName : 'Puts', style:{'align':'center'},
  children: [
    { field: 'put_vol', headerName:'Volatility', cellStyle: comparePut,  sortable: true, flex: 1, cellRenderer: props => {
                    if (!isNaN(props.value) && !isNaN(props.data.put_npv)) return `${(props.value * 100)|0}%`;}},
    { field: 'put_npv', headerName:'Price' ,  sortable: true, flex: 1, cellStyle: comparePut, valueFormatter: numberFormatter},
    { field: 'put_delta', headerName:'Delta' ,  sortable: true, flex: 1, cellStyle: comparePutGreeks, valueFormatter: numberFormatter},
    { field: 'put_gamma', headerName:'Gamma' ,  sortable: true, flex: 1, cellStyle: comparePutGreeks, valueFormatter: numberFormatter},
    { field: 'put_vega', headerName:'Vega' ,  sortable: true, flex: 1, cellStyle: comparePutGreeks, valueFormatter: numberFormatter},
    { field: 'put_theta', headerName:'Theta' ,  sortable: true, flex: 1, cellStyle: comparePutGreeks, valueFormatter: numberFormatter},
    { field: 'put_rho', headerName:'Rho' ,  sortable: true, flex: 1, cellStyle: comparePutGreeks, valueFormatter: numberFormatter},
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
            var updated_row = greeksGridRef.current.api.getDisplayedRowAtIndex(node.rowIndex);
            greeksGridRef.current.api.flashCells({ rowNodes: [updated_row] });

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
  },

  flash_stike(strike) {

      var selected_node = undefined;

      greeksGridRef.current.api.forEachNode(node =>
      {
        if ( selected_node === undefined && node.data.Strike == strike.data.Strike )
        {

          selected_node = node;
          node.setSelected(true);
          greeksGridRef.current.api.ensureIndexVisible(node.rowIndex);
          var updated_row = greeksGridRef.current.api.getDisplayedRowAtIndex(node.rowIndex);
          greeksGridRef.current.api.flashCells({ rowNodes: [updated_row] });

          return true;
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
