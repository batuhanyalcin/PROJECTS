import { SafeAreaView, StyleSheet, Text, Image, View, ScrollView, Button, FlatList, Touchable, TouchableOpacity } from 'react-native'
import React from 'react'
import { Table, TableWrapper, Row, Rows, Col } from 'react-native-table-component'
import SpaghettiGraph from '../Models/SpaghettiGraph'
import * as pc from '../Models/ParameterCalculation'
import ListItem from '../components/ListItem'
import * as DocumentPicker from 'expo-document-picker';
import * as FileSystem from 'expo-file-system';
import * as Sharing from 'expo-sharing';
import axios from 'axios';
import * as Speech from 'expo-speech'
import {setDbLink, getDbLink} from '../config/dblink'
import {getPatientID} from '../config/user'
import { LinearGradient } from 'expo-linear-gradient'


export default function TestResult({navigation, route}) {
  const accX = pc.movingAverageFilter(pc.movingAverageFilter(route.params.accX, 14), 10);
  const accZ = pc.movingAverageFilter(pc.movingAverageFilter(route.params.accZ, 14), 10);

  //const duration = route.params.duration
  const milliseconds = route.params.milliseconds
  Speech.speak("The test has ended", {language: 'en'})
  const dts = []
  for (let i = 0; i < milliseconds.length - 1; i++) {
    dts.push((milliseconds[i+1] - milliseconds[i]) / 1000)
  }
  const duration = dts.reduce((a, b) => a+b, 0)
  console.log(`Sum is: ${dts.reduce((a, b) => a+b, 0)}`)
  console.log("Length of dts is " + dts.length)
  console.log("length of accX is " + accX.length)

  const listData = [
    {key: "Sway Area", value: pc.get95ellipse(accX, accZ)[2].toFixed(2)},
    {key: "Path Length (m/s^2)", value: pc.calculatePathLength(accX, accZ, dts).pl?.toFixed(2)},
    {key: "Path Length (Coronal) (m/s^2)", value: pc.calculatePathLength(accX, accZ, dts).plx?.toFixed(2)},
    {key: "Path Length (Sagittal) (m/s^2)", value: pc.calculatePathLength(accX, accZ, dts).plz?.toFixed(2)},
    {key: "Normalized Path Length (s^3/m)", value: (duration / pc.calculatePathLength(accX, accZ, dts).pl)?.toFixed(2)},
    {key: "Jerk (m^2/s^5)", value: pc.calculateJerk(accX, accZ, dts).jerk.toFixed(2)},
    {key: "Jerk (m^2/s^5) (Coronal)", value: pc.calculateJerk(accX, accZ, dts).jerkX.toFixed(2)},
    {key: "Jerk (m^2/s^5) (Sagittal)", value: pc.calculateJerk(accX, accZ, dts).jerkZ.toFixed(2)},
    {key: "Mean Velocity (m/s)", value: pc.calculateMeanVelocity(accX, accZ, dts).meanVel.toFixed(4)},
    {key: "Mean Velocity (m/s) (Coronal)", value: pc.calculateMeanVelocity(accX, accZ, dts).meanVelX.toFixed(4)},
    {key: "Mean Velocity (m/s) (Sagittal)", value: pc.calculateMeanVelocity(accX, accZ, dts).meanVelZ.toFixed(4)}
  ]
  
  const listArr = []
  for (let i = 0; i < listData.length; i++) {
    listArr.push(
      <ListItem key={i} text={listData[i].key} value={listData[i].value} />
    )
  }


  var csvString = "accX, accZ\n"
  for (let i = 0; i < accX.length; i++) {
    csvString = csvString.concat(`${accX[i]}, ${accZ[i]}\n`)
  }

  

  
  const [fileUri, setFileUri] = React.useState(null);

  // create the file when the component mounts
  React.useEffect(() => {
    async function createFile() {
      const uri = `${FileSystem.documentDirectory}balanceResults.csv`;
      await FileSystem.writeAsStringAsync(uri, csvString, {
        encoding: FileSystem.EncodingType.UTF8,
      });
      setFileUri(uri);
    }
    createFile();
  }, []);

  // handle the button press
  async function handleSharePress() {
    try {
      await Sharing.shareAsync(fileUri, {
        mimeType: 'text/csv',
        dialogTitle: 'Share File',
        UTI: 'public.comma-separated-values-text',
      });
    } catch (error) {
      console.error('Error sharing file: ', error);
    }
  }

  async function handleDatabasePress() {
  const query = `INSERT INTO TESTRESULT 
  (SwayArea, PathLength, PathLengthCor, PathLengthSag, NormalizedPathLength, Jerk, JerkCor, JerkSag, MeanVel, MeanVelCor, MeanVelSag, AccX, AccZ, PatientID) 
  VALUES 
  ('${listData.find(item => item.key === "Sway Area").value}',
  '${listData.find(item => item.key === "Path Length (m/s^2)").value}', 
  '${listData.find(item => item.key === "Path Length (Coronal) (m/s^2)").value}', 
  '${listData.find(item => item.key === "Path Length (Sagittal) (m/s^2)").value}', 
  '${listData.find(item => item.key === "Normalized Path Length (s^3/m)").value}', 
  '${listData.find(item => item.key === "Jerk (m^2/s^5)").value}', 
  '${listData.find(item => item.key === "Jerk (m^2/s^5) (Coronal)").value}', 
  '${listData.find(item => item.key === "Jerk (m^2/s^5) (Sagittal)").value}', 
  '${listData.find(item => item.key === "Mean Velocity (m/s)").value}', 
  '${listData.find(item => item.key === "Mean Velocity (m/s) (Coronal)").value}', 
  '${listData.find(item => item.key === "Mean Velocity (m/s) (Sagittal)").value}', 
  '${JSON.stringify(accX)}', 
  '${JSON.stringify(accZ)}', 
  '${getPatientID()}')`;
    console.log(query);
    axios.defaults.headers.post['Content-Type'] ='application/x-www-form-urlencoded';
    axios
      .post(getDbLink(), {query: query, action:"test_result_entry"})
      .then((response) => {
        console.log(response);
      })
      .catch((error) => {
        console.log(error);
      });
  }
  

  return (
    <SafeAreaView style={styles.container} >
      <ScrollView overScrollMode='never' vertical={true} style={{backgroundColor: 'rgb(250, 250, 250)', width: '100%', flex: 1, flexDirection:'column', borderRadius: 15}}>
        <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>Test Result</Text>
        <View style={{flex: 1, height:480, marginLeft: 10, marginRight: 10, alignItems: 'center', backgroundColor: 'rgba(0,0,0,0)'}}>
          <SpaghettiGraph x={accX} y={accZ} style={{margin: 10}}/>
        </View>
        <View style={{height: 90, flexDirection: 'row', paddingLeft: 15, paddingRight: 15, marginBottom: 15}}>
          <TouchableOpacity style={styles.buttonStyle} onPress={handleSharePress} disabled={!fileUri}> 
              <Text style={styles.buttonTextStyle}>Share Results</Text>
              <Image source={require('../assets/upload.png')} style={{width: 30, height: 30, marginLeft: 'auto', marginRight: 'auto', marginBottom: 'auto'}}/>
          </TouchableOpacity>
          <TouchableOpacity style={styles.buttonStyle} onPress={handleDatabasePress} disabled={!fileUri}> 
              <Text style={styles.buttonTextStyle}>Save Results</Text>
              <Image source={require('../assets/diskette.png')} style={{width: 30, height: 30, marginLeft: 'auto', marginRight: 'auto', marginBottom: 'auto'}}/>
          </TouchableOpacity>
        </View>
        <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>Metrics</Text>
        <View style={{flex: 1, flexDirection: 'column'}}>
          {listArr}
        </View>

      </ScrollView>
    </SafeAreaView>
  )


}

const styles = StyleSheet.create({
  container: { flex: 1, padding: 16, paddingTop: 100, backgroundColor: '#fff', alignItems: 'center' },
  head: { height: 40, backgroundColor: '#e8e8e8' },
  wrapper: { flexDirection: 'row'},
  title: { flex: 1, backgroundColor: 'white' },
  row: { height: 40 },
  titleText: { textAlign: 'left', paddingLeft: 15, fontWeight: 700 },
  text: { textAlign: 'left', paddingLeft: 15 },
  scrollView: {
    backgroundColor: 'rgb(245, 245, 245)',
    marginTop: -1,
    borderRadius: 15,

    
  },
  listContainer: {
    height: 65,
    backgroundColor: 'white', 
    margin: 10,
    marginTop: 5,
    marginBottom: 5, 
    flex: 1, 
    borderRadius: 15,
    padding: 10,
    flexDirection: 'row',
    
    shadowColor: 'black',
    shadowOffset: { width: 0, height: 0 },
    shadowOpacity: 0.2,
    shadowRadius: 5,
  },
  listText: {flex: 1, fontSize: 13, alignSelf: 'center', textAlign: 'right', paddingRight: 15},
  listTitle: {flex: 2, fontSize: 14, fontWeight: 'bold', alignSelf: 'center'},
  buttonStyle: {
    flex: 1,
    marginLeft: 10,
    marginRight: 10,
    borderRadius: 13,
    shadowColor: '#171717',
    shadowOffset: {width: 0, height: 4},
    shadowOpacity: 0.2,
    shadowRadius: 5,
    elevation: 8,
    backgroundColor: '#0e3363'
  },
  buttonTextStyle: {
    fontSize: 18,
    color: 'white',
    fontWeight: "bold",
    alignSelf: "center",
    marginTop: 'auto',
    marginBottom: 10,

  },

});
/*
const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  head: { height: 40, backgroundColor: 'orange' },
  wrapper: { flexDirection: 'row' },
  title: { flex: 1, backgroundColor: '#2ecc71' },
  row: { height: 28 },
  text: { textAlign: 'center' },

});
*/