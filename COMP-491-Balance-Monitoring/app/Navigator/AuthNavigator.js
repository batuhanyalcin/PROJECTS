import { StyleSheet, Text, View } from 'react-native'
import React from 'react';
import { createStackNavigator } from '@react-navigation/stack';
import LoginScreen from '../Screens/LoginScreen';
import RegisterScreen from '../Screens/RegisterScreen';
import ForgotPassword from '../Screens/ForgotPassword';
import TestChoiceScreen from '../Screens/TestChoiceScreen';
import BalanceTestScreen from '../Screens/BalanceTestScreen';
import HomeScreen from '../Screens/HomeScreen';
import ProfileScreen from '../Screens/ProfileScreen';
import OldTests from '../Screens/OldTests';
import TestResult from '../Screens/TestResult';
import TestScreen from '../Screens/TestScreen';
import AboutUsScreen from '../Screens/AboutUsScreen';
import OnboardingScreen from '../Screens/OnboardingScreen';
import SurveyScreen from '../Screens/SurveyScreen';
import SurveyInfoScreen from '../Screens/SurveyInfoScreen';
import SurveyResultScreen from '../Screens/SurveyResultScreen'
import ResultHistory from '../Screens/ResultHistory';
import SurveyHistory from '../Screens/SurveyHistory';




const Stack = createStackNavigator();


export default function AuthNavigator() {
  return (
   <Stack.Navigator screenOptions={{headerShown:false}}>
        <Stack.Screen name='LoginScreen' component={LoginScreen} />
        <Stack.Screen name='RegisterScreen' component={RegisterScreen} />
        <Stack.Screen name='ForgotPassword' component={ForgotPassword} />
        <Stack.Screen name="TestChoiceScreen" component={TestChoiceScreen} />
        <Stack.Screen name="OnboardingScreen" component={OnboardingScreen} />
        <Stack.Screen name="BalanceTestScreen" component={BalanceTestScreen} />
        <Stack.Screen name="HomeScreen" component={HomeScreen} />
        <Stack.Screen name="ProfileScreen" component={ProfileScreen} />
        <Stack.Screen name="OldTests" component={OldTests} />
        <Stack.Screen name="TestResult" component={TestResult} />
        <Stack.Screen name="TestScreen" component={TestScreen} />
        <Stack.Screen name="ResultHistory" component={ResultHistory} />
        <Stack.Screen name="AboutUsScreen" component={AboutUsScreen} />
        <Stack.Screen name="SurveyScreen" component={SurveyScreen} />
        <Stack.Screen name="SurveyInfoScreen" component={SurveyInfoScreen} />
        <Stack.Screen name="SurveyResultScreen" component={SurveyResultScreen} />
        <Stack.Screen name="SurveyHistory" component={SurveyHistory} />
   </Stack.Navigator>
  )
}

