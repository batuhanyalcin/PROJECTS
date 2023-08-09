import { StyleSheet, Text, View,TextInput } from 'react-native'
import React from 'react'
import { MaterialCommunityIcons } from '@expo/vector-icons'
import defaultStyle from '../config/styles';
import colors from '../config/colors';

export default function AppTextInput({icon,width="100%",...otherProps}) {
  return (
    <View style={[styles.container,{width:"95%"}]}>
        {icon && <MaterialCommunityIcons
        name={icon}
        size={28} color={colors.medium}
        style={styles.icon}
        /> }

        <TextInput 
        placeholderTextColor={colors.medium}
        style={[defaultStyle.text,{width:"80%"}]} {...otherProps}
        />

    </View>
  )
}

const styles = StyleSheet.create({
  
    container :{
        borderColor: '#e8e8e8',
        borderWidth:1.2,
        borderRadius: 100,
        marginVertical: 10,
        marginHorizontal:10,
        height: 60,
        padding: 10,

        //backgroundColor : colors.gray,
        
        flexDirection : "row",
        padding:15,
        marginVertical:10
        
    },
    icon:{
        
      marginRight:10
        
    }
})