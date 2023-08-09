import React, { useState, useEffect, useRef } from 'react';
import { StyleSheet, Text, TouchableOpacity, View} from 'react-native';
import * as Speech from 'expo-speech'
import { DeviceMotion } from 'expo-sensors';
import { CountdownCircleTimer, useCountdown } from 'react-native-countdown-circle-timer';
import Svg, { Path, LinearGradient, Stop, Defs } from 'react-native-svg';
import TestStartButton from '../components/TestStartButton';

//import Button from '../components/Button'
let accX = [];
let accY = [];
let accZ = [];
let milliseconds = [];




export default function BalanceTestScreen({navigation,route}) {
  const [data, setData] = useState({});
  

  //const [graphComp, setGraphComp] = useState(<View></View>)
  //const [subscription, setSubscription] = useState(null);
  
 
  const [buttonDisabled, setButtonDisabled] = useState(false);
  const dt = 0.01 // seconds
  const totalTime = 20 // seconds
  const numOfIterations = Math.floor(totalTime / dt)
  var i = 0
  const [isRemainingTimeSpeaking, setIsRemainingTimeSpeaking] = useState(false)

  const [time, setTime] = useState(3)
  const timerRef = useRef(time)
  DeviceMotion.setUpdateInterval(dt * 1000)

  const [testEnded, setTestEnded] = useState(false)
  
  const [isPlaying, setIsPlaying] = React.useState(false)
  const [timePassed, setTimePassed] = React.useState(0.0)
  const duration = totalTime
  const {
    path,
    pathLength,
    stroke,
    strokeDashoffset,
    remainingTime,
    elapsedTime,
    size,
    strokeWidth,
  } = useCountdown({ isPlaying: isPlaying, duration, colors: 'url(#your-unique-id)'})

  const _subscribe = () => {
    DeviceMotion.addListener((devicemotionData) => {
      setData(devicemotionData.acceleration);
      accX.push(devicemotionData.acceleration.x)
      accY.push(devicemotionData.acceleration.y)
      accZ.push(devicemotionData.acceleration.z)
      milliseconds.push(Date.now())
    });
    DeviceMotion.setUpdateInterval(dt * 1000)
  };
  
  function startTest() {
    setButtonDisabled(true)
    setTime(3)
    timerRef.current = 3
    
    Speech.speak("Starting the test. Please stay still.", 
    { language: 'en', 
      onDone: () => {
        Speech.speak(timerRef.current.toString(), {language: 'en'})
        const timerId = setInterval(() => {
          timerRef.current -= 1;
          if (timerRef.current < 0) {
            clearInterval(timerId);
          }
          else {
            if (timerRef.current != 0) {
              Speech.speak(timerRef.current.toString(), {language: 'en'})
            }
            else {
              setTestState(TestState.TestOngoing)
              setButtonDisabled(false)
              Speech.speak("The test has started.", {language: 'en'})
              i = 0;
              setIsPlaying(true);
              _subscribe();

            }
            setTime(timerRef.current);
          }
          return () => clearInterval(timerId);
        }, 1000)
      }
    })
  }

  function pauseTest() {
    setTestState(TestState.TestPaused)
    setIsPlaying(false);
    _unsubscribe();
    setTestEnded(true)
    console.log(`Time passed is: ${timePassed}`)
  }

  function onTestEnded() {
    _unsubscribe();
    //setGraphComp(<SpaghettiGraph x={accX} y={accZ}/>)
    setTestEnded(true)

    navigation.replace('TestResult', {accX: accX, accZ: accZ, duration: totalTime, milliseconds: milliseconds})
  }

  const _unsubscribe = () => {
    DeviceMotion.removeAllListeners();
  };

  useEffect(() => {
    return () => {
      DeviceMotion.removeAllListeners();
      Speech.stop();
      accX = [];
      accY = [];
      accZ = [];
      milliseconds = [];

     
    }
  }, []
  )

  const TestState = {
    TestNotStarted: {buttonText: "Start Test", buttonColor: ['#090979', '#00d4ff'], buttonFunction: startTest},
    TestOngoing: {buttonText: "Pause Test", buttonColor: ['#833ab4', '#458cfc'], buttonFunction: pauseTest },
    TestPaused: {buttonText: "See Results", buttonColor: ['#187a43', '#0e98ad'], buttonFunction: onTestEnded },
  }
  
  const [testState, setTestState] = useState(TestState.TestNotStarted)

 
  
  return (
    <View style={styles.container}>
      <View style={{ width: size, height: size, position: 'relative' }}>
        <CountdownCircleTimer
        strokeWidth={10}
        isPlaying={isPlaying}
        duration={totalTime}
        colors={["#090979", "#36669c", "#41a0ae", "#3ec995", "#74C365"]}
        colorsTime={[20, 15, 10, 5, 0]}
        onComplete={onTestEnded}
        elapsedTime={elapsedTime}
        >
        {({ remainingTime, color }) => {

          if (remainingTime == 5 && !isRemainingTimeSpeaking) {
            console.log(isRemainingTimeSpeaking)
            setIsRemainingTimeSpeaking(true)
            console.log(isRemainingTimeSpeaking)
            Speech.speak("5 seconds remaining.", {language: 'en'})
          }
          return (
          <Text style={{ color, fontSize: 40, fontWeight: 'bold' }}>
            {remainingTime}
          </Text>
          )
          }
        }
        </CountdownCircleTimer>
      </View>
      
      <TestStartButton title={testState.buttonText} onPress={testState.buttonFunction} disabled={buttonDisabled} colors={buttonDisabled ? ['#808080', '#c5c5c5'] : testState.buttonColor} />
    </View>
  );
}

/*
<View style={{flexDirection: 'row', justifyContent: 'space-evenly', flexWrap: 'wrap', alignItems: 'center'}}>
        <DonutChart />
        
      </View>
*/

/*<CountdownCircleTimer
        isPlaying={isPlaying}
        duration={totalTime}
        colors={["#004777", "#F7B801", "#A30000", "#A30000"]}
        onComplete={() => {}}
    >
      {({ remainingTime, color }) => (
        <Text style={{ color, fontSize: 40, fontWeight: 700 }}>
          {remainingTime}
        </Text>
      )}
    </CountdownCircleTimer>*/
const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  time: {
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    position: 'absolute',
    left: 0,
    top: 0,
    width: '100%',
    height: '100%'
  }
});
