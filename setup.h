const char SETUP_page[] PROGMEM = R"=====( 
  <!DOCTYPE html><html>
  <head>
  <title>Ebm Electronics</title>
  <meta name='viewport' content='user-scalable=yes,initial-scale=1.0,width=device-width'>
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
  table{font-family:arial,sans-serif;font-size:0.9em;border-collapse:collapse;width:95%;}
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
  a{font-size:75%;}
  p{font-size:75%;}
  </style></head><body>
    <h1>Ebm Electronics </h1>
<h3 class='rcorners_m'>Configuration</h3><br>
<CENTER> 
<FORM method="post" action="/form"> 
<TABLE> 
<TR><TD colspan=2><B style=color:black>Device: <B>@@NAMEDIS@@</B></B></TD></TR> 
<TR><TD colspan=2><B style=color:black>Serial: </B><B>@@ESPID@@</B></TD></TR> 
<TR><TD colspan=2></TD></TR> 
<TR><TD><B style=color:black>MAC:</B> @@ESPMAC@@</TD><TD><B style=color:black>Status</B></TD></TR> 
<TR><TD colspan=2></TD></TR> 

<TR><TD><B>WIFI Network</B> &nbsp;&nbsp;<INPUT class=button TYPE=TEXT name=ssidnet  >   </TD> <TD>@@L3@@</TD></TR> 
<TR><TD colspan=2><B>Password</B> &emsp;&emsp;<INPUT class=button TYPE=PASSWORD name=ssidpass  minlength="8" > &emsp;<INPUT class=button TYPE=SUBMIT VALUE="SAVE" name=savenet >  </TD> </TR> 

<TR><TD colspan=2><B>Restart Device</B> &emsp; <INPUT class=button TYPE=SUBMIT VALUE="RESTART" name=saveres >  </TD> </TR> 


<TR><TD colspan=2><CENTER><A href= "http://ebmelectronics.com">www.ebmelectronics.com</a></CENTER></TD></TR> </TABLE> </FORM>
 </CENTER> 

<footer>&copy; Electrobiomedical 2023</footer>
  
  
  </body></html>
)====="; 
