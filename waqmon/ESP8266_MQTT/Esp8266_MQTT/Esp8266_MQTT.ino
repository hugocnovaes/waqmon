#include "config.h"

// set up the 'example' feed
AdafruitIO_Feed *ph_feed = io.feed("ph");
AdafruitIO_Feed *temperature_feed = io.feed("temperature");
// AdafruitIO_Feed *teststring_feed = io.feed("teststring");
// AdafruitIO_Feed *teststring2_feed = io.feed("teststring2");

void setup() {
  // start the serial connection
  Serial.begin(9600);

  // wait for serial monitor to open
  ph_feed->onMessage(handleMessage);
  temperature_feed->onMessage(handleMessage);

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

  //faz leitura dos valores atuais no adafruit, somente para sincronizar
  ph_feed->get();
  temperature_feed->get();
}

void loop() {
  io.run();



  if (Serial.available() > 0) {

    // String dataString = Serial.readString();
    // teststring_feed->save(dataString);
    // // Serial.print("dataString: "); Serial.println(dataString); 
    // String dataString2 = Serial.readString();
    // teststring2_feed->save(10);

    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim(); // Limpa espaços em branco

    // Para depuração no Monitor Serial do ESP8266:
    // Serial.print("ESP Received raw: ");
    // Serial.println(incomingData);

    if (incomingData.startsWith("pH_DATA:")) {
      // Extrai o valor de pH da string
      String phValueString = incomingData.substring(incomingData.indexOf(':') + 1);
      phValueString.trim(); // Limpa espaços em branco do valor extraído

      Serial.print("DATA01 - ESP Received pH value: ");
      Serial.println(phValueString);

      // Envia o valor para o feed "ph" no Adafruit IO
      // A biblioteca Adafruit IO pode enviar strings que representam números.
      ph_feed->save(phValueString);
      Serial.println("DATA01 - pH value sent to Adafruit IO.");
    }

    if (incomingData.startsWith("temp_DATA:")) {
      // Extrai o valor de temperature da string
      String temperatureValueString = incomingData.substring(incomingData.indexOf(':') + 1);
      temperatureValueString.trim(); // Limpa espaços em branco do valor extraído

      Serial.print("DATA02 - ESP Received temperature value: ");
      Serial.println(temperatureValueString);

      // Envia o valor para o feed "ph" no Adafruit IO
      // A biblioteca Adafruit IO pode enviar strings que representam números.
      temperature_feed->save(temperatureValueString);
      Serial.println("DATA02 - temperature value sent to Adafruit IO.");
    }
  }

  // if(Serial.available()){
  //   String valorPH = Serial.readString();
  //   ph_feed->save(valorPH);
  // }

  delay(3000);
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.print("Message received from Adafruit IO feed: ");
  Serial.print(data->feedName()); // Mostra de qual feed veio
  Serial.print(" = ");
  Serial.println(data->value()); // Mostra o valor recebido
}
