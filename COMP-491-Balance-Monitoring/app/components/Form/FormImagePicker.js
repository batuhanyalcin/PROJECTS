import { StyleSheet, Text, View } from 'react-native'
import React from 'react'
import ErrorMessage from './ErrorMsg';
import ImageInputList from "../ImageInputList";
import { useFormikContext } from 'formik';

function FormImagePicker({ name }) {
  const { errors, setFieldValue, touched, values } = useFormikContext();
  const imageUris = values[name];

  const handleAdd = (uri) => {
    console.log("test");
    if(uri===null){
      console.log("test22");
      setFieldValue(
        name,
        imageUris.filter((imageUri) => imageUri !== uri).concat(null)
      );

    }else{
      console.log("test333");
      setFieldValue(name, [...imageUris, uri]);
    }

  };

  const handleRemove = (uri) => {
    console.log("test2");
    console.log(uri);
    setFieldValue(
      name,
      imageUris.filter((imageUri) => imageUri !== uri).concat(null)
    );
  };

  return (
    <>
      <ImageInputList
        imageUris={imageUris}
        onAddImage={handleAdd}
        onRemoveImage={handleRemove}
      />
      <ErrorMessage error={errors[name]} visible={touched[name]} />
    </>
  );
}

export default FormImagePicker;