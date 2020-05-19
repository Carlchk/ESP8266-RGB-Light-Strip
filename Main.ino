#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include "shared.h"
#ifdef __AVR__
#include <avr/power.h> 
#endif

#define LED_PIN    1
#define LED_COUNT 144
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html lang="zh-hk">

<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="description" content="A front-end template that helps you build fast, modern mobile web apps.">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0">
  <title>Dashboard</title>

  <!-- Add to homescreen for Chrome on Android -->
  <meta name="mobile-web-app-capable" content="yes">

  <!-- Add to homescreen for Safari on iOS -->
  <meta name="apple-mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-status-bar-style" content="black">
  <meta name="apple-mobile-web-app-title" content="Material Design Lite">

  <!-- Tile icon for Win8 (144x144 + tile color) -->
  <meta name="msapplication-TileImage" content="images/touch/ms-touch-icon-144x144-precomposed.png">
  <meta name="msapplication-TileColor" content="#3372DF">

  <script src="https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js"
    integrity="sha256-CJWfUCeP3jLdUMVNUll6yQx37gh9AKmXTRxvRf7jzro=" crossorigin="anonymous"></script>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>


  <link rel="stylesheet"
    href="https://fonts.googleapis.com/css?family=Roboto:regular,bold,italic,thin,light,bolditalic,black,medium&amp;lang=en">
  <link rel="stylesheet" href="https://fonts.googleapis.com/icon?family=Material+Icons">
  <link rel="stylesheet" href="https://code.getmdl.io/1.3.0/material.cyan-light_blue.min.css">
  <link rel="stylesheet"
    href="{Firebase-API key}">
</head>

<body>
  <script>
    //build: 1.0.1e
    function clearRadioGroup() {
      var radio = document.getElementsByName("light_mode");
      for (var i = 0; i < radio.length; i++) {
        radio[i].parentNode.MaterialRadio.uncheck();
      }
    }

    function DateConvert(timestamp) {
      var date = new Date(timestamp * 1000);
      var hours = date.getHours();
      var xxx = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate();
      var minutes = "0" + date.getMinutes();
      var seconds = "0" + date.getSeconds();     
      var formattedTime = xxx + " " + hours + ':' + minutes.substr(-2) + ':' + seconds.substr(-2);
      //formattedTime = formattedTime.toString();
      console.log(formattedTime);
      return formattedTime;
    }

    function checkDesk(address) {
      var today = new Date();
      var date = today.getFullYear() + '-' + (today.getMonth() + 1) + '-' + today.getDate();
      var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
      var dateTime = date + ' ' + time;
      if (address.substring(0, 3) == "rgb") {
        address = address.replace(/[^0-9\.,]+/g, "");
        var temp = address.split(',');
        var R = temp[0];
        var G = temp[1];
        var B = temp[2];
        address = "RGB?R=" + R + "&G=" + G + "&B=" + B;
      }
      console.log(address);
      var db = firebase.firestore();
      var docRef = db.collection("").doc("");
      docRef.set({
          status: address,
          t: new Date()
        })
        .then(function () {
          console.log("Document successfully written!");
        })
        .catch(function (error) {
          console.error("Error writing document: ", error);
        });
      document.getElementById("myIframe").src = address;

    }

    function init() {
      var db = firebase.firestore();
      var DataGot;
      db.settings({
        timestampsInSnapshots: true
      });
      var status = document.getElementById("status");
      var docRef = db.collection("").doc("");
      docRef.get().then(function (doc) {
        if (doc.exists) {
          DataGot = doc.data();
          console.log(DataGot);
          status.innerHTML = "Last Update: " + DateConvert(DataGot["t"]);
        } else {
          console.log("No such document!");
        }
      }).catch(function (error) {
        console.log("Error getting document:", error);
      });
    }


    function update(picker) {
      //console.log(picker.toRGBString());
      clearRadioGroup();
      checkDesk(picker.toRGBString());
    }
  </script>
  <div class="demo-layout mdl-layout mdl-js-layout mdl-layout--fixed-drawer mdl-layout--fixed-header">
    <header class="demo-header mdl-layout__header mdl-color--grey-100 mdl-color-text--grey-600">
      <div class="mdl-layout__header-row">
        <span class="mdl-layout-title">Dashboard</span>
        <div class="mdl-layout-spacer"></div>
        <div class="mdl-textfield mdl-js-textfield mdl-textfield--expandable">
          <label class="mdl-button mdl-js-button mdl-button--icon" for="search">
            <i class="material-icons">search</i>
          </label>
          <div class="mdl-textfield__expandable-holder">
            <input class="mdl-textfield__input" type="text" id="search">
            <label class="mdl-textfield__label" for="search">Enter your query...</label>
          </div>
        </div>
        <button class="mdl-button mdl-js-button mdl-js-ripple-effect mdl-button--icon" id="hdrbtn">
          <i class="material-icons">more_vert</i>
        </button>
        <ul class="mdl-menu mdl-js-menu mdl-js-ripple-effect mdl-menu--bottom-right" for="hdrbtn">
          <li class="mdl-menu__item">About</li>
          <li class="mdl-menu__item">Contact</li>
          <li class="mdl-menu__item">Legal information</li>
        </ul>
      </div>
    </header>
    <div class="demo-drawer mdl-layout__drawer mdl-color--blue-grey-900 mdl-color-text--blue-grey-50">
      <header class="demo-drawer-header">
        <img src="https://visualpharm.com/assets/381/Admin-595b40b65ba036ed117d3b23.svg" class="demo-avatar">
        <div class="demo-avatar-dropdown">
          <span>Admin</span>
          <div class="mdl-layout-spacer"></div>

        </div>
      </header>
      <nav class="demo-navigation mdl-navigation mdl-color--blue-grey-800">
        <a class="mdl-navigation__link" href=""><i class="mdl-color-text--blue-grey-400 material-icons"
            role="presentation">home</i>Home</a>
        <div class="mdl-layout-spacer"></div>
      </nav>
    </div>
    <main class="mdl-layout__content mdl-color--grey-100">
      <div class="mdl-grid demo-content">
        <div class="demo-cards mdl-cell mdl-cell--4-col mdl-cell--8-col-tablet mdl-grid mdl-grid--no-spacing">
          <div
            class="Lighting-cards mdl-card mdl-shadow--2dp mdl-cell mdl-cell--4-col mdl-cell--4-col-tablet mdl-cell--12-col-desktop">
            <div class="mdl-card__title mdl-card--expand mdl-color--teal-300">
              <h2 class="mdl-card__title-text">Lighting Control</h2>
            </div>
            <div class="mdl-card__supporting-text mdl-color-text--grey-600">
              <!-- #region -->
              <ul class="demo-list-control mdl-list">
                <li class="mdl-list__item">
                  <span class="mdl-list__item-primary-content">
                    White
                  </span>
                  <span class="mdl-list__item-secondary-action">
                    <label class="demo-list-radio mdl-radio mdl-js-radio mdl-js-ripple-effect" for="list-option-1">
                      <input type="radio" id="list-option-1" class="mdl-radio__button" name="light_mode" value="White"
                        onchange="checkDesk(this.value)" />
                    </label>
                  </span>
                </li>
                <li class="mdl-list__item">
                  <span class="mdl-list__item-primary-content">
                    Rainbow
                  </span>
                  <span class="mdl-list__item-secondary-action">
                    <label class="demo-list-radio mdl-radio mdl-js-radio mdl-js-ripple-effect" for="list-option-2">
                      <input type="radio" id="list-option-2" class="mdl-radio__button" name="light_mode" value="Rainbow"
                        onchange="checkDesk(this.value)" />
                    </label>
                  </span>
                </li>
                <li class="mdl-list__item">
                  <span class="mdl-list__item-primary-content">
                    Off
                  </span>
                  <span class="mdl-list__item-secondary-action">
                    <label class="demo-list-radio mdl-radio mdl-js-radio mdl-js-ripple-effect" for="list-option-3">
                      <input type="radio" id="list-option-3" class="mdl-radio__button" name="light_mode"
                        onchange="checkDesk(this.value)" value="Off" />
                    </label>
                  </span>
                </li>
                <li class="mdl-list__item">
                  <span class="mdl-list__item-primary-content">
                    <!--<span id="rgb-str"></span><br/>-->
                  </span>
                  <input class="mdl-textfield__input jscolor {width:243 , height:150, position:'bottom',
                  borderColor:'#FFF', insetColor:'#FFF'}" onchange="update(this.jscolor)">
                </li>
              </ul>
              <!-- #region -->
            </div>
          </div>

          <div class="demo-separator mdl-cell--1-col"></div>
          <div
            class="demo-options mdl-card mdl-color--deep-purple-500 mdl-shadow--2dp mdl-cell mdl-cell--4-col mdl-cell--3-col-tablet mdl-cell--12-col-desktop">
            <div class="mdl-card__supporting-text mdl-color-text--blue-grey-50">
              <h3>TV options</h3>
              <ul>
                <li>
                  <label for="chkbox1" class="mdl-checkbox mdl-js-checkbox mdl-js-ripple-effect">
                    <input type="checkbox" id="chkbox1" class="mdl-checkbox__input">
                    <span class="mdl-checkbox__label">Test 1</span>
                  </label>
                </li>
                <li>
                  <label for="chkbox2" class="mdl-checkbox mdl-js-checkbox mdl-js-ripple-effect">
                    <input type="checkbox" id="chkbox2" class="mdl-checkbox__input">
                    <span class="mdl-checkbox__label">Test 2</span>
                  </label>
                </li>
                <li>
                  <label for="chkbox3" class="mdl-checkbox mdl-js-checkbox mdl-js-ripple-effect">
                    <input type="checkbox" id="chkbox3" class="mdl-checkbox__input">
                    <span class="mdl-checkbox__label">Test 3</span>
                  </label>
                </li>
                <li>
                  <label for="chkbox4" class="mdl-checkbox mdl-js-checkbox mdl-js-ripple-effect">
                    <input type="checkbox" id="chkbox4" class="mdl-checkbox__input">
                    <span class="mdl-checkbox__label">Test 4</span>
                  </label>
                </li>
              </ul>
            </div>
            <div class="mdl-card__actions mdl-card--border">
              <a href="#" class="mdl-button mdl-js-button mdl-js-ripple-effect mdl-color-text--blue-grey-50">Change
                location</a>
              <div class="mdl-layout-spacer"></div>
              <i class="material-icons">location_on</i>
            </div>
          </div>
        </div>
      </div>
      <iframe id="myIframe" width="25" height="25" frameBorder="0" style="visibility: hidden"></iframe>
    </main>
  </div>
  <script src="https://code.getmdl.io/1.3.0/material.min.js"></script>
  <script src="https://www.gstatic.com/firebasejs/6.0.2/firebase-app.js"></script>
  <script src="https://www.gstatic.com/firebasejs/6.0.2/firebase-firestore.js"></script>
  <script>
    var firebaseConfig = {
      apiKey: "",
      authDomain: "", //firebase API configuration
      databaseURL: "",
      projectId: "",
      storageBucket: "",
      messagingSenderId: "",
      appId: ""
    };
    // Initialize Firebase
    firebase.initializeApp(firebaseConfig);
  </script>
</body>

</html>
)=====";

#define LED 2  

const char* ssid = ""; 
const char* password = "";

ESP8266WebServer server(80); 
void LEDrainbow() {
  Serial.println("LED mode - Rainbow");
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); 
    delay(2);  
  }
}

void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}

void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; 
 server.send(200, "text/html", s);
}

void handleLEDon() { 
 Serial.println("LED mode - white");
 colorWipe(strip.Color(255, 255, 255)); // White
}

void handleLEDoff() { 
 Serial.println("LED mode - off");
 colorWipe(strip.Color(0,   0,   0)); //LED off
 digitalWrite(LED_PIN, LOW);
}

void handleRGBArg() {
    int R,G,B;
    R = server.arg("R").toInt();
    G = server.arg("G").toInt();
    B = server.arg("B").toInt();
   
    colorWipe(strip.Color(R,G,B));
}

void setup(void){
  Serial.begin(9600);
  WiFi.begin(ssid, password);  
  Serial.println("");
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pinMode(LED,OUTPUT); 
  digitalWrite(LED,HIGH);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
 
  server.on("/", handleRoot);      
  server.on("/White", handleLEDon); 
  server.on("/Off", handleLEDoff);
  server.on("/Rainbow", LEDrainbow);
  server.on("/RGB", handleRGBArg);   


  server.begin();                  
  Serial.println("HTTP server started");
  strip.begin();           
  strip.show();            
  strip.setBrightness(50); 
}

void loop(void){
  server.handleClient();     
}
