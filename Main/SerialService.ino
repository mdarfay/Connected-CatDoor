void checkChipFromSlave() {
  String chip = "";
  while(Serial.available()) {
   chip += (char) Serial.read();
 }

 Serial.write( getPermissionFromChip(chip, 1) ); // send permission to get IN to slave (assume slave is outside and let the cats in)
}


char checkChipFromMaster(String chip) {
  Serial2.write(chip.c_str());
  while(!Serial2.available()); // active wait for a quick answer
  char permission = (char) Serial2.read();
  return permission;
}


// givePermissionIn is a boolean to tell which permission we want
int getPermissionFromChip(String chip, int givePermissionIn) {
  for(int i=0; i<nbCats; i++) {
    if(strcmp(cats[i].chip.c_str(), chip.c_str())==0) {
      if(givePermissionIn) {
        return cats[i].permission_in;
      } else {
        return cats[i].permission_out;
      }
    }
  }
  return -1;
}
