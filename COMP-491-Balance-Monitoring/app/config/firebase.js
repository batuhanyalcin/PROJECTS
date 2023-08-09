import firebase from 'firebase/compat/app';
import 'firebase/compat/auth';
import Constants from 'expo-constants';
import 'firebase/compat/firestore';
import 'firebase/compat/storage';




// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: Constants.manifest.extra.apiKey,
  authDomain: Constants.manifest.extra.authDomain,
  projectId: Constants.manifest.extra.projectId,
  storageBucket: Constants.manifest.extra.storageBucket,
  messagingSenderId: Constants.manifest.extra.messagingSenderId,
  appId: Constants.manifest.extra.appId,
};

// Initialize Firebase
let Firebase = firebase.initializeApp(firebaseConfig);
export const firestore = firebase.firestore();
export default Firebase;


//Create user
export const createUserProfile = async (UserAuth,images,additionalData) => {
  if(!UserAuth) return;
  const userRef = firestore.doc('balance_app/${UserAuth.uid}');
  const snapshot = await userRef.get();
  let newImages = [];
  for(let i=0;i<images.length;i++){
    let response = await fetch(Images[i]);
    let blob = await response.blob();
    let ref = Firebase.storage().ref().child('balance_app/${Date.now()}');
    await (ref.put(blob));
    let link = await ref.getDownloadURL();
    newImages.push(link);

  }
  if(!snapshot.exists){
    const {email} = UserAuth;
    const createdAt = new Date();

    try{
      await userRef.set({
        id:UserAuth.uid,
        email,
        createdAt,
        images: newImages,
        ...additionalData
      })}catch(err){      }
  }
  return userRef;
}

//Test Handle