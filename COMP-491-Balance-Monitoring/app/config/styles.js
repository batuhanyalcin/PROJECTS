import {Platform} from 'react-native';
import colors from './colors';

export default {
    colors,
    text :{
        fontWeight: 'light',
        color:colors.dark,
        fontSize:18,
        fontFamily: Platform.OS === "android" ? "Roboto" : "Avenir"
    }
}