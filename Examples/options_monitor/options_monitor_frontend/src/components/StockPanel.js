/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import { useEffect, useState } from 'react';
import React from 'react';
import PricerHelper from '../helpers/PricerHelper'
import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';
import { createContext, useCallback } from 'react';

const { forwardRef, useRef, useImperativeHandle } = React;

const StockPanel = React.forwardRef ((props, ref) => {

  const stockGridRef = React.useRef();
  const [stockPrices, setStockPrices] = useState([]);

  const stockGridColumnDefs = [
    {
      headerName : 'Stock Tickers',
      children: [
        { field: 'Symbol',  sortable: true, width:70, cellStyle: { 'fontWeight': 'bold' }},
        { field: 'Name',   sortable: true, width:175 },
        { field: 'Price',  sortable: false,width:30, flex:1,  cellStyle: {}, editable: true, cellStyle: { 'textAlign': 'right'},
          valueFormatter: params => {
                if (!isNaN(params.value))
                 return parseFloat(params.value).toFixed(2);
              } },
        { field: 'dividendRate', headerName:'Dividend',  sortable: false,width:30, flex:1,  cellStyle: {}, editable: true, cellStyle: { 'textAlign': 'right'},

                cellRenderer: props => {
                  if (!isNaN(props.value))
                  {
                    return `${(props.value)}`;
                  };
                },

                valueFormatter: params => {
                      if (!isNaN(params.value))
                       return parseFloat(params.value).toFixed(2);
                    } },


      ]}
  ];

  useEffect(() => {

  if ( props.symbols === undefined )
    return;

    setStockPrices(props.symbols);

    if ( stockGridRef.current.api != null )
    {
      stockGridRef.current.api.refreshCells();

      stockGridRef.current.api.forEachNode( node=> {
      if ( node.data.Symbol === props.defaultTicker ) {
        node.setSelected(true);
        }
      })
    }

},  [props.symbols]);


  const stockGridOptions = {

    rowSelection: 'single',
    getRowHeight: (params) => 25,

    onRowSelected(event) {

      if ( !event.node.selected )
        return;

      props.getVols(event.data);
    },


    onGridReady: event => {

      props.gridReadyCallback();

    }
  }

  const onCellEditingStopped = useCallback((updated_price) =>
  {
      props.priceChangeCallback( updated_price );
  }, []);

   return (
      <div>
        <div className="App-Row">
            <div className="ag-theme-balham-dark" style={{verticalAlign:"top",height:"40vh", width: "100%", display: "inline-block", margin: "auto", padding:"5px"}}>
            <AgGridReact
                      rowData={stockPrices}
                      columnDefs={stockGridColumnDefs}
                      gridOptions={stockGridOptions}
                      onCellEditingStopped={onCellEditingStopped}
                      ref={stockGridRef}>
            </AgGridReact>
            </div>
          </div>
      </div>
     )
  });

export default StockPanel;
