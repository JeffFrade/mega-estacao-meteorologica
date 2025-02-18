void gpsAvailable() {
  Serial.println(F("Iniciando GPS"));
  
  bool gpsExit = false;
  
  while (!gpsExit) {
    while (Serial2.available() > 0) {
      if (gps.encode(Serial2.read())) {
        gpsExit = gpsDisplayInfo();
      }
    }
  
    if (millis() > 5000 && gps.charsProcessed() < 10) {
      displayHeader(32);
      display.setTextSize(1);
      display.println(F("Falha no GPS"));
      display.display();
      
      Serial.println(F("Sinal GPS não detectado"));
      digitalWrite(redLed, HIGH);
      delay(5000);
      digitalWrite(redLed, LOW);
      delay(5000);
    }
  }
}

bool gpsDisplayInfo() {
    Serial.println(F("Iniciando GPS"));
    delay(2000);
    
    if (gps.location.isValid()) {      
      Serial.print(F("Latitude: "));
      Serial.println(gps.location.lat(), 6);
      Serial.print(F("Longitude: "));
      Serial.println(gps.location.lng(), 6);
      Serial.println("GPS Ok");
      
      return true;
    }

    displayHeader(32);
    display.setTextSize(1);
    display.println(F("GPS Sem Sinal"));
    display.display();
    
    Serial.println(F("Falha na Localizacao do GPS"));
    digitalWrite(redLed, HIGH);
    delay(3000);
    digitalWrite(redLed, LOW);
    delay(3000);
  
  return false;
}
