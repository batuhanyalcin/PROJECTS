import react from "react";
import { StyleSheet, View, Text } from "react-native";

export default function ListItem(props) {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>{props.text}</Text>
      <Text style={styles.text}>{props.value}</Text>
    </View>
  )
    /*
    return (
        <View style={styles.container}>
            {props.title ? <Text style={styles.title}>{props.title}</Text> : null}
            {props.items.map((item => <Text style={styles.text}>{item}</Text>))}
      </View>
    )
    */
}

const styles = StyleSheet.create({
    container: {
        height: 65,
        backgroundColor: 'white', 
        margin: 10,
        marginTop: 5,
        marginBottom: 5, 
        flex: 1, 
        borderRadius: 15,
        padding: 10,
        flexDirection: 'row',
        
        shadowColor: 'black',
        shadowOffset: { width: 0, height: 0 },
        shadowOpacity: 0.2,
        shadowRadius: 5,
      },
    text: {flex: 1, fontSize: 13, alignSelf: 'center', textAlign: 'right', paddingRight: 15},
    title: {flex: 2, fontSize: 14, fontWeight: 'bold', alignSelf: 'center'}
  });