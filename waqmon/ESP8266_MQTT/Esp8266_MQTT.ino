// Adafruit IO Dashboard Setup Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// set up the 'example' feed
AdafruitIO_Feed *ph = io.feed("ph");
AdafruitIO_Feed *temperature = io.feed("temperature");

void setup() {

  // start the serial connection
  Serial.begin(9600);

  // wait for serial monitor to open
  ph->onMessage(message);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  ph->get();



}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  if(Serial.available()){
    String valorPH = Serial.readString();
    ph->save(valorPH);
  }
}

void message(AdafruitIO_Data *data) {
  Serial.println(data->value());
}
