#include <ESP8266WiFi.h> 
#include <ThingerESP8266.h>
#include <DHT.h>

//deklarasikan 
#define DHTPIN D2                               // Pin data sensor DHT11 terhubung
#define DHTTYPE DHT11                           // Tipe sensor DHT
#define USERNAME "dimas019"                    //Username di thinger.io
#define DEVICE_ID "posttest3_dandayvin"        //device id di thinger.io
#define DEVICE_CREDENTIAL "R87gDT58O&9L3BTx"   // generate device credential di thinger.io

//deklarasikan Wifi dan passnya
#define SSID "arya" //nama wifi
#define SSID_PASSWORD "12345678" //password wifi


#define LED_GREEN D8    // Pin untuk lampu hijau
#define LED_YELLOW D7   // Pin untuk lampu kuning
#define LED_RED D6      // Pin untuk lampu merah
#define BUZZER D5       // Pin untuk buzzer

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
DHT dht(DHTPIN, DHTTYPE);

//Variabel Suhu Celcius, fahrenheit, kelvin dan kelembapan
float celcius;
float fahrenheit;
float kelvin;
float kelembaban;

void setup() {
    pinMode(BUZZER, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    dht.begin();
    thing.add_wifi(SSID, SSID_PASSWORD);

    thing["dht11"] >> [](pson& out){
      out["kelembapan"] = kelembaban;
      out["C"] = celcius;
      out["F"] = fahrenheit;
      out["K"] = kelvin;
    };
}

void loop() {
    thing.handle();
    float hum = dht.readHumidity(); 
    float cel = dht.readTemperature(); 
    float fah = dht.computeHeatIndex(cel, hum, true);
    float kel = cel + 273.15; 

    //membaca nilai suhu tiap temperatur
    kelembaban = hum; 
    celcius = cel; 
    fahrenheit = fah; 
    kelvin = kel; 

    if (celcius >= 36){
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_GREEN, LOW);
      tone(BUZZER, 100);
    }
    
    if (celcius >= 30 && celcius <= 36){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_GREEN, LOW);
      noTone(BUZZER);
    }

    if (celcius <= 30){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_GREEN, HIGH);
      noTone(BUZZER);
    }
}