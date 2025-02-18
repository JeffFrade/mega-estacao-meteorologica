void beginMqs() {
  mq3.init();
  mq3.setRegressionMethod(1);
  //_PPM =  a*ratio^b
  mq3.setR0(0.45);
  
  mq4.init();
  mq4.setRegressionMethod(1);
  //_PPM =  a*ratio^b
  mq4.setR0(14.23);
  
  mq135.init();
  mq135.setRegressionMethod(1);
  //_PPM =  a*ratio^b
  mq135.setR0(9.03);
  
  mq7.init();
  mq7.setRegressionMethod(1);
  //_PPM =  a*ratio^b
  mq7.setR0(5.90);
  
  mq8.init();
  mq8.setRegressionMethod(1);
  //_PPM =  a*ratio^b
  mq8.setR0(0.91);
  
  mq9.init();
  mq9.setRegressionMethod(1);
  //_PPM =  a*ratio^b
  mq9.setR0(13.93);

  mq131.init();
  mq131.setRegressionMethod(1);
  //_PPM =  a*ratio^b

  mq136.init();
  mq136.setRegressionMethod(1);
  //_PPM =  a*ratio^b
}

void calibrateMqs() {
  Serial.println(F("Calibrando sensores"));
  digitalWrite(yellowLed, HIGH);
  
  float mq3calcR0 = 0, mq4calcR0 = 0, mq135calcR0 = 0, mq7calcR0 = 0, mq8calcR0 = 0, mq9calcR0 = 0, mq131calcR0 = 0, mq136calcR0 = 0;
  
  for (int i = 1; i <= 10; i ++) {
    updateMqs();
    
    mq3calcR0 += mq3.calibrate(ratioMQ3CleanAir);
    mq4calcR0 += mq4.calibrate(ratioMQ4CleanAir);
    mq135calcR0 += mq135.calibrate(ratioMQ135CleanAir);
    mq7calcR0 += mq7.calibrate(ratioMQ7CleanAir);
    mq8calcR0 += mq8.calibrate(ratioMQ8CleanAir);
    mq9calcR0 += mq9.calibrate(ratioMQ9CleanAir);
    mq131calcR0 += mq8.calibrate(ratioMQ131CleanAir);
    mq136calcR0 += mq9.calibrate(ratioMQ136CleanAir);
  }
  
  mq3.setR0(mq3calcR0 / 10);
  mq4.setR0(mq4calcR0 / 10);
  mq135.setR0(mq135calcR0 / 10);
  mq7.setR0(mq7calcR0 / 10);
  mq8.setR0(mq8calcR0 / 10);
  mq9.setR0(mq9calcR0 / 10);
  mq131.setR0(mq131calcR0 / 10);
  mq136.setR0(mq136calcR0 / 10);

  Serial.print(F("(MQ3 - MQ136):"));
  Serial.print(mq3calcR0 / 10); Serial.print(" | ");
  Serial.print(mq4calcR0 / 10); Serial.print(" | ");
  Serial.print(mq135calcR0 / 10); Serial.print(" | ");
  Serial.print(mq7calcR0 / 10); Serial.print(" | ");
  Serial.print(mq8calcR0 / 10); Serial.print(" | ");
  Serial.print(mq9calcR0 / 10); Serial.print(" | ");
  Serial.print(mq131calcR0 / 10); Serial.print(" | ");
  Serial.print(mq136calcR0 / 10); Serial.println(" |");
  digitalWrite(yellowLed, LOW);
  Serial.println("Calibragem concluida!");
}

void updateMqs()
{
  mq3.update();
  mq4.update();
  mq135.update();
  mq7.update();
  mq8.update();
  mq9.update();
  mq131.update();
  mq136.update();
}
