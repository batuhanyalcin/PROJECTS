import { StyleSheet, Text, TouchableOpacity, View, Image } from 'react-native'
import React from 'react'
import colors from '../config/colors'
import { LinearGradient } from 'expo-linear-gradient';

export default function TestSelectButton({title,onPress,imgSource}) {
  return (
    
   <TouchableOpacity style={[styles.button]}
   onPress={onPress}
   >
     <LinearGradient
        colors={['#84DDF1', '#64a0bb']}
        useAngle={true}
        angle={160}
        angleCenter={{ x: 1, y: 1 }}
        start={[0, 1]}
        end={[0, 0]}
        style={[styles.button,{ borderRadius: 20 }]}
      >
        <Image 
        style={styles.figure}
        source={imgSource}
        />
        <Text style={styles.text}>{title}</Text>
        </LinearGradient>
   </TouchableOpacity>
  )
}

const styles = StyleSheet.create({
    button : {
        justifyContent:'center',
        padding: 10,
        height: 150,
        width:"100%",
        marginVertical:10,
        flexDirection: 'row',
        alignItems: 'center',
    },
    figure:{
       marginRight: 15
    },
    text : {
        color :colors.white,
        fontSize:28,
        fontWeight:900
    }
})