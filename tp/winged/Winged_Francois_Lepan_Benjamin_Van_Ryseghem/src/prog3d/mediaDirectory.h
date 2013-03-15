#ifndef MEDIADIRECTORY_H
#define MEDIADIRECTORY_H

#define MEDIA_DIRECTORY "../media"

#include <string>

std::string mediaDirectory();
void checkFile(const std::string &filename);
std::string resourceFile(const std::string &resourceName);

#endif // MEDIADIRECTORY_H

