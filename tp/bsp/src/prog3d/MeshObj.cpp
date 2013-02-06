#include "MeshObj.h"
#include "Vector3.h"
#include <fstream>
#include "Error.h"

using namespace std;
using namespace prog3d;


MeshObj::MeshObj() {
  _normalVertex.clear();
  _vertex.clear();
  _face.clear();
  _normalFace.clear();
}

MeshObj::~MeshObj() {

}


void MeshObj::check() {
  cout << "nb vertices = " << _vertex.size() << endl;
  cout << "nb faces = " << _face.size() << endl;
  for(vector<MeshFace>::iterator i=_face.begin();i!=_face.end();i++) {
    cout << "(";
    MeshFace::iterator j=(*i).begin();
    cout << *j; j++;
    for(;j!=(*i).end();j++) {
      cout << "," << *j;
    }
    cout << ")" << endl;
  }
}

void MeshObj::read(const string &filename) {
  fstream file;
  file.open(filename.c_str(),ios::in);
  file.width(20);
  if (!file.is_open()) {
    throw Error(string("cant load file data").append(filename),__LINE__,__FILE__);
  }

  char s[200]; // dummy to read non-interpreted line
  char read[30];
  char c;
  double x,y,z;
  unsigned int index,indexTexture,indexNormal;
  MeshFace face;

  bool readingFace=false;
  while (!file.eof()) {
    file.clear();
    file >> read;
    if (string(read).compare("v")==0) {
      file >> x >> y >> z;
      _vertex.push_back(Vector3(x,y,z));
      continue;
    }
    if (string(read).compare("f")==0) {
      face.clear();
      readingFace=true;
      while (readingFace) {
        file >> index;
        if (!file.fail()) {
          face.push_back(index-1); // starts at index 1 so -1
          file >> c;
          if (file.fail()) continue;
          if (c=='/') {
            file >> indexTexture;
            if (file.fail()) {
              file.clear();
            }
            file >> c;
            if (file.fail()) continue;
            if (c=='/') {
              file >> indexNormal;
            }
            else file.putback(c);
          }
          else file.putback(c);

        }
        else {
          readingFace=false;
        }
      }
      _face.push_back(face);
      continue;
    }
    file.getline(s,200);
  }
  file.close();
}


void MeshObj::computeNormalFace(unsigned int i) {
  Vector3 s1;
  Vector3 s2;
  Vector3 s3;
  Vector3 n;

  double dist=0;

  MeshFace::iterator it1=_face[i].begin();
  MeshFace::iterator it2=_face[i].begin()+1;
  MeshFace::iterator it3=_face[i].begin()+2;
  bool stop=false;
  bool normalOk=false;
  while (!normalOk && !stop) {
    s1=_vertex[*it1];
    s2=_vertex[*it2];
    s3=_vertex[*it3];
    Vector3 v1(s1,s2);
    Vector3 v2(s2,s3);
    n.cross(v1,v2);
    dist=n.length();
    if (dist>1e-05) {
      normalOk=true;
    }
    else {
      it3++;
      if (it3==_face[i].end()) {
        it2++;
        it3=it2+1;
        if (it3==_face[i].end()) {
          it1++;
          it2=it1+1;
          it3=it2+1;
        }
      }
      if (it3==_face[i].end()) {
        stop=true;
//        throw Error("Normal problem",__LINE__,__FILE__);
      }
    }
  }

  if (stop) {
    n.set(0.0,0.0,0.0);
  }
  else  n.scale(1.0/dist);
  _normalFace[i]=n;
}


void MeshObj::computeNormal() {
  _normalVertex.resize(_vertex.size());
  _normalFace.resize(_face.size());
  vector<unsigned int> nbFace; //nbFace per vertex
  nbFace.resize(_vertex.size());
  for(unsigned int i=0;i<_vertex.size();i++) {
    _normalVertex[i].set(0,0,0);
    nbFace[i]=0;
  }
  for(unsigned int i=0;i<_face.size();i++) {
    computeNormalFace(i);
  }
  for(unsigned int i=0;i<_face.size();i++) {
    for(unsigned int j=0;j<_face[i].size();j++) {
      _normalVertex[_face[i][j]]+=_normalFace[i];
      nbFace[_face[i][j]]++;
    }
  }
  for(unsigned int i=0;i<_vertex.size();i++) {
    _normalVertex[i]/=nbFace[i];
  }
}

void MeshObj::scaleInBox(double left,double right,double bottom,double top,double znear,double zfar) {
  Vector3 mini(_vertex[0]);
  Vector3 maxi(_vertex[0]);

  for(unsigned int i=1;i<_vertex.size();i++) {
    mini.setMinCoordinate(_vertex[i]);
    maxi.setMaxCoordinate(_vertex[i]);
  }
  for(unsigned int i=0;i<_vertex.size();i++) {
    _vertex[i].x((_vertex[i].x()-mini.x())/(maxi.x()-mini.x())*(right-left)+left);
    _vertex[i].y((_vertex[i].y()-mini.y())/(maxi.y()-mini.y())*(top-bottom)+bottom);
    _vertex[i].z((_vertex[i].z()-mini.z())/(maxi.z()-mini.z())*(zfar-znear)+znear);
  }
}

void MeshObj::scaleInBoxMin(double left,double right,double bottom,double top,double znear,double zfar) {
  Vector3 mini(_vertex[0]);
  Vector3 maxi(_vertex[0]);

  for(unsigned int i=1;i<_vertex.size();i++) {
    mini.setMinCoordinate(_vertex[i]);
    maxi.setMaxCoordinate(_vertex[i]);
  }

  Vector3 diag(mini,maxi);
  unsigned int which;
  double scale=diag.max(&which);
  scale=Vector3(right-left,top-bottom,zfar-znear).get(which)/scale;


  for(unsigned int i=0;i<_vertex.size();i++) {
    _vertex[i].x((_vertex[i].x()-mini.x())*scale+left+((right-left)-(maxi.x()-mini.x())*scale)/2.0);
    _vertex[i].y((_vertex[i].y()-mini.y())*scale+bottom+((top-bottom)-(maxi.y()-mini.y())*scale)/2.0);
    _vertex[i].z((_vertex[i].z()-mini.z())*scale+znear+((zfar-znear)-(maxi.z()-mini.z())*scale)/2.0);
  }
}

