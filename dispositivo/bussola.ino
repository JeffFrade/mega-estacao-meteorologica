void initBussola() {
   while (!mag.begin()) {
    Serial.println(F("Erro ao iniciar a bussola"));

    displayHeader(32);
    display.setTextSize(2);
    display.println(F("Bussola"));
    display.display();

    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
    
    digitalWrite(redLed, HIGH);
    delay(3500);
    digitalWrite(redLed, LOW);
    delay(3500);
  }
}

float obterPosicao() {
  sensors_event_t event;
  mag.getEvent(&event);
  
  float heading = atan2(event.magnetic.y, event.magnetic.x);
 
  float declinationAngle = 0.22;
  heading += declinationAngle;
 
  if (heading < 0)
    heading += 2 * PI;
 
  if (heading > 2 * PI)
    heading -= 2 * PI;
 
  float headingDegrees = heading * 180 / M_PI;

  return headingDegrees;
}

String obterDirecao(float headingDegrees) {
  String pos = "";
  
  if (headingDegrees >= 337.5 || headingDegrees < 22.5) {
    pos = "N";
  } else if (headingDegrees >= 22.5 && headingDegrees < 67.5) {
    pos = "NE";
  } else if (headingDegrees >= 67.5 && headingDegrees < 112.5) {
    pos = "L";
  } else if (headingDegrees >= 112.5 && headingDegrees < 157.5) {
    pos = "SE";
  } else if (headingDegrees >= 157.5 && headingDegrees < 202.5) {
    pos = "S";
  } else if (headingDegrees >= 202.5 && headingDegrees < 247.5) {
    pos = "SO";
  } else if (headingDegrees >= 247.5 && headingDegrees < 292.5) {
    pos = "O";
  } else if (headingDegrees >= 292.5 && headingDegrees < 337.5) {
    pos = "NO";
  }

  return pos;
}
