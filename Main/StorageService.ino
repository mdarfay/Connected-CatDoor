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
      key = i_string + ".permission_in";
      c.permission_in = preferences.getInt(key.c_str());
      key = i_string + ".permission_out";
      c.permission_out = preferences.getInt(key.c_str());

      cats[i] = c;
    }
  }
}


void saveCatData(int index, boolean updateNbCats) {
  struct cat c = cats[index];
  String index_string = String(index);
  String key = index_string + ".chip"; 
  preferences.putString(key.c_str(), c.chip.c_str());
  key = index_string + ".name";
  preferences.putString(key.c_str(), c.cat_name.c_str());
  key = index_string + ".permission_in";
  preferences.putInt(key.c_str(), c.permission_in);
    key = index_string + ".permission_out";
  preferences.putInt(key.c_str(), c.permission_out);

  if(updateNbCats) {
    key = "nbCats";
    preferences.putInt(key.c_str(),nbCats);
  }
}

void deleteCatData(int index) {
  // Remove last cat
  String i_string = String(nbCats);
  String key = i_string + ".chip";
  preferences.remove(key.c_str());
  key = i_string + ".name";
  preferences.remove(key.c_str());
  key = i_string + ".permission_in";
  preferences.remove(key.c_str());
    key = i_string + ".permission_out";
  preferences.remove(key.c_str());

  if (index < nbCats) {
    // Override last cat at index
    saveCatData(index, true);
  }
}
