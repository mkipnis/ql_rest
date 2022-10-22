/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import { useEffect, useState } from 'react';

import uuid from 'react-uuid'
import React, { createContext, useCallback } from 'react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import {AgGridColumn, AgGridReact} from 'ag-grid-react';

import { SwapIndexes, SwapIndexTenors } from '../data/SwapIndexes'
import { MarketData } from '../data/MarketData'

import helpers from './helpers'
import PricerHelper from './PricerHelper'

const { forwardRef, useRef, useImperativeHandle } = React;

const MarketDataPanel = React.forwardRef ((props, ref) => {

  const marketDataGridRef = React.useRef();
  const indexRef = React.useRef();

  const [forwardCurve, setForwardCurve] = useState();

  const marketDataGridColumnDefs = [
    {
    getRowHeight: (params) => 25,
    headerName : 'Market Data - Forward Curve',
    children: [
      { field: 'ticker', name: 'Ticker', cellStyle: {'textAlign': 'left', padding:'3px'}, width: 'auto',  sortable: true },
      { field: 'Tenor',   cellStyle: {'textAlign': 'right', padding:'3px'}, flex: 1,  sortable: false },
      { field: 'rate',   cellStyle: {'textAlign': 'right', padding:'3px'},  flex: 1,  sortable: false,  editable: true}
    ]}
  ];


  useImperativeHandle(ref, () => ({
    reset_selected_tenor(tenor_in) {

      marketDataGridRef.current.api.forEachNode( node=> {
      if ( node.data.Tenor === tenor_in ) {
          node.setSelected(true);
        }
      })
    }
  }));



  const marketDataGridOptions = {

    getRowHeight: (params) => 25,
    rowSelection: 'single',
    onRowSelected(event) {

      if ( !event.node.selected )
        return;

        props.setup.tenorReset(event.data.Tenor);
    },

  }

  const reprice_curve = ( business_date, swap_index, market_data ) =>
  {

    if ( swap_index ===undefined || market_data === undefined )
      return

    var price_request_out = {};

    price_request_out["request_id"] = uuid();
    price_request_out["operation"] = "PRICE_CURVE";
    price_request_out["business_date"] = business_date;
    price_request_out["fixing_type"] = swap_index.FixingType;

    price_request_out["index"] = helpers.get_index_object(price_request_out, swap_index );
    helpers.populate_curve_object(price_request_out, swap_index, market_data);


    var curve_tenor_array = [];
    curve_tenor_array.push("1M");
    curve_tenor_array.push("3M");
    curve_tenor_array.push("6M");
    curve_tenor_array.push("9M");
    for (var tenor = 1; tenor <= 30; tenor++)
    {
      var swap_curve_venor = tenor + "Y";
      curve_tenor_array.push(swap_curve_venor);
    }

    price_request_out["swap_curve_point_request"] = curve_tenor_array;

    props.setup.pricing_start();

    var results = PricerHelper.submit_request(price_request_out, (pricingResults) =>
    {
      props.setup.curve_price_callback(pricingResults);
    });
  };


  useEffect(() => {


      if ( props.setup.pricingData.curvePricingData === undefined )
        return;

      indexRef.current = props.setup.pricingData.curvePricingData;

      setForwardCurve(indexRef.current.forward_curve);

      reprice_curve(
                    indexRef.current.business_date,
                    indexRef.current.index,
                    indexRef.current.forward_curve
                  );


    }, [ props.setup.pricingData.curvePricingData ] );



    const onCellEditingStopped = useCallback((event) =>
    {
        reprice_curve(indexRef.current.business_date, indexRef.current.index, marketDataGridRef.current.props.rowData);
    }, []);


   return (
      <div>
        <div className="App-Row">
            <div className="ag-theme-balham-dark" style={{verticalAlign:"top",height:"40vh", width: "100%", display: "inline-block",
              margin: "auto", padding:"10px"}}>
            <AgGridReact
                        rowData={forwardCurve}
                        columnDefs={marketDataGridColumnDefs}
                        gridOptions={marketDataGridOptions}
                        onCellEditingStopped={onCellEditingStopped}
                        ref={marketDataGridRef}>
            </AgGridReact>
            </div>
          </div>
      </div>
     )
  });

export default MarketDataPanel;
