#include "Scene.h"
#include "UtilGL.h"
#include "tinyxml.h"
#include "IntersectionArray.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Error.h"
#include "mediaDirectory.h"

/**
@file
@author Fabrice Aubert
*/

using namespace std;
using namespace prog3d;



/**
Calcul de l'intersection avec la scène : retourne le point le plus proche parmi toutes les intersections calculées (par csg()->intersection).
Remarque : seuil permet de considérer uniquement les points tels que lambda>seuil
*/
Intersection *Scene::intersection(const Ray &ray,double seuil) {
  IntersectionArray intersectionArray;
  Intersection *result=NULL;

  intersectionArray.clear();
  if (csg()) csg()->intersection(ray,&intersectionArray); // calcul de l'intersection entre le rayon et l'arbre csg => au retour interList contient toutes les intersections

  if (intersectionArray.empty()) {
    return NULL;
  }

  // on sélectionne celle qui est la plus proche et dont le lambda est plus grand que seuil (pour par exemple considérer uniquement ce qui se trouve devant l'écran, et pas derriere avec lambda >0).
  IntersectionArray::iterator i=intersectionArray.begin();

  while (!result && i<intersectionArray.end()) {
    if ((*i)->lambda()>seuil) {
      result=*i;
      *i=NULL;
    }
    else i++;
  }

  if (result) {

    result->incident(ray);
    result->computePointNormal();
  }

  intersectionArray.deleteAll();

  return result;

}

/** ********************************************************************** */


Scene::Scene() {
  //ctor
  _csg=NULL;
  _light.clear();
  _intensity.clear();
  _camera.position(Vector3(0,0,0));
  _trackball.setIdentity();
}

Scene::~Scene() {
  //dtor
  if (_csg) delete _csg;
}

const Vector3 &Scene::lightPosition(unsigned int i) const {
  return _light[i];
}

double Scene::lightIntensity(unsigned int i) {
  return _intensity[i];
}

void Scene::addLight(const Vector3 &p,double intensity) {
  _light.push_back(p);
  _intensity.push_back(intensity);
}


CsgTree *Scene::readNode(TiXmlElement *elem,Material currentMat) {
  CsgTree *res;
  const char *attribute;
  res=new CsgTree();
  unsigned int node=0;
  if (strcmp(elem->Value(),"node")==0) {
    attribute=elem->Attribute("operation");
    if (strcmp(attribute,"difference")==0) {
      res->operation(CsgTree::DIFFERENCE);
    }
    else if (strcmp(attribute,"union")==0) {
      res->operation(CsgTree::UNION);
    }
    else if (strcmp(attribute,"intersection")==0) {
      res->operation(CsgTree::INTERSECTION);
    }

  }
  else if (strcmp(elem->Value(),"primitive")==0) {
    attribute=elem->Attribute("type");
    if (strcmp(attribute,"cube")==0) {
      res->primitive(new Cube());
    }
    else if (strcmp(attribute,"sphere")==0) {
      res->primitive(new Sphere());
    }
    else if (strcmp(attribute,"cone")==0) {
      res->primitive(new Cone());
    }
    else if (strcmp(attribute,"cylinder")==0) {
      res->primitive(new Cylinder());
    }

  }
  TiXmlElement *child;
  Matrix4 translate,rotate,scale;
  translate.setIdentity();
  rotate.setIdentity();
  scale.setIdentity();
  child=elem->FirstChildElement();
  double x,y,z,angle,red,green,blue,shininess,coefficient,index;
  while(child) {
    if (strcmp(child->Value(),"translate")==0) {
      child->QueryDoubleAttribute("x",&x);
      child->QueryDoubleAttribute("y",&y);
      child->QueryDoubleAttribute("z",&z);
      translate=Matrix4::translation(Vector3(x,y,z));
    }
    else if (strcmp(child->Value(),"rotate")==0) {
      child->QueryDoubleAttribute("angle",&angle);
      child->QueryDoubleAttribute("x",&x);
      child->QueryDoubleAttribute("y",&y);
      child->QueryDoubleAttribute("z",&z);
      rotate.rotate(angle,Vector3(x,y,z));
    }
    else if (strcmp(child->Value(),"scale")==0) {
      child->QueryDoubleAttribute("x",&x);
      child->QueryDoubleAttribute("y",&y);
      child->QueryDoubleAttribute("z",&z);
      scale=Matrix4::scaling(Vector3(x,y,z));
    }
    else if (strcmp(child->Value(),"diffuse")==0) {
      child->QueryDoubleAttribute("red",&red);
      child->QueryDoubleAttribute("green",&green);
      child->QueryDoubleAttribute("blue",&blue);
      currentMat.diffuse(Color(red,green,blue));
    }
    else if (strcmp(child->Value(),"ambient")==0) {
      child->QueryDoubleAttribute("red",&red);
      child->QueryDoubleAttribute("green",&green);
      child->QueryDoubleAttribute("blue",&blue);
      currentMat.ambient(Color(red,green,blue));
    }
    else if (strcmp(child->Value(),"specular")==0) {
      child->QueryDoubleAttribute("red",&red);
      child->QueryDoubleAttribute("green",&green);
      child->QueryDoubleAttribute("blue",&blue);
      child->QueryDoubleAttribute("shininess",&shininess);
      currentMat.specular(Color(red,green,blue));
      currentMat.shininess(shininess);
    }
    else if (strcmp(child->Value(),"reflection")==0) {
      child->QueryDoubleAttribute("coefficient",&coefficient);
      currentMat.reflectionCoefficient(coefficient);
    }
    else if (strcmp(child->Value(),"refraction")==0) {
      child->QueryDoubleAttribute("coefficient",&coefficient);
      child->QueryDoubleAttribute("index",&index);
      currentMat.refractionCoefficient(coefficient);
      currentMat.refractionIndex(index);
    }
    else if ((strcmp(child->Value(),"node")==0) || (strcmp(child->Value(),"primitive")==0)) {
      node++;
      if ((node>2) || res->isLeaf()) throw Error("Reading CSG : node found into primitive or there are more than 2 children",__LINE__,__FILE__);
      if (node==1) res->left(readNode(child,currentMat));
      else res->right(readNode(child,currentMat));
    }
    child=child->NextSiblingElement();
  }
  res->matrix(translate*rotate*scale);
  if (res->isLeaf()) {
    res->primitive()->material(currentMat);
  }
  return res;
}

void Scene::read(const string &resource) {
  string name=resourceFile(resource);
  TiXmlDocument doc(name.c_str());

  if (!doc.LoadFile()) {
    throw Error(string("cant locate the file ").append(name),__LINE__,__FILE__);
  }

  TiXmlHandle handle(&doc);
  TiXmlElement *elem=handle.FirstChildElement().ToElement();

  double x,y,z,intensity;

  Material currentMat;
  while (elem) {
    if ((strcmp(elem->Value(),"node")==0) || (strcmp(elem->Value(),"primitive")==0)) {
      _csg=readNode(elem,currentMat);
    }
    else if (strcmp(elem->Value(),"light")==0) {
      elem->QueryDoubleAttribute("x",&x);
      elem->QueryDoubleAttribute("y",&y);
      elem->QueryDoubleAttribute("z",&z);
      elem->QueryDoubleAttribute("intensity",&intensity);
      addLight(Vector3(x,y,z),intensity);
    }
    elem=elem->NextSiblingElement();
  }
}



void Scene::drawGL() {


  glPushMatrix();

  glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT);
  glDisable(GL_LIGHTING);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  for(unsigned int i=0; i<nbLight(); i++) {
    glPushMatrix();
    glTranslatef(lightPosition(i).x(),lightPosition(i).y(),lightPosition(i).z());
    glColor3f(0,1,0);
    glScalef(0.2,0.2,0.2);
    UtilGL::drawSphere();
    glPopMatrix();
  }
  glPopAttrib();


  Vector4 p;

  for(unsigned int i=0; i<7; i++) {
    glDisable(GL_LIGHT0+i);
    if (i<nbLight()) {
      glEnable(GL_LIGHT0+i);
      p.set(lightPosition(i));
      glLightfv(GL_LIGHT0+i,GL_POSITION,p.fv());
      p.set(Vector3(lightIntensity(i),lightIntensity(i),lightIntensity(i)));
      glLightfv(GL_LIGHT0+i,GL_DIFFUSE,p.fv());
      glLightfv(GL_LIGHT0+i,GL_SPECULAR,p.fv());
      p.set(Vector3(0,0,0));
      glLightfv(GL_LIGHT0+i,GL_AMBIENT,p.fv());
    }
  }
  p.set(Vector3(1,1,1));
  glLightfv(GL_LIGHT0,GL_AMBIENT,p.fv());


  if (_csg) _csg->drawParent();
  glPopMatrix();
}

void Scene::prepareCsg() {
  if (_csg) _csg->cumulateMatrix();
}







