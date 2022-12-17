/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import React from 'react'
import { Line } from 'react-chartjs-2';
import { Chart as ChartJS } from 'chart.js/auto';
import { Chart }            from 'react-chartjs-2';
import { useEffect, useState } from 'react';


const { forwardRef, useRef, useImperativeHandle } = React;

const SmilePanel = React.forwardRef ((props, ref) => {

  const chartRef = useRef();

  const [data, setData] = useState({
    labels: [],
    datasets: [],
  });

  useEffect(() => {

          var strikes = [];
          var put_vols = [];
          var call_vols = [];
          var name = "N/A";

          if ( props != undefined && props.volData != undefined ) {

            for (const vol of props.volData)
            {
              put_vols.push(vol.put_vol)
              call_vols.push(vol.call_vol)
              strikes.push(vol.Strike)
            }

          }

          const char_data = {
            labels: strikes,
            datasets: [
              {
                data : call_vols,
                fill: true,
                backgroundColor: 'rgba(75,192,192,0.2)',
                borderColor: 'rgba(75,192,192,1)',
                label : 'Calls'
              },
              {
                data : put_vols,
                fill: true,
                backgroundColor: 'rgba(255, 99, 132,0.2)',
                borderColor: 'rgb(255, 99, 132)',
                label : 'Puts'
              }
          ]
          };

          setData(char_data);

  }, [props.volData]);


  return <Line data={data}
  options={{
         onClick: function(evt, element) {
           if(element.length > 0)
           {
             var strike = props.volData[element[0].index]
             props.strikeSelectedCallback(strike);
            }
         }}}
  ref={chartRef}/>;
});

export default SmilePanel;
