// This #include statement was automatically added by the Particle IDE.
#include "serialHelpper.h"

// This #include statement was automatically added by the Particle IDE.
//#include <DFRobotDFPlayerMini.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// This value will store the last time we published an event
long lastPublishedAt = 0;
// this is the time delay before we should publish a new event
// from this device
int publishAfter = 1000;
//define button Pin
int butPin = D8;
int PIRPin = A0;
int souPin = A1;

//define neopixel ring
#define PIXEL_PIN D0
#define PIXEL_COUNT 24
#define PIXEL_TYPE SK6812RGBW

Adafruit_NeoPixel ring = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
//GRB not RGB
uint32_t w = ring.Color(255, 255, 255);
uint32_t r = ring.Color(0, 255, 0);
uint32_t y = ring.Color(255, 255, 0);
uint32_t g = ring.Color(255, 0, 0);
uint32_t c = ring.Color(0, 255, 255);
uint32_t b = ring.Color(0, 0, 255);
uint32_t m = ring.Color(255, 0, 255);
uint32_t k = ring.Color(0, 0, 0);
//a dark navy blue color to sub for black its called d for dark
uint32_t d = ring.Color(2, 39, 27);

//speaker output global variables as described on developer GitHub
//DFRobotDFPlayerMini miniDFPlayer;


void setup()
{
    Serial.begin(115200);
 Serial.println( "starting up" );
 Serial1.begin(9600);
 delay( 2000 );
 
 Serial.println( "Initalizing dfmini" );

 execute_CMD(0x3F, 0, 0); // Send request for initialization parameters

 //while (Serial1.available()<10) // Wait until initialization parameters are received (10 bytes)
 //delay(30); // Pretty long delays between succesive commands needed (not always the same)
 
 delay( 2000 );

 Serial.println( "Initalizing volume" );

 // Initialize sound to very low volume. Adapt according used speaker and wanted volume
 execute_CMD(0x06, 0, 0x30); // Set the volume (0x00~0x30)
 
 delay( 1000 );
 
 Serial.println( "Initalizing sound file" );

 execute_CMD(0x0D,0,1);
 
 Particle.publish( "diot2019/busshelter/activity", "mexico" );
    /*//Serial and MP# setup from: https://www.dfrobot.com/blog-277.html
    //Initialize Serial
    Serial1.begin(9600);
    delay( 2000 );
    execute_CMD(0x3F, 0, 0); // Send request for initialization parameters
    
    delay( 2000 );
    //while (Serial1.available()<10) { // Wait until initialization parameters are received (10 bytes)
    //    delay(30); // Pretty long delays between succesive commands needed (not always the same)
    //}
    
    execute_CMD(0x06, 0, 30); // Set the volume (0x00~0x30)
    delay(1000);
    play_track(1);

  // Initialize sound to very low volume. Adapt according used speaker and wanted volume
  
  /*Serial1.begin(9600);
  miniDFPlayer.setTimeOut(500);
  //original volume was 10
  miniDFPlayer.volume(5);
  //Test MP3
  miniDFPlayer.play(1);
  delay(10000);
  miniDFPlayer.play(3);
  delay(10000);
  miniDFPlayer.play(5);
  delay(10000);*/
  
  // We'll want to subscribe to an event thats fairly unique

  // From the Particle Docs
  // A subscription works like a prefix filter.

  // Basically this will match any event that starts with 'db2018/paired/'
  // This is a feature we'll useto figure out if our event comes from
  // this device or another (see publishMyEvent below)
  //Subscribe to a set of events published when an IFTTT widget is triggered by a tweet
  //Particle.subscribe(  "ChenMutiatLama/2019/Group4/LamaAlFulaij", react );
  
  //button is readable
  pinMode(butPin, INPUT_PULLUP); 
  pinMode(PIRPin, OUTPUT);
  pinMode(souPin, OUTPUT);
  //Initialize all pixels to 'off'
  ring.begin();
  ring.show();

}

void play_track(unsigned char track_num) {
    execute_CMD(0x12, 0, track_num);
}

void loop()
{
    if (Serial1.available()>=10){ // There is at least 1 returned message (10 bytes each)
          // Read the returned code
          byte Returned[10];
          for (byte k=0; k<10; k++)
            Returned[k] = Serial1.read();
    }
    
    /*
    int movement = analogRead(PIRPin);
    int sound = analogRead(souPin);
    //Particle.publish("Move: ",(String)PIRvalue);
    //Particle.publish("Sound: " ,(String)souvalue);
    if(movement>305){//if(movement>310){//if(movement>305){//if(movement>320){//if(movement>350){
        //publishEvent("SocialStopReact", "Move: "+(String)movement);
        //delay(500);

    }
    if(sound>75){//if(sound>85){//if(sound>95){//if(sound>105){
        //publishEvent("SocialStopReact", "Sound: "+(String)sound);
        //delay(500);
    }
    //if(PIRValue>){}
    // publish my event
    // if the button is pressed
    int butValue = digitalRead(butPin);
    if(butValue==LOW){
        //publishEvent("blah");
    }
    
    // delay for a bit
    delay(200);
    */
    
}

/*This  function "publishEvent" is an adaptaion of the previous
funciton "publishMyEvent" that takes a string eventID and a string 
data. The eventID is added to the eventName variable to help 
specify which events each particle (or busStop) should subscribe 
to and act upon. The data ensures that there is a parsable String 
regardless of what event is being subscribed to. 
*/
/*
void publishEvent(String eventID, String data)
{
  // Remember that a device can publish at rate of about 1 event/sec,
  // with bursts of up to 4 allowed in 1 second.
  // Back to back burst of 4 messages will take 4 seconds to recover.
  // So we want to limit the amount of publish events that happen.

  // check that it's been 10 secondds since our last publish
  if( lastPublishedAt + publishAfter < millis() )
  {
      // Remember our subscribe is matching  "db2018/paired/"
      // We'll append the device id to get more specific
      // about where the event came from

      // System.deviceID() provides an easy way to extract the device
      // ID of your device. It returns a String object of the device ID,
      // which is used to identify your device.

      String eventName = "ChenMutiatLama/2019/Group4/"+eventID + System.deviceID();

      // now we have something like "diot/2019/paired/0123456789abcdef"
      // and that corresponds to this devices info

      // then we share it out
      Particle.publish( eventName, data );

      // And this will get shared out to all devices using this code

      // we just pubished so capture this.
      lastPublishedAt = millis();
  }

}

// Our event handlde requires two bits of information
// This gives us:
// A character array that consists of the event name
// A character array that contains the data published in the event we're responding to.
void react(const char *event, const char *data){
    
    String m = strstr(data, "mexic" );
    String mCaps = strstr(data, "Mexic" );
    String r = strstr(data, "russia" );
    String rCaps = strstr(data, "Russia" );
    String s = strstr(data, "syria" );
    String sCaps = strstr(data, "Syria" );
    String i = strstr(data, "israel" );
    String iCaps = strstr(data, "Israel" );
    String v = strstr(data, "venezuela" );
    String vCaps = strstr(data, "Venezuela" );
    
    if(m!=NULL|| mCaps!=NULL){
        mexicanFlag();
    }
    else if(r!=NULL|| rCaps!=NULL){
        russianFlag();
    }
    else if(s!=NULL|| sCaps!=NULL){
        syrianFlag();
    }
    else if(i!=NULL|| iCaps!=NULL){
        israeliFlag();
    }
    else if(v!=NULL|| vCaps!=NULL){
        venezuelanFlag();
    }
    else{
        rainbow();
    }
    turnoff();
    
}

//outputs mexican flag colors to above specified neopixel
void mexicanFlag(){
    delay(100);
    uint32_t flag[3] = {r,w,g};
    flagColors(flag);
}

//outputs syrian flag colors to above specified neopixel
void syrianFlag(){
    delay(100);
    uint32_t flagA[2] = {r,w};
    uint32_t flagB[3] = {g,w,g};
    uint32_t flagC[2] = {w,d};
    flagColors(flagA);
    flagColors(flagB);
    flagColors(flagB);
    
}

//outputs russian flag colors to above specified neopixel
void russianFlag(){
    delay(100);
    uint32_t flag[3] = {w,b,r};
    flagColors(flag);
}


//outputs israeli flag colors to above specified neopixel
void israeliFlag(){
    delay(100);
    uint32_t flag[3] = {b,w,b};
    flagColors(flag);
}

//outputs venezuelan flag colors to above specified neopixel
void venezuelanFlag(){
    delay(100);
    uint32_t flag[3] = {y,b,r};
    flagColors(flag);
}

//outputs mexican flag colors to above specified neopixel
void rainbow(){

    uint32_t rainbow[] = {w,r,y,g,c,b,m,k};
    for(int c=0; c<8; c++) {
        for(uint16_t p=0; p< ring.numPixels(); p++) {
        	//change the color
    		ring.setPixelColor(p, rainbow[c] );
        	//show the color
    		ring.show();
        	delay( 100 );
        }
    }
}
//Flashes neopixel leaving all the lights off
void turnoff(){

    uint32_t onoff[] = {k};
    flagColors(onoff);
}

//outputs an array of colors to the above specified neopixel
void flagColors(uint32_t colors[]){
    delay( 100 );
    for(int c=0; c< sizeof(colors)/sizeof(colors[0]); c++) {
        for(uint16_t p=0; p< ring.numPixels(); p++) {
        	//change the color
    		ring.setPixelColor(p, colors[c] );
        	//show the color
    		ring.show();
        	delay( 100 );
        }
    }
}
*/
