/* ASCII    | DATA
 * 48 - 57  | 0 - 9
 * 65 - 90  | A - Z
 */

int VRx = A1;
int VRy = A0;
int SW = 2;

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

void setup() {
  Serial.begin(9600); 
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  
}

void loop() {
|
  SW_state = digitalRead(SW);

  Serial.println("X: "+String(xPosition) + "Y: " + String(yPosition));
  
  //Serial.print("H"+String(xPosition)+"D"+String(yPosition)+"D"+String(SW_state)+"T");
  

  delay(500);
  
}
