void setupServo() {
  // configure servo PWM functionalitites
  ledcSetup(servoChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(servoPin, servoChannel);

  // removes annoying sound due to pwm + wifi
  ledcDetachPin(SPEAKER_PIN);
  pinMode(SPEAKER_PIN, INPUT);

  closeServo();
  M5.begin(true, false, true);
}


void openServo(){
  ledcWrite(servoChannel, 34);
  //master 34, slave 16
  isOpen = 1;
  //delay(1000);
}

void closeServo(){
  ledcWrite(servoChannel, 24);
  //master 24, slave 1
  isOpen = 0;
  //delay(1000);
}
