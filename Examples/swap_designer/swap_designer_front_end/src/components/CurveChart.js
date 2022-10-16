/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import React from 'react'
import { Line } from 'react-chartjs-2';
import { Chart as ChartJS } from 'chart.js/auto';
import { Chart }            from 'react-chartjs-2';
import { useEffect, useState } from 'react';

//import Helpers from './Helpers';


const { forwardRef, useRef, useImperativeHandle } = React;

const CurveChart = React.forwardRef ((swapCurve, ref) => {

  const chartRef = useRef();

  const [data, setData] = useState({
    labels: [],
    datasets: [],
  });

  useEffect(() => {

          var labels = [];
          var rates = [];
          var name = "N/A";

          var swap_curve = {}

          if ( swapCurve.swapCurve !== undefined) {

            //var curve_date = 0;
            name = swapCurve.swapIndex.Name;

            for (const [key, value] of Object.entries(swapCurve.swapCurve))
            {
              rates.push(value * 100.0);
              labels.push(key);
            }
          }

          //var name = swapCurve.swapCurve.swapIndex.Name;

          const char_data = {
            labels: labels,
            datasets: [ { data : rates,fill: true,
            backgroundColor: "rgba(75,192,192,0.2)",
            borderColor: "rgba(75,192,192,1)",
            label : name} ]
          };

          setData(char_data);

  }, [swapCurve]);


  return <Line data={data} ref={chartRef}/>;
});

export default CurveChart;
