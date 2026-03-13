let userId = localStorage.getItem("userId");

function startCamera(videoId){

    const video = document.getElementById(videoId);
    
    if(!video){
    console.log("Video element not found:", videoId);
    return;
    }
    
    if(!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia){
    alert("Camera API not supported in this browser");
    return;
    }
    
    navigator.mediaDevices.getUserMedia({
    video:{width:640,height:480},
    audio:false
    })
    .then(stream=>{
    video.srcObject = stream;
    video.play();
    })
    .catch(err=>{
    console.log("Camera error:",err);
    alert("Camera permission blocked or camera unavailable");
    });
    
    }


// REGISTER
async function registerUser(){

let name=document.getElementById("regName").value;
let email=document.getElementById("regEmail").value;
let password=document.getElementById("regPassword").value;

let res=await fetch("/auth/register",{

method:"POST",
headers:{"Content-Type":"application/json"},
body:JSON.stringify({name,email,password})

});

let data=await res.json();

alert(data.message);

window.location="/login.html";

}


// LOGIN
async function loginUser(){

let email=document.getElementById("loginEmail").value;
let password=document.getElementById("loginPassword").value;

let res=await fetch("/auth/login",{

method:"POST",
headers:{"Content-Type":"application/json"},
body:JSON.stringify({email,password})

});

let data=await res.json();

if(data.user_id){

localStorage.setItem("userId",data.user_id);

window.location="/dashboard.html";

}

}


// CAPTURE FACE
async function captureFace(){

let video=document.getElementById("camera");

let canvas=document.createElement("canvas");

canvas.width=video.videoWidth;
canvas.height=video.videoHeight;

let ctx=canvas.getContext("2d");

ctx.drawImage(video,0,0);

let blob=await new Promise(resolve=>canvas.toBlob(resolve,"image/jpeg"));

let formData=new FormData();

formData.append("file",blob,"face.jpg");

await fetch("/users/"+userId+"/biometrics",{

method:"POST",
body:formData

});

alert("Face uploaded");

}


// BOOK FLIGHT
async function bookFlight(){

let flightId=document.getElementById("flightId").value;

await fetch("/bookings",{

method:"POST",
headers:{"Content-Type":"application/json"},
body:JSON.stringify({user_id:userId,flight_id:flightId})

});

alert("Flight booked");

loadFlights();

}


// LOAD FLIGHTS
async function loadFlights(){

let container=document.getElementById("flightCards");

if(!container) return;

let res=await fetch("/users/"+userId+"/flights");

let flights=await res.json();

container.innerHTML="";

flights.forEach(f=>{

let div=document.createElement("div");

div.className="ticket";

div.innerHTML=
`
<h3>✈ Flight ${f.flight_number}</h3>
<p><b>Destination:</b> ${f.destination}</p>
<p><b>Status:</b> ${f.status}</p>
`;

container.appendChild(div);

});

}


// GATE SCAN
async function scanPassenger(){

let result=document.getElementById("scanResult");

result.innerHTML="Scanning passenger...";

let res=await fetch("/scan",{method:"POST"});

let text=await res.text();

result.innerHTML=text;

}


// START CAMERAS
window.addEventListener("DOMContentLoaded",()=>{

startCamera("camera");
startCamera("gateCamera");
loadFlights();

});