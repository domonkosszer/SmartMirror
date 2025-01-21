#include<SPI.h> 
#include<nRF24L01.h> 
#include<RF24.h> 
 
 
//enclosed is the code for multiplexer switching 
byte controlPinsLoneMux[] = {B00000000, //using switched digital pins to set the mux pins from 0-7 //enclosed is the code for multiplexer switching 
                             B10000000, //7.6.5.4.3.2.1.0 
                             B01000000, 
                             B11000000, 
                             B00100000, 
                             B10100000, 
                             B01100000, 
                             B11100000 
                            }; 
int muxValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int muxValuesA[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  1-8 
int muxValuesB[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  9-17 
int muxValuesC[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  18-24 


int muxValuesZ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte muxValuesZA[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  1-8 
byte muxValuesZB[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  9-17 
byte muxValuesZC[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  18-24 

int calValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int calValuesA[] = {0,0,0,0,0,0,0,0};
int calValuesB[] = {0,0,0,0,0,0,0,0};
int calValuesC[] = {0,0,0,0,0,0,0,0};

int calValuesZ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int calValuesZA[] = {0,0,0,0,0,0,0,0};
int calValuesZB[] = {0,0,0,0,0,0,0,0};
int calValuesZC[] = {0,0,0,0,0,0,0,0};

bool lowSensors = false;
float lTouch = 0;
unsigned long lTtime = 0;
unsigned long lTclock = 0;
bool reset = true;

bool lowSensorsZX = false;
float lTouchZX = 0;
unsigned long lTtimeZX = 0;
unsigned long lTclockZX = 0;
bool resetZX = true;
 
int calibrate; 
int buttonPin = 14; 
bool printing; 
int calibrated; 
int eight; 
bool expiredCal; 
bool firstWrng; 
bool autocalibration = false; 
bool push; 
long OutputInterval = 1500; 
unsigned long rStart = 0;
long rIntrvl = 60000;
unsigned long BaseTime; 
unsigned long CurrentTime; 
unsigned long PreviousTime; 
unsigned long expired; 
bool expire; 
bool printrecal; 
unsigned long lastwrng; 
unsigned long calStart; 
unsigned long wrngStrt; 
unsigned int wrngIntrvl = 900000 /*30000*/; 
unsigned int expireIntrvl =  7200000 /*60000*/; 
bool adjusted = false;
unsigned long expireStrt; 
unsigned long expireLst; 
bool aligned; 
bool pushed; 
double IRsetting; 
bool calibrationStored = false;
bool calibratedZ = false;
unsigned long powerOnTime = 0;
long selfAdjTime = 500;
bool selfCal = false;

bool n = false;
bool t = false;
bool c = false;
bool d = false;
bool b = false;
bool z = false;
bool y = false;
bool r = false;

bool cPrinted = true;
bool dPrinted = true;
bool wrote = false;

unsigned long StartTime; 

int LowX = 0;
int Low1X = 0;
int Low2X = 0;
int Low3X = 0;
int Low4X = 0;
int lTrend = 0;
float lastX = 0;
float lastY = 0;

int highTouch1 = 0;
int highTouch2 = 0;
int highTouch3 = 0;
int lowTouch1 = 0;
int lowTouch2 = 0;
int lowTouch3 = 0;
int lastDiff = 0;
int currentDiff = 0;

int LowZX = 0;
int Low1ZX = 0;
int Low2ZX = 0;
int Low3ZX = 0;
int Low4ZX = 0;
int lTrendZX = 0;
float lastZX = 0;

int highTouch1ZX = 0;
int highTouch2ZX = 0;
int highTouch3ZX = 0;
int lowTouch1ZX = 0;
int lowTouch2ZX = 0;
int lowTouch3ZX = 0;
int lastDiffZX = 0;
int currentDiffZX = 0;

const int calTime = 2000; 
unsigned long currentCal; 
 
RF24 radio(8,9); 
 
//const uint64_t wAddress[] = {0xD31373D0B2LL};

const byte address[6] = "00001";

  
//const byte rxAddr[6] = "TRAN1"; 
 
void setup() { 
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setChannel(5);           
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  radio.stopListening();

  Serial.println("Transmitter ready.");


  DDRD = B11100000; //setting digital pins 6-4 to HIGH 
  BaseTime = millis(); 
  //pinMode(buttonPin, INPUT); 
  //pinMode(6, OUTPUT);
  //pinMode(5, OUTPUT); 
  //pinMode(4, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(14, OUTPUT);  //receivers
  digitalWrite(14, HIGH); 
  pinMode(15, OUTPUT);  //receivers
  digitalWrite(15, HIGH); 
} 
 
void setPinMux(int outputMuxPin) { 
  PORTD = controlPinsLoneMux[outputMuxPin];       //setting sewdxa{;;e'f}digital 4-6 to follow the truth table instructions 
 
} 

void muxVals() {
  Serial.println();
  Serial.println(F("Values for multiplexer:"));
  Serial.println(F("==========================="));
  for (int a = 0; a < 24; a++) {
    Serial.print(a);
    Serial.print(F(" = "));
    Serial.println(muxValues[a]);
  }
  Serial.println(F("==========================="));
} 

void muxValsZ() {
  Serial.println();
  Serial.println(F("Values for multiplexer Z:"));
  Serial.println(F("==========================="));
  for (int a = 0; a < 24; a++) {
    Serial.print(a);
    Serial.print(F(" = "));
    Serial.println(muxValuesZ[a]);
  }
  Serial.println(F("==========================="));
} 
 
void calVals() { 
  Serial.println(); 
  Serial.println(F("Calibrated Values:")); 
  Serial.println(F("===========================")); 
  for (int a = 0; a < 24; a++) { 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(calValues[a]); 
  } 
  Serial.println(F("===========================")); 
} 

void calMuxDiff() { 
  Serial.println(); 
  Serial.println(F("Current Difference Between Calibrated Values and Current Values:")); 
  Serial.println(F("===========================")); 
 for (int a = 0; a < 24; a++){ 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValues[a] - calValues[a]); 
 }
  Serial.println(F("==========================="));
}

void calMuxDiffZA() { 
  Serial.println(); 
  Serial.println(F("Current Difference Between Calibrated Values and Current ZY Values:")); 
  Serial.println(F("===========================")); 
 for (int a = 0; a < 8; a++){ 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZA[a] - calValuesA[a]); 
 }
  Serial.println(F("===========================")); 
}

void calMuxDiffZB(){
 for (int a = 0; a < 8; a++){
  for (int b = 8; b <16; b++){ 
  if ( a == 0 && b == 8){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 1 && b == 9){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 2 && b == 10){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 3 && b == 11){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 4 && b == 12){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 5 && b == 13){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 6 && b == 14){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 7 && b == 15){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
 }
 }
  Serial.println(F("==========================="));
  
}

void calMuxDiffZC(){
 for (int a = 0; a < 8; a++){ 
  for (int b = 16; b < 24; b++){
   if ( a == 0 && b == 16){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 1 && b == 17){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 2 && b == 18){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 3 && b == 19){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 4 && b == 20){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 5 && b == 21){
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 6 && b == 22){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 7 && b == 23){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
 }
 }
  Serial.println(F("===========================")); 
}

  void muxValsZ3() { 
  for (int c1 = 0; c1 < 8; c1++) { 
  for (int c = 16; c < 24; c++) { 
  if (c == 16 && c1 == 0){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 17 && c1 == 1){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 18 && c1 == 2){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 19 && c1 == 3){
    Serial.print(F("ZX")); 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 20 && c1 == 4){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 21 && c1 == 5){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 22 && c1 == 6){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 23 && c1 == 7){
    Serial.print(F("ZX")); 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} 
 

 
void timeloop(){ 
currentCal = millis(); 
} 

bool calArray (){
LowX = 0;
Low1X = 0;
Low2X = 0;
Low3X = 0;
Low4X = 0;

LowZX = 0;
Low1ZX = 0;
Low2ZX = 0;
Low3ZX = 0;
Low4ZX = 0;

int threesX = 0;
int twoosX = 0;
int onesX = 0;
int zeroesX = 0;

int threesZX = 0;
int twoosZX = 0;
int onesZX = 0;
int zeroesZX = 0;

bool low = true;
bool paired = false;
   
   calValues[0] = muxValues[0];
   calValues[1] = muxValues[1];
   calValues[2] = muxValues[2];
   calValues[3] = muxValues[3];
   calValues[4] = muxValues[4];
   calValues[5] = muxValues[5];
   calValues[6] = muxValues[6];
   calValues[7] = muxValues[7];
   calValues[8] = muxValues[8];
   calValues[9] = muxValues[9];
   calValues[10] = muxValues[10];
   calValues[11] = muxValues[11];
   calValues[12] = muxValues[12];
   calValues[13] = muxValues[13];
   calValues[14] = muxValues[14];
   calValues[15] = muxValues[15];
   calValues[16] = muxValues[16];
   calValues[17] = muxValues[17];
   calValues[18] = muxValues[18];
   calValues[19] = muxValues[19];
   calValues[20] = muxValues[20];
   calValues[21] = muxValues[21];
   calValues[22] = muxValues[22];
   calValues[23] = muxValues[23];

   calValuesZ[0] = muxValuesZ[0];
   calValuesZ[1] = muxValuesZ[1];
   calValuesZ[2] = muxValuesZ[2];
   calValuesZ[3] = muxValuesZ[3];
   calValuesZ[4] = muxValuesZ[4];
   calValuesZ[5] = muxValuesZ[5];
   calValuesZ[6] = muxValuesZ[6];
   calValuesZ[7] = muxValuesZ[7];
   calValuesZ[8] = muxValuesZ[8];
   calValuesZ[9] = muxValuesZ[9];
   calValuesZ[10] = muxValuesZ[10];
   calValuesZ[11] = muxValuesZ[11];
   calValuesZ[12] = muxValuesZ[12];
   calValuesZ[13] = muxValuesZ[13];
   calValuesZ[14] = muxValuesZ[14];
   calValuesZ[15] = muxValuesZ[15];
   calValuesZ[16] = muxValuesZ[16];
   calValuesZ[17] = muxValuesZ[17];
   calValuesZ[18] = muxValuesZ[18];
   calValuesZ[19] = muxValuesZ[19];
   calValuesZ[20] = muxValuesZ[20];
   calValuesZ[21] = muxValuesZ[21];
   calValuesZ[22] = muxValuesZ[22];
   calValuesZ[23] = muxValuesZ[23];

for (int a:calValuesA){
  if (a == 0) ++zeroesX;
  if (a == 1) ++onesX;
  if (a == 2) ++twoosX;
  if (a == 3) ++threesX;
}

for (int b:calValuesB){
  if (b == 0) ++zeroesX;
  if (b == 1) ++onesX;
  if (b == 2) ++twoosX;
  if (b == 3) ++threesX;
}

for (int c:calValuesC){
  if (c == 0) ++zeroesX;
  if (c == 1) ++onesX;
  if (c == 2) ++twoosX;
  if (c == 3) ++threesX;
}

for (int a:calValuesZA){
  if (a == 0) ++zeroesZX;
  if (a == 1) ++onesZX;
  if (a == 2) ++twoosZX;
  if (a == 3) ++threesZX;
}

for (int b:calValuesZB){
  if (b == 0) ++zeroesZX;
  if (b == 1) ++onesZX;
  if (b == 2) ++twoosZX;
  if (b == 3) ++threesZX;
}

for (int c:calValuesZC){
  if (c == 0) ++zeroesZX;
  if (c == 1) ++onesZX;
  if (c == 2) ++twoosZX;
  if (c == 3) ++threesZX;
}
  calibrationStored = true;
  paired = true;   
  low = false;  
Serial.println();
Serial.print(F("Calibration Successful! "));

Serial.println();
Serial.print("Calibration Failed!");

Serial.println();
Serial.print("There are this many zeroes for X: ");
Serial.print(zeroesX);

Serial.println();
Serial.print("There are this many ones for X: ");
Serial.print(onesX);
    
Serial.println();
Serial.print("There are this many twoos for X: ");
Serial.print(twoosX);
  
Serial.println();
Serial.print("There are this many threes for X: ");
Serial.print(threesX);

Serial.println();
Serial.print("There are this many zeroes for ZX: ");
Serial.print(zeroesZX);

Serial.println();
Serial.print("There are this many ones for ZX: ");
Serial.print(onesZX);
    
Serial.println();
Serial.print("There are this many twoos for ZX: ");
Serial.print(twoosZX);
  
Serial.println();
Serial.print("There are this many threes for ZX: ");
Serial.print(threesZX);

return paired; 
}

void loop() {   
lTclock = millis();

char input = 'O';
bool written = true;

float TouchedX = {0};
float TouchedZX = {0};
bool TouchX = false;
bool TouchZX = false;
bool tapped = false;  
unsigned long tappedTime = NULL;  
bool fingerDown = false;  

float touchedValuesX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float touchedValuesZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float sendingX[] = {0,0}; 
float sendingZX[] = {0,0};     
  
bool sent = false;
 
byte message = 3; 

CurrentTime = millis(); 
calibrate = digitalRead(buttonPin); 
     
if (autocalibration == true){ 
expire = false; 
autocalibration = false; 
calStart = millis(); 
expireStrt = millis(); 
push = true; 
pushed = true; 
expire = false; 
firstWrng = false; 
calibrated = 2; 
calArray();
} 
   
if(calibrate == HIGH){ // added for the recal printing glitch 
 printrecal = false;   
  } 

 while (pushed == true){ 
  currentCal = millis(); 
  BaseTime = millis(); 
Serial.flush(); 
Serial.println();   
if (printrecal == false){ 
Serial.print(F("Calibrating..."));  
} 
else { 
  Serial.print(F("Auto-recalibrating...")); 
}  
if (currentCal <= calStart + calTime){   
    pushed = false; ;  
  } 
 }
   

if (CurrentTime - expireStrt >= 7200000 && CurrentTime - expireStrt >= expireIntrvl && firstWrng != true ){  
Serial.println(); 
Serial.print(F("It has been 2 hours since the last calibration, you should consider recalibrating.")); 
expireStrt = millis(); 
expired = millis(); 
expire = true; 
firstWrng = true; 
wrngStrt = millis(); 
calibrated = 1; 
} 
 
if (firstWrng == true && CurrentTime - wrngIntrvl >= wrngStrt ){ 
Serial.println(); 
Serial.print(F("Recalibration recommended!")); 
wrngStrt = millis(); 
lastwrng = millis(); 
 
    } 
 
 if (expire == true && CurrentTime - 1860000 >= expired && expired > 1) { // expired > 1 added for gltch 
  autocalibration = true; 
  printrecal = true; 
 }  
 
if (expire == true && CurrentTime - 7200000 >= expired){ 
  calibrated = 0; 
} 
for(int b = 8; b < 24; b ++){
 for (int a = 0; a < 8; a++) { 
    setPinMux(a);
    muxValuesZ[a] = analogRead(2);
    muxValues[a] = analogRead(5);
    if( a == 0 && b == 8){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 1 && b == 9){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 2 && b == 10){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 3 && b == 11){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 4 && b == 12){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 5 && b == 13){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 6 && b == 14){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 7 && b == 15){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 0 && b == 16){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 1 && b == 17){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 2 && b == 18){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 3 && b == 19){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 4 && b == 20){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 5 && b == 21){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 6 && b == 22){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 7 && b == 23){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
 }
  
  } 

int touched1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched4[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched5[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched6[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched3[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int fourTouches = 0;
int threeTouches = 0;
int fiveTouches = 0;
int twoTouches = 0;
int oneTouches = 0;
int fourTouch = 0;
int threeTouch = 0;
int fiveTouch = 0;
int twoTouch = 0;
int oneTouch = 0;
float touchedValuesY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float rTouchedValuesY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool noPrint = false;
int group[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
float adjustedGroup[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedDiff[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedCalibrated[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float groupCoef[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int low[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int bLow[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int highestVariance = 0;
int highestPoint = 0;
int calibratedHighestPoint = 0;
bool done = false;
int touch = 25;
int down = 0;
int currentHighestGroup = -1;
float percentTouch[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowCons[] = {0,0,0};
int highCons[] = {0,0,0};
int replaced[] = {0,0,0,0};
int calculatedReplace = 0;
bool addHalf = false;
bool subHalf = false;
int secondTouch = 50;

float rtouchedValuesX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int touched1ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touchedZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched4ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched5ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched6ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched3ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int fourTouchesZX = 0;
int threeTouchesZX = 0;
int fiveTouchesZX = 0;
int twoTouchesZX = 0;
int oneTouchesZX = 0;
int fourTouchZX = 0;
int threeTouchZX = 0;
int fiveTouchZX = 0;
int twoTouchZX = 0;
int oneTouchZX = 0;
float touchedValuesZXY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float rTouchedValuesZXY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool noPrintZX = false;
int groupZX[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
float adjustedGroupZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedDiffZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedCalibratedZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float groupCoefZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowZX[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int bLowZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int highestVarianceZX =0;
int highestPointZX = 0;
int calibratedHighestPointZX = 0;
bool doneZX = false;
int touchZ = 25;
int downZX = 0;
int currentHighestGroupZX = 0;
float percentTouchZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowConsZX[] = {0,0,0};
int highConsZX[] = {0,0,0};
int replacedZX[] = {0,0,0,0};
int calculatedReplaceZX = 0;
bool addHalfZX = false;
bool subHalfZX = false;
int secondTouchZX = 50;
float rtouchedValuesZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

if (calibrationStored == true){ 

   for (int a = 0; a <24; a++){
    
 if (muxValues[a] <= calValues[a] - 4){
n = false;
down++;  
  if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] != -1 && low[7] != -1 && low[8] != -1 && low[9] == -1){

low[9] = a;

group[a] = muxValues[a];

}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] != -1 && low[7] != -1 && low[8] == -1 && low[9] == -1){

low[8] = a;

group[a] = muxValues[a];
}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] != -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){

low[7] = a;

group[a] = muxValues[a];
}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){

low[6] = a;

group[a] = muxValues[a];
}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){

low[5] = a;

group[a] = muxValues[a];
}

  if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[4] = a;
    group[a] = muxValues[a];
  }
  if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[3] = a;
    group[a] = muxValues[a];
  }
  if (low[0] != -1 && low[1] != -1 && low[2] == -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[2] = a;
    group[a] = muxValues[a];
  }
  if (low[0] != -1 && low[1] == -1 && low[2] == -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[1] = a;
    group[a] = muxValues[a];
  }
  if (low[0] == -1 && low[1] == -1 && low[2] == -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[0] = a;
    group[a] = muxValues[a];
  }
  done = true;
 }
  }

  
if(done == true){

if(lTtime + 50 >= lTclock && lTtime > 0 && lTclock > 0){

if(low[2] != -1){
  for(int a = 0; a < 10; a++){
    if(low[a] >= lTouch + 3 && lTouch != 0 || low[a] <= lTouch - 3 && lTouch != 0){
      low[a] = -1;
    }
  }
}
}
  
  for(int a = 0; a < 24; a++){
    if (group[a] > currentHighestGroup){
    currentHighestGroup = group[a];
    highestPoint = a; 
    }
  }
calibratedHighestPoint = calValues[highestPoint];
 if(currentHighestGroup > 0.00){
  
  if(low[0] > -1){
 groupCoef[low[0]] = calibratedHighestPoint/calValues[low[0]]; 
 }
  
  if(low[1] > -1){
 groupCoef[low[1]] = calibratedHighestPoint/calValues[low[1]];
 }
 
  if(low[2] > -1){
 groupCoef[low[2]] = calibratedHighestPoint/calValues[low[2]];
  }

  if(low[3] > -1){
 groupCoef[low[3]] = calibratedHighestPoint/calValues[low[3]];
  }
  
  if(low[4] > -1){
 groupCoef[low[4]] = calibratedHighestPoint/calValues[low[4]];
  }
  if(low[5] > -1){
groupCoef[low[5]] = calibratedHighestPoint/calValues[low[5]];
}

if(low[6] > -1){
groupCoef[low[6]] = calibratedHighestPoint/calValues[low[6]];
}

if(low[7] > -1){
groupCoef[low[7]] = calibratedHighestPoint/calValues[low[7]];
}

if(low[8] > -1){
groupCoef[low[8]] = calibratedHighestPoint/calValues[low[8]];
}

if(low[9] > -1){

groupCoef[low[9]] = calibratedHighestPoint/calValues[low[9]];
}

 for (int a = 0; a < 24; a++){
  if(groupCoef[a] > 0.00){
    if(group[a] > -1){
      adjustedGroup[a] = groupCoef[a] * group[a];
    }
    adjustedCalibrated[a] = calValues[a] * groupCoef[a];
    adjustedDiff[a] = adjustedCalibrated[a] - adjustedGroup[a]; 
   }
 }

for(int a = 0; a < 24; a++){

if (adjustedDiff[a] > adjustedDiff[touch]){

touch = a;

}
}

for (int a = 0; a < 24; a++){
  if (adjustedDiff[a] > adjustedDiff[secondTouch] && adjustedDiff[a] != adjustedDiff[touch]){
secondTouch = a;
}
}

if(currentDiff > 0){
  lastDiff = currentDiff;
  currentDiff = adjustedDiff[touch];
}

if(currentDiff == 0){
  currentDiff = adjustedDiff[touch];
}

for(int a = 0; a <24; a++){
  if(adjustedDiff[a] > calculatedReplace && adjustedDiff[a] < touch){
    calculatedReplace = a;
  }
}

lTrend = touch - lTouch;

currentDiff = adjustedDiff[touch];

int difference = lTouch - touch; 
  
if(lTtime + 50 >= lTclock && lTtime > 0 && lTclock > 0){
int closeGroup[] = {-1,-1,-1,-1,-1};
int closeTouch = -1;
int rcloseTouch = 0;

if(lTouch >= 2){
closeGroup[0] = lTouch - 2;
}
if(lTouch >= 1){
closeGroup[1] = lTouch - 1;
}

closeGroup[2] = lTouch;
if(lTouch <= 22){
closeGroup[3] = lTouch + 1;
}
if(lTouch <= 21){
closeGroup[4] = lTouch + 2;
}

for (int a = 0; a < 5; a++){
  if(closeGroup[a] != -1){
 if(adjustedDiff[closeGroup[a]] > adjustedDiff[closeTouch]){
  closeTouch = a;

  if(closeTouch == 0){
    rcloseTouch = lTouch - 2;
  }
  if(closeTouch == 1){
    rcloseTouch = lTouch - 1;
  }
  if(closeTouch == 2){
    rcloseTouch = lTouch;
  }
  if(closeTouch == 3){
    rcloseTouch = lTouch + 1;
  }
  if(closeTouch == 4){
    rcloseTouch = lTouch + 2;
  }
 }
  }
}

if(rcloseTouch == lTouch + 2 || rcloseTouch == lTouch - 2){
  int closeTouch2 =0;
   for (int a = 1; a < 4; a++){
    if(closeGroup[a] != -1){
if(adjustedDiff[closeGroup[a]] > adjustedDiff[closeTouch2]){
  closeTouch2 = a;
}
    }
}
}

if(touch >= lTouch + 3 || touch <= lTouch - 3 && lTouch != 0){
  touch = rcloseTouch;
}

  currentDiff = adjustedDiff[touch];

if(touch == lTouch){
  if(currentDiff - lastDiff > 0 && lastDiff > 0){
    addHalf = true;
  }
  if(lastDiff - currentDiff > 0 && lastDiff > 0){
    subHalf = true;
  }
}


}
  
  if(touch < 25 && noPrint == false){
  Serial.println();
  Serial.print("y:");
  if(addHalf == false && subHalf == false){
  Serial.print(touch + 1);
  sendingX[1] = touch + 1;
  }
  if(addHalf == true){
    Serial.print(touch + 1.5);
    sendingX[1] = touch + 1.5;
  }
  if(subHalf == true){
    Serial.print(touch + .5);
    sendingX[1] = touch + .5;
  }
  lTouch = touch;

  sendingX[0] = 0;
    
    if (sendingX[1] != 0){
      if(radio.write(&sendingX, sizeof(sendingX))){
        Serial.println();
        Serial.print("~~~~~~~~~~~~~~Sent touch: ");
        Serial.print(sendingX[1]);    
      }
    }
    
  }

 
}
lTtime =  millis();
}
}

 if(lTtime + 2000 <= lTclock && lTtime > 0 && lTclock > 0){
lTouch = 0;
}


if (TouchX == true && r == false || TouchZX == true && r == false){
  for (int a = 0; a < 24; a++){
    if (touchedValuesX[a] != 0){
      TouchedX = a; 
      sendingX[0] = 0;
      sendingX[1] = TouchedX;
      if (sendingX[1] != 0){
   if(!radio.write(sendingX, sizeof(sendingX))){
    Serial.println();
    Serial.print(F("~~~~~~~~~~~~Failed to Send X!~~~~~~~~~~~~~~~~~"));
    }
   else {
    Serial.println();
    Serial.print("~~~~~~~~~~~~~~Sent touch: ");
    Serial.print(sendingX[1]);
  }
 }
    }
  }
} 

if (TouchX == true){ 
  fingerDown = true; 
} 

if (Serial.available()){
  input = Serial.read();
  if (input == 'n'){
    n = true;
    t = false;
    c = false;
    d = false;
    z = false;
    b = false;
    y = false;
    r = false;
    wrote = false;
  }
  if (input == 'z'){
    z = true;
    n = false;
    t = false;
    c = false;
    d = false;
    b = false;
    y = false;
    r = false;
    wrote = false;
  }
 if (input == 't'){
  t = true;
  n = false;
  c = false;
  d = false;
  z = false;
  b = false;
  y = false;
  r = false;
  wrote = false;
  }
  
  if (input == 'd'){
  dPrinted = false;
  d = true;
  n = false;
  c = false;
  t = false;
  z = false;
  b = false;
  y = false;
  r = false;
  wrote = false;
  }
  
  if (input == 'r'){
    if(adjusted == true){
    rStart = millis();
  Serial.println();
  Serial.print("Starting self-touch adjustment!"); 
  Serial.println();
  Serial.print("Hands off the touch area please!");
  }
  if(wrote == false){
    written = false;
  }
  else{
    written = true;
  }
  cPrinted = false;
  
  r = true;
  n = false;
  t = false;
  d = false;
  z = false;
}

 if (input == 'b'){
  dPrinted = false;
  d = true;
  n = false;
  c = false;
  t = false;
  z = false;
  y = false;
  r = false;
  wrote = false;
  }
  
if (input == 'c'){
  cPrinted = false;
  c = true;
  n = false;
  t = false;
  d = false;
  z = false;
  y = false;
  r = false;
  wrote = false;

}

if (input == 'a'){
  autocalibration = true;
}
}

if (n == true){
  if (CurrentTime - BaseTime >= OutputInterval){ 
    muxVals();
  BaseTime = CurrentTime; 
  printing = true; 
}
else {printing = false;} 
} 

if (z == true){
  if (CurrentTime - BaseTime >= OutputInterval){ 
  muxValsZ();
  BaseTime = CurrentTime; 
  printing = true;  
}
else {printing = false;} 
} 

if (c == true && cPrinted == false){  
     calVals();

  cPrinted = true; 
}

if (d == true){
  if (calibrationStored == true){
  if (CurrentTime - BaseTime >= OutputInterval){ 
    calMuxDiff();

  BaseTime = CurrentTime; 
  printing = true; 
 
}
else {printing = false;} 
  }

  else {
  if (dPrinted == false){
  Serial.println();
  Serial.print(F("The X sensors have not been calibrated hence there is no difference available!"));
  dPrinted = true;
  }
} 
}

if (b == true){
  if (calibrationStored == true){
  if (CurrentTime - BaseTime >= OutputInterval){ 
  calMuxDiffZA(); 
  calMuxDiffZB(); 
  calMuxDiffZC(); 
  BaseTime = CurrentTime; 
  printing = true; 
}
else {printing = false;} 
  }

  else {
  if (dPrinted == false){
  Serial.println();
  Serial.print(F("The sensors ZX have not been calibrated hence there is no difference available!"));
  dPrinted = true;
  }
} 
}

if (r == true){
if(CurrentTime - rStart >= rIntrvl && CurrentTime > rStart){
  n = true;
  r = false;
  Serial.println();
  Serial.print("Ending self-touch adjustment!");
  adjusted = true;
  digitalWrite(10, LOW);
}
if(written == false){
  digitalWrite(10, HIGH);
  rStart = millis();
  Serial.println();
  Serial.print("Starting self-touch adjustment!"); 
  Serial.println();
  Serial.print("Hands off the touch area please!");
  written = true;
  wrote = true;
  
}
for(int a=0; a<24; a++){
 if(muxValues[a] <= calValues[a] - 2){
  calValues[a] = muxValues[a];
 }
  
}
}

lTclock = millis();

}
