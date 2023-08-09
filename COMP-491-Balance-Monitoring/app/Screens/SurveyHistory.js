import { SafeAreaView, StyleSheet, Text, View, ScrollView, Button, Dimensions, Image, FlatList, TouchableOpacity} from 'react-native'
import React, { useEffect, useState } from 'react'
import {getOptions, getEmotionalScore, getFunctionalScore, getOverallScore, getPhysicalScore, getSurveyQueryResult, setEmotionalScore, setFunctionalScore, setOverallScore, setPhysicalScore, getSurveyOptions} from '../config/user'
import SelectDropdown from 'react-native-select-dropdown'
import {ProgressChart} from "react-native-chart-kit";
import colors from '../config/colors';



export default function ResultHistory({navigation, route}) {
  if(getSurveyQueryResult().length > 0){


    const [surveyResult, setSurveyResult] = useState({
      overall: getOverallScore(),
      physical: getPhysicalScore(),
      functional : getFunctionalScore(),
      emotional: getEmotionalScore(),
      data: {
        labels: ["Overall:", "Emotional:", "Functional:", "Physical:"], // optional
        data: [getOverallScore(), getEmotionalScore(), getFunctionalScore(), getPhysicalScore()],
        colors: [
          "rgba(255, 0, 0,0.5)",
          "rgba(238, 130, 238,0.6)",
          "rgba(106, 90, 205,0.5)",
          "rgba(60, 179, 113,0.2)",]
      }
    })

    useEffect(() => {

    }, [surveyResult]); 

    function handleOptionChange(e) {
      var surveyQueryResult = getSurveyQueryResult();

      setOverallScore(surveyQueryResult[e].OverallScore);
      setFunctionalScore(surveyQueryResult[e].FunctionalScore);
      setPhysicalScore(surveyQueryResult[e].PhysicalScore);
      setEmotionalScore(surveyQueryResult[e].EmotionalScore);
      setSurveyResult({
        ...surveyResult,
        
        overall: getOverallScore(),
        physical: getPhysicalScore(),
        functional : getFunctionalScore(),
        emotional: getEmotionalScore(),
        data: {
          labels: ["Overall:", "Emotional:", "Functional:", "Physical:"], // optional
          data: [getOverallScore(), getEmotionalScore(), getFunctionalScore(), getPhysicalScore()],
          colors: [
            "rgba(255, 0, 0,0.5)",
            "rgba(238, 130, 238,0.6)",
            "rgba(106, 90, 205,0.5)",
            "rgba(60, 179, 113,0.2)",]
        }
      });
      
    }

    return (
      <SafeAreaView style={styles.container} >
        <ScrollView overScrollMode='never' vertical={true} style={{backgroundColor: 'rgb(250, 250, 250)', width: '100%', flex: 1, flexDirection:'column', borderRadius: 15}}>
          <View><Text style = {styles.text2}>Survey Result History</Text></View>
          <SelectDropdown
              data={getSurveyOptions()}
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
        <View style = {styles.container}>
            <ProgressChart
                data={surveyResult.data}
                width={Dimensions.get("window").width - 10}
                height={220}
                strokeWidth={16}
                hasLegend={true}
                withCustomBarColorFromData={true}
                radius={35}
                chartConfig={{
                backgroundGradientFromOpacity: 0.5,
                backgroundGradientToOpacity: 1,
                backgroundColor: colors.white,
                backgroundGradientFrom: colors.white,
                backgroundGradientTo: colors.white,
                propsForLabels: { fill: colors.black, fontSize: 9, fontWeight: 500},
                decimalPlaces: 2,
                color: (opacity = 1, _index) => `rgba(128,128,128,${opacity})`,
                }}
                style={{ marginVertical: 8, borderRadius: 10 }}
            />
    </View>
        </ScrollView>
        
      </SafeAreaView>
    );
  } else {
   print('aa');
    return (
      <SafeAreaView style={styles.container} >
        <ScrollView overScrollMode='never' vertical={true} style={{backgroundColor: 'rgb(250, 250, 250)', width: '100%', flex: 1, flexDirection:'column', borderRadius: 15}}>
          <Text style={{height: 50, fontSize: 20, fontWeight: 'bold', alignSelf: 'baseline', paddingLeft: 30}}>No Result to Show! Do a Survey</Text>
        </ScrollView>
      </SafeAreaView>
    );
  }

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

    
  },container2: {
        
    marginTop: Dimensions.get('window').height/5,
    alignItems: 'center',
    justifyContent:'center',
    
   

   
},
text2:{
   
    alignSelf: 'center',
    textAlignVertical: 'center',
    fontSize: 20,
    fontWeight: 800
    //marginTop: 350,
    //marginLeft: 100
    
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