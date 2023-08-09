import { NavigationContainer } from '@react-navigation/native';
import authStorage from "./app/auth/storage";
import AppLoading from 'expo-app-loading';
import AuthContext from './app/auth/context';
import MainNavigator from './app/Navigator/MainNavigator';
import AuthNavigator from './app/Navigator/AuthNavigator';
import { useEffect, useState } from 'react';
import * as SplashScreen from 'expo-splash-screen';
import Firebase from './app/config/firebase';
import OfflineNotice from './app/components/offlineNotice';

SplashScreen.preventAutoHideAsync();

export default function App() {
  const db = Firebase.firestore();
  const [appIsReady, setAppIsReady] = useState(false);
  const [user, setUser] = useState();
  const [isReady, setReady] = useState(false);

  const restoreUser = async () => {
    const user = await authStorage.getData();
    /*if (user) setUser(user);*/
    if(user){
      const {uid} = JSON.parse(user);
      await db.collection('balance_app').doc(uid).get()
      .then(snapshot => setUserData(snapshot.data()));
    }
  };

  useEffect(() => {
    async function prepare() {
      try {
       

        await restoreUser();
      
      } catch (e) {
        console.warn(e);
      } finally {
        setAppIsReady(true);
        await SplashScreen.hideAsync();
      }
    }
    prepare();
  }, []);

 
  if(!isReady){
    return <AppLoading onError={console.warn} startAsync={restoreUser} onFinish={()=> setReady(true)}/>
  }
  return (
   <AuthContext.Provider value={{user,setUser}}>
    <OfflineNotice/>
    
     <NavigationContainer>
     {user ? <MainNavigator /> : <AuthNavigator />}
     </NavigationContainer>
   </AuthContext.Provider>
  );
}


