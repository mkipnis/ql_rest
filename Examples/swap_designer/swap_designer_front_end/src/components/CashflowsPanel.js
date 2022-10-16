/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import { useEffect, useState } from 'react';
import React from 'react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import {AgGridColumn, AgGridReact} from 'ag-grid-react';

const { forwardRef, useRef, useImperativeHandle } = React;

const CashflowsPanel = React.forwardRef ((props, ref) => {

  const cashflowsGridRef = React.useRef();

  const [cashflows, setCashflows] = useState();

  const amountFormatter = (param) => {

    return parseInt(param)
      .toString()
      .replace(/(\d)(?=(\d{3})+(?!\d))/g, '$1,');
    };

  const cashflowsGridColumnDefs = [
    { field: 'business_date', headerName: 'BusinessDate', sortable: true, width:'150px',cellStyle: {'textAlign': 'center', padding:'3px'}, },
    {
      headerName : 'FixedLeg',
      children: [
      { field: 'fixed_leg_coupon_start_date',  headerName: 'Start',   cellStyle: {'textAlign': 'right', padding:'3px'}, sortable: false, width:'100px' },
      { field: 'fixed_leg_coupon_end_date',  headerName: 'End', cellStyle: {'textAlign': 'right', padding:'3px'},  sortable: false, width:'100px' },
      { field: 'fixed_leg_count_amount',   headerName: 'Amount',  valueFormatter:function (params) {
            if (params.value !== undefined )
            {
              return amountFormatter(params.value);
            }
        }, cellStyle: {'textAlign': 'right', padding:'3px'}, sortable: false, width:'100px'},
    ]
    },
    {
      headerName : 'FloatingLeg',
      children: [
        { field: 'floating_leg_coupon_start_date', headerName: 'Start',  cellStyle: {'textAlign': 'right', padding:'3px'},  sortable: false, width:'100px', flex: 1  },
        { field: 'floating_leg_coupon_end_date',  headerName: 'End',  cellStyle: {'textAlign': 'right', padding:'3px'},  sortable: false, width:'100px', flex: 1  },
        { field: 'floating_leg_accrual_period',   cellStyle: {'textAlign': 'right', padding:'3px'},  sortable: false, hide: true, flex: 1  },
        { field: 'floating_leg_fixing_date',   cellStyle: {'textAlign': 'right', padding:'3px'},  sortable: false, hide: true, flex: 1  },
        { field: 'floating_leg_spread',   cellStyle: {'textAlign': 'right', padding:'3px'},  sortable: false, hide: true,flex: 1  },
        { field: 'floating_leg_amount', headerName: 'Amount',  valueFormatter:function (params) {
          if (params.value !== undefined )
          {
            return amountFormatter(params.value);
          }
        }, cellStyle: {'textAlign': 'right', padding:'3px'},  sortable: false, width:'100px', flex: 1 },
      ]
    }
  ];

  const cashflowsGridOptions = {

    getRowHeight: (params) => 25,
    //getRowHeight: (params) => 25
}

useEffect(() => {

  if ( props.results === undefined || props.results.fixed_leg.cashflows === undefined || props.results.floating_leg.cashflows === undefined )
    return;

  var cashflows = {};

  for (const [date, cashflow] of Object.entries(props.results.fixed_leg.cashflows))
  {
    if ( cashflows[date] === undefined )
    {
      cashflows[date] = {};
      cashflows[date]['business_date'] = date;
      cashflows[date]['fixed_leg_coupon_start_date'] = cashflow.accrual_start_date;
      cashflows[date]['fixed_leg_coupon_end_date'] = cashflow.date;
      cashflows[date]['fixed_leg_count_amount'] = cashflow.amount;
    }
  }

  for (const [date, cashflow] of Object.entries(props.results.floating_leg.cashflows))
  {
    if ( cashflows[date] === undefined )
      cashflows[date] = {};

    cashflows[date]['business_date'] = date;
    cashflows[date]['floating_leg_coupon_start_date'] = cashflow.accrual_start_date;
    cashflows[date]['floating_leg_coupon_end_date'] = cashflow.date;
    cashflows[date]['floating_leg_accrual_period'] = cashflow.accrual_period;
    cashflows[date]['floating_leg_fixing_date'] = cashflow.fixing_date;
    cashflows[date]['floating_leg_spread'] = cashflow.spread;
    cashflows[date]['floating_leg_amount'] = cashflow.amount;

  }

  var cashflows_out = Object.values(cashflows)
  setCashflows(cashflows_out);

  var defaultSortModel = [
    { colId: 'business_date', sort: 'asc', sortIndex: 0 }
  ];

  cashflowsGridRef.current.columnApi.applyColumnState({ state: defaultSortModel });

},  [props.results]);



   return (
      <div>
        <div className="App-Row">
            <div className="ag-theme-balham-dark" style={{verticalAlign:"top",height:"40vh", width: "100%", display: "inline-block", margin: "auto", padding:"10px"}}>
            <AgGridReact rowData={cashflows}
                        columnDefs={cashflowsGridColumnDefs}
                        gridOptions={cashflowsGridOptions}
                          ref={cashflowsGridRef}>
            </AgGridReact>
            </div>
          </div>
      </div>
     )
  });

export default CashflowsPanel;
