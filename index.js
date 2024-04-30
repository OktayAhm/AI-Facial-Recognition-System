import { initializeApp } from 'https://www.gstatic.com/firebasejs/8.2.2/firebase-app.js';
import { getFirestore, collection, getDocs } from 'https://www.gstatic.com/firebasejs/8.2.2/firebase-auth.js';
// Follow this pattern to import other Firebase services
// import { } from 'firebase/<service>';

// TODO: Replace the following with your app's Firebase project configuration
const firebaseConfig = {
    
    authDomain: "face-rec-dd032.firebaseapp.com",
    databaseURL: "https://face-rec-dd032-default-rtdb.firebaseio.com/",
    projectId: "face-rec-dd032",
    storageBucket: "face-rec-dd032.appspot.com"
};

const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

// Get a list of cities from your database
async function getCities(db) {
  const citiesCol = collection(db, 'cities');
  const citySnapshot = await getDocs(citiesCol);
  const cityList = citySnapshot.docs.map(doc => doc.data());
  return cityList;
}

 var d  = new Date();
          var datestring = d.getFullYear()+"-"+("0"+(d.getMonth()+1)).slice(-2)+ "-"+("0" + d.getDate()).slice(-2) ;
          var refR = firebase.database().ref().child(datestring);
          refR.on("child_added",snap=>{
          var name = snap.child("Name").val();
          var time = snap.child("Time").val();
          //console.log(name);
          var table = document.getElementById("main_table");
          var row = table.insertRow(-1);
          var cell1 = row.insertCell(-1);
          var cell2 = row.insertCell(-1);
          cell1.innerHTML = name;
          cell2.innerHTML = time;
          });