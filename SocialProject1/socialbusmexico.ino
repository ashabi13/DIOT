// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// This value will store the last time we published an event
long lastPublishedAt = 0;
// this is the time delay before we should publish a new event
// from this device
int publishAfter = 1000;
//define button Pin
int butPin = D8;

//define neopixel ring
#define PIXEL_PIN D0
#define PIXEL_COUNT 24
#define PIXEL_TYPE SK6812RGBW

Adafruit_NeoPixel ring = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup()
{

  // We'll want to subscribe to an event thats fairly unique

  // From the Particle Docs
  // A subscription works like a prefix filter.

  // Basically this will match any event that starts with 'db2018/paired/'
  // This is a feature we'll useto figure out if our event comes from
  // this device or another (see publishMyEvent below)
  //Subscribe to a set of events published when an IFTTT widget is triggered by a tweet
  Particle.subscribe(  "ChenMutiatLama/2019/Group4/LamaAlFulaij", mexicanFlag );
  //button is readable
  pinMode(butPin, INPUT_PULLUP);  
  //Initialize all pixels to 'off'
  ring.begin();
  ring.show();

}

void loop()
{
    // publish my event
    // if the button is pressed
    int butValue = digitalRead(butPin);
    if(butValue==LOW){
        publishMyEvent();
    }
    // delay for a bit
    delay(200);
}


void publishMyEvent()
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

      String eventName = "ChenMutiatLama/2019/Group4/" + System.deviceID();

      // now we have something like "diot/2019/paired/0123456789abcdef"
      // and that corresponds to this devices info

      // then we share it out
      Particle.publish( eventName, "data goes here" );

      // And this will get shared out to all devices using this code

      // we just pubished so capture this.
      lastPublishedAt = millis();
  }

}

// Our event handlde requires two bits of information
// This gives us:
// A character array that consists of the event name
// A character array that contains the data published in the event we're responding to.
void mexicanFlag(const char *event, const char *data){
    delay(100);
    uint16_t i;
    //mexican flag colors
    uint32_t g = ring.Color(0, 255, 0,0);
    uint32_t w = ring.Color(255, 255, 255,0);
    uint32_t r = ring.Color(255, 0, 0,0);
    //for loops for each light changes to the colors of the mexican flag
    //first green
    for(i=0; i< ring.numPixels(); i++) {
    ring.setPixelColor(i, g );
    	ring.show();
    	delay( 100 );
    }
    //then white
    for(i=0; i< ring.numPixels(); i++) {
    ring.setPixelColor(i, w );
    	ring.show();
    	delay( 100 );
    }
    //then red
    for(i=0; i< ring.numPixels(); i++) {
    ring.setPixelColor(i, r );
    	ring.show();
    	delay( 100 );
    }
    //wait a little at the end
    delay( 100 );
}
