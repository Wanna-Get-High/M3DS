#ifndef MEDIADIRECTORY_H
#define MEDIADIRECTORY_H

// changer éventuellement le répertoire où se trouvent toutes les ressources
#define MEDIA_DIRECTORY "../media"

#include <string>


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

std::string mediaDirectory();
void checkFile(const std::string &filename);
std::string resourceFile(const std::string &resourceName);

#endif // MEDIADIRECTORY_H

