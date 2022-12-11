/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import { useEffect, useState } from 'react';

import uuid from 'react-uuid'
import React, { createContext, useCallback } from 'react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import {AgGridColumn, AgGridReact} from 'ag-grid-react';

const { forwardRef, useRef, useImperativeHandle } = React;

const VolPanel = React.forwardRef ((props, ref) => {

  const marketDataGridRef = React.useRef();
  const indexRef = React.useRef();

  const marketDataGridColumnDefs = [
    {
    getRowHeight: (params) => 25,
    headerName : 'Market Data - Vols',
    children: [
      { field: 'Strike', cellStyle: {'textAlign': 'right', padding:'3px'},  flex: 1,  sortable: false,  editable: true },
      { field: 'call_vol', headerName:'Call Volatility',  cellStyle: {'textAlign': 'right', padding:'3px'},  flex: 1,  sortable: false,  editable: true,
        cellRenderer: props => {
          if (!isNaN(props.value)) return `${(props.value * 100)|0}%`;
        }},
      { field: 'put_vol', headerName:'Put Volatility',  cellStyle: {'textAlign': 'right', padding:'3px'},  flex: 1,  sortable: false,  editable: true,
        cellRenderer: props => {
                if (!isNaN(props.value)) return `${(props.value * 100)|0}%`;
      }}
    ]}
  ];

  const marketDataGridOptions = {

    getRowHeight: (params) => 25,
    rowSelection: 'single',
    suppressScrollOnNewData: true,

    onRowClicked(event) {
      props.strikeSelectedCallback(event);
    },

    deltaRowDataMode: true,
    getRowNodeId: function (data) {
      return data.Strike;
    },
  }

  const onCellEditingStopped = useCallback((updated_vol) =>
  {
      props.volChangeCallback( updated_vol );
  }, []);


  useEffect(() => {

    if ( props.volData != undefined )
    {
      console.log("Ticker : " + props.workingPrice );

      var selected_node = undefined;

      marketDataGridRef.current.api.forEachNode( node=>
        {
          if ( selected_node === undefined && node.data.Strike >= props.workingPrice )
          {
            selected_node = node;
            node.setSelected(true);
            marketDataGridRef.current.api.ensureIndexVisible(node.rowIndex);

            return true;
          }
      })
    }

  }, [props.workingPrice, props.volData]);

   return (
      <div>
        <div className="App-Row">
            <div className="ag-theme-balham-dark" style={{verticalAlign:"top",height:"40vh", width: "100%", display: "inline-block", margin: "auto", padding:"5px"}}>
            <AgGridReact
                        rowData={props.volData}
                        columnDefs={marketDataGridColumnDefs}
                        gridOptions={marketDataGridOptions}
                        onCellEditingStopped={onCellEditingStopped}
                        onCellEditingStarted ={ (event) => {
                        } }

                        ref={marketDataGridRef}>
            </AgGridReact>
            </div>
          </div>
      </div>
     )
  });

export default VolPanel;
