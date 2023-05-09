/*
   version para webserver  sin funciones smart, con udp
   no puede utilizarse alexa con esta version
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
//#include <WS2812FX.h>
#include "RTClib.h"
#include <RCSwitch.h>
  #include "StringSplitter.h"


//WEB pages
#include "index.h"
#include "setup.h"


//UDP
unsigned int localUdpPort = 8394;  // local port to listen on
unsigned int remoteUdpPort = 1112;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  remotaIP[] = "192.168.0.44";  // a reply string to send back
char  remotaIPB[] = "255.255.255.255";  // a reply string to send back
char  replyPacket[] = "#EBMOK";  // a reply string to send back
WiFiUDP Udp;


//-------------------VARIABLES GLOBALES--------------------------
int contconexion = 0;
unsigned long previousMillis = 0;

//String borrado,ubicacion,currenttime,intermax,intermin,intermins,intermaxs,strlectura,estadoala,ssidnet, ssidpass,ssidnets, ssidpasss,udpportc,udpipc,udpports,udpips,redlocal,iplocal;

int scheduleusertc,senAnadig,anRangeInf,anRangeSup,actSensor,sensorStatus,conMode,salidaActivada,timeunit,tiempoApagar,stimon,conout,funcionactivada,epochActual;
String flancoInp,timeOn,timeOff,currenttime,horascn, pwm_val,rgbw_val,button_val,item1,item0, item2, item3,canal;

String sensor_act, valorpwm_A, valorpwm_B, comfun;
String inSerial, relayEnb="0", sensorEnb="0";
int conectado = 0,statusntp =0,contaserverntp=0;
bool verificar = true,hayserverntp=true;
String func = "0", funNum, funTim, funBri;
String typeSensor="0", grupo="G1";

//IDs
String ChipId = String(ESP.getChipId(), HEX); //MAC del dispositivo
String tipoDvc = "ebm-C_4C01SC1_";//tipo de dispositivo mostrado en mdns
String firmDvc = "ebm-S_0.1.23"; //version firmware
String versDvc = "0.1"; //version de hardware
String infoDvc = "Controller light, 4-Channels 0-10V, 2-Relays, Smartled, Sensor, RTC, COM"; //info dispositivo
String addrDMX = "000";
String newHostname = "ebmControl";

//PROPIEDADES CONTROLADORA EBM
//CONTROLADORA 2-SALIDAS 0-10V, 2-RELES, SMARTLED, COM UART, RTC, 1-LED, 1-PULSADOR, 1-SENSOR

//VARIABLES WIFI
char ssid[50];
char pass[50];
const char *passConf = "ebm12345";
String ssidConf = String("ebmSmartControl-") + ChipId;
String mensaje = "";


//firmware config
String host = "ebmSmart_" + tipoDvc + ChipId;
const char* update_path = "/firmware";
const char* update_username = "admin";
const char* update_password = "ebm12345";

//SERVICIO NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

//DEFINICION PIN INPUT RF
//const int pinRF = 0;

//DEFINICION PIN SENSOR
const int sensor = A0;
double sensor_value=0;

//DEFINICION PINES PWM
#define ledred 5
#define ledgreen 16
#define ledblue 12
#define ledwhite 13
#define ledwarm1 5
#define ledcool1 16
#define ledwarm2 12
#define ledcool2 13

//DEFINICIONES PINES RELES
#define rele1 5
//#define rele2
//#define rele3
//#define rele4
//#define rele5
//#define rele6
//#define rele7
//#define rele8


//DEFINICIONES PINES RELES
#define led 15
#define pulsador 0
#define LED      2

//------------INSTANCIAS--------------------------------------------------
RTC_PCF8563 rtc;
WiFiClient espClient;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
RCSwitch mySwitch = RCSwitch();

//--------------------------------------------------------------



//------------------------SETUP-----------------------------
void setup() {


  // Inicia Serial
  Serial.begin(115200); //9600 PARA COMUNICACION UART/RS485 A SENSORES/TECLADOS/OTROS
  EEPROM.begin(1024);
  Serial.println("");
  Serial.println("INICIANDO");

  //INICIAR PINES
  pinMode(led, OUTPUT); // led
  pinMode(pulsador, INPUT);  //pulsador
  pinMode(rele1, OUTPUT); //rele1

  //INICIA RX MODULO RF
  //mySwitch.enableReceive(pinRF);  // Receiver on interrupt 0 => that is pin #2

  //INICIAR N ULTIMOS VALORES en memoria
  typeSensor=leer(290,2);
  //valorpwm_A=leer(100);
  //valorpwm_B=leer(150);
  //analogWriteFreq(new_frequency);  //Cambiar frecuencia por defecto de 1KHz
   analogWrite(ledwarm2, leer(245,3).toInt());
   String valcalidoss=leer(245,3);
   Serial.println("valor calid: "+valcalidoss);
  analogWrite(ledcool2, leer(248,3).toInt());
   String valfriooss=leer(248,3);
   Serial.println("valor frio: "+valfriooss);

   if(leer(245,3).toInt()<=10||leer(248,3).toInt()<=10){
    digitalWrite(rele1, LOW);
    }
   else{digitalWrite(rele1, HIGH);} 
//Serial.println("valor frio",leer(248,3));

  
  //******  INICIAR UDP   *****************
  Udp.begin(localUdpPort);


  
  //INICIAR SERVER
  // server.on("/", main); //esta es la pagina de configuracion
  //  server.on("/setup", config); //esta es la pagina de configuracion

   server.on("/",      handleRoot);
 // server.on("/download", File_Download);
  server.on("/config", handleSetup);
  server.on("/form", handleForm); 
  server.on("/setPWM", handle_pwm);
  server.on("/setRGBW", handle_rgbw);
  server.on("/setButton", handle_button);
  
  //server.on("/", paginaconf); //esta es la pagina de configuracion
 // server.on("/guardar_conf", guardar_conf); //Graba en la eeprom la configuracion
  //server.on("/escanear", escanear); //Escanean las redes wifi disponibles
  //if (!MDNS.begin("ebmSmart_"+tipoDvc+ChipId)) {             // Start the mDNS responder for esp8266.local
  if (!MDNS.begin(host)) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error iniciando MDNS !");
  }
  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS responder started");
  server.begin();
  WiFi.hostname(newHostname.c_str());
  //ingrese a configuracion wifi durante 1 min
  // INCIAR WIFI
  leer(0, 50).toCharArray(ssid, 50);
   leer(50, 50).toCharArray(pass, 50);
  Serial.println("mode " + String(WiFi.getMode()));
  Serial.println("ssidi " + String(ssid));
  Serial.println("ssidp " + String(pass));
  modoconf();
  //Inicie RED
 setup_wifi();

  //Serial.println("entre al loop");



}

//--------------------------LOOP--------------------------------
void loop() {

  server.handleClient();
  MDNS.update();
  timeClient.update();
  //  Serial.println("entre al loop");
  //     Serial.println("WiFi conectado");
  //      Serial.println(WiFi.localIP());
  //       Serial.print("Connected to ");
  //  Serial.println(WiFi.SSID());
  delay(500);

  // PROCESO SENSOR

  //analogo 
  /*
if(senAnadig==0 )
  {
    sensor_value = analogRead(sensor);
    if (sensor_value >= anRangeInf && sensor_value <= anRangeSup) // entrada sensor  y habilitador
        {
          actSensor=1;
          if(sensorEnb=="1")
          {
          String typeSensor=leer(290,2);
            int sensorStasensor_value = analogRead(sensor);
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.print("#01302029"+typeSensor+String(sensorStatus) + "_" + ChipId + "%");
            Udp.endPacket();
          }
        }
    }

//digital
if(senAnadig==1)
  {
    sensor_value = analogRead(sensor);
    if (sensor_value >= 255 && flancoInp=="1") // entrada sensor  y habilitador
        {
          actSensor=1;
          if(sensorEnb=="1")
          {
          
            int sensorStasensor_value = analogRead(sensor);
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.print("#01302029"+typeSensor+String(sensorStatus) + "_" + ChipId + "%");
            Udp.endPacket();
          }
        }
      if (sensor_value <= 25 && flancoInp=="0") // entrada sensor  y habilitador
        {
          actSensor=1;
          if(sensorEnb=="1")
          {
         
            int sensorStasensor_value = analogRead(sensor);
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.print("#01302029"+typeSensor+String(sensorStatus) + "_" + ChipId + "%");
            Udp.endPacket();
          }
        }   
    }


   //PROCESO Control

if(conMode==0)//schedule enciende a la hora guardada y apaga cuando se cumpla la hora
       { 
         int currentHour = timeClient.getHours();
         int currentMinute = timeClient.getMinutes();
         String currentime=String(currentHour)+":"+String(currentMinute);
        if(timeOn==currentime)
          {}
         if(timeOff==currentime)
          {} 
        //obtener la hora actual
        //es hora de encendido?
        //es hora de apagado?

        
        }
else if (conMode==1)//timer enciende el tiempo configurado
        {
          int epochActual = timeClient.getEpochTime();
         // int stimonc=stimon.toInt();
          //sensor se a activado?
          // ya paso el tiempo?
          
          if(actSensor==1)
            {
            salidaActivada=1;
            if(timeunit=1){ tiempoApagar=epochActual+(60*stimon);}//minutos
            else if(timeunit=0){tiempoApagar=epochActual+(stimon);}//segundos
            if(conout==1)//pwm
              {//func = "0"; grabar(280, 1, "0");
                if (funcionactivada==1)
                {
                  //ejecutar funcion
                  
                  }
                else{  
                    analogWrite(ledwarm2, leer(245,3).toInt()); 
                    analogWrite(ledcool2, leer(248,3).toInt()); 
                    analogWrite(ledwarm1, leer(239,3).toInt()); 
                    analogWrite(ledcool1, leer(242,3).toInt()); 
                    analogWrite(ledred, leer(227,3).toInt()); 
                    analogWrite(ledgreen, leer(230,3).toInt()); 
                    analogWrite(ledblue, leer(233,3).toInt()); 
                    analogWrite(ledwhite, leer(236,3).toInt());
                    digitalWrite(rele1, leer(251,1).toInt()); 
                    //digitalWrite(rele2, LOW); grabar(252,1,"0");}
                  }
                }  
            else if(conout==2)//smart
              {
                //aqui va funciones de smart cuando se active timer
                }
              
            
            actSensor=0;
            }
          if(epochActual>=tiempoApagar && salidaActivada==1)
            {
             //detener funcion smart o pwm 
            analogWrite(ledwarm2, 0);
            analogWrite(ledcool2, 0);
            analogWrite(ledwarm1, 0);
            analogWrite(ledcool1, 0);
            analogWrite(ledred,   0);
            analogWrite(ledgreen, 0);
            analogWrite(ledblue,  0);
            analogWrite(ledwhite, 0);
            digitalWrite(rele1, LOW);
            //digitalWrite(rele2, LOW);
            
            
            salidaActivada=0;
            }
          
        }
        
*/
  //PROCESO SWITCH RF
/*
 * *buscar como matricularlos y asociarlos a las salidas
 */
 /*  if (mySwitch.available()) {

    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );

    mySwitch.resetAvailable();
  }

  //PROCESO  EJECUTAR FUNCION LED O SMARTbu
  if (func == "1")
  { Serial.println("entre a función");


    //    // Fill the dots one after the other with a color
    //void colorWipe(uint32_t c, uint8_t wait) {
    //  for(uint16_t i=0; i<20; i++) {
    //    strip.setPixelColor(i, c);
    //    strip.show();
    //    delay(wait);
    //  }
    //}

  //              for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 2)
          //                {
          //                user_pwm1_setvalue(fadeValue);
          //                 user_pwm2_setvalue(255-fadeValue);
          //                 HAL_Delay(50);
          //                }
          //
          //
          //              for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 2)
          //                  {
          //                  user_pwm1_setvalue(fadeValue);
          //                  user_pwm2_setvalue(255-fadeValue);
          //                  HAL_Delay(50);
          //                  }

  }


  //PROCESO RECIBIR UDP
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());

    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }

    //analogWrite(13, valorpwm_B.toInt());

    Serial.printf("UDP packs: %s\n", incomingPacket);
    
      if (incomingPacket[0] == '#' && incomingPacket[3] == '0' && incomingPacket[4] == '5') //inicio de trama y comando de controladora de iluminación
  {
    if (incomingPacket[5] == '0' && incomingPacket[6] == '5') //funcion broadcast
    {
      commands();
      }//broadcast


    else if (incomingPacket[5] == '1' && incomingPacket[6] == '4') //controladora especifica con serial
    {
      String valueS = String(incomingPacket[19]) + String(incomingPacket[20]) + String(incomingPacket[21])+String(incomingPacket[22]) + String(incomingPacket[23]) + String(incomingPacket[24])+String(incomingPacket[25]) + String(incomingPacket[26]) + String(incomingPacket[27])+String(incomingPacket[28]) + String(incomingPacket[29]) + String(incomingPacket[30]); //# serial
      inSerial = valueS;
      if(inSerial==ChipId)
        {
          commands();
        }
      
    }//controladora especifica

  }//trama

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(String(replyPacket) + "_" + ChipId + "%");
    Udp.endPacket();
  }
  //udp receive
*/
  /*unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 5000) { //envia la temperatura cada 5 segundos
   previousMillis = currentMillis;
   Serial.println("Funcionado...");
   }*/
}// cierre loop

void handle_pwm() {
  
 pwm_val = server.arg("PWMval"); // reading from slider on html pagae

 Serial.print("slider val: ");
 Serial.println(pwm_val);

 StringSplitter *splitter = new StringSplitter(pwm_val, ',', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
 
 
    item0 = splitter->getItemAtIndex(0); //grupo
    Serial.println("Item @ index " + String(0) + ": " + String(item0));
    item1 = splitter->getItemAtIndex(1); //slider name
    Serial.println("Item @ index " + String(1) + ": " + String(item1));
    item2 = splitter->getItemAtIndex(2); //slider value
    Serial.println("Item @ index " + String(2) + ": " + String(item2));

    if(item1=="cW"){canal="022";}
    if(item1=="cR"){canal="023";}
    if(item1=="cG"){canal="024";}
    if(item1=="cB"){canal="025";} 
    

    Udp.beginPacket(remotaIPB, remoteUdpPort);
    Udp.print("#01,4,05,"+canal+","+item0+","+item2+","+ ChipId + "%");
    Udp.endPacket();
    delay(200);
    Udp.beginPacket(remotaIPB, remoteUdpPort);
    Udp.print("#01,4,05,"+canal+","+item0+","+item2+","+ ChipId + "%");
    Udp.endPacket();
    
 server.send(200,"text/plane","0"); // handling the webpage update
}

void handle_rgbw() {
  
 rgbw_val = server.arg("RGBWval"); // reading from slider on html pagae

 Serial.print("rgbw val: ");
 Serial.println(rgbw_val);

 StringSplitter *splitter = new StringSplitter(rgbw_val, ',', 4);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
 
 
    item0 = splitter->getItemAtIndex(0); 
    Serial.println("Item @ index " + String(0) + ": " + String(item0));
    item1 = splitter->getItemAtIndex(1); 
    Serial.println("Item @ index " + String(1) + ": " + String(item1));
    item2 = splitter->getItemAtIndex(2); 
    Serial.println("Item @ index " + String(2) + ": " + String(item2));
    item3 = splitter->getItemAtIndex(3); 
    Serial.println("Item @ index " + String(3) + ": " + String(item3));
   
    
    Udp.beginPacket(remotaIPB, remoteUdpPort);
    Udp.print("#01,4,05,019,"+item0+","+item1+","+item2+","+item3+","+ ChipId + "%");
    Udp.endPacket();
    delay(200);
    Udp.beginPacket(remotaIPB, remoteUdpPort);
    Udp.print("#01,4,05,019,"+item0+","+item1+","+item2+","+item3+","+ ChipId + "%");
    Udp.endPacket();

 server.send(200,"text/plane","0"); // handling the webpage update
}

void handle_button() {
  
 button_val = server.arg("Buttonval"); // reading from slider on html pagae

 Serial.print("button val: ");
 Serial.println(button_val);

 StringSplitter *splitter = new StringSplitter(button_val, ',', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
 
 // Serial.println("Item count: " + String(itemCount));
    item0 = splitter->getItemAtIndex(0); 
    Serial.println("Item @ index " + String(0) + ": " + String(item0));
    item1 = splitter->getItemAtIndex(1); 
    Serial.println("Item @ index " + String(1) + ": " + String(item1));
    item2 = splitter->getItemAtIndex(2); 
    Serial.println("Item @ index " + String(2) + ": " + String(item2));
   
    
    Udp.beginPacket(remotaIPB, remoteUdpPort);
    Udp.print("#01,4,05,031,"+item0+","+item2+","+ ChipId + "%");
    Udp.endPacket();
    delay(200);
    Udp.beginPacket(remotaIPB, remoteUdpPort);
    Udp.print("#01,4,05,031,"+item0+","+item2+","+ ChipId + "%");
    Udp.endPacket();

 server.send(200,"text/plane","0"); // handling the webpage update
}

void func_command() {//funcion para guardar el comando de funcion pwm
  func = "1"; grabar(280, 1, "1");
  String valueF = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]); //# funcion
  String valueB = String(incomingPacket[13]) + String(incomingPacket[14]) + String(incomingPacket[15]); //# brillo
  String valueT = String(incomingPacket[16]) + String(incomingPacket[17]) + String(incomingPacket[18]); //# tiempo
  funNum = valueF;  grabar(281, 3, funNum);
  funBri = valueB; grabar(287, 3, funBri);
  funTim = valueT; grabar(284, 3, funTim);
  }

void func_commandS() {//funcion para guardar el comando de funcion smart
  func = "2"; grabar(280, 1, "2");
  String valueF = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]); //# funcion
  String valueB = String(incomingPacket[13]) + String(incomingPacket[14]) + String(incomingPacket[15]); //# brillo
  String valueT = String(incomingPacket[16]) + String(incomingPacket[17]) + String(incomingPacket[18]); //# tiempo
  funNum = valueF;  grabar(281, 3, funNum);
  funBri = valueB; grabar(287, 3, funBri);
  funTim = valueT; grabar(284, 3, funTim);
  
}

void commands() {
  
      if (incomingPacket[8] == '0' && incomingPacket[9] == '0') //comando estado de entradas y salidas
      {
        String staSens = "012sd";
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.print("#xx:" + staSens + ", MAC:" + ChipId + "%" );
        Udp.endPacket();
      }

      if (incomingPacket[8] == '0' && incomingPacket[9] == '1') //comando info device
      {
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.print("#xx:" + infoDvc + ", MAC:" + ChipId + ", VerHdw:" + versDvc + ", VerFir:" + firmDvc + ", Address DMX:" + addrDMX + ", TypeDvc:" + tipoDvc + "%" );
        Udp.endPacket();
      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '0') //comando led tunnable
      { func = "0"; grabar(280, 1, "0");
        if (incomingPacket[7] == '1') //canal 1
        {
          String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
          int valuei = 2.5 * valueS.toInt();
          int valuew = 255 - valuei;

          analogWrite(ledwarm1, valuew);
          analogWrite(ledcool1, valuei);
          Serial.println("Valor PWM tunn1: " + valueS);
          grabar(239, 3, String(valuew));
          grabar(242, 3, String(valuei));
        }
        if (incomingPacket[7] == '2') //canal 2
        {
          String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
          int valuei = 2.5 * valueS.toInt();
          int valuew = 255 - valuei;
          analogWrite(ledwarm2, valuew);
          analogWrite(ledcool2, valuei);
          Serial.println("Valor PWM tunn2: " + valueS);
          grabar(245, 3, String(valuew));
          grabar(248, 3, String(valuei));
        }
      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '1') //comando led calido
      { func = "0"; grabar(280, 1, "0");
        if (incomingPacket[7] == '1') //canal 1
        {
          String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
          int valuei = 2.5 * valueS.toInt();
          analogWrite(ledwarm1, valuei);
          Serial.println("Valor PWM warm1: " + valueS);
          grabar(239, 3, valueS);

          if (valuei <= 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, LOW);
            grabar(251, 1, "0");
          }
          else if (valuei > 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, HIGH);
            grabar(251, 1, "1");
          }

        }
        if (incomingPacket[7] == '2') //canal 2
        {
          String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
          int valuei = 2.5 * valueS.toInt();
          analogWrite(ledwarm2, valuei);
          Serial.println("Valor PWM warm2: " + valueS);
          grabar(245, 3, valueS);

          if (valuei <= 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, LOW);
            grabar(251, 1, "0");
          }
          else if (valuei > 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, HIGH);
            grabar(251, 1, "1");
          }

        }
      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '2') //comando led frio o blanco
      { func = "0"; grabar(280, 1, "0");
        if (incomingPacket[7] == '1') //canal 1
        {
          String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
          int valuei = 2.5 * valueS.toInt();
          analogWrite(ledcool1, valuei);
          Serial.println("Valor PWM cool1: " + valueS);
          grabar(242, 3, valueS);

          if (valuei <= 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, LOW);
            grabar(251, 1, "0");
          }
          else if (valuei > 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, HIGH);
            grabar(251, 1, "1");
          }

        }
        if (incomingPacket[7] == '2' || incomingPacket[7] == '0') //canal 2 o blanco
        {
          String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
          int valuei = 2.5 * valueS.toInt();
          analogWrite(ledcool2, valuei);
          Serial.println("Valor PWM cool2: " + valueS);
          grabar(248, 3, valueS);

          if (valuei <= 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, LOW);
            grabar(251, 1, "0");
          }
          else if (valuei > 25 && relayEnb == "1" )
          {
            digitalWrite(rele1, HIGH);
            grabar(251, 1, "1");
          }

        }
      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '3') //comando led rojo
      { func = "0"; grabar(280, 1, "0");
        String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
        int valuei = 2.5 * valueS.toInt();
        analogWrite(ledred, valuei);
        Serial.println("Valor PWM red: " + valueS);
        grabar(227, 3, valueS);
      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '4') //comando led verde
      { func = "0"; grabar(280, 1, "0");
        String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
        int valuei = 2.5 * valueS.toInt();
        if (incomingPacket[7] == '1') //Smart
        {
          // strip.setBrightness(255); // 1/3 brightness
          // colorWipe(strip.Color(255, 0, 0), 70); // Red
        }
        else {
          analogWrite(ledgreen, valuei);
          Serial.println("Valor PWM green: " + valueS);
          grabar(230, 3, valueS);
        }
      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '5') //comando led azul
      { func = "0"; grabar(280, 1, "0");
      String valueS = String(incomingPacket[10]) + String(incomingPacket[11]) + String(incomingPacket[12]);
        int valuei = 2.5 * valueS.toInt();
        if (incomingPacket[7] == '1') //Smart
        {
          // strip.setBrightness(255); // 1/3 brightness
          // colorWipe(strip.Color(255, 0, 0), 70); // Red
        }
        else {
          analogWrite(ledgreen, valuei);
          Serial.println("Valor PWM blue: " + valueS);
          grabar(233, 3, valueS);
        }
      }
      
      if (incomingPacket[8] == '2' && incomingPacket[9] == '6') //comando programas
      {

      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '7') //comando funciones
      {
        //fade, wipe, rainbow, blink, all on, all off, strober
        //velocidad, intensidad,
        //para smart led o rgb strip

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == 'x') //apagar todos los canales sin guardar en memoria
        { func = "0"; grabar(280, 1, "0");
          analogWrite(ledwarm2, 0);
          analogWrite(ledcool2, 0);
          analogWrite(ledwarm1, 0);
          analogWrite(ledcool1, 0);
          analogWrite(ledred, 0);
          analogWrite(ledgreen, 0);
          analogWrite(ledblue, 0);
          analogWrite(ledwhite, 0);
          digitalWrite(rele1, LOW);
          //digitalWrite(rele2, LOW);
        }
        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '0') //apagar todos los canales guardando en memoria
        { func = "0"; grabar(280, 1, "0");
          analogWrite(ledwarm2, 0); grabar(245, 3, "000");
          analogWrite(ledcool2, 0); grabar(248, 3, "000");
          analogWrite(ledwarm1, 0); grabar(239, 3, "000");
          analogWrite(ledcool1, 0); grabar(242, 3, "000");
          analogWrite(ledred, 0); grabar(227, 3, "000");
          analogWrite(ledgreen, 0); grabar(230, 3, "000");
          analogWrite(ledblue, 0); grabar(233, 3, "000");
          analogWrite(ledwhite, 0); grabar(236, 3, "000");
          digitalWrite(rele1, LOW); grabar(251, 1, "0");
          //digitalWrite(rele2, LOW); grabar(252,1,"0");
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '1') //apagar  canales 1
        { func = "0"; grabar(280, 1, "0");
          analogWrite(ledwarm1, 0); grabar(239, 3, "000");
          analogWrite(ledcool1, 0); grabar(242, 3, "000");
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '2') //apagar canales 2
        { func = "0"; grabar(280, 1, "0");
          analogWrite(ledwarm2, 0); grabar(245, 3, "000");
          analogWrite(ledcool2, 0); grabar(248, 3, "000");
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '3') //strober canal blanco
        { 
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '4') //strober blanco rgb
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '5') //strober azul
        { 
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '6') //strober rojo
        { 
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '7') //strober verde
        { 
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '8') //strober policia
        { 
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '0' && incomingPacket[12] == '9') //strober random
        { 
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '0') //fade entre rgb
        { 
          func_command();
         }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '1') //fade canal 1
        { 
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '2') //fade canal 2
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '3') //apagar todos los canales
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '4') //apagar todos los canales
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '5') //apagar todos los canales
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '6') //apagar todos los canales
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '7') //apagar todos los canales
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '8') //apagar todos los canales
        {
          func_command();
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '1' && incomingPacket[12] == '9') //apagar todos los canales
        {
          func_command();
        }
//////////FUNCIONES SMART///////
        
      
        //fade, wipe, rainbow, blink, all on, all off, strober
        //velocidad, intensidad,
        //para smart led o rgb strip

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == 'x') //apagar todos los canales sin guardar en memoria
        { 
          func_commandS();
        }
        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '0') //apagar todos los canales guardando en memoria
        {
          func_commandS(); 
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '1') //apagar  canales 1
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '2') //apagar canales 2
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '3') //strober canal blanco
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '4') //strober blanco rgb
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '5') //strober azul
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '6') //strober rojo
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '7') //strober verde
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '8') //strober policia
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '0' && incomingPacket[12] == '9') //strober random
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '0') //fade entre rgb
        { 
          func_commandS();
          //              for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 2)
          //                {
          //                user_pwm1_setvalue(fadeValue);
          //                 user_pwm2_setvalue(255-fadeValue);
          //                 HAL_Delay(50);
          //                }
          //
          //
          //              for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 2)
          //                  {
          //                  user_pwm1_setvalue(fadeValue);
          //                  user_pwm2_setvalue(255-fadeValue);
          //                  HAL_Delay(50);
          //                  }
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '1') //fade canal 1
        { 
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '2') //fade canal 2
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '3') //apagar todos los canales
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '4') //apagar todos los canales
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '5') //apagar todos los canales
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '6') //apagar todos los canales
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '7') //apagar todos los canales
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '8') //apagar todos los canales
        {
          func_commandS();
        }

        if (incomingPacket[10] == '1' && incomingPacket[11] == '1' && incomingPacket[12] == '9') //apagar todos los canales
        {
          func_commandS();
        }
      }//funciones

      if (incomingPacket[8] == '2' && incomingPacket[9] == '8') //comando grupos
      {

      }

      if (incomingPacket[8] == '2' && incomingPacket[9] == '9') //comando sensor
      {
        if (incomingPacket[12] == '1' ) //comando estado sensor
            {
            String typeSensor=leer(290,2);
            int sensorStasensor_value = analogRead(sensor);
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.print("#01302029"+typeSensor+String(sensorStatus) + "_" + ChipId + "%");
            Udp.endPacket();
            }
            
        if (incomingPacket[12] == '2' ) //comando habilitar sensor
            {
            String valueEnb =  String(incomingPacket[15]); //# brillo
            sensorEnb = valueEnb; grabar(179, 1, sensorEnb);
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.print("#savesensor_" + ChipId + "%");
            Udp.endPacket();
            }
      }
      if (incomingPacket[8] == '3' && incomingPacket[9] == '0') //comando reles
      {
      /*  if (incomingPacket[10] == '0' && incomingPacket[11] == '1') //rele1
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele1, LOW);grabar(251,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele1, HIGH); grabar(251,1,"1");}
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '2') //rele2
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele2, LOW);grabar(252,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele2, HIGH); grabar(252,1,"1");}
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '3') //rele3
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele3, LOW);grabar(253,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele3, HIGH); grabar(253,1,"1");}
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '4') //rele4
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele4, LOW);grabar(254,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele4, HIGH); grabar(254,1,"1");}
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '5') //rele5
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele5, LOW);grabar(255,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele5, HIGH); grabar(255,1,"1");}
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '6') //rele6
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele6, LOW);grabar(256,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele6, HIGH); grabar(256,1,"1");}
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '7') //rele7
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele7, LOW);grabar(257,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele7, HIGH); grabar(257,1,"1");}
        }

        if (incomingPacket[10] == '0' && incomingPacket[11] == '8') //rele8
        {
          if (incomingPacket[12] == '0' ) {digitalWrite(rele8, LOW);grabar(258,1,"0"); }
          else if (incomingPacket[12] == '1' )  {digitalWrite(rele8, HIGH); grabar(258,1,"1");}
        }

        if (incomingPacket[10] == '9' && incomingPacket[11] == '9') //all on
        {
          if (incomingPacket[12] == 'x' )
          {
          digitalWrite(rele1, HIGH); 
          digitalWrite(rele2, HIGH); 
          digitalWrite(rele3, HIGH); 
          digitalWrite(rele4, HIGH); 
          digitalWrite(rele5, HIGH); 
          digitalWrite(rele6, HIGH); 
          digitalWrite(rele7, HIGH); 
          digitalWrite(rele8, HIGH); 
          }
          else{
          digitalWrite(rele1, HIGH); grabar(251,1,"1");
          digitalWrite(rele2, HIGH); grabar(252,1,"1");
          digitalWrite(rele3, HIGH); grabar(253,1,"1");
          digitalWrite(rele4, HIGH); grabar(254,1,"1");
          digitalWrite(rele5, HIGH); grabar(255,1,"1");
          digitalWrite(rele6, HIGH); grabar(256,1,"1");
          digitalWrite(rele7, HIGH); grabar(257,1,"1");
          digitalWrite(rele8, HIGH); grabar(258,1,"1");
            }
        }
        if (incomingPacket[10] == '0' && incomingPacket[11] == '0') //all off
        {
           if (incomingPacket[12] == 'x' )
          {
          digitalWrite(rele1, LOW);
          digitalWrite(rele2, LOW);
          digitalWrite(rele3, LOW);
          digitalWrite(rele4, LOW);
          digitalWrite(rele5, LOW);
          digitalWrite(rele6, LOW);
          digitalWrite(rele7, LOW);
          digitalWrite(rele8, LOW);
         }
         else{
         digitalWrite(rele1, LOW);grabar(251,1,"0"); 
          digitalWrite(rele2, LOW);grabar(252,1,"0"); 
          digitalWrite(rele3, LOW);grabar(253,1,"0"); 
          digitalWrite(rele4, LOW);grabar(254,1,"0"); 
          digitalWrite(rele5, LOW);grabar(255,1,"0"); 
          digitalWrite(rele6, LOW);grabar(256,1,"0"); 
          digitalWrite(rele7, LOW);grabar(257,1,"0"); 
          digitalWrite(rele8, LOW);grabar(258,1,"0");
         }
         
        }*/
     
      }//reles

 
  
}


//serial incoming
void serialEvent( ) {
  if (Serial.available() > 0 ) {
    char Dato = Serial.read();
    if (Dato == 'A')
    {
     // modoconf();
    }

    else if (Dato == 'B')
    { digitalWrite(16, LOW);

    }
  }
}

void handleRoot() { 
//  SendHTML_Header();
//  leermem();
 // horaact();
String s = MAIN_page; 
/*  s.replace("@@L1@@", intervalo); 
  s.replace("@@L2@@", borrado); 
  s.replace("@@L3@@", ubicacion); 
  s.replace("@@L4@@", currenttime);*/
  //Serial.println("puntoB");
  //s.replace("@@ESPMAC@@", ChipId); 
  server.send(200, "text/html", s); 
 //  append_page_footer();
 // SendHTML_Content();
 // SendHTML_Stop(); // Stop is needed because no content length was sent
  }

void handleSetup() { 
//  SendHTML_Header();
//  leermem();
  //horaact();
  
  String s = SETUP_page; 
 leer(0, 50).toCharArray(ssid, 50);
  s.replace("@@L3@@", ssid); 
  s.replace("@@ESPID@@", firmDvc); 
  s.replace("@@NAMEDIS@@", tipoDvc);
  s.replace("@@ESPMAC@@", ChipId); 
  server.send(200, "text/html", s); 
 //  append_page_footer();
 // SendHTML_Content();
 // SendHTML_Stop(); // Stop is needed because no content length was sent
  }
    
  void handleForm() { 
    String ssidfo = server.arg("ssidnet"); 
    String passfo = server.arg("ssidpass"); 
    String SAVEWIFI = server.arg("savenet"); 
    String resetesp = server.arg("saveres"); 
  
 
   if (resetesp=="RESTART"){ 
      Serial.println("RESET");
      delay(5000);
      ESP.restart();
      }
    
    if (SAVEWIFI=="SAVE"){ 
      Serial.println("SAVEWIFI");
      grabar(0, 50, ssidfo); 
       grabar(50, 50, passfo); 
        }
server.sendHeader ("Location", "/config"); 
server.send (302, "text/plain", "Updated–– Press Back Button"); //This Line Keeps It on Same Page 
delay (500);
}
//------------------------SETUP WIFI-----------------------------
void setup_wifi() {
  // Conexión WIFI
  Serial.println("entre a STA ");
  WiFi.mode(WIFI_STA); //para que no inicie el SoftAP en el modo normal
  WiFi.begin(ssid, pass);
  Serial.println("mode " + String(WiFi.getMode()));
  Serial.println("ssid " + WiFi.SSID());
  Serial.println("password " + String(pass));
  verificar = true;

  while (WiFi.status() != WL_CONNECTED and contconexion < 50) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(250);
    Serial.print(".");
    //  digitalWrite(15, HIGH);
    delay(250);
    //   digitalWrite(15, LOW);
  }
  if (contconexion < 50) {
    WiFi.setAutoReconnect(true);
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    //   digitalWrite(15, HIGH);
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    Serial.print("ESP Board chip id:  ");
    Serial.println(String(ESP.getChipId(), HEX));


    // httpUpdater.setup(&server, update_path, update_username, update_password);
    //  server.on("/", paginaconf); //esta es la pagina de configuracion
    //  server.on("/guardar_conf", guardar_conf); //Graba en la eeprom la configuracion
    //  server.on("/escanear", escanear); //Escanean las redes wifi disponibles
    //  server.begin();
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
    digitalWrite(13, LOW);
    conectado = 1; modoconf();
  }
}



//--------------------MODO_CONFIGURACION------------------------
void modoconf() {
  int configuro = 0;
  delay(100);

  WiFi.mode(WIFI_AP); //para que no inicie el SoftAP en el modo normal
  WiFi.softAP(ssidConf, passConf);
  httpUpdater.setup(&server, update_path, update_username, update_password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("mode " + String(WiFi.getMode()));
  Serial.print("IP del acces point: ");
  Serial.println(myIP);
  Serial.println("WebServer iniciado... esperando a que se conecten");
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
while (verificar) //debe quedarse cuando oprimimos el boton o cuando se conecta
  {
    if (WiFi.softAPgetStationNum() >= 1 || conectado == 1)
    {
      Serial.println("alguien se conecto...");
      while (true) {
        server.handleClient();
      }

    }

    configuro++;
    delay(150);
    if (configuro == 200) {
      verificar = false;
    }
  }

}

//---------------------GUARDAR CONFIGURACION-------------------------
void guardar_conf() {

  Serial.println(server.arg("ssid"));//Recibimos los valores que envia por GET el formulario web
  grabar(0, 50, server.arg("ssid"));
  Serial.println(server.arg("pass"));
  grabar(50, 50, server.arg("pass"));

  mensaje = "Configuracion Guardada...Reiniciando en 5 segundos";
  delay(5000);
  ESP.restart();
  //paginaconf();
}

//----------------Función para grabar en la EEPROM-------------------
void grabar(int addr, int leng, String a) {
  int tamano = a.length();
  char inchar[leng];
  a.toCharArray(inchar, tamano + 1);
  for (int i = 0; i < tamano; i++) {
    EEPROM.write(addr + i, inchar[i]);
  }
  for (int i = tamano; i < leng; i++) {
    EEPROM.write(addr + i, 255);
  }
  EEPROM.commit();
}

//-----------------Función para leer la EEPROM------------------------
String leer(int addr, int leng) {
  byte lectura;
  String strlectura;
  for (int i = addr; i < addr + leng; i++) {
    lectura = EEPROM.read(i);
    if (lectura != 255) {
      strlectura += (char)lectura;
    }
  }
  return strlectura;
}

//---------------------------ESCANEAR----------------------------
void escanear() {
  int n = WiFi.scanNetworks(); //devuelve el número de redes encontradas
  Serial.println("escaneo terminado");
  if (n == 0) { //si no encuentra ninguna red
    Serial.println("no se encontraron redes");
    mensaje = "no se encontraron redes";
  }
  else
  {
    Serial.print(n);
    Serial.println(" redes encontradas");
    mensaje = "";
    for (int i = 0; i < n; ++i)
    {
      // agrega al STRING "mensaje" la información de las redes encontradas
      mensaje = (mensaje) + "<p>" + String(i + 1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") Ch: " + WiFi.channel(i) + " Enc: " + WiFi.encryptionType(i) + " </p>\r\n";
      //WiFi.encryptionType 5:WEP 2:WPA/PSK 4:WPA2/PSK 7:open network 8:WPA/WPA2/PSK
      delay(10);
    }
    Serial.println(mensaje);
   // paginaconf();
  }
}
