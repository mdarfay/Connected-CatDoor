void setupCatData() {
  int nbCats_tmp = preferences.getInt("nbCats");
  if(nbCats_tmp > 0) {
    nbCats = nbCats_tmp;

    String i_string;
    String key;

    for (int i=0; i<nbCats; i++) {
      struct cat c;
      i_string = String(i);
      key = i_string + ".chip";
      c.chip = preferences.getString(key.c_str());
      key = i_string + ".name";
      c.cat_name = preferences.getString(key.c_str());
      key = i_string + ".permission";
      c.permission = preferences.getInt(key.c_str());

      cats[i] = c;
    }
  }
}


void saveCatData(int index) {
  Serial.println("index of save:");
  Serial.println(index);
  struct cat c = cats[index];
  String index_string = String(index);
  String key = index_string + ".chip"; 
  preferences.putString(key.c_str(), c.chip.c_str());
  key = index_string + ".name";
  Serial.println("name of save:");
  Serial.println(c.cat_name.c_str());
  Serial.println(c.cat_name);
  preferences.putString(key.c_str(), c.cat_name.c_str());
  key = index_string + ".permission";
  preferences.putInt(key.c_str(), c.permission);
  key = "nbCats";
  preferences.putInt(key.c_str(),index);
}
