float getEarthHumidityPercentage() {
  float humiditySensor = analogRead(humidityPin);
  humiditySensor = 1023 - humiditySensor;
  
  return humiditySensor / 1023 * 100;
}
