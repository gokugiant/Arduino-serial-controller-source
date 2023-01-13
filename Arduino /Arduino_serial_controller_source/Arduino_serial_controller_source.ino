#include <SoftwareSerial.h>

const int BT_STATE_PIN = 4;
int btState = 0;
const int BT_RX_PIN = 3;
const int BT_TX_PIN = 2;

SoftwareSerial btSerial(BT_TX_PIN, BT_RX_PIN); // RX, TX

const int RED_LED = 10;
const int YELLOW_LED = 9;
const int GREEN_LED = 8;

// Array holds all available lights
const int LIGHT_ARRAY[] = {GREEN_LED, YELLOW_LED, RED_LED}; 

const int WHITE_BUTTON = 7;
bool lastWhiteButtonStatus = false;

int currentLight = -1;   // remember the current active light

const int POTI = 0;
int currentPotiValue = -1;
int potiTolerance = 5;  // The tolerance the poti has to chance before an serial update

void setup() {
  Serial.begin(9600);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  pinMode(WHITE_BUTTON, INPUT);
  pinMode(btState,INPUT); 

  Serial.println("Init BT Sofware Serial:");
  btSerial.begin(9600);
}

void loop() {
  // Chech for changes on the poti
  int potiValue = analogRead(POTI);
  if(currentPotiValue < potiValue - potiTolerance || 
     currentPotiValue > potiValue + potiTolerance){
    currentPotiValue = potiValue;
    serialUpdate();
  }

  // Check the button status
  bool whiteButtonIsPressed = digitalRead(WHITE_BUTTON);

  if(lastWhiteButtonStatus != whiteButtonIsPressed){
    delay(50);  // debounce the button
    
    if(whiteButtonIsPressed){
      switchLight();
    }
    lastWhiteButtonStatus = whiteButtonIsPressed;

    serialUpdate();
  }


  // Serial connection management
  // Something happen over hardware serial (Computer Terminal)
  if(Serial.available()){
    //currentLight = Serial.parseInt();
    btSerial.write(Serial.read());
  }

  // Something happen over software serial (Bluetooth SPP Module)
  if (btSerial.available()){
    Serial.println("BT Serial recieved");
    char charRead = btSerial.read();
    Serial.write(charRead);
    switchLight();
  }
}

// Switch between the lights in the currentLight array
void switchLight(){
  allLightsOff();
  
  digitalWrite(LIGHT_ARRAY[currentLight], true);

  currentLight++;
  if(currentLight > 2) currentLight = 0;
}

// Switch all lights off
void allLightsOff(){
  for (int i=0; i<sizeof LIGHT_ARRAY/sizeof LIGHT_ARRAY[0]; i++)
    digitalWrite(LIGHT_ARRAY[i], false);
}

void serialUpdate(){
    Serial.print("LEDStatus:");
    
    Serial.print(currentLight);
    Serial.print(",BUTTON_PRESSED:");
    Serial.print(lastWhiteButtonStatus);
    Serial.print(",POTI:");
    int potiRange = map(currentPotiValue,0 , 1023, 0, 100);
    Serial.println(potiRange);
}
