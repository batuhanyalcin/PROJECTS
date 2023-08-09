import { StyleSheet, Text, TouchableOpacity, View } from 'react-native'
import React from 'react'
import colors from '../config/colors'
import { LinearGradient } from 'expo-linear-gradient';
export default function Button({title,onPress, disabled, colors = ['#090979', '#00d4ff']}) {
  return (
    
   <TouchableOpacity 
    style={[styles.button]}
    onPress={onPress}
    disabled={disabled}
   >
     <LinearGradient
        colors={colors}
        useAngle={true}
        angle={60}
        angleCenter={{ x: 0, y: 1 }}
        start={[0, 0]}
        end={[1, 1]}
        style={[styles.button,{ borderRadius: 200 }]}
      >
        <Text style={styles.text}>{title}</Text>
        </LinearGradient>
   </TouchableOpacity>
  )
}

const styles = StyleSheet.create({
    button : {
        borderRadius: 200,
        justifyContent:'center',
        alignItems:'center',
        padding: 15,
        width: 100,
        height: 100,
        marginVertical:10,
        marginTop: '10%'
    
    },
    text : {
        color :colors.white,
        fontSize:14,
        fontWeight:800
    }
})