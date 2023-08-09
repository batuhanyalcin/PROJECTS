import { Image, StyleSheet, Text, TouchableOpacity, View, useWindowDimensions} from 'react-native'
import React, { useState } from 'react'
import * as Yup from "yup";
import useAuth from '../auth/useAuth'
import ActivityIndicator from '../components/ActivityIndicator';
import { ErrorMessage, Form,FormField,SubmitButton } from '../components/Form';
import Screen from '../components/Screen';
import colors from '../config/colors'
import AppText from '../components/AppText';
import axios from 'axios';
import { MaterialCommunityIcons } from '@expo/vector-icons';
import Constants  from 'expo-constants';
import Firebase from '../config/firebase';
import {setPatientID, getPatientID, setPatientName, getPatientName} from '../config/user'
import {getDbLink} from '../config/dblink'
import Button from '../components/Button';

const validationSchema = Yup.object().shape({
  email: Yup.string().email('Must be a valid email').max(255).required('Email is required'),
  password: Yup.string().max(255).required('Password is required')
})

export default function LoginScreen({navigation,route}) {
  const [loginFailed, setLoginFailed] = useState(false);
  const [loading,setLoading] = useState(false);
  const [errorMsg,setErrorMsg] = useState(false);
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");

  const auth = useAuth();
  const handleSubmit = async () => {
    try {
      setLoading(true);
      axios.defaults.headers.post['Content-Type'] = 'application/x-www-form-urlencoded';
      axios
        .post(getDbLink(), {email: email, action: "login", password: password})
        .then((response) => {
          console.log(response);
          console.log(response.data.result); // Access the result property of the response
          if (response.data.result) {
            setPatientID(response.data.patientID);
            setPatientName(response.data.pName);
            //console.log(getPatientID())
            //console.log(getPatientName())
            navigation.navigate('TestChoiceScreen');
          }
        })
        .catch((error) => {
          console.log(error);
          setErrorMsg(true);
        })

      setLoading(false);

      console.log("Out handle");

    } catch (error) {
      setErrorMsg(true);
    }
  }


  return (
    <>
    <ActivityIndicator visible={loading}/>
    <Screen style={styles.container}>
    { errorMsg && <View style={styles.msg}>

      <AppText>Email Not Verified / Wrong Email</AppText>
      <TouchableOpacity onPress={() => setErrorMsg(false)}>
      <MaterialCommunityIcons name='close' size={20} />
      </TouchableOpacity>
    </View>}
   <Image 
     style={styles.logo}
     source={require("../assets/logo.png")}
     />
     <AppText>Welcome to Balance App</AppText>
     <Form
     initialValues={{
       email:'',
       password:''
     }}
 
     onSubmit={handleSubmit}
     validationSchema={validationSchema}
    
     > 

 
     <ErrorMessage error="Invalid Email/password" visible={loginFailed} />
     <FormField
        icon="email"
       maxLength={255}
       name="email"
       placeholder="Email"
       onChangeText={setEmail}
     />
     <FormField
       icon="eye-off"
       name="password"
       placeholder="Password"
       secureTextEntry={true}
       onChangeText={setPassword}
     />
     <Button title="Login" onPress={handleSubmit}/>
     <TouchableOpacity style={styles.redirect} onPress={() => navigation.navigate('RegisterScreen')} >
       <Text>Create an account</Text>
     </TouchableOpacity>
     <TouchableOpacity style={styles.redirect} onPress={() => navigation.navigate('TestChoiceScreen')} >
       <Text>Skip Login</Text>
     </TouchableOpacity>
     </Form>

    </Screen>
    </>
   )
 }
 
 const styles = StyleSheet.create({
     container: {
         flex: 1,
         backgroundColor: colors.white,
         alignItems: 'center',
         padding:10,
         justifyContent: 'center',
         borderWidth:4,
         borderColor:colors.grey
       },
       logo:{
         width: 200,
         height: 200,
         marginBottom:60,
         alignSelf:'center'
       },
       text:{
         textAlign:"center"
       },
       redirect:{
         marginTop:10,
         marginBottom:10
       },
       msg:{
        width:"95%",
        height:"8%",
        backgroundColor: "#FF6263",
        display:"flex",
        flexDirection:"row",
        alignItems:"center",
        justifyContent:"space-evenly",
        borderRadius:10,
        marginBottom:10,
       },
  
 })