import { Image, ScrollView, StyleSheet, Text, TouchableOpacity, View } from 'react-native'
import React, { useState } from 'react'
import colors from '../config/colors'
import * as Yup from "yup";
import useAuth from '../auth/useAuth'
import Constants  from 'expo-constants';
import ActivityIndicator from '../components/ActivityIndicator';
import { ErrorMessage, Form,FormField,SubmitButton,FormImagePicker } from '../components/Form';
import Screen from '../components/Screen';
import useApi from '../hooks/useApi';
import authApi from "../api/auth";
import Button from '../components/Button';

import axios from 'axios';
import Firebase, { createUserProfile } from '../config/firebase';
import {setDbLink, getDbLink} from '../config/dblink'
import {getPatientID} from '../config/user'



const validationSchema = Yup.object().shape({
  
  name:Yup.string().required().min(1).label("Name"),
  Phone_no:Yup.number().required().min(10).label("Phone no."),
  email: Yup.string().email('Must be a valid email').max(255).required('Email is required'),
  image: Yup.array().min(1, "Please Select Atleast on Image"),
  password: Yup.string().required().min(4).label("Password"),
  gender: Yup.string().required().min(1).label("Gender"),
  age: Yup.number().required().min(1).label("Age"),
  weight: Yup.number().required().min(1).label("Weight"),
  height:Yup.number().required().min(1).label("Height")
  
})


export default function RegisterScreen({navigation}) {
  const auth = useAuth();
  const [error, setError] = useState();
  const [loading,setLoading] =useState(false);
  const [name, setName] = useState("");
  const [Phone_no, setPhoneNo] = useState("");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [gender, setGender] = useState("");
  const [age, setAge] = useState("");
  const [weight, setWeight] = useState("");
  const [height, setHeight] = useState("");
  const [image, setImage] = useState("");


  const HandleSubmit = async() => {
    //const { name, email, Phone_no, gender, age, weight, height, password } = values;
   
    /*
    console.warn("This is a test warning");
    console.log("hello");
    try{
      setLoading(true);
      const {user} = await Firebase.auth().createUserWithEmailAndPassword(email,password);
      console.log(user);
      await createUserProfile(user,images,{name,Phone_no});
      setLoading(false);
    } catch (error){

    };
      console.log(Constants.manifest.extra.API_URL);
      */
     console.log("In handle");
     //const express= require('express');
     //const app=express();
     //app.use(cors());
      
     //const cors = require('cors');
     //app.use(cors());
      // Make HTTP POST request to PHP script
      console.log(name);
      console.log(email);
      query = "INSERT into hospital_patient (PName, Email, PhoneNumber, Gender, Age, Weight, Height, PPassword) VALUES ('" + name + "', '"+email+"', '"+Phone_no+"', '"+gender+"', '"+age+"', '"+weight+"', '"+height+"', '"+password+"');"
      console.log(query);
      axios.defaults.headers.post['Content-Type'] ='application/x-www-form-urlencoded';
      axios
        .post(getDbLink(), {query: query,action:"register"})
        .then((response) => {
          console.log(response);
          console.log('Navigating');
          navigation.navigate('LoginScreen');
        })
        .catch((error) => {
          console.log(error);
        });
        
        console.log(getDbLink());
        
   

        
  
}

  return (
   <>
    <ActivityIndicator visible={loading} /> 
    <Screen style={styles.container}>
        <ScrollView>
        <Image
          style={styles.logo}
          source={require("../assets/logo.png")}
        />
        <Form
        initialValues={{
          name:"",
          Phone_no:"",
          email:"",
          images:[],
          password:"",
          gender: "",
          age: "",
          weight: "",
          height:""
        }}
          
          onSubmit={HandleSubmit}
        validationSchema={validationSchema}
        >
            
            <FormField maxLength={255} 
            name="name" 
            placeholder="Name"  
            onChangeText={setName}/>
            
            <FormField maxLength={255} 
            name="email" 
            placeholder="Email" 
            onChangeText={setEmail}/>

            <FormField
            keyboardType="numeric"
            maxLength={10}
            name="Phone_no"
            placeholder="Phone number"
            onChangeText={setPhoneNo}
          />

     
             <FormField 
            maxLength={1}
            name="gender"
            placeholder="Gender M for male Female for F"
            onChangeText={setGender}
            />

            <FormField
            keyboardType="numeric"
            maxLength={3} 
            name="weight"
            placeholder="Weight as Kg."
            onChangeText={setWeight}
            />

            <FormField
            keyboardType="numeric"
            maxLength={3} 
            name="height"
            placeholder="Height as cm"
            onChangeText={setHeight}
            />

            <FormField 
            keyboardType="numeric"
            maxLength={3} 
            name="age"
            placeholder="Age"
            onChangeText={setAge}
            />
            <FormField 
            name="password"
            placeholder="Your Password"
            secureTextEntry={true}
            onChangeText={setPassword}
            />

            <Button title="Register" onPress={HandleSubmit}/>

            
            <ErrorMessage error={error} />
            </Form>
        </ScrollView>
      </Screen>
   </>
  )
}

const styles = StyleSheet.create({
  container: {
    backgroundColor:"white",
    flex: 1,
    alignItems: 'center',
    padding:10,
    justifyContent: 'center',
    borderWidth:4,
    borderColor:colors.grey
  },
  logo: {
    marginTop: 25,
    height: 150,
    width: 150,
    alignSelf:"center"
  },
  image_picker: {
    marginBottom: 20
  }
})