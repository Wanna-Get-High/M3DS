#include "Material.h"
#include <iostream>

using namespace prog3d;
using namespace std;


Material::~Material() {};

Material::Material() {
  diffuse(Color(0.0,0.0,0.0));
  specular(Color(0,0,0.0));
  ambient(Color(0,0.0,0));
  shininess(0.0);
  reflectionCoefficient(0.0);
  refractionCoefficient(0.0);
  refractionIndex(0.0);
  _mapKd=NULL;
}

void Material::mapDiffuse(Texture *t) {
  _mapKd=t;
}

Texture *Material::mapDiffuse() {
  return _mapKd;
}

void Material::setGL() {
  float tab[4];
  tab[0]=ambient().r();
  tab[1]=ambient().g();
  tab[2]=ambient().b();
  tab[3]=alpha();
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,tab);

  tab[0]=diffuse().r();
  tab[1]=diffuse().g();
  tab[2]=diffuse().b();
  tab[3]=alpha();
//  cout << "Diffuse = " << diffuse() << endl;
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,tab);

  tab[0]=specular().r();
  tab[1]=specular().g();
  tab[2]=specular().b();
  tab[3]=alpha();
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,tab);

  glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,(int)shininess());

}

void Material::activeTexture() {
  if (_mapKd) {
    _mapKd->enable();
  }
}

ostream& prog3d::operator <<(std::ostream &s,const Material &q) {
    s << "(a=" << q.ambient() << ",d=" << q.diffuse() << ",s=" << q.specular() << ")";
    return s;

}



