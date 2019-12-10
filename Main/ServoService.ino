void setupServo() {
  // configure servo PWM functionalitites
  ledcSetup(servoChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(servoPin, servoChannel);

  // removes annoying sound due to pwm + wifi
  ledcDetachPin(SPEAKER_PIN);
  pinMode(SPEAKER_PIN, INPUT);

  openServo();
  M5.begin(true, false, true);
}


void openServo(){
  ledcWrite(servoChannel, 16);
  isOpen = 1;
  //delay(1000);
}

void closeServo(){
  ledcWrite(servoChannel, 1);
  isOpen = 0;
  //delay(1000);
}
