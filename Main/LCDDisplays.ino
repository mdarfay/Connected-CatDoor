void lcdDrawHome() {
  M5.Lcd.clear(BLACK);
  lcdDrawBtnBInfos();
  lcdDrawBtnAInfos();
}

void lcdDrawBtnBInfos() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.println("Connected CatDoor");
  M5.Lcd.setCursor(3, 100);
  M5.Lcd.setTextColor(PINK);
  M5.Lcd.println("Press button B to start");
  M5.Lcd.println("connection");
  //Arrow, vertical
  M5.Lcd.drawLine(160, 180, 160, 239, PINK);
  M5.Lcd.drawLine(159, 180, 159, 239, PINK);
  M5.Lcd.drawLine(161, 180, 161, 239, PINK);
  //Arrow, left
  M5.Lcd.drawLine(160, 239, 140, 210, PINK);
  M5.Lcd.drawLine(159, 239, 139, 210, PINK);
  M5.Lcd.drawLine(161, 239, 141, 210, PINK);
  //Arrow, right
  M5.Lcd.drawLine(160, 239, 180, 210, PINK);
  M5.Lcd.drawLine(159, 239, 179, 210, PINK);
  M5.Lcd.drawLine(161, 239, 181, 210, PINK);
}

void lcdDrawBtnAInfos() {
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(3, 55);
  M5.Lcd.println("Press button A to open");
  M5.Lcd.println("manually");
  //Arrow, vertical
  M5.Lcd.drawLine(65, 180, 65, 239, YELLOW);
  M5.Lcd.drawLine(64, 180, 64, 239, YELLOW);
  M5.Lcd.drawLine(66, 180, 66, 239, YELLOW);
  //Arrow, left
  M5.Lcd.drawLine(65, 239, 45, 210, YELLOW);
  M5.Lcd.drawLine(64, 239, 44, 210, YELLOW);
  M5.Lcd.drawLine(66, 239, 46, 210, YELLOW);
  //Arrow, right
  M5.Lcd.drawLine(65, 239, 85, 210, YELLOW);
  M5.Lcd.drawLine(64, 239, 84, 210, YELLOW);
  M5.Lcd.drawLine(66, 239, 86, 210, YELLOW);
}

void lcdDrawConnectionPage() {
  M5.Lcd.println("Access Point OK.");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 60);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.print("Connect to \"");
  M5.Lcd.print(apssid);
  M5.Lcd.println("\",");
  M5.Lcd.setCursor(20, 85);
  M5.Lcd.print("and go to ");
  M5.Lcd.print(WiFi.softAPIP());
  M5.Lcd.println(" !");
  M5.Lcd.setCursor(120, 115);
  M5.Lcd.setTextColor(ORANGE);
  M5.Lcd.println("=^._.^=");

  M5.Lcd.setTextColor(CYAN);
  M5.Lcd.setCursor(60, 190);
  M5.Lcd.println("Press button C to");
  M5.Lcd.setCursor(100, 210);
  M5.Lcd.println("disconnect");
  //Arrow, vertical
  M5.Lcd.drawLine(255, 215, 255, 239, CYAN);
  M5.Lcd.drawLine(254, 215, 254, 239, CYAN);
  M5.Lcd.drawLine(256, 215, 256, 239, CYAN);
  //Arrow, left
  M5.Lcd.drawLine(255, 239, 245, 230, CYAN);
  M5.Lcd.drawLine(254, 239, 244, 230, CYAN);
  M5.Lcd.drawLine(256, 239, 246, 230, CYAN);
  //Arrow, right
  M5.Lcd.drawLine(255, 239, 265, 230, CYAN);
  M5.Lcd.drawLine(254, 239, 264, 230, CYAN);
  M5.Lcd.drawLine(256, 239, 266, 230, CYAN);
  
}
