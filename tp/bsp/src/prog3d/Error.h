#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <string>
#include <exception>

/**


@author F. Aubert
Use : throw Error(message,__LINE__,__FILE__)
*/

// critical error : stop the program
void error(std::string mesg,int line,std::string fichier);

// exception
class Error:public std::exception {
    std::string message;
    public:
    Error(std::string msg,int line,std::string fichier);
    virtual ~Error() throw();

    virtual const char * what() const throw();

    void show();

};
#endif
