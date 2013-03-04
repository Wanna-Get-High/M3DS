/**

  @author F. Aubert
  **/
#include "Shader.h"
#include "Error.h"

#include <sstream>
#include <iostream>
#include <fstream>

using namespace prog3d;
using namespace std;

static void errorQuery(string mesg="test error") {
  cout << "==============================" << endl;
  GLuint err=glGetError();
  if (err>0) {
    cout << "Error GL : " << gluErrorString(err) << endl;
    cout << "Message : " << mesg << endl;
  } else {
    cout << "no error at this point" << endl;
    cout << "==============================" << endl;
  }
}

Shader *Shader::_current=NULL;

Shader::Shader() {
  _isInit=false;
}

Shader::~Shader() {
}

void Shader::init() {
  programId=glCreateProgram();
  fragmentId=glCreateShader(GL_FRAGMENT_SHADER);
  vertexId=glCreateShader(GL_VERTEX_SHADER);

  glAttachShader(programId,vertexId);
  glAttachShader(programId,fragmentId);


  _isInit=true;
}

string *Shader::readFile(string nom) {
  ifstream f(nom.c_str());

  if (f.fail()) error("fichier shader inexistant",__LINE__,__FILE__);

  string *result=new string;
  stringstream buf;

  buf << f.rdbuf();
  *result=buf.str();
  return result;
}

void Shader::readSource(GLuint id,string nom) {
  string *source=readFile(nom);
  const char *buf=source->c_str();
  glShaderSource(id,1,&buf,NULL);
  delete source;
}


void Shader::compile(GLuint id) {
  glCompileShader(id);

  int compile_ok;
  int info_length;

  glGetShaderiv(id,GL_COMPILE_STATUS,&compile_ok);
  glGetShaderiv(id,GL_INFO_LOG_LENGTH,&info_length);


   cout << (compile_ok?"ok":"failed") << endl;
  if (!compile_ok) {
    char *info=new char[info_length];
    glGetShaderInfoLog(id,info_length,NULL,info);
    cout << "Info Log :" << endl;
    cout << info;
    cout << endl;
    delete info;
    throw ErrorD("Shader compilation error");
  }



}

void Shader::readVertex(string nom) {
  readSource(vertexId,nom+".vert");
  cout << "compilation vertex " << nom+".vert ";
  compile(vertexId);
}


void Shader::readFragment(string nom) {
  readSource(fragmentId,nom+".frag");
  cout << "compilation fragment " << nom+".frag ";
  compile(fragmentId);
}

void Shader::read(string nom) {
  cout << "*** Lecture/compilation/link shader ***" << endl << nom << endl;
  if (!_isInit) init();
  readVertex(nom);
  readFragment(nom);
  link();
  cout << "**********************************" << endl;
}

void Shader::link() {
  glLinkProgram(programId);


  int link_ok,info_length;

  glGetProgramiv(programId,GL_LINK_STATUS,&link_ok);
  glGetProgramiv(programId,GL_INFO_LOG_LENGTH,&info_length);


  cout << "statut link : " << (link_ok?"ok":"failed") << endl;
  if (!link_ok) {
    char *info=new char[info_length];
    glGetProgramInfoLog(programId,info_length,NULL,info);
    cout << "Info Log :" << endl;
    cout << info;
    cout << endl;
    delete info;
    throw ErrorD("Link shader program error");
  }





  // redeclare Uniform already set
  map<string,int> copie=_mapUniform;
  map<string,int>::iterator it;
  for(it=copie.begin();it!=copie.end();it++) {
    string nom=it->first;
    _mapUniform.erase(nom);
    declareUniform(nom);
  }

}

void Shader::enable() {
  glUseProgram(programId);
  Shader::_current=this;
}

void Shader::disable() {
  glUseProgram(0);
  Shader::_current=NULL;
}

int Shader::declareUniform(string nom) {
  int result;
  result=glGetUniformLocation(programId,nom.c_str());
  _mapUniform[nom]=result;
  return result;
}


int Shader::findLocation(string nom) {
 map<string,int>::iterator loc;
  if ((loc=_mapUniform.find(nom))==_mapUniform.end()) {
    return declareUniform(nom);
  }
  else
    return loc->second;
}

void Shader::uniform(const string &nom,float value) {
  int loc=findLocation(nom);
  glUniform1f(loc,value);
}

void Shader::uniform(int loc,float value) {
  glUniform1f(loc,value);
}

void Shader::uniform(const string &nom,int value) {
  int loc=findLocation(nom);
  glUniform1i(loc,value);
}

void Shader::uniform(int loc,int value) {
  glUniform1i(loc,value);
}


void Shader::uniform(const string &nom,const Vector3 &v) {
  int loc=findLocation(nom);
  glUniform3fv(loc,1,v.fv());
}

void Shader::uniform(int loc,const Vector3 &v) {
  glUniform3fv(loc,1,v.fv());
}

void Shader::uniform(const string &nom,const Matrix4 &m) {
  int loc=findLocation(nom);
  glUniformMatrix4fv(loc,1,GL_FALSE,m.fv());
}

void Shader::uniform(int loc,const Matrix4 &m) {
  glUniformMatrix4fv(loc,1,GL_FALSE,m.fv());
}


void Shader::uniform(const string &nom,const Vector4 &p) {
  int loc=findLocation(nom);
  glUniform4fv(loc,1,p.fv());
}

void Shader::uniform(int loc,const Vector4 &p) {
  glUniform4fv(loc,1,p.fv());
}


GLuint Shader::id() {
  return programId;
}

Shader *Shader::current() {
  return Shader::_current;
}






