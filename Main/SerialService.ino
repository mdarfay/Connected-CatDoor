void sendPermissionToSlave() {
  String chip = "";
  while(Serial.available() && chip.length() < 11) {
   chip += (char) Serial.read();
 }
 //M5.Lcd.println(chip);
 int permission = getPermissionFromChip(chip, 1);
 //M5.Lcd.print("permission: ");
 //M5.Lcd.print(permission);
 Serial.write(permission); // send permission to get IN to slave (assume slave is outside and let the cats in)
}


int sendChipToMaster(String chip) {
  Serial2.write(chip.c_str());
  while(!Serial2.available()); // active wait for a quick answer
  int permission = (int) Serial2.read();
  return permission;
}


// givePermissionIn is a boolean to tell which permission we want
int getPermissionFromChip(String chip, int givePermissionIn) {
  for(int i=0; i<nbCats; i++) {
    if(strcmp(cats[i].chip.c_str(), chip.c_str())==0) {
      if(givePermissionIn) {
        if(cats[i].permission_in) {
         cats[i].is_out = 0; // cat asks to get in, has the right to
        }
        return cats[i].permission_in;
      } else {
        if(cats[i].permission_out) {
         cats[i].is_out = 1; // cat asks to get out, has the right to
        }
        return cats[i].permission_out;
      }
    }
  }
  return 0;
}
