#include <fstream>
#include <string>
#include <iostream>
#include "mediaDirectory.h"
#include "Error.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace std;

static string _mediaDirectory="";


void completeDirectory(std::string *dirName) {
  if (!dirName->empty() && dirName->at(dirName->length()-1)=='/') {
    dirName->erase(dirName->length()-1,1);
  }
}


std::string mediaDirectory() {
  if (_mediaDirectory.empty()) {
    _mediaDirectory=std::string(MEDIA_DIRECTORY);
    completeDirectory(&_mediaDirectory);
  }
  return _mediaDirectory;
}

bool fileExist(const std::string &filename) {
  fstream f;
  f.open(filename.c_str());
  if (f.good()) {
    f.close();
    return true;
  }
  return false;
}

void checkFile(const std::string &filename) {
  if (!fileExist(filename)) throw ErrorD("cant locate file "+filename+" (change the MEDIA_DIRECTORY in mediaDirectory.h ?)");
}

std::string resourceFile(const std::string &filename) {
  string res=mediaDirectory()+"/"+filename;
  checkFile(res);
  return res;
}

