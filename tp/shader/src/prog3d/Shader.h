#ifndef INC_SHADER_H
#define INC_SHADER_H

#include "glsupport.h"
#include <string>
#include <map>
#include "Vector3.h"
#include "Matrix4.h"

namespace prog3d {

  class Shader {
    GLuint programId,vertexId,fragmentId;
    bool _isInit;
    std::map<std::string,int> _mapUniform;

    static Shader *_current;

  public:
    Shader();
    virtual ~Shader();

    void init();
    void read(std::string nom);
    void readVertex(std::string nom);
    void readFragment(std::string nom);
    void link();

    void enable();
    void disable();

    int declareUniform(std::string nom);
    void uniform(const std::string &nom,float value);
    void uniform(int loc,float value);
    void uniform(const std::string &nom,int value);
    void uniform(int loc,int value);
    void uniform(const std::string &nom,const Vector3 &v);
    void uniform(int loc,const Vector3 &v);
    void uniform(const std::string &nom,const Matrix4 &m);
    void uniform(int loc,const Matrix4 &m);
    void uniform(const std::string &nom,const Vector4 &p);
    void uniform(int loc,const Vector4 &p);

    GLuint id();

    static Shader *current();

  private:
    void readSource(GLuint id,std::string nom);
    void compile(GLuint id);
    std::string *readFile(std::string nom);
    int findLocation(std::string nom);
  };

}

#endif

