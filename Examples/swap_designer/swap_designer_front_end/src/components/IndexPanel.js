/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import { useEffect, useState } from 'react';
import React from 'react';
import { SwapIndexes } from '../data/SwapIndexes'

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import {AgGridColumn, AgGridReact} from 'ag-grid-react';

const { forwardRef, useRef, useImperativeHandle } = React;

const IndexPanel = React.forwardRef ((props, ref) => {

  const indexGridRef = React.useRef();


  const indexGridColumnDefs = [
    {
    headerName : 'Interest Rate Index',
    children: [
    { field: 'Currency',  sortable: true, width:80, cellStyle: { 'padding-top': 3 }},
    { field: 'Index',   sortable: true, width:70, cellStyle: { 'padding-top': 3 } },
    { field: 'Tenor',  sortable: false, width:70 , cellStyle: { 'padding-top': 3 }},
    { field: 'FixingType',  sortable: false, flex:1 , cellStyle: { 'padding-top': 3 }},
  ]}
  ];

  const indexGridOptions = {

    rowSelection: 'single',

    getRowHeight: (params) => 25,

    onRowSelected(event) {

      if ( !event.node.selected )
        return;

      props.indexChangeCallback(event.data);
    },


    onGridReady: event => {
      /* setGridApi(params.api);
      params.api.setRowData(orderData);//this is null which i want not null*/
      indexGridRef.current.api.setRowData(SwapIndexes);
      //Report_date_change();
      indexGridRef.current.api.forEachNode( node=> {
      if ( node.rowIndex === 0 ) {
        // Master open detail.  You could also call node.setSelected( true ); for alternate design.
        node.setSelected(true);
        //setIndex(node.data);
        // props.stockPickChangeCallback( node.data.id_key );
        }
      })
    }
  }

  /*
  useEffect(() => {

        // props.indexChangeCallback(index);

    }, [index]);
*/

   return (
      <div>
        <div className="App-Row">
            <div className="ag-theme-balham-dark" style={{verticalAlign:"top",height:"40vh", width: "100%", display: "inline-block", margin: "auto", padding:"10px"}}>
            <AgGridReact
                      //rowData={Indexes}
                      columnDefs={indexGridColumnDefs}
                      gridOptions={indexGridOptions}
                      ref={indexGridRef}>
            </AgGridReact>
            </div>
          </div>
      </div>
     )
  });

export default IndexPanel;
