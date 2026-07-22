// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getDatabase, push, ref, set } from "firebase/database";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyDuAeN0FrNvAHx0_rMP9F9JrJjiiFC_dO4",
  authDomain: "landbot-92fa9.firebaseapp.com",
  databaseURL: "https://landbot-92fa9-default-rtdb.firebaseio.com",
  projectId: "landbot-92fa9",
  storageBucket: "landbot-92fa9.firebasestorage.app",
  messagingSenderId: "216362459559",
  appId: "1:216362459559:web:937f69b81161ab5035d682"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase(app)
export async function pushDataWithFirebaseId(score, uuid) {
  if (!uuid) throw new Error("UUID is required");

  // Create a reference to the collection node
  const playerRef = ref(db, `players/${uuid}`, );
  
    await set(playerRef, {
      score
    });  
  // You can grab the generated ID if you need it
}