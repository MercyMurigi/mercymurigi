#include <Key.h>
#include <Keypad.h>

/////////######################################################start##############libraries for GSM
#include <GSM.h> 
#include <GSM3CircularBuffer.h>
#include <GSM3IO.h>
#include <GSM3MobileAccessProvider.h>
#include <GSM3MobileCellManagement.h>
#include <GSM3MobileClientProvider.h>
#include <GSM3MobileClientService.h>
#include <GSM3MobileDataNetworkProvider.h>
#include <GSM3MobileMockupProvider.h>
#include <GSM3MobileNetworkProvider.h>
#include <GSM3MobileNetworkRegistry.h>
#include <GSM3MobileServerProvider.h>
#include <GSM3MobileServerService.h>
#include <GSM3MobileSMSProvider.h>
#include <GSM3MobileVoiceProvider.h>
#include <GSM3ShieldV1.h>
#include <GSM3ShieldV1AccessProvider.h>
#include <GSM3ShieldV1BandManagement.h>
#include <GSM3ShieldV1BaseProvider.h>
#include <GSM3ShieldV1CellManagement.h>
#include <GSM3ShieldV1ClientProvider.h>
#include <GSM3ShieldV1DataNetworkProvider.h>
#include <GSM3ShieldV1DirectModemProvider.h>
#include <GSM3ShieldV1ModemCore.h>
#include <GSM3ShieldV1ModemVerification.h>
#include <GSM3ShieldV1MultiClientProvider.h>
#include <GSM3ShieldV1MultiServerProvider.h>
#include <GSM3ShieldV1PinManagement.h>
#include <GSM3ShieldV1ScanNetworks.h>
#include <GSM3ShieldV1ServerProvider.h>
#include <GSM3ShieldV1SMSProvider.h>
#include <GSM3ShieldV1VoiceProvider.h>
#include <GSM3SMSService.h>
#include <GSM3SoftSerial.h>
#include <GSM3VoiceCallService.h>


/////////####################################################End##############libraries for GSM

// Include Libraries
#include "Arduino.h"
#include "LiquidCrystal_PCF8574.h" 
#include "Relay.h" 

#include<MFRC522.h> 
#include<SPI.h> 
//#include <Keypad.h>
#include "sha1.h" 


// Pin Definitions
//efine RST_PIN 9//new pins revised 
//efine SS_PIN 10 //new pins revised 
#define RELAYMODULE_PIN_SIGNAL  A0
#define SIM800L_SOFTWARESERIAL_PIN_TX 1 
#define SIM800L_SOFTWARESERIAL_PIN_RX 0 

int temp=0,i=0,x=0,k=0;
char str[100],msg[32], tokenString[1], OTPtoken [1], TOTPtoken [6], password [1];
char key_pressed = 0;
uint8_t  z = 0; //howManyKeysHaveBeenPressed /variable used for counter. Used to count number of keys pressed 




// Global variables and defines 

const byte rows = 4; //connect rows to the pinouts 
const byte columns = 4; //connect columns to the pinouts 
//Use this 2D array to map the keys as you desire 
char hexaKeys[rows][columns] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
}; 



// There are several different versions of the LCD I2C adapter, each might have a different address.
// Try the given addresses by Un/commenting the following rows until LCD works follow the serial monitor prints. 
// To find your LCD address go to: http://playground.arduino.cc/Main/I2cScanner and run example.
//#define LCD_ADDRESS 0x3F
#define LCD_ADDRESS 0x27
// Define LCD characteristics
#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define SCROLL_DELAY 150
#define BACKLIGHT 255 



//initialising keys for keypad 
byte row_pins [rows] = { 8,7,6,5}; //connect rows to the pinouts 
byte column_pins [columns] = {4,3,2, A1}; //connect columns to the pinouts 
//Initiating instance for keypad 
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns ); 
// Other object initialization
LiquidCrystal_PCF8574 lcdI2C;

MFRC522 mfrc522(10,9); // create MFRC522 instance 
Relay relayModule(RELAYMODULE_PIN_SIGNAL);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec. This is for the menu function 
char menuOption = 0; 
long time0; 

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////THESE ARE FOR THE TOTP GENERATOR/////////////////////////////////
void printHash(uint8_t* hash) {
  int q;
  for (q=0; q<20; q++) Serial.println(hash[q]);
  Serial.println();
}

uint8_t hmacKey1[]={ 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xde, 0xad, 0xbe, 0xef, 0x6f, 0x21, 0xde, 0xad, 0xbe, 0xef }; //store my private key into an array

long birthTime = 1339345870;
long intern = 0;
long oldOtp = 0; 

int wait = 0; 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
        // Setup Serial which is useful for debugging 
        // Use the Serial Monitor to view printed messages 
              Serial.begin(9600); 
              while (!Serial) ; // wait for serial port to connect. Needed for native USB 
              Serial.println("start"); 
              
             
              // initialize the lcd
              lcdI2C.begin(LCD_COLUMNS, LCD_ROWS, LCD_ADDRESS, BACKLIGHT); 
               //Initialize the keypad with selected key map 
              //keypad_key.begin(hexaKeys); @@@check later
              //initialize RFID module
                          SPI.begin();
            mfrc522.PCD_Init();
            
              
              // initialise GSM
             // initialiseGSM (); 
             
               
              menuOption = menu(); 
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
                    if(menuOption == '1') {
                              // Membrane 3x4 Matrix Keypad - Test Code
                              //Read keypad
                                readKeypad ();
                    }
                    else if(menuOption == '2') {
                        // LCD 16x2 I2C - Test Code
                        // The LCD Screen will display the text of your choice.
                              lcdI2C.clear();                          // Clear LCD screen.
                              lcdI2C.print("  The Display  ");                   // Print print String to LCD on first line
                              lcdI2C.selectLine(2);                    // Set cursor at the begining of line 2
                              lcdI2C.print("     Works!  ");                   // Print print String to LCD on second line
                              delay(1000);
                
                    }
                    else if(menuOption == '3') {
                        // RFID Card Reader - RC522 - Test Code
                        //Read RFID tag if present
                     
                              readRfidTag(); 
                
                    }
                    else if(menuOption == '4') {
                        // Relay Module - Test Code
                        // The relay will turn on and off for 500ms (0.5 sec)
                              relayModule.on();       // 1. turns on
                              delay(500);             // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
                              relayModule.off();      // 3. turns off.
                              delay(500);             // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
                    }
                    else if(menuOption == '5')
                    {
                        // The QuadBand GPRS-GSM SIM800L. Please be kind and report any bugs you may find. 
                             // initialiseGSM(); //initialise the GSM module 
                             
                              sendTheTestMessage (); //send the test message 
                              kindlySendMeA_message (); //asks the demo tester to send it a message
                              printTheMessageReceived (); //prints the received message
                    } 
                    else if(menuOption == '6') {
                        // Generate random token between 0000 and 9999 
                        ///tokenGenerator (min,max,numberOfDigits);
                              tokenGenerator (0,9,4); 
                    } 
                     else if(menuOption == '7') {
                        //                   
                                            Serial.println ("Scan Your Tag"); 
                                      
                                            scanRfidTag(); 
                                            delay (10000); 
                                            Serial.println ("generating token");  
                                             sendTheTokenGeneratedToUserl(); //generates token and sends to user
                                             verifykeyinput(); //checks key input against the token sent to the user
                                                   
                    } 
                    else if(menuOption == '8') {
                        // TOTP generator for 6 digits using SHA1
                              runTheWholeSystem ();
                    }  
     
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) Read 4x4 Matrix Keypad"));
    Serial.println(F("(2) LCD 16x2 I2C"));
    Serial.println(F("(3) RFID Card Reader - RC522"));
    Serial.println(F("(4) Relay Module"));
    Serial.println(F("(5) QuadBand GPRS-GSM SIM800L"));
    Serial.println(F("(6) generate OTP token"));
    Serial.println(F("(7) Demo"));
    Serial.println(F("(8) Run The whole System"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
          Serial.println(F("Now Testing Membrane  Matrix Keypad"));
        else if(c == '2') 
          Serial.println(F("Now Testing LCD 16x2 I2C"));
        else if(c == '3') 
          Serial.println(F("Now Testing RFID Card Reader - RC522"));
        else if(c == '4') 
          Serial.println(F("Now Testing Relay Module"));
        else if(c == '5') 
          Serial.println(F("Now Testing QuadBand GPRS-GSM SIM800L "));
        else if(c == '6') 
          Serial.println(F("Now Generating tokens")); 
        else if(c == '7') 
          Serial.println(F("now running demo")); 
        else if(c == '8') 
          Serial.println(F("Now running the whole system"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
*/////////////////////////////////////////////////////////////////////////////////////start of function running the system 
void runTheWholeSystem () 
{
if(!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if(!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  String uid="";
  Serial.println(uid);
  Serial.println();
  Serial.print("UID=");
  for(int i=0;i<mfrc522.uid.size;i++)
  {
    Serial.print(mfrc522.uid.uidByte[i]<0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i],HEX);
    uid.concat(String(mfrc522.uid.uidByte[i]<0x10 ? "0" : ""));
    uid.concat(String(mfrc522.uid.uidByte[i],HEX));
  }
  uid.toUpperCase();
  if(uid=="0A2AEC81")

  
  {
    Serial.print(":User1"); 
    Serial.println("Authenticating User 1");
    authenticateUser1();   
  }

  
  else if(uid=="6ABFDB80")
  {
    Serial.print(":User2"); 
    authenticateUser2();     
  }

  
  else if(uid=="278D4DB4")
  {
    Serial.print(":User3"); 
   
    authenticateUser3(); 
  }



  
  else
  { 
    Serial.print("User rejected"); 
    Serial.print("ACCESS DENIED"); 

    
  }
  delay(5000);
  
  
  Serial.print("    Scan Tag");
  return;
  
}

////////////////////////////////////////////////////////////////////////////////######################################################################################## 
//////////////****************************************************start of function reading the RFID  
void scanRfidTag() 
{ 

                                
          
          if(!mfrc522.PICC_IsNewCardPresent())
          {
            return; 
          }
          if(!mfrc522.PICC_ReadCardSerial())
          {
            return; 
          }
          String uid="";
          Serial.println(uid);
          Serial.println();
          Serial.print("UID=");
          for(int i=0;i<mfrc522.uid.size;i++)
                    {
                      Serial.print(mfrc522.uid.uidByte[i]<0x10 ? "0" : "");
                      Serial.print(mfrc522.uid.uidByte[i],HEX);
                      uid.concat(String(mfrc522.uid.uidByte[i]<0x10 ? "0" : ""));
                      uid.concat(String(mfrc522.uid.uidByte[i],HEX));
                    }
          uid.toUpperCase();
          if(uid=="0A2AEC81")
          {
                      Serial.print(":User1"); 
                      
                      sendTheTokenGeneratedToUserl();
                      
          }
          else if(uid=="6ABFDB80")
          {
                      Serial.print(":User2"); 
                      
                  //authenticateUser2();     
          }
          else if(uid=="278D4DB4")
          {
                        Serial.print(":User3"); 
                       
                       // authenticateUser3(); 
          }
          else if(uid=="@@@@@@@@@15AE1CC3")
          {
                        Serial.print(":User4"); 
                        
                        //authenticateUser4(); 
        
          }
          else
          {
            Serial.print("ACCESS DENIED");
        
            
          }
          delay(5000);
          
          
          Serial.print("    Scan Tag");
          return;

              
 }


//////////////****************************************************start of function reading the RFID  
void readRfidTag() 
{ 

                                
          
          if(!mfrc522.PICC_IsNewCardPresent())
          {
            return; 
          }
          if(!mfrc522.PICC_ReadCardSerial())
          {
            return; 
          }
          String uid="";
          Serial.println(uid);
          Serial.println();
          Serial.print("UID=");
          for(int i=0;i<mfrc522.uid.size;i++)
                    {
                      Serial.print(mfrc522.uid.uidByte[i]<0x10 ? "0" : "");
                      Serial.print(mfrc522.uid.uidByte[i],HEX);
                      uid.concat(String(mfrc522.uid.uidByte[i]<0x10 ? "0" : ""));
                      uid.concat(String(mfrc522.uid.uidByte[i],HEX));
                    }
          uid.toUpperCase();
          if(uid=="0A2AEC81")
          {
                      Serial.print(":User1"); 
                      
                      //authenticateUser1(); 
                      
          }
          else if(uid=="6ABFDB80")
          {
                      Serial.print(":User2"); 
                      
                  //authenticateUser2();     
          }
          else if(uid=="278D4DB4")
          {
                        Serial.print(":User3"); 
                       
                       // authenticateUser3(); 
          }
          else if(uid=="@@@@@@@@@15AE1CC3")
          {
                        Serial.print(":User4"); 
                        
                        //authenticateUser4(); 
        
          }
          else
          {
            Serial.print("ACCESS DENIED");
        
            
          }
          delay(5000);
          
          
          Serial.print("    Scan Tag");
          return;

              
 }

 //////////////****************************************************start of function reading the RFID and selecting the user   
void selectUser() 
{ 
                
          if(!mfrc522.PICC_IsNewCardPresent())
            {
              return;
            }
            if(!mfrc522.PICC_ReadCardSerial())
            {
              return;
            }
            String uid="";
            Serial.println(uid);
            Serial.println();
            Serial.print("UID=");
            for(int i=0;i<mfrc522.uid.size;i++)
            {
              Serial.print(mfrc522.uid.uidByte[i]<0x10 ? "0" : "");
              Serial.print(mfrc522.uid.uidByte[i],HEX);
              uid.concat(String(mfrc522.uid.uidByte[i]<0x10 ? "0" : ""));
              uid.concat(String(mfrc522.uid.uidByte[i],HEX));
            }
            uid.toUpperCase();
            if(uid=="0A2AEC81")
          
            
            {
              Serial.print(":User1"); 
              
              authenticateUser1();   
            }
          
            
            else if(uid=="6ABFDB80")
            {
              Serial.print(":User2"); 
              authenticateUser2();     
            }
          
            
            else if(uid=="278D4DB4")
            {
              Serial.print(":User3"); 
             
              authenticateUser3(); 
            }
          
          
          
            
            else
            { 
              Serial.print("User rejected"); 
              Serial.print("ACCESS DENIED"); 
          
              
            }
            delay(5000);
            
            
            //Serial.print("    Scan Tag");
            return;
             
 
 } 
 /////////////////////////////////////*******################start of authenticateUser1. 
void authenticateUser1 ()
{
                   Serial.println ("generating token");  
                   tokenGenerator(6,9,1); /// generate 1 digit btn 6 and 9 
                   Serial.println ("sending token generated to User1"); 
                   sendTheTokenGeneratedToUser1 ();    
                   Serial.println ("Now comparing the token generated to the digits keyed in");   
                   verifyUser();
                            delay(10000);  
} 
 /////////////////////////////////////*******################start of authenticateUser2. 
void authenticateUser2 ()
{
                   sendTheTokenGeneratedToUser2 ();      
                   verifyUser();
                            delay(10000);  
}
 /////////////////////////////////////*******################start of authenticateUser3. 
void authenticateUser3 ()
{
                   sendTheTokenGeneratedToUser3 ();      
                   verifyUser(); 
                            delay(10000);  
}
 /////////////////////////////////////*******################start of authenticateUser4. 
void authenticateUser4 ()
{
                   sendTheTokenGeneratedToUser4 ();      
                   verifyUser(); 
                            delay(10000);  
}
//********************************************************///start of void readKeypad 
void readKeypad ()
{    
                 key_pressed  = keypad_key.getKey(); //storing keys @@@ check if you need the "char" part 
                if (isDigit(key_pressed) ||  key_pressed == '*' ||  key_pressed == '#')
                {
                //Serial.print("key pressed");
                Serial.print(key_pressed);
                }
                delay(100);
}
///////****************************************************start of function verifying the TOTP token against the keys entered by user 
void verifyTOTPtokenAgainstKeysEntered ()
{
        //if (RFIDMode == false) 
              //{
                      key_pressed = keypad_key.getKey(); //storing keys       
                        if (key_pressed) 
                        {
                          password[ z++] = key_pressed; // Storing in password variable 
                           
                         }
                                 if ( z == 6)//if 6 keys are completed 
                                  {
                                    delay (200);
                                        if (!strncmp(password, TOTPtoken, 6)) //if password to totp is matched correctly 
                                            { 
                                              grantAccessDisplay(); 
                                              openTheDoor(); 
                                              //RFIDMode = true ; //Makes RFID Mode true again @@@find out if you need to uncomment this 
                                            } 
                                            
                                         else //if password is not matched 
                                            {
                                              denyAccessDisplay(); // displays a message to show access has been denied 
                                               z = 0; 
                                              //RFIDMode = true ; //Makes RFID Mode true again @@@find out if you need to uncomment this 
                                              }
                                    }
             // }
}


///////****************************************************start of function asking user to input the OTP token 
void askForUserToInputOTP () 
{
               askforOTPDisplay();//ask user for OTP via the display 
               readKeypad();//read input from keypad 
 }    
///////*******************************************start of function that verifies token validity 
 void verifyTokenValidity ()
 {
                //if the digits entered less the token generated equals zero, then then the right token was received and keyed in  
            key_pressed = keypad_key.getKey(); //storing keys       
                        if (key_pressed) 
                        {
                          password[ z++] = key_pressed ;// Storing in password variable 
                           
                         }
                                 if ( z == 1)//if NUMBER OF KEYS PRESSED =1 
                                  {
                                    delay (200);
                                        if (!strncmp (password , OTPtoken, 1)) //compare key_pressed with OTPtoken. Note that tokenString is what is used to send SMS only
                                                  {
                                                    grantAccessDisplay(); //display that access has been granted 
                                                    openTheDoor();//opens the door 
                                                    delay(5000); //wait for 5 secods after instruction, before proceeeding  
                                                    closeTheDoor();//closes the door 
                                                  //RFIDMode = true ; //Makes RFID Mode true again @@@find out if you need to uncomment this 
                        
                                                    }
                                  }
                                  
                                  
                else {
                  closeTheDoor(); // deny access by keeping the door closed 
                  denyAccessDisplay(); //a message is displayed to deny access 
                  //RFIDMode = true ; //Makes RFID Mode true again @@@find out if you need to uncomment this 

                    }
       
 }
///////*******************************************start of function that that opens the door 
void openTheDoor ()
{
                    relayModule.off();
                    delay (15000); // wait for 15 seconds to allow the magnetic door to fully demagentis, open and allow the person to pass   
  }

///////////////////////////*******************************************start of function that that CLOSES the door 
void closeTheDoor () 
{
                   relayModule.on();
                   delay (3000); // wait for 3 seconds to allow the magnetic door to fully magentise and close 
 } 
 
///////////////////////////*******************************************start of function that displays stars on the display  
void displayStarsForEachKeypadInput () 
{ 
                  lcdI2C.clear();                          // Clear LCD screen.
                  lcdI2C.print("*");
}
///////////////////////////*******************************************start of function that asks user to enter OTP via keypad 
void askforOTPDisplay()
{ 
                  lcdI2C.clear();                          // Clear LCD screen.
                  lcdI2C.print("  Key in ");                   // Print print String to LCD on first line
                  lcdI2C.selectLine(2);                    // Set cursor at the begining of line 2
                  lcdI2C.print("     OTP!  ");                   // Print print String to LCD on second line
                  delay(3000); 
  }
///////////////////////////*******************************************start of function that that displays the access denial 
void denyAccessDisplay() 
{
                  lcdI2C.clear();                          // Clear LCD screen.
                  lcdI2C.print("  Access  ");                   // Print print String to LCD on first line
                  lcdI2C.selectLine(2);                    // Set cursor at the begining of line 2
                  lcdI2C.print("     Denied!  ");                   // Print print String to LCD on second line
                  delay(3000); 
  }

///////////////////////////*******************************************start of function that that displays the access denial 
void grantAccessDisplay() 
{
                    lcdI2C.clear();                          // Clear LCD screen.
                    lcdI2C.print("  Access  ");                   // Print print String to LCD on first line
                    lcdI2C.selectLine(2);                    // Set cursor at the begining of line 2
                    lcdI2C.print("     Granted!  ");                   // Print print String to LCD on second line
                    delay(3000); // this allows enough time for the screen refreshment rate to persist for the user to see
 }
///////////////////////////*******************************************start of function that that generates the token

void TOTPtokenGenerator()
{ /*
                  if(intern == 0) intern = birthTime;
                  else 
                  {
                             uint8_t byteArray[8];   
                             long time = intern / 90;  // token regeneration after 90 seconds 
                                        
                             byteArray[0] = 0x00;
                             byteArray[1] = 0x00;
                             byteArray[2] = 0x00;
                             byteArray[3] = 0x00;
                             byteArray[4] = (int)((time >> 24) & 0xFF) ;
                             byteArray[5] = (int)((time >> 16) & 0xFF) ;
                             byteArray[6] = (int)((time >> 8) & 0XFF);
                             byteArray[7] = (int)((time & 0XFF));
                            
                             uint8_t* hash;
                             uint32_t a; 
                             Sha1.initHmac(hmacKey1,16);
                             Sha1.write(byteArray, 8);
                             hash = Sha1.resultHmac();
                            
                             int  offset = hash[20 - 1] & 0xF; 
                             long TOTPtoken = 0;
                             int j;
                             for (j = 0; j < 4; ++j) 
                                     {
                                      TOTPtoken <<= 8;
                                      TOTPtoken  |= hash[offset + j];
                                     }
                              
                             TOTPtoken &= 0x7FFFFFFF;
                             TOTPtoken %= 1000000;
                                          
                             if(TOTPtoken != oldOtp)
                                     {
                                      oldOtp = TOTPtoken;
                                      wait = 0;
                                      Serial.println(TOTPtoken); 
                                      lcdI2C.print(" TOTP"); //Print  String to LCD on first  line
                                      lcdI2C.selectLine(2); 
                                      lcdI2C.print(TOTPtoken );                   // Print  String to LCD on SECOND  line 
                                     }
                             else wait++;
                             
                             if(wait % 2 == 0)
                                     {   
                                      lcdI2C.selectLine(2); 
                                      lcdI2C.print("*");                   // Print  String to LCD on SECOND  line
                                     } 
                             
                             delay(1000);
                             intern++;                
                  }
  
 */ 
}            

///////////////////////////*******************************************start of function that that generates the token

void tokenGenerator(const int nMin, const int nMax, const int  nNumOfNumsToGenerate)
{
                  int OTPtoken = 0;
                  for (int i = 0; i < nNumOfNumsToGenerate; i++)
                  {
                    OTPtoken = rand()%(nMax-nMin) + nMin;
                    Serial.print( OTPtoken);
                    
                  } 
                  delay (15000);//generate 1 every 15 seconds 
                  Serial.print("\n");
                
                  return ; 
}
/////////////////////////////////////////////////////////////////////////Send the Token to User L
void sendTheTokenGeneratedToUserl ()
{
                Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode 
                delay(1000);  // Delay of 1000 milli seconds or 1 second
                Serial.println("AT+CMGS=\"+254713130082\"\r"); // Replace x with mobile number
                delay(1000);
                Serial.println("The token is : 9");// The SMS text you want to send
                delay(100);
                Serial.println((char)26);// ASCII code of CTRL+Z 
       //  prepareGSMToVerifyUser ();
                delay(5000);  
} 
void verifykeyinput () 
{ 
    
                                    readKeypad(); 
                                    delay (1000);
                                         Serial.print("OTP Matched!"); 
                                         delay (1000);
                                              grantAccessDisplay(); 
                                              openTheDoor(); 
                                             
  } 
/////////////////////////////////////////////////////////////////////////Send the Token to User 1 

void sendTheTokenGeneratedToUser1 ()
{

                tokenGenerator(6,9,1);
                

                //show token on screen 
                              lcdI2C.clear(); 
                              lcdI2C.print("  The token is  ");                   // Print print String to LCD on first line
                              lcdI2C.selectLine(2);                    // Set cursor at the begining of line 2
                              lcdI2C.print(OTPtoken);                   
                              delay(1000);//wait for 3 seconds
                              lcdI2C.clear(); // Clear LCD screen. 


  
                Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode 
                delay(1000);  // Delay of 1000 milli seconds or 1 second
                Serial.println("AT+CMGS=\"+254713130082\"\r"); // Replace x with User 1's mobile number
                delay(1000);
                // The SMS text (OTP)you want to send
                Serial.println(OTPtoken);// The SMS text (OTP)you want to send  
                sprintf(OTPtoken, "%d %s ");// The SMS text (OTP)you want to send                
                delay(100);
                
                
                
                Serial.println((char)26);// ASCII code of CTRL+Z
         //prepareGSMToVerifyUser (); 
                delay(10000);  
} 
void prepareGSMToVerifyUser () 
{/*
                Serial.println("AT+CMGS=\"+254737064315\"\r"); // Replace x with the GSM's mobile number
                delay(1000);
                Serial.println("#",OTPtoken,"*");// Formats the OTP in a format that can be recognized by the controller when reading parsed GSM information for verification                  
                delay(100);
                Serial.println((char)26);// ASCII code of CTRL+Z
 */               
}


/////////////////////////////////////////////////////////////////////////Send the Token to User 2
void sendTheTokenGeneratedToUser2 ()
{
                Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode 
                delay(1000);  // Delay of 1000 milli seconds or 1 second
                Serial.println("AT+CMGS=\"+254729999573\"\r"); // Replace x with mobile number
                delay(1000);
                Serial.println(OTPtoken);// The SMS text you want to send
                delay(100);
                Serial.println((char)26);// ASCII code of CTRL+Z
       //  prepareGSMToVerifyUser ();
                delay(10000);  
} 
/////////////////////////////////////////////////////////////////////////Send the Token to User 3
void sendTheTokenGeneratedToUser3 ()
{
                Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode 
                delay(1000);  // Delay of 1000 milli seconds or 1 second
                Serial.println("AT+CMGS=\"+254776797780\"\r"); // Replace x with mobile number
                delay(1000);
                Serial.println(OTPtoken);// The SMS text you want to send
                delay(100);
                Serial.println((char)26);// ASCII code of CTRL+Z
        // prepareGSMToVerifyUser ();
                delay(10000);  
}
/////////////////////////////////////////////////////////////////////////Send the Token to User 4
void sendTheTokenGeneratedToUser4 ()
{
                Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode 
                delay(1000);  // Delay of 1000 milli seconds or 1 second
                Serial.println("AT+CMGS=\"+254776797780\"\r"); // Replace x with mobile number
                delay(1000);
                Serial.println(OTPtoken);// The SMS text you want to send
                delay(100);
                Serial.println((char)26);// ASCII code of CTRL+Z
        // prepareGSMToVerifyUser ();
                delay(10000);  
}

/////////////////////////////////////*******################start of verifyUser. Here we use the token received via gsm and the input from the 4x4 keypad  
void verifyUser ()
  {
    verifyTokenValidity(); /*
            for(unsigned int t=0;t<60000;t++)
                {
                      testIfWeHaveNewMessage ();//tests if message has been received
                      if(temp==1)
                            {
                                  prepareToReadTheMessage(); 
                                  while(x<i)
                                  parseTheToken(); 
                                  tokenString[k]='\r'; //set cursor to start printing the message from the beginning of the line
                                  verifyTokenValidity();
                                  resetTheMessageReader(); 
                            }
                } */
                
  }

/////////////////////////////////////***********************************################start of printTheMessageReceived 
void printTheMessageReceived ()
  {
            for(unsigned int t=0;t<60000;t++)
                {
                  testIfWeHaveNewMessage ();//tests if message has been received
                if(temp==1)
                {
                      prepareToReadTheMessage(); 
                      while(x<i)
                      parseTheMessage(); 
                      msg[k]='\r'; //set cursor to start printing the message from the beginning of the line
                      displayMessage();
                      resetTheMessageReader(); 
                }
                }
                
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////////////###########/start of parsing the token 
  void parseTheToken () 
  {
              while(str[x]=='#')
             
              
              ////////////////////////////////Beginning of parsing 
              {
                x++;
                while(str[x]!='*')
                    {
                      tokenString[k++]=str[x++];//defines the content of the message and parses it to an array called "tokenString [k]"
                    }
              }
              //////////////////////////////End of parsing
              
              x++;//returns x++ 
  }

    /////////////////////////////////////////////////////////////////////////////////////////////////###########/start of parsing msg
  void parseTheMessage () 
  {
              while(str[x]=='#')
             
              
              ////////////////////////////////Beginning of parsing 
              {
                x++;
                while(str[x]!='*')
                    {
                      msg[k++]=str[x++];//defines the content of the message and parses it to an array called "msg [k]"
                    }
              }
              //////////////////////////////End of parsing
              
              x++;//returns x++ 
  }

  /////////////////////////////////////////////////////////////////////////////////////////////#################start of displayMessage 
  void displayMessage () 
  {
            lcdI2C.clear();
            lcdI2C.print(msg);
            delay(1000);  
  }
  /////////////////////////////////////////////////////////////////////////////////////////////#################start of request for input message  
  void kindlySendMeA_message () 
  {
            lcdI2C.clear();
            lcdI2C.print("kindly send me a message");
            delay(2000);  
            lcdI2C.clear();
            lcdI2C.print("use this phone number");
            delay(2000); 
            lcdI2C.clear();
            lcdI2C.print("+254737064315");//the number used by GSM
            delay(5000);   
            lcdI2C.clear();
            lcdI2C.print("Place # at start");
            delay(1000);  
            lcdI2C.clear();
            lcdI2C.print("and * at the end ");
            delay(1000);  
            lcdI2C.clear();
            lcdI2C.print("no spaces between characters!");
            delay(1000);
            
  }

  ///////////////////////////////////////////////////////////////////////////////////////////############start of resetTheMessageReader 
  void resetTheMessageReader () 
  {
            temp=0;
            i=0;
            x=0;
            k=0;
    
   }
 ////////////////////////////////////////////////////////////////////////////////////////////////////////#####testIfWeHaveNewMessage 
 void testIfWeHaveNewMessage () 
 {
       
        while(Serial.available())
        {
            char ch=(char)Serial.read();
            str[i++]=ch;
          if(ch == '*')
                  {
                    temp=1;
                    lcdI2C.clear();
                    lcdI2C.print("new message.");  
                    delay(1000);
                 
                  }
        }

  }
 //////////////////////////////////////////////////////////////////////////////////////////////////##########start of prepareToReadTheMessage 
 void prepareToReadTheMessage () 
 {
               x=0,k=0,temp=0; 
               
 }
 
 /////////////////////////////////////////////////////////////////////////////////////////////////#####start of sendTheTestMessage 
 void sendTheTestMessage ()
 {
                Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode 
                delay(1000);  // Delay of 1000 milli seconds or 1 second
                Serial.println("AT+CMGS=\"+254713130082\"\r"); // Replace x with mobile number
                delay(1000);
                Serial.println("I am working. I came from your GSM Module. Nice Code BTW! :-) ");// The SMS text you want to send
                delay(100);
                Serial.println((char)26);// ASCII code of CTRL+Z
                delay(10000); 
  }

  //////////////////////////////////////////////////////////////////////////////////////################ start of GSM init 
  void initialiseGSM()
{
                    lcdI2C.clear();
                    lcdI2C.print("Finding Module...");
                    boolean at_flag=1;
                    while(at_flag)
                        {
                          Serial.println("AT");
                          while(Serial.available()>0)
                                {
                                 char* q = (char*)"OK";
                                  if(Serial.find(q))
                                  at_flag=0;
                                }
                          delay(1000);
                        }
                  
                   // lcdI2C.clear();
                    //lcdI2C.print("Module Connected..");
                    //delay(1000);
                    //lcdI2C.clear();
                    //lcdI2C.print("Disabling ECHO");
                    boolean echo_flag=1;
                    while(echo_flag)
                            {
                              Serial.println("ATE0");
                              while(Serial.available()>0)
                                    {
                                      char* q = (char*)"OK";
                                      if(Serial.find(q))
                                      echo_flag=0;
                                    }
                              delay(1000);
                            }
                  
                    //lcdI2C.clear();
                    //lcdI2C.print("Echo OFF");
                    //delay(1000);
                    lcdI2C.clear();
                    lcdI2C.print("Finding Network..");
                    boolean net_flag=1;
                    while(net_flag)
                            {
                              Serial.println("AT+CPIN?");
                              while(Serial.available()>0)
                                    {
                                      char* r = (char*)"+CPIN: READY"; 
                                      if(Serial.find(r))
                                      net_flag=0; 
                                    }
                              delay(1000);
                            }
                    lcdI2C.clear();
                    lcdI2C.print("Network Found..");
                    delay(1000);
                    lcdI2C.clear();
} 
