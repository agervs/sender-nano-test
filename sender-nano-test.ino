// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
SoftwareSerial softSerial(2, 3); //RX, TX
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
RFID nano; //Create instance

char iconStatus = 'o';
//int loopCount = 0;
//int lunchDetected = 0;
//int bookDetected = 0;
//int waterDetected = 0;
//int numberOfTimesLunchDetected = 0;
//int numberOfTimesBookDetected = 0;
//int numberOfTimesWaterDetected = 0;
//int lunchNotDetectedCount = 0;
//int bookNotDetectedCount = 0;
//int waterNotDetectedCount = 0;
int lunchDetectedThroughThisLoop;
int bookDetectedThroughThisLoop;
int waterDetectedThroughThisLoop;

void setup() {
  Serial.begin(115200);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.setClock(400000);  
  Wire.onRequest(requestEvent); // register event

  while (!Serial); //Wait for the serial port to come online
  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    while (1); //Freeze!
  }
  nano.setRegion(REGION_NORTHAMERICA); //Set to North America
  nano.setReadPower(2300);
  nano.startReading();
}

void loop() {
// loopCount = loopCount + 1;
 lunchDetectedThroughThisLoop = 0;
 bookDetectedThroughThisLoop = 0;
 waterDetectedThroughThisLoop = 0;

 if (nano.check() == true) 
  {
    byte responseType = nano.parseResponse();
    
    if (responseType == RESPONSE_IS_KEEPALIVE) {
    //      Serial.println(F("Scanning"));
        iconStatus = "o"; 
    } else if (responseType == RESPONSE_IS_TAGFOUND) {
    
      byte tagEPCBytes = nano.getTagEPCBytes();
      String content= "";
      byte letter;
      for (byte i = 0 ; i < tagEPCBytes ; i++) {
        content.concat(String(nano.msg[31 + i] < 0x10 ? " 0" : " "));
        content.concat(String(nano.msg[31 + i], HEX));
      }

      // print out the tag information
//      Serial.print("[");
//      Serial.print(content.substring(12));
//      Serial.print("]");
//      Serial.println();
    
      if (content.substring(12) == " 72 21 dd b1 63 2c a7 76") {
//        lunchDetectedThroughThisLoop = 1; // indicate the lunch was detected in this loop
        iconStatus = 'l';
      } else if (content.substring(12) == " 73 0d db 71 63 2c a7 6d") {
//         bookDetectedThroughThisLoop = 1;
         iconStatus = 'b'; 
      } else if (content.substring(12) == " 73 0d db b1 63 2c a7 6e") {
//         waterDetectedThroughThisLoop = 1;
         iconStatus = 'w'; 
      } else {
        iconStatus = 'o'; 
      }
    
    } else if (responseType == ERROR_CORRUPT_RESPONSE) {
//      Serial.println("Bad CRC");
    } else {
      //Unknown response
      Serial.print("Unknown error");
    }
  } else {
    iconStatus = 'o'; 
  }

  
//  if (lunchDetectedThroughThisLoop == 1) {
//    numberOfTimesLunchDetected = numberOfTimesLunchDetected + 1;
//  }
//  if (bookDetectedThroughThisLoop == 1) {
//    numberOfTimesBookDetected = numberOfTimesBookDetected + 1;
//  }
//  if (waterDetectedThroughThisLoop == 1) {
//    numberOfTimesWaterDetected = numberOfTimesWaterDetected + 1;
//  }
//  
//  if (loopCount == 6000) {
//    
//    if(numberOfTimesLunchDetected <= 1) {
//      lunchNotDetectedCount = lunchNotDetectedCount + 1;
//      if(lunchNotDetectedCount == 3){
//        lunchDetected = 0;
//        lunchNotDetectedCount = 0;
//      }
//    } else {
//      lunchNotDetectedCount = 0;
//      lunchDetected = 1;
//    } 
//
//    if(numberOfTimesBookDetected <= 1) {
//      bookNotDetectedCount = bookNotDetectedCount + 1;
//      if(bookNotDetectedCount == 3){
//        bookDetected = 0;
//        bookNotDetectedCount = 0;
//      }
//    } else {
//      bookNotDetectedCount = 0;
//      bookDetected = 1;
//    }
//
//    if(numberOfTimesWaterDetected <= 1) {
//      waterNotDetectedCount = waterNotDetectedCount + 1;
//      if(waterNotDetectedCount == 3){
//        waterDetected = 0;
//        waterNotDetectedCount = 0;
//      }
//    } else {
//      waterNotDetectedCount = 0;
//      waterDetected = 1;
//    }
//    numberOfTimesLunchDetected = 0;
//    numberOfTimesBookDetected = 0;
//    numberOfTimesWaterDetected = 0;
//    loopCount = 0;
//  } 
  
//  if (lunchDetected == 1 && bookDetected == 1 && waterDetected == 1) {
//    iconStatus = "lbwooo";
//  } else if (lunchDetected == 1 && bookDetected == 0 && waterDetected == 0) {
//    iconStatus = "looooo"; 
//  } else if (lunchDetected == 1 && bookDetected == 1 && waterDetected == 0) {
//    iconStatus = "lboooo"; 
//  } else if (lunchDetected == 1 && bookDetected == 0 && waterDetected == 1) {
//    iconStatus = "lowooo"; 
//  } else if (lunchDetected == 0 && bookDetected == 1 && waterDetected == 0) {
//    iconStatus = "oboooo"; 
//  } else if (lunchDetected == 0 && bookDetected == 1 && waterDetected == 1) {
//    iconStatus = "obwooo"; 
//  } else if (lunchDetected == 0 && bookDetected == 0 && waterDetected == 1) {
//    iconStatus = "oowooo"; 
//  } else {
//    iconStatus = "oooooo";
//  }

//  Serial.print(loopCount);
//  Serial.print(iconStatus);
//  Serial.println();
}

boolean setupNano(long baudRate) {
  nano.begin(softSerial);
  softSerial.begin(baudRate); 
  while(!softSerial); 
  while(softSerial.available()) softSerial.read();
  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE){
    nano.stopReading();
    Serial.println(F("Module continuously reading. Asking it to stop..."));
    delay(1500);
  } else {
    softSerial.begin(115200);
    nano.setBaud(baudRate);
    softSerial.begin(baudRate);
  }

  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false); 
  nano.setTagProtocol();
  nano.setAntennaPort();
  return (true);
}


// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(iconStatus); // respond with message of 6 bytes
  // as expected by master
}
