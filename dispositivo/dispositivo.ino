#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <BH1750.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <MQUnifiedsensor.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// MQ Sensors
#define mq3pin A0
#define mq4pin A1
#define mq135pin A2
#define mq7pin A3
#define mq8pin A6
#define mq9pin A7
#define mq131pin A8
#define mq136pin A9

// MQ Configs
#define board "Arduino Mega 2560"
#define ratioMQ3CleanAir 60 // RS / R0 = 60 ppm 
#define ratioMQ4CleanAir 4.4 // RS / R0 = 4.4 ppm 
#define ratioMQ135CleanAir 3.6 // RS / R0 = 10 ppm 
#define ratioMQ7CleanAir 27.5 // RS / R0 = 27.5 ppm
#define ratioMQ8CleanAir 70 // RS / R0 = 70 ppm   
#define ratioMQ9CleanAir 9.6 // RS / R0 = 9.6 ppm 
#define ratioMQ131CleanAir 15 // RS / R0 = 15 ppm 
#define ratioMQ136CleanAir 3.6 // RS / R0 = 3.6 ppm 
#define adc_bit_resolution 10 // 10 bit ADC 
#define voltage_resolution 5 // Volt resolution to calc the voltage

// GUVA(UV)
#define guvaPort A10

// Soil Humidity
#define humidityPin A11

// OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
#define header "Teste"

// BMP280
#define bmpAddress 0x76
// Caso não funcione trocar para 0x77

// DS18B20 (Water Temperature Sensor)
#define PINO_ONEWIRE 28

// DHT
#define dhtType DHT22
#define dhtPin 22

// LEDs
#define redLed 23
#define yellowLed 24
#define greenLed 25
#define blueLed 26

MQUnifiedsensor mq3(board, voltage_resolution, adc_bit_resolution, mq3pin, "MQ-3");
MQUnifiedsensor mq4(board, voltage_resolution, adc_bit_resolution, mq4pin, "MQ-4");
MQUnifiedsensor mq135(board, voltage_resolution,adc_bit_resolution, mq135pin, "MQ-135");
MQUnifiedsensor mq7(board, voltage_resolution, adc_bit_resolution, mq7pin, "MQ-7");
MQUnifiedsensor mq8(board, voltage_resolution, adc_bit_resolution, mq8pin, "MQ-8");
MQUnifiedsensor mq9(board, voltage_resolution, adc_bit_resolution, mq9pin, "MQ-9");
MQUnifiedsensor mq131(board, voltage_resolution, adc_bit_resolution, mq131pin, "MQ-131");
MQUnifiedsensor mq136(board, voltage_resolution, adc_bit_resolution, mq136pin, "MQ-136");
Adafruit_BMP280 bmp;
DHT dht(dhtPin, dhtType);
BH1750 bh;
StaticJsonDocument<2048> doc;
DynamicJsonDocument docSend(1024);
OneWire oneWire(PINO_ONEWIRE);
DallasTemperature sensor(&oneWire);
DeviceAddress endereco_temp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void displayHeader(int pos);

const unsigned char iconBitmap[] PROGMEM = {
  0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x30, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x31, 0xf3, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x71, 0x9f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x31, 0xff, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x30, 0xf7, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x70, 0x07, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x30, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x10, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x18, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x04, 0x07, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x04, 0x07, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x04, 0x07, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x04, 0x07, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x08, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x18, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0x70, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7c, 0x70, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfc, 0x70, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfc, 0xe0, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfc, 0xe0, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfd, 0xe0, 0x3f, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfd, 0xc0, 0x1f, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xfc, 0x1f, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xfe, 0x07, 0x8f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xfe, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xfe, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7f, 0xfe, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x3f, 0xfe, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0xfe, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0xfe, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1e, 0xfe, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1c, 0xfe, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1c, 0x7c, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x3c, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup()
{
  Serial.begin(9600);
  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(humidityPin, INPUT);

  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, HIGH);

  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("Erro ao inicializar display"));
      digitalWrite(greenLed, LOW);
      digitalWrite(blueLed, LOW);
      
      digitalWrite(redLed, HIGH);
  }
  
  displayHeader(32);
  display.setTextSize(1);
  display.println("Inicializando...");
  display.display();

  dht.begin();

 while (!bmp.begin(bmpAddress)) {
    Serial.println(F("Erro ao iniciar o BMP280"));

    displayHeader(32);
    display.setTextSize(2);
    display.println("BMP280");
    display.display();

    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
    
    digitalWrite(redLed, HIGH);
    delay(2000);
    digitalWrite(redLed, LOW);
    delay(2000);
  }

  while (!bh.begin()) {
    Serial.println(F("Erro ao iniciar o BH1750"));

    displayHeader(32);
    display.setTextSize(2);
    display.println("BH1750");
    display.display();

    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
    
    digitalWrite(redLed, HIGH);
    delay(3000);
    digitalWrite(redLed, LOW);
    delay(3000);
  }

  while (!sensor.getAddress(endereco_temp, 0)) {
      Serial.println(F("Erro ao iniciar o DS20B18"));
    
      displayHeader(32);
      display.setTextSize(2);
      display.println("DS18B20");
      display.display();
    
      digitalWrite(blueLed, LOW);
      digitalWrite(blueLed, LOW);
      digitalWrite(redLed, LOW);
      delay(4000);
      digitalWrite(redLed, HIGH);
      delay(4000);
  }

  initBussola();
  beginMqs();
  
  displayHeader(32);
  display.setTextSize(1);
  display.println("Inicializado!");
  display.println("Aguardando Execucao");
  display.display();

  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
}

void loop()
{
    digitalWrite(blueLed, HIGH);
    
    updateMqs();
    
    String body = "";
    float alcohol, benzene, hexane, ch4, smoke, co2, toluene, nh4, acetone, co, h2, fg, o3, h2s, cl2, nox, ozone, temperature, humidity, temperature_water = 0;
    float pos = obterPosicao();
    String directionCompass = obterDirecao(pos);
    float uv = readUvIndex();
    float soilHumidity = getEarthHumidityPercentage();
    float luminosity = bh.readLightLevel();
    float pressure = bmp.readPressure();
    float alt = bmp.readAltitude();
    
    do {
      temperature_water = sensor.getTempC(endereco_temp);
    } while (temperature_water == -127);

    mq3.setA(0.3934); 
    mq3.setB(-1.504); // Alcohol
    alcohol = mq3.readSensor(); 
  
    mq3.setA(4.8387); 
    mq3.setB(-2.68); // Benzene
    benzene = mq3.readSensor(); 
    
    mq3.setA(7585.3); 
    mq3.setB(-2.849); // Hexane
    hexane = mq3.readSensor(); 
  
    mq4.setA(1012.7); 
    mq4.setB(-2.786); // CH4
    ch4 = mq4.readSensor(); 
  
    mq4.setA(30000000);
    mq4.setB(-8.308); // Smoke
    smoke = mq4.readSensor(); 
   
    mq135.setA(110.47);
    mq135.setB(-2.862); //CO2 
    co2 = mq135.readSensor(); 
    
    mq135.setA(44.947);
    mq135.setB(-3.445); // Toluene
    toluene = mq135.readSensor(); 
    
    mq135.setA(102.2 );
    mq135.setB(-2.473); // NH4 
    nh4 = mq135.readSensor(); 
    
    mq135.setA(34.668);
    mq135.setB(-3.369); // Acetone
    acetone = mq135.readSensor(); 
   
    mq7.setA(99.042);
    mq7.setB(-1.518); // CO
    co = mq7.readSensor(); 
  
    mq8.setA(976.97);
    mq8.setB(-0.688); // H2
    h2 = mq8.readSensor();
  
    mq9.setA(1000.5);
    mq9.setB(-2.186); // Flamable Gas
    fg = mq9.readSensor();

    mq131.setA(23.943);
    mq131.setB(-1.11); // Ozone
    o3 = mq131.readSensor();

    mq136.setA(36.737);
    mq136.setB(-3.536); // H2S
    h2s = mq136.readSensor();

    mq131.setA(47.209);
    mq131.setB(-1.186); // Chlorine
    cl2 = mq131.readSensor();

    mq131.setA(-462.43);
    mq131.setB(-2.204); // NOx
    nox = mq131.readSensor();

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    docSend["temperatura"] = temperature;
    docSend["umidade"] = humidity;
    docSend["alcool"] = alcohol;
    docSend["benzeno"] = benzene;
    docSend["hexano"] = hexane;
    docSend["metano"] = ch4;
    docSend["fumaca"] = smoke;
    docSend["dioxido_carbono"] = co2;
    docSend["tolueno"] = toluene;
    docSend["amonia"] = nh4;
    docSend["acetona"] = acetone;
    docSend["monoxido_carbono"] = co;
    docSend["hidrogenio"] = h2;
    docSend["gases_inflamaveis"] = fg;
    docSend["ozonio"] = o3;
    docSend["sulfeto_hidrogenio"] = h2s;
    docSend["cloro"] = cl2;
    docSend["oxido_hirdogenio"] = nox;
    docSend["luminosidade"] = luminosity;
    docSend["uv"] = uv;
    docSend["temperatura_liquido"] = temperature_water;
    docSend["umidade_solo"] = soilHumidity;
    docSend["pressao"] = pressure;
    docSend["altitude"] = alt;
    docSend["bussola"]["graus"] = pos;
    docSend["bussola"]["direcao"] = directionCompass;

    Serial.print(F("Temperatura: "));
    Serial.print(temperature);
    Serial.println(F(" °C"));

    Serial.print(F("Umidade: "));
    Serial.print(humidity);
    Serial.println(F(" %"));

    Serial.print(F("Álcool: "));
    Serial.print(alcohol);
    Serial.println(F(" ppm"));

    Serial.print(F("Benzeno: "));
    Serial.print(benzene);
    Serial.println(F(" ppm"));

    Serial.print(F("Hexano: "));
    Serial.print(hexane);
    Serial.println(F(" ppm"));

    Serial.print(F("Metano: "));
    Serial.print(ch4);
    Serial.println(F(" ppm"));

    Serial.print(F("Fumaça: "));
    Serial.print(smoke);
    Serial.println(F(" ppm"));

    Serial.print(F("Dióxido de Carbono: "));
    Serial.print(co2);
    Serial.println(F(" ppm"));

    Serial.print(F("Tolueno: "));
    Serial.print(toluene);
    Serial.println(F(" ppm"));

    Serial.print(F("Amônia: "));
    Serial.print(nh4);
    Serial.println(F(" ppm"));

    Serial.print(F("Acetona: "));
    Serial.print(acetone);
    Serial.println(F(" ppm"));

    Serial.print(F("Monóxido de Carbono: "));
    Serial.print(co);
    Serial.println(F(" ppm"));

    Serial.print(F("Hidrogênio: "));
    Serial.print(h2);
    Serial.println(F(" ppm"));

    Serial.print(F("GLP: "));
    Serial.print(fg);
    Serial.println(F(" ppm"));

    Serial.print(F("Ozônio: "));
    Serial.print(o3);
    Serial.println(F(" ppm"));

    Serial.print(F("Sulfeto de Hidrogênio: "));
    Serial.print(h2s);
    Serial.println(F(" ppm"));

    Serial.print(F("Cloro: "));
    Serial.print(cl2);
    Serial.println(F(" ppm"));

    Serial.print(F("Óxido de Hidrogênio: "));
    Serial.print(nox);
    Serial.println(F(" ppm"));

    Serial.print(F("Luminosidade: "));
    Serial.print(luminosity);
    Serial.println(F(" lux"));

    Serial.print(F("UV: "));
    Serial.print(uv);
    Serial.println(F(" IUV"));

    Serial.print(F("Temperatura do Líquido: "));
    Serial.print(temperature_water);
    Serial.println(F(" °C"));

    Serial.print(F("Umidade do Solo: "));
    Serial.print(soilHumidity);
    Serial.println(F(" %"));

    Serial.print(F("Pressão: "));
    Serial.print(pressure);
    Serial.println(F(" Pa"));

    Serial.print(F("Altitude: "));
    Serial.print(alt);
    Serial.println(F(" m"));

    Serial.print(F("Bússola: "));
    Serial.print(pos);
    Serial.print(F(" - "));
    Serial.println(directionCompass);

    displayHeader(60);
    display.setTextSize(1);
    display.setCursor(52, 16);
    display.println("Temperatura:");
    display.setTextSize(2);
    display.setCursor(52, 26);
    display.println(String(temperature, 1));

    display.setTextSize(1);
    display.setCursor(52, 40);
    display.println("Umidade:");
    display.setTextSize(2);
    display.setCursor(52, 50);
    display.println(String(humidity, 2));

    display.drawBitmap(0, 0, iconBitmap, 128, 64, WHITE);
    display.display();
  
    serializeJson(docSend, body);

    digitalWrite(blueLed, LOW);
    
    Serial.println(body);
    
    Serial.println(F("Medicoes concluidas"));
    Serial.println(F("-------------------------------------------"));
}
