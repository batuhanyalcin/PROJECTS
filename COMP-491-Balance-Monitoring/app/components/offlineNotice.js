import { StyleSheet, Text, View } from 'react-native'
import React from 'react'
import { useNetInfo } from '@react-native-community/netinfo'
import colors from '../config/colors'
import AppText from '../components/AppText';

export default function offlineNotice() {
    const netInfo =useNetInfo();
    if(netInfo.type !== "unknown" && netInfo.isInternetReachable === false ){
        return (
            <View>
              <AppText style={styles.text}>Ethernet Connection Not Found</AppText>
            </View>
          )

    }
    return null;

}

const styles = StyleSheet.create({
    container:{
        alignItems:'center',
        backgroundColor:colors.light,
        borderRadius:15,
        height:100,
        justifyContent:'center',
        width:100,
        overflow:"hidden"
    },
    image:{
        width:'100%',
        height:'100%'
    },
    text:{
        color:colors.white,
    }
})