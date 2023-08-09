import { useContext } from "react";
import AuthContext from "./context";
import authStorage from './storage';
import Firebase from "../config/firebase";



export default useAuth = () => {
    const {user,setUser} = useContext(AuthContext);
    const db = Firebase.firestore()



    const logout = () => {
        setUser(null);
        authStorage.removeData();
    }

    const logIn = (user) => {
            db.collection('balance_app').doc(user.uid).get()
            .then(snapshot =>
                setUser(snapshot.data())
            );

            authStorage.storeData(user);
            setUser(user);
    }

    return {user,setUser,logout,logIn};
  }
   
