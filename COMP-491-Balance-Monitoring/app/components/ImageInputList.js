import { ScrollView, StyleSheet, Text, View } from 'react-native'
import React, { useRef } from 'react'
import ImageInput from './ImageInput'
import colors from '../config/colors';

export default function ImageInputList({imageUris=[],onRemoveImage,onAddImage}) {
    const scrollView = useRef();
  return (
    <View>
     <ScrollView 
     horizontal
     ref={scrollView}
     onContentSizeChange={() => scrollView.current.scrollToEnd()}
     >
            <View style={styles.container}>
                <ImageInput   imageUri={imageUris[0] ? imageUris[0] : null}
                onChangeImage={uri => onAddImage(uri)} 
                onRemoveImage={(uri) => onRemoveImage(uri)}
                onPress={() => onRemoveImage(uri)}
                 />
            </View>
     </ScrollView>
    </View>
  )
}

const styles = StyleSheet.create({
    container:{
        flexDirection:"row",
        display:"flex",
        alignSelf:"center",
        borderWidth:0.5,
        borderColor:colors.gray,
        borderRadius:5
        // elevation: 5
    },
    image:{
        marginRight:10
    }
})