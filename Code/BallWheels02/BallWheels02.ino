// Radio
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

//**************remote control****************
struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
    int16_t menuDown;  
    int16_t Select;    
    int16_t menuUp;  
    int16_t toggleBottom;  
    int16_t toggleTop; 
    int16_t toggle1;
    int16_t toggle2;
    int16_t mode;  
    int16_t RLR;
    int16_t RFB;
    int16_t RT;
    int16_t LLR;
    int16_t LFB;
    int16_t LT;
};

RECEIVE_DATA_STRUCTURE mydata_remote;

int RLR = 0;
int RFB = 0;
int RT = 0;
int LLR = 0;
int LFB = 0;
int LT = 0;

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long interval = 10;        // time constant for timer

int forwards;
int backwards;
int left;
int right;
int CW;
int CCW;


int A2output;
int A3output;
int A4output;
int A5output;
int A10output;
int A11output;

void setup() {

   
    // initialize serial communication
    Serial.begin(115200);
    
    radio.begin();
    radio.openWritingPipe(addresses[0]); // 00002
    radio.openReadingPipe(1, addresses[1]); // 00001
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);

    
}   // end of setup

// ********************* MAIN LOOP *******************************

void loop() {  
      
        currentMillis = millis();
        if (currentMillis - previousMillis >= 10) {  // start timed event
          
            previousMillis = currentMillis;


            // check for radio data
            if (radio.available()) {
                    radio.read(&mydata_remote, sizeof(RECEIVE_DATA_STRUCTURE));   
            }  

            // threshold remote data
            // some are reversed based on stick wiring in remote
            RFB = ((thresholdStick(mydata_remote.RFB))*-1)/2;   
            RLR = (thresholdStick(mydata_remote.RLR))/2;
            LT = ((thresholdStick(mydata_remote.LT))*-1)/2;
 
            /*
            Serial.print(RFB);
            Serial.print(" , ");
            Serial.print(RLR); 
            Serial.print(" , ");
            Serial.println(LT);
            */
            

            if (RFB >= 1) {
                forwards = RFB;
            }
            else if (RFB <= -1) {
                backwards = abs(RFB);
            }
            else {
                forwards = 0;
                backwards = 0;
            }

            if (RLR >= 1) {
                right = RLR;
            }
            else if (RLR <= -1) {
                left = abs(RLR);
            }
            else {
                left = 0;
                right = 0;
            }

            if (LT >= 1) {
              CW = LT;
            }
            else if (LT <=-1) {
              CCW = abs(LT);
            }
            else {
                CW = 0;
                CCW = 0;
            }

            Serial.print(forwards);
            Serial.print(" , ");
            Serial.print(backwards);
            Serial.print(" , ");
            Serial.print(right);
            Serial.print(" , ");
            Serial.print(left);
            Serial.print(" , ");
            Serial.print(CW);
            Serial.print(" , ");
            Serial.print(CCW);
            Serial.println(" , ");

            A2output = backwards + (left*0.5) + CCW;
            A3output = forwards + (right*0.5) + CW;
            A4output = backwards + (right*0.5) + CW;
            A5output = forwards + (left*0.5) + CCW; 
            A10output = right + CCW;
            A11output = left + CW;
  
            A2output = constrain(A2output,0,255);
            A3output = constrain(A3output,0,255);
            A4output = constrain(A4output,0,255);
            A5output = constrain(A5output,0,255);
            A10output = constrain(A10output,0,255);
            A11output = constrain(A11output,0,255);

            analogWrite(2, A2output);
            analogWrite(3, A3output);
            analogWrite(4, A4output);
            analogWrite(5, A5output);
            analogWrite(10, A10output);
            analogWrite(11, A11output);

 
      
        }     // end of timed loop         
   
}       // end  of main loop
