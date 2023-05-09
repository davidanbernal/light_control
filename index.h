const char MAIN_page[] PROGMEM = R"=====( 
  <!DOCTYPE html><html lang=en ontouchmove>
  <head>
  <title>Ebm Electronics</title>
  <meta name='viewport' content='user-scalable=yes,initial-scale=1.0,width=device-width'>
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <script src="https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js"></script>
  <script>

    function updateSliderPWM(element) {
      var sliderName = element.id;
      var sliderValue = document.getElementById(element.id).value;
      var groupValue = document.getElementById("group").value;
     // console.log(sliderName);
      var xhr = new XMLHttpRequest();
      
      xhr.open("GET", "/setPWM?PWMval="+groupValue +","+sliderName+","+sliderValue, true);
      //xhr.open("GET", "http://localhost:1880/setPWM?PWMval="+groupValue +","+sliderName+","+sliderValue, true);
      xhr.send();
      }
    

    function update(picker) {
    document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);    
    var groupValue = document.getElementById("group").value;
    var rgbValuea = document.getElementById("change_color").href= Math.round(picker.rgb[0]) + "," +  Math.round(picker.rgb[1]) + "," + Math.round(picker.rgb[2]) ;//rgb
    //console.log(groupValue);
    //console.log(rgbValuea);
    var xhr = new XMLHttpRequest();
   // xhr.open("GET", "http://localhost:1880/setRGBW?RGBWval="+groupValue+","+rgbValuea, true);
    xhr.open("GET", "/setRGBW?RGBWval="+groupValue+","+rgbValuea, true);
    xhr.send();
    }

    function updateButton(element) {
    var buttonName = element.id;
    var buttonValue = document.getElementById(element.id).value;
    var groupValue = document.getElementById("group").value;
    //console.log(buttonName);
    //console.log(buttonValue);
    //console.log(groupValue);
    var xhr = new XMLHttpRequest();
   // xhr.open("GET", "http://localhost:1880/setButton?Buttonval="+groupValue +","+buttonName+","+buttonValue, true);
    xhr.open("GET", "/setButton?Buttonval="+groupValue +","+buttonName+","+buttonValue, true);
    xhr.send();
    }
    
    </script>

  <style>
  body{max-width:85%;margin:0 auto;font-family:arial;font-size:105%;text-align:center;color:#757575;background-color:#fFfFfF;}
  ul{list-style-type:none;margin:0.1em;padding:0;border-radius:0.375em;overflow:hidden;background-color:#ffffff;font-size:1em;}
  li{float:left;border-radius:0.375em;border-right:0.06em solid #bbb;}last-child {border-right:none;font-size:85%}
   li a{display: block;border-radius:0.375em;padding:0.44em 0.44em;text-decoration:none;font-size:85%}
  li a:hover{background-color:#EAE3EA;border-radius:0.375em;font-size:85%}
  section {font-size:0.88em;}
  h1{color:white;border-radius:0.5em;font-size:1em;padding:0.2em 0.2em;background:#558ED5;}
  h2{color:orange;font-size:1.0em;}
  h3{font-size:0.8em;}
  table{font-family:arial,sans-serif;font-size:0.9em;border-collapse:collapse;width:85%;}
  th,td {border:0.06em solid #dddddd;text-align:left;padding:0.3em;border-bottom:0.06em solid #dddddd;}
  tr:nth-child(odd) {background-color:#eeeeee;}
  .rcorners_n {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:20%;color:white;font-size:75%;}
  .rcorners_m {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:50%;color:white;font-size:75%;}
  .rcorners_w {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:70%;color:white;font-size:75%;}
  .column{float:left;width:50%;height:45%;}
  .row:after{content:'';display:table;clear:both;}
  *{box-sizing:border-box;}
  footer{color:#ffffff; background-color:#558ED5; text-align:center;padding:0.3em 0.3em;border-radius:0.375em;font-size:80%;}
  .button{color:#757575; border-color: #558ED5; background-color:white;}
   .button:hover{border-radius:2px; background-color:#558ED5;color: #fFfFfF;  cursor: pointer;}
 /*button{border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;color:white;font-size:130%;}
 .buttonsm{border-color: #fFfFfF; border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:9%; color:white;font-size:70%;}
  .buttonm {border-color: #fFfFfF; border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:15%;color:white;font-size:70%;}
  .buttonw {border-color: #fFfFfF; border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:40%;color:white;font-size:70%;}*/
  .sliderw { -webkit-appearance: none; margin: 20px; width: 15rem; height: 1rem;border-radius:0.5em;  overflow: hidden; transition: opacity .2s;}
    .sliderw::-webkit-slider-thumb { -webkit-appearance: none;width: 1rem; height: 1rem; background: black; cursor: ew-resize;  box-shadow: -10rem 0 0 10rem #676a6e ;}            
    .sliderw::-webkit-slider-runnable-track {   background: linear-gradient(to right, #293043, #293043), #cccec3d7;  background-size: var(--background-size, 0%) 100%;  background-repeat: no-repeat; }

    .sliderr { -webkit-appearance: none; margin: 20px; width: 15rem; height: 1rem;border-radius:0.5em;  overflow: hidden; transition: opacity .2s;}
    .sliderr::-webkit-slider-thumb { -webkit-appearance: none;width: 1rem; height: 1rem; background: black; cursor: ew-resize;  box-shadow: -10rem 0 0 10rem red ;}            
    .sliderr::-webkit-slider-runnable-track {   background: linear-gradient(to right, #293043, #293043), #cccec3d7;  background-size: var(--background-size, 0%) 100%;  background-repeat: no-repeat; }
   
    .sliderg { -webkit-appearance: none; margin: 20px; width: 15rem; height: 1rem;border-radius:0.5em;  overflow: hidden; transition: opacity .2s;}
    .sliderg::-webkit-slider-thumb { -webkit-appearance: none;width: 1rem; height: 1rem; background: black; cursor: ew-resize;  box-shadow: -10rem 0 0 10rem green ;}            
    .sliderg::-webkit-slider-runnable-track {   background: linear-gradient(to right, #293043, #293043), #cccec3d7;  background-size: var(--background-size, 0%) 100%;  background-repeat: no-repeat; }

    .sliderb { -webkit-appearance: none; margin: 20px; width: 15rem; height: 1rem;border-radius:0.5em;  overflow: hidden; transition: opacity .2s;}
    .sliderb::-webkit-slider-thumb { -webkit-appearance: none;width: 1rem; height: 1rem; background: black; cursor: ew-resize;  box-shadow: -10rem 0 0 10rem blue ;}            
    .sliderb::-webkit-slider-runnable-track {   background: linear-gradient(to right, #293043, #293043), #cccec3d7;  background-size: var(--background-size, 0%) 100%;  background-repeat: no-repeat; }



/*    .slider { -webkit-appearance: none; margin: 20px; width: 15rem; height: 1rem;border-radius:0.5em;  overflow: hidden; 
            transition: opacity .2s;}
    .slider::-webkit-slider-thumb { -webkit-appearance: none;width: 1rem; height: 1rem; background: black; cursor: ew-resize;  box-shadow: -10rem 0 0 10rem #43e5f7 ;}
    .slider::-webkit-slider-runnable-track {   background: linear-gradient(to right, #293043, #293043), #f6fa02;  background-size: var(--background-size, 0%) 100%;  background-repeat: no-repeat; }
        .slider::-webkit-slider-thumb::before { height: 10px;  margin-top: -1px;   position: absolute; background: #003249; content: ''; }

 .slider { -webkit-appearance: none; margin: 20px; width: 60%; height: 2rem; background: #FFD65C; overflow: hidden; accent-color: #F55050;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
   .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 1rem; height: 2rem; background: #003249; cursor: pointer; }
    .slider::-moz-range-thumb { width: 60%; height: 25px; background: #003249; cursor: pointer; } 
    .slider::-webkit-slider-runnable-track { height: 10px; -webkit-appearance: none;color: #13bba4;  margin-top: -1px;   }
    .slider::-ms-fill-lower{background: #2349b1;}
   .slider::-ms-fill-lower{background: #2349b1;}*/
  a{font-size:75%;}
  p{font-size:75%;}
  </style>
  </head><body tabIndex=0>
    <h1>Ebm Electronics </h1>
<h3 class='rcorners_m'>Control Dimmer RGBW</h3><br>
<CENTER> 

    <hr noshade>
<div>
  <B>Grupos </B> 
  <select id=group  class=button  name="select">
    <option value="G1" selected>Todo</option>
    <option value="G2" >Superior</option>
    <option value="G3">Arco</option>
    <option value="G4">Plantas</option>
    
    
  </select>  <br><br>
    <B>Output 1 - White</B> <br> <input class="sliderw" type="range" value="%vchannelWSlider%" min="0" max="100" id="cW" onchange="updateSliderPWM(this)"></input><output class="bubble"></output><br><br>
    <B>Output 2 - Red</B>   <br> <input class="sliderr" type="range" value="%vchannelRSlider%" min="0" max="100" id="cR" onchange="updateSliderPWM(this)"></input><output class="bubble"></output><br><br>
    <B>Output 3 - Green</B> <br> <input class="sliderg" type="range" value="%vchannelGSlider%" min="0" max="100" id="cG" onchange="updateSliderPWM(this)"></input><output class="bubble"></output><br><br>
    <B>Output 4 - Blue</B>  <br> <input class="sliderb" type="range" value="%vchannelBSlider%" min="0" max="100" id="cB" onchange="updateSliderPWM(this)"></input><output class="bubble"></output><br><br>
    
   
    <hr noshade>
  </div>

<div id="smartleds" >
  
  <B href="#" id="change_color">Color Picker </B> <br>
  
<input class="jscolor {onFineChange:'update(this)'}" id="rgb"><br><br>
<button class="button" name=botona value="1" id="buttonON" onclick="updateButton(this)">ALL ON</button> <button class="button" name=botona value="0" id="buttonOFF" onclick="updateButton(this)">ALL OFF</button> <br><br>
<hr noshade> 
</div>


<br>
<CENTER><A href= "http://ebmelectronics.com">www.ebmelectronics.com</a></CENTER> 
 </CENTER> 

<footer>&copy; Electrobiomedical 2023</footer>
  
  
  </body></html>
  )====="; 