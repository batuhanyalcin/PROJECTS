import { StyleSheet, Text, TouchableOpacity, View, Image } from 'react-native'
import React from 'react'
import colors from '../config/colors'
import { LinearGradient } from 'expo-linear-gradient';

export default function SurveySelectButton({title,onPress,isSelected}) {
    const GREEN = '#36c9bb';
    const GRAY = colors.gray;
    
    return (
    
    
   <TouchableOpacity style={[styles.button]}
   onPress={onPress}
   >
     <LinearGradient
        colors={isSelected ? ['#099773', '#43b692']: ['#243748', '#4b749f']}
        useAngle={true}
        angle={60}
        angleCenter={{ x: 0, y: 1 }}
        start={[0, 0]}
        end={[1, 0]}
        style={[styles.button,{ borderRadius: 10 }]}
      >
        
        <Text style={styles.text}>{title}</Text>
        </LinearGradient>
   </TouchableOpacity>
  )
}

const styles = StyleSheet.create({
    button : {
        justifyContent:'center',
        padding: 10,
        
        width:"100%",
        
        flexDirection: 'row',
        alignItems: 'center',
    },
    
    text : {
        color :colors.white,
        fontSize:20,
        fontWeight:800
    }
})