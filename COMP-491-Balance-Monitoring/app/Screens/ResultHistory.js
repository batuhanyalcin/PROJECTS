import { SafeAreaView, StyleSheet, Text, View, ScrollView, Button, FlatList} from 'react-native'
import React, { useEffect, useState } from 'react'
import { Table, TableWrapper, Row, Rows, Col } from 'react-native-table-component'
import SpaghettiGraph from '../Models/SpaghettiGraph'
import * as pc from '../Models/ParameterCalculation'
import ListItem from '../components/ListItem'
import * as DocumentPicker from 'expo-document-picker';
import * as FileSystem from 'expo-file-system';
import * as Sharing from 'expo-sharing';
import axios from 'axios';
import {setDbLink, getDbLink} from '../config/dblink'
import {getPatientID, getQueryResult, setQueryResult, setAccX, getAccX, setAccZ, getAccZ, setListArr, getListArr, setOptions, getOptions} from '../config/user'
import SelectDropdown from 'react-native-select-dropdown'


export default function ResultHistory({navigation, route}) {
  if(getQueryResult().length > 0){


    const [testResult, setTestResult] = useState({
      accX: getAccX(),
      accZ: getAccZ(),
      spaghettiGraphComp: <SpaghettiGraph x={getAccX()} y={getAccZ()} style={{margin: 10}}/>,
      listArr: getListArr()
    })

    useEffect(() => {

    }, [testResult]); 

    function handleOptionChange(e) {
      setAccX(JSON.parse(getQueryResult()[e].AccX));
      console.log(getAccX()[0]);
      setAccZ(JSON.parse(getQueryResult()[e].AccZ));
      setListArr(getQueryResult(), e);

      setTestResult({
        ...testResult,
        
        listArr: getListArr(),
        accZ: getAccZ(),
        accX: getAccX(),
        //spaghettiGraphComp: <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>New Change</Text>
        spaghettiGraphComp: <SpaghettiGraph x={getAccX()} y={getAccZ()} style={{margin: 10}}/>
      });
    }

    return (
      <SafeAreaView style={styles.container} >
        <ScrollView overScrollMode='never' vertical={true} style={{backgroundColor: 'rgb(250, 250, 250)', width: '100%', flex: 1, flexDirection:'column', borderRadius: 15}}>
          <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>Test Result</Text>
          <SelectDropdown
              data={getOptions()}
              defaultValueByIndex = {0}
              onSelect={(selectedItem, index) => {
                  console.log(selectedItem, index)
                  handleOptionChange(index);
              }}
              buttonTextAfterSelection={(selectedItem, index) => {
                  // text represented after item is selected
                  // if data array is an array of objects then return selectedItem.property to render after item is selected
                  return selectedItem
              }}
              rowTextForSelection={(item, index) => {
                  // text represented for each item in dropdown
                  // if data array is an array of objects then return item.property to represent item in dropdown
                  return item
              }}
          />
          <View style={{flex: 1, height:500, margin: 10, alignItems: 'center', backgroundColor: 'rgba(0,0,0,0)'}}>
              {testResult.spaghettiGraphComp}
          </View>
          <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>Metrics</Text>
          <View style={{flex: 1, flexDirection: 'column'}}>
              {testResult.listArr}
          </View>
        </ScrollView>
        
      </SafeAreaView>
    );
  } else {
    return (
      <SafeAreaView style={styles.container} >
        <ScrollView overScrollMode='never' vertical={true} style={{backgroundColor: 'rgb(250, 250, 250)', width: '100%', flex: 1, flexDirection:'column', borderRadius: 15}}>
          <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>No Result to Show! Do a Balance Test</Text>
        </ScrollView>
      </SafeAreaView>
    );
  }

            



  
/*
  return (
    <SafeAreaView style={styles.container} >
      <ScrollView overScrollMode='never' vertical={true} style={{backgroundColor: 'rgb(250, 250, 250)', width: '100%', flex: 1, flexDirection:'column', borderRadius: 15}}>
        <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>Test Result</Text>
        <View style={{flex: 1, height:500, margin: 10, alignItems: 'center', backgroundColor: 'rgba(0,0,0,0)'}}>
          <SpaghettiGraph x={accX} y={accZ} style={{margin: 10}}/>
        </View>
        <Button title='Save to files' onPress={handleSharePress} disabled={!fileUri}></Button>
        <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>Metrics</Text>
        <View style={{flex: 1, flexDirection: 'column'}}>
          {listArr}
        </View>

      </ScrollView>
      
    </SafeAreaView>
  )

          <select value={value} onChange={handleChange}>
            <option value="Orange">Orange</option>
            <option value="Radish">Radish</option>
            <option value="Cherry">Cherry</option>
        </select>
*/

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
  listTitle: {flex: 2, fontSize: 14, fontWeight: 'bold', alignSelf: 'center'}
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