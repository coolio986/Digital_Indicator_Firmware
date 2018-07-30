/*===========================================================
* Project: Digital_Indicator_Firmware
* Developled using Arduino v1.8.5
* Developed by: Anthony Kaul(3D Excellence LLC) in collaboration with Filabot (Triex LLC)
* This firmware converts SPC (Statistical Process Control) into signals that can be recognized by a PC
* All rights reserved


* Version information:
* v1.0 - Initial Release
* ===========================================================*/

int req = 5; //REQ line goes to pin 5 of SPC cable
int dat = 2; //Data line goes to pin 2 of SPC cable
int clk = 3; //Clock line goes to pin 3 of SPC cable

String dataStream;

void setup()
{
  Serial.begin(9600);
  pinMode(req, OUTPUT);
  pinMode(clk, INPUT_PULLUP);
  pinMode(dat, INPUT_PULLUP);
  digitalWrite(req,HIGH); // set request at high first time through
}

void loop()
{
  
  digitalWrite(req, HIGH); // generate set request
  
  for(int i = 0; i < 13; i++ )
  {
    int k = 0;
    
    for (int j = 0; j < 4; j++)
    {
      
      while( digitalRead(clk) == HIGH) {  } // hold until clock is high
      while( digitalRead(clk) == LOW) { } // hold until clock is low
      
      dataStream += digitalRead(dat);
    }
       
  }
    
  Serial.println(dataStream);
  dataStream = "";
  
  digitalWrite(req,LOW);
    
  delay(50); 
}
