#include "ControlVector3.h"
#include "ViewportUtil.h"
#include "Vector3.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include "Error.h"
#include "glsupport.h"
#include "Segment.h"

/*
You have to initialize the view (transformation from local/eye/window...) by calling setView() from an OpenGL display
process (it sets the view from the current MODELVIEW and PROJECTION matrices)



Fabrice Aubert
*/


using namespace std;
using namespace prog3d;

ControlVector3::~ControlVector3() {
  delete _view;
  if (_isOwner) {
    for(unsigned int i=0;i<_data.size();i++) {
      delete _data[i];
    }
  }
}

ControlVector3::ControlVector3() {
  _view=new ViewportUtil();
  _selected=NULL;
  _data.clear();
  _isOwner=false;
}

void ControlVector3::createData(unsigned int nb) {
  if (!_isOwner && (_data.size()!=0)) throw Error("Data already set from another object",__LINE__,__FILE__);
  if (_data.size()==0) {
    _isOwner=true;
  }
  else
  {
    for(unsigned int i=0;i<_data.size();i++) {
      delete _data[i];
    }
    _data.clear();
  }
  for(unsigned int i=0;i<nb;i++) {
    _data.push_back(new Vector3(0,0,0));
  }
}

vector<Vector3*>::iterator ControlVector3::indexOf(Vector3 *p) {
  return find(_data.begin(),_data.end(),p);
}

void ControlVector3::setView() {
  _view->setView();
}

void ControlVector3::deleteData() {
  if (!_isOwner) throw Error("cant delete : controled points are not owned by the controller",__LINE__,__FILE__);
  for(unsigned int i=0;i<_data.size();i++) {
    delete _data[i];
  }
  _data.clear();
  _selected=NULL;
}


void ControlVector3::backup() {
  _backup.clear();
  for (std::vector<Vector3*>::iterator i=_data.begin();i!=_data.end();i++) {
    _backup.push_back(*(*i));
  }
}


void ControlVector3::restore() {
  vector<Vector3>::iterator j=_backup.begin();
  for (vector<Vector3*>::iterator i=_data.begin();i!=_data.end();i++,j++) {
    (*i)->set(*j);
  }
}


Vector3 ControlVector3::add(const Vector2 &mouse,unsigned *where) {
  Vector3 res;
  if (size()>1) {
    Vector2 a=_view->windowCoordinate(point(0));
    double d=mouse.distance2(a);
    a=_view->windowCoordinate(point(size()-1));
    int i;
    if (mouse.distance2(a)<d) i=size();
    else i=0;
    res=_view->applyLocalWindow(mouse,point(size()-1));
    *where=i;
  } else {
    *where=0;
    res=_view->applyLocalWindow(mouse,-0.9);
  }
  return res;
}

Vector3 ControlVector3::addEnd(const Vector2 &mouse) {
  Vector3 res;
  res=_view->applyLocalWindow(mouse,-0.9);
  if (_isOwner) {
    _data.push_back(new Vector3(res));
  }
  return res;
}

/*
void ControlVector3::insert(const Vector2 &mouse,Vector3 *p) {
  AVector3::iterator i=find(_data->begin(),_data->end(),p);
  Vector3 pointSouris=_view->applyLocalWindow(mouse,*p);
  _data->insert(i+1,new Vector3(pointSouris));
}

void ControlVector3::erase(Vector3 *p) {
  AVector3::iterator i=find(_data->begin(),_data->end(),p);
  _data->erase(i);
  delete p;
}


void ControlVector3::insert(Vector2 mouse,Vector3 *a,Vector3 *b) {
  AVector3::iterator ia=find(_data->begin(),_data->end(),a);
  AVector3::iterator ib=find(_data->begin(),_data->end(),b);
  AVector3::iterator i;
  if (ia>ib) i=ia;
  else i=ib;


  // compute an approximative (no perspective correction) depth for the inserted point (from the computation of the projected point mouse onto the edge in 2d)
  Vector2 a2d(_view->windowCoordinate(*a));
  Vector2 b2d(_view->windowCoordinate(*b));
  Vector2 u(b2d);
  u.sub(a2d);
  Vector2 am(mouse);
  am.sub(a2d);

  double lambda=am.dot(u)/u.dot(u);

  Vector3 ref(*b);
  ref.sub(*a);
  ref.scaleAdd(lambda,*a);


  Vector3 pointSouris=_view->applyLocalWindow(mouse,ref);
  _data->insert(i,new Vector3(pointSouris));
}
*/

void ControlVector3::data(vector<Vector3> *d) {
  _data.clear();
  _selected=NULL;
  for(unsigned int i=0;i<d->size();i++) {
    _data.push_back(&(*d)[i]);
  }
}


void ControlVector3::appendData(vector<Vector3> *d) {
  _selected=NULL;
  for(unsigned int i=0;i<d->size();i++) {
    _data.push_back(&(*d)[i]);
  }
}

void ControlVector3::data(vector< vector<Vector3> > *d) {
  _data.clear();
  _selected=NULL;
  for(unsigned int i=0;i<d->size();i++) {
    for(unsigned int j=0;j<(*d)[i].size();j++) {
      _data.push_back(&(*d)[i][j]);
    }
  }
}


Vector3 ControlVector3::insertNearest(const Vector2 &mouse,double seuil,int *where) {
  Vector3 res;
  if (_data.size()<2) {
    res=addEnd(mouse);
    *where=_data.size()-1;
  }
  else {
    unsigned int which;
    bool ok=nearestSegment(mouse,&which,seuil);
    cout << "nearest segment = " << which << endl;
    if (ok) {
      Vector3 extreme1=*_data[which];
      res=_view->applyLocalWindow(mouse, extreme1);
      *where=which;

    }
    else *where=-1;
  }
  return res;
}

void ControlVector3::draw() {
  glColor3f(0,0,1);
  glBegin(GL_LINE_STRIP);
  for (vector<Vector3*>::iterator a=_data.begin();a!=_data.end();a++) {
    glVertex3dv((*a)->dv());
  }
  glEnd();
  glPointSize(5);
  glBegin(GL_POINTS);
  for (vector<Vector3*>::iterator a=_data.begin();a!=_data.end();a++) {
    glVertex3dv((*a)->dv());
//		  (*a)->print("Point = ");
  }
  glEnd();
  glPointSize(1);
}



bool ControlVector3::nearestSegment(const Vector2 &m,unsigned int *which,double seuil) {
  bool res=true;
  Vector3 *a;
  Vector3 *b;
  unsigned int it=0;
  a=_data[it++];
  b=_data[it++];
  *which=0;
  Vector2 pA=_view->windowCoordinate(*a);
  Vector2 pB=_view->windowCoordinate(*b);
  Segment ab(pA,pB);
  double dMin=ab.distance2(m);
  for(;it<_data.size();it++) {
    a=b;
    b=_data[it];
    pA=pB;
    pB=_view->windowCoordinate(*b);
    ab.setAB(pA,pB);
    double d=ab.distance2(m);
    if (d<dMin) {
      dMin=d;
      *which=it-1;
    }
  }
  if (sqrt(dMin)>seuil) {
    *which=0;
    res=false;
  }
  return res;
}


unsigned int ControlVector3::nearestPoint(const Vector2 &m) {
  Vector3 *proche;
  Vector2 pproche;
  vector<Vector3*>::iterator it=_data.begin();
  int index=0;
  int indexMin=0;
  proche=*(it++);
  pproche=_view->windowCoordinate(*proche);
  double dMin=m.distance2(pproche);
  while (it!=_data.end()) {
    index++;
    Vector3 *a=*(it++);
    pproche=_view->windowCoordinate(*a);
    double d=m.distance2(pproche);
    if (d<dMin) {
      indexMin=index;
      dMin=d;
    }
  }
  return indexMin;
}


void ControlVector3::controlMouse(Vector3 *p,const Vector2 &m) {
  Vector3 a;
  a.set(_view->applyLocalWindow(m, *p));
  p->set(a);
}

void ControlVector3::startMouse(const Vector2 &m) {
  Vector3 *nearest;
  nearest=_data[nearestPoint(m)];
  _startingMouse.set(_view->applyLocalWindow(m,*nearest));
}

void ControlVector3::controlTranslate(const Vector2 &m) {
//        if (!startingMouse) throw Error("no startingMouse (call startMouse in the mouse pressed callback)",__LINE__,__FILE__);
  Vector3 where;
  where.set(_view->applyLocalWindow(m,_startingMouse));
  Vector3 t;
  t.setVector(_startingMouse,where);
  restore();
  for (vector<Vector3*>::iterator i=_data.begin();i!=_data.end();i++) {
    (*i)->add(t);
  }
}

void ControlVector3::controlScale(const Vector2 &m) {
//        if (!startingMouse) throw Error("no startingMouse (call startMouse in the mouse pressed callback)",__LINE__,__FILE__);
  Vector3 where;
  where.set(_view->applyLocalWindow(m,_startingMouse));
  Vector3 t;
  restore();
//        startingMouse.print();
//        where.print();
  t.set(where.x()/_startingMouse.x(),where.y()/_startingMouse.y(),where.z()/_startingMouse.z());
//        t.sVector(startingMouse,where);
//        t.scaleAdd(0.1,Vector3d(1,1,1));
  for (vector<Vector3*>::iterator i=_data.begin();i!=_data.end();i++) {
    (*i)->scale(t.x(),t.y(),t.z());
  }
}


void ControlVector3::selectNearest(const Vector2 &m,double seuil) {
  if (_data.size()==0) _selected=NULL;
  else {
    _selected=_data[nearestPoint(m)];
    Vector2 selected(_view->applyWindowLocal(*_selected));
    if (selected.distance(m)>seuil) {
      _selected=NULL;
    }
  }
}

void ControlVector3::controlSelected(const Vector2 &m) {
  if (_selected)
    controlMouse(_selected,m);
}


