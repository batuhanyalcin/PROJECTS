import React from 'react';
import { StyleSheet, View, Button } from 'react-native';
import Plotly from 'react-native-plotly';
import { get95ellipse } from './ParameterCalculation';



export default function SpaghettiGraph(props)  {
    //Median Filter
    //const med_filtered_x = movingAverageFilter(props.x, 7);
    //const med_filtered_y = movingAverageFilter(props.y, 7);

    //Moving Average Filter
    //const movingAverageX = movingAverageFilter(props.x, 7);
    //const movingAverageY = movingAverageFilter(props.y, 7);
    
    //Low-pass + moving avarage filter
    //const com_filtered_x = combine_L_MovAvg_Filter(props.x,100);
    //const com_filtered_y = combine_L_MovAvg_Filter(props.y,100);


    //console.log(med_filtered_X);
    
    //const [x, setX] = React.useState([1, 2, -3, 4, 5])
    //const [y, setY] = React.useState([1, 2, 3, 4, -8])
    
    
    const data = {
        __id: 'up',
        x: props.x,
        y: props.y,
        type: 'scatter',
        marker: {
          color: 'rgb(54, 73, 153)',
        }
    };

    //var [elx, ely] = getEllipsePoints(data.x, data.y);
    var [elx, ely, area] = get95ellipse(data.x, data.y);

    var new_data = [{x: data.x, y: data.y, mode: "lines", name: 'Acceleration'}, {x: elx, y: ely, mode: "lines", name: '95 Ellipse'}];

    state = {
        data: [data],
        layout: { 
            title: {text: '<b>Spagetti Graph</b>',
                    font: {
                      family: 'Helvetica Neue',
                      weight: 'bold',
                      size: 15
                    }  
                  },
            xaxis: {fixedrange: true},
            yaxis: {fixedrange: true},
            paper_bgcolor: 'rgba(233,233,233, 0)',
            plot_bgcolor: 'rgba(200,100,10, 0)'
        }
    };

    
    return (
      <View style={styles.container}>
        <View style={styles.chartRow}>
          <Plotly
            data={new_data}
            layout={state.layout}
            config={{'displayModeBar': false}}
            style={{backgroundColor: 'rgba(0,0,0,0)'}}
          />
        </View>
      </View>
    );
  }


const styles = StyleSheet.create({
  buttonRow: {
    flexDirection: 'row',
    backgroundColor: 'rgba(0,0,0,0)'
  },
  chartRow: {
    flex: 1,
    width: '100%',
    backgroundColor: 'rgba(0,0,0,0)'
    
  },
  container: {
    paddingTop: 0,
    width: '100%',
    height: '100%',
    backgroundColor: 'rgba(155,155,155, 0)',
    alignItems: 'center',
    justifyContent: 'center'
    
  }
});


function getEllipsePoints(x, y){
  var cov = require( 'compute-covariance' );

  var centerX = x.reduce((a, b) => a + b, 0)/ x.length;
  var centerY = y.reduce((a, b) => a + b, 0)/ y.length;

  var cov_mat = cov( [x, y] );
  var a = cov_mat[0][0]; 
  var b = cov_mat[0][1];
  var c = cov_mat[1][1];

  var r1 = (a + c)/ 2 + Math.sqrt(Math.pow(((a - c) / 2), 2) + Math.pow(b, 2));
  var r2 = (a + c)/ 2 - Math.sqrt(Math.pow(((a - c) / 2), 2) + Math.pow(b, 2));
  var theta = Math.atan2(r1-a, b);
  var counter = 0;
  
  const elx = [];
  const ely = [];
  counter = 0;
  for (var i = 0; i < 360; i += 0.1){
    var angle = i/180*Math.PI;
    elx[counter] = centerX + Math.cos(theta) * r1 * Math.cos(angle) - Math.sin(theta) * r2 * Math.sin(angle);
    ely[counter] = centerY + Math.sin(theta) * r1 * Math.cos(angle) + Math.cos(theta) * r2 * Math.sin(angle);
    counter++;
  }

  return [elx, ely];
}

function medianFilter(x, w) {
  const y = [];
  const window = [];
  let i;
  for (i = 0; i < x.length; i++) {
    const xi = x[i];
    window.push(xi);
    if (window.length > w) {
      window.shift();
    }
    const sortedWindow = window.slice().sort((a, b) => a - b);
    const middleIndex = Math.floor(sortedWindow.length / 2);
    const median = sortedWindow[middleIndex];
    y.push(median);
  }
  return y;
};

function movingAverageFilter(x, w) {
  console.log("000")
  y = [];
  const hw = Math.floor(w/2)
  var avg = 0;
  console.log("111")
  for (let i = 0; i < hw; i++) {
    const sublist = x.slice(i, i+hw)
    avg = sublist.reduce((a, b) => a + b, 0) / sublist.length;
    y.push(avg)
  }
  console.log("222")
  
  for (let i = Math.floor(w/2); i < x.length - Math.floor(w/2); i++) {
    const sublist = x.slice(i - hw, i + hw + 1)
    avg = sublist.reduce((a, b) => a + b, 0) / sublist.length;
    y.push(avg)
  }
  console.log("333")

  for (let i = x.length - Math.floor(w/2); i < x.length; i++) {
    const sublist = x.slice(i - hw, i)
    avg = sublist.reduce((a, b) => a + b, 0) / sublist.length;
    y.push(avg)
  }
  console.log("444")
  return y
}


function combine_L_MovAvg_Filter(data,sampleRate) {
  // Define the cutoff frequency for the low-pass filter
  const cutoffFrequency = 20; // in Hz

  // Define the window size for the moving average filter
  const windowSize = 100; // in samples

  // Define the filter coefficients for the low-pass filter
  const RC = 1.0 / (2.0 * Math.PI * cutoffFrequency);
  const dt = 1.0 / sampleRate;
  const alpha = RC / (RC + dt);
  let lowPassFilteredData = [data[0]];
  for (let i = 1; i < data.length; i++) {
    const filteredValue = alpha * data[i] + (1 - alpha) * lowPassFilteredData[i - 1];
    lowPassFilteredData.push(filteredValue);
  }

  // Define the filter coefficients for the moving average filter
  const movingAverageFilter = new Array(windowSize).fill(1.0 / windowSize);
  let smoothedData = [];
  for (let i = 0; i < data.length - windowSize + 1; i++) {
    const windowData = data.slice(i, i + windowSize);
    const smoothedValue = movingAverageFilter.reduce((acc, val) => acc + val * windowData.shift(), 0);
    smoothedData.push(smoothedValue);
  }
  return smoothedData;
}