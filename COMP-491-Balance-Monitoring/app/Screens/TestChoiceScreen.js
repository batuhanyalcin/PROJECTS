import {Image, StyleSheet, Text, TouchableOpacity, View, ScrollView} from 'react-native'
import React from 'react'
import Screen from '../components/Screen';
import colors from '../config/colors';
import AppText from '../components/AppText';
import { LinearGradient } from 'expo-linear-gradient';
import TestSelectButton from '../components/TestSelectButton';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import { NavigationContainer } from '@react-navigation/native';
import axios from 'axios';
import {setDbLink, getDbLink} from '../config/dblink'
import {getPatientID, getQueryResult, setQueryResult, setAccX, getAccX, setAccZ, getAccZ, setListArr, getListArr, setOptions, getOptions, getPatientName, setSurveyQueryResult, getSurveyQueryResult, setEmotionalScore, setFunctionalScore, setOverallScore, setPhysicalScore, setSurveyOptions, getSurveyOptions} from '../config/user'

function HomeScreen() {
  return (
    <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}>
      <Text>Home!</Text>
    </View>
  );
}

function SettingsScreen() {
  return (
    <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}>
      <Text>Settings!</Text>
    </View>
  );
}

const Tab = createBottomTabNavigator();

export default function TestChoiceScreen({navigation,route}) {

  async function handleSurveyHistoryButton(){
    var query = "Select * from SurveyResult where PatientID = '"+getPatientID()+"';";
    axios.defaults.headers.post['Content-Type'] ='application/x-www-form-urlencoded';
    axios
    .post(getDbLink(), {query: query, action:"survey_result_retrieval"})
    .then((response) => {
        //console.log(response);
        if(response.data.result){
            var surveyQueryResult = response.data.queryResult;
            console.log(surveyQueryResult);
            setSurveyQueryResult(response.data.queryResult)
        
            if(surveyQueryResult.length > 0){
              var allSurveyDates = []
              for(i = 0; i < surveyQueryResult.length; i++){
                allSurveyDates.push(surveyQueryResult[i].SurveyDate)
              }
              setSurveyOptions(allSurveyDates);
              console.log(getSurveyOptions());
              setOverallScore(surveyQueryResult[0].OverallScore);
              setFunctionalScore(surveyQueryResult[0].FunctionalScore);
              setPhysicalScore(surveyQueryResult[0].PhysicalScore);
              setEmotionalScore(surveyQueryResult[0].EmotionalScore)
              navigation.navigate('SurveyHistory');
            }
            
          }
        })
        .catch((error) => {
            console.log(error);
        });
  }

  async function handleResultHistoryButton(){
    var query = "SELECT * FROM TESTRESULT WHERE PatientID = '"+ getPatientID() +"'";
    //console.log(query);
    axios.defaults.headers.post['Content-Type'] ='application/x-www-form-urlencoded';
    axios
    .post(getDbLink(), {query: query, action:"test_result_retrieval"})
    .then((response) => {
        //console.log(response);
        if(response.data.result){
            queryResult = response.data.queryResult;
            console.log(queryResult);
            setQueryResult(response.data.queryResult)
            var queryResult = getQueryResult();
            if(queryResult.length > 0){
              console.log(queryResult);

              setOptions(queryResult);
  
              setAccZ(JSON.parse(queryResult[0].AccZ));
              console.log(getAccZ().length);
              setAccX(JSON.parse(queryResult[0].AccX));
              console.log(getAccX().length);
  
  
              setListArr(queryResult, 0);
  
              navigation.navigate('ResultHistory');
            }
            
          }
        })
        .catch((error) => {
            console.log(error);
        });

  }

    //console.log(getPatientName())
  return (
    <Screen style={styles.container}>
        <View style = {styles.logo}>
         <LinearGradient
            colors={['#d1eeae', '#94bbe9']}
            style={[styles.centered, {borderRadius:30}]}
            useAngle={true}
            angle={60}
            start={{ x: 0, y: 0 }}
            end={{ x: 1.5, y: 1 }}
          >
          <AppText style= {styles.text}>Welcome {getPatientName()}</AppText>
          </LinearGradient>
        </View>

        <View style = {styles.header}>
          <AppText style= {styles.headerText}>Take a test </AppText>
        </View>
        <ScrollView vertical={true} style={{backgroundColor: 'rgb(250, 250, 250)', width: '100%', flex: 1, flexDirection:'column', borderRadius: 15}}>
        <TestSelectButton title="Balance Test"  style={styles.redirect} onPress={() => navigation.navigate('OnboardingScreen')} imgSource = {require("../assets/balance_button_figure.png")}  />

        <TestSelectButton title="Questionnaire"  style={styles.redirect} onPress={() => navigation.navigate('SurveyInfoScreen')} imgSource = {require("../assets/questionnaire_fig.png")}/>
       
        <TestSelectButton title="Result History"  style={styles.redirect} onPress={() => handleResultHistoryButton()}  />

        <TestSelectButton title="Survey History"  style={styles.redirect} onPress={() => handleSurveyHistoryButton()}  />

        </ScrollView>
     
      

    </Screen>
  )
}

const styles = StyleSheet.create({
    container: {
      flex: 1,
      backgroundColor: colors.grey,
      alignItems: 'center',
      padding:10,
      borderWidth:4,
      borderColor:colors.grey
    },
    logo: {
      height: "30%",
      width: "90%",
      marginTop: 30,
    },
    centered: {
      flex: 1 ,
      justifyContent: 'center'
    },
    text:{
      marginLeft:20,
      fontWeight: 800,
      fontSize: 36,
      color:'white'
    },
    header:{
      height: "10%"
    },
    headerText:{
      //TODO fix
      marginLeft:-150,
      fontWeight: 700,
      color:'#4A4747',
      fontSize: 28,
      marginVertical: 20,
      justifyContent: 'center'
    },
 
  })