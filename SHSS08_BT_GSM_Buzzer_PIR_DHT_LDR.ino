//Final Code
 
 #include <SoftwareSerial.h>
 #include <TinyDHT.h>
    //============GSM
    SoftwareSerial sim(10,9);//connect GSM rx to 9, and GSM tx to 10
    int _timeout;
    String _buffer;
    String number = "+256761337333"; //-> change with your number
 //============Bluetooth
 SoftwareSerial bluetooth(11,12);//connect BT rx to 12, and BT tx to 11
 char incoming_data=0;
 boolean incoming_data_received=false;
    //==============================Temp==========================================
    #define dht1Pin 7   // Digital pin connected to the first DHT sensor
    #define DHTTYPE DHT22     // DHT 11

 DHT dht1(dht1Pin, DHTTYPE);
 float temperature1;
 //=============================LightIntensity====================================
 #define ldrPin A0// first pin after lcd pins
 int lightIntensity1=0;
    //=============================Light/Relays======================================
    #define lightPin1 2
    #define lightPin2 3
    #define lightPin3 4
     #define lightPin4 5
 //==============================Thresholds inputs=======================================
 // Define averageTemperature
 int TH=40; 
 // Define averageLightIntensity
 int HI=25000;
 //==============================PIRCAMTrig=======================================
 int pirCam=6;
 int pirCamState=0;
 int camActivationPin=15;
 bool act=false;
//===============================AlarmTone========================================
int alarm=8;
bool alarmState=false;





void setup() {
 //=================GSM
 delay(7000); //delay for 7 seconds to make sure the modules get the signal
 Serial.begin(9600); // Initialize serial communication
 _buffer.reserve(50);
 Serial.println("Sistem Started...");
 sim.begin(9600);
    //================Bluetooth
    bluetooth.begin(9600);
    //===============================================================================
    dht1.begin();
   
 //==========================
 pinMode(ldrPin,INPUT);
 pinMode( pirCam,INPUT);
pinMode(camActivationPin,OUTPUT);
 //==========================
 pinMode(lightPin1, OUTPUT);
 pinMode(lightPin2, OUTPUT);
 pinMode(lightPin3, OUTPUT);
 pinMode(lightPin4, OUTPUT);
 
}










void loop() {
  
  BluetoothFunction();
  Serial.begin(9600); 
  // Reading temperatures from DHT sensors
  temperature1 = dht1.readTemperature();
  // Reading light intensity from photoresistors
  lightIntensity1 = analogRead(ldrPin);
  // Reading PIRCAM state
  pirCamState = digitalRead(pirCam);
  Serial.println(pirCamState);

//=========================================Action===================================

 // delay(1000); // Delay for stability
//====================================
 if (sim.available() > 0){
   // Serial.write(sim.read());
}

//======================light
if(lightIntensity1<=100){
digitalWrite(lightPin1,HIGH);
  
}
else{

 digitalWrite(lightPin1,LOW); 
}
//=====================PIR
if (pirCamState==1){
  act=true;

}
if(act==true && alarmState==true){

tone(alarm,200);
//=============this below makes it repeat call
digitalWrite (camActivationPin, HIGH);// this is GPIO 12 on the ESP32 where PIR is supposed to be connected!

 _buffer.reserve(50);
//Serial.println("Sistem Started...");

sim.begin(9600);

//============call
callNumber();
//============and sent sms
SendMessage("check your home app"); delay(100);
//delay(2000);
 act=false; //reset
}

digitalWrite (camActivationPin, LOW);
SerialDisplay();
}







//==================================================Nested Functions==========================================================================================================
void SerialDisplay(){
// Print temperatures to serial monitor
  Serial.println("TEMPERATURE ");
  Serial.print(temperature1);
  Serial.print(" ");// space
  Serial.print("lightIntensity ");
  Serial.print(lightIntensity1);
  Serial.print(" ");// space
  Serial.print("PIRCAM state ");
  Serial.println(pirCamState);
}
