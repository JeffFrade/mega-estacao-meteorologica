int readUvIndex() { 
    int vout = 0;
    int sensorValue = 0;
    int sum = 0;
    
    for (int i = 0; i < 1024; i++) {
      sensorValue = analogRead(guvaPort);
      sum = sensorValue + sum;
      delay(2);
    }
    
    vout = sum >> 10;
    vout = vout * 4980.0 / 1024;
    
    if (vout < 50) {
      return 0;
    } else if (vout < 227) {
      return 1;
    } else if (vout < 318) {
      return 2;
    } else if (vout < 408) {
      return 3;
    } else if (vout < 503) {
      return 4;
    } else if (vout < 606) {
      return 5;
    } else if (vout < 696) {
      return 6;
    } else if (vout < 795) {
      return 7;
    } else if (vout < 881) {
      return 8;
    } else if (vout < 976) {
      return 9;
    } else if (vout < 1079) {
      return 10;
    } else {
      return 11;
    }
}
