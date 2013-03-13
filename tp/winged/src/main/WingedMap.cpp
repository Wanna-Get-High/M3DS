#include "WingedMap.h"
#include "Error.h"


/**
@file
@author F. Aubert

*/


using namespace std;
using namespace prog3d;


WingedMap::WingedMap() {
  //ctor
}

WingedMap::~WingedMap() {
  //dtor
}

WVertex *WingedMap::wvertex(unsigned int i) {
  return _wvertex[i];
}

WEdge *WingedMap::findEdge(WVertex *v1,WVertex *v2) {
  bool found=false;
  unsigned int i,j;
  i=0;
  while ((i<_vertexEdge[v1->index()].size()) && !found) {
    j=0;
    while ((j<_vertexEdge[v2->index()].size()) && !found) {
      if (_vertexEdge[v1->index()][i]==_vertexEdge[v2->index()][j])
        found=true;
      else
        j++;
    }

    if (!found) i++;
  }

  if (found) return _vertexEdge[v1->index()][i]; else return NULL;
}


void WingedMap::read(const string &nom,Winged *winged) {
  _wvertex.clear();
  _vertexEdge.clear();
  _mesh=new MeshObj();
  _mesh->read(nom);
  _mesh->computeNormal();
  _mesh->scaleInBoxMin(-1,1,-1,1,-1,1);
//  soup->triangulate();
//  soup->eraseDuplicateVertex();

  /*
  cout << "NbVertex Soup = " << _mesh->nbVertex() << endl;
  cout << "NbFace Soup = " << _mesh->nbFace() << endl;
*/
  WEdge *we;
  WFace *wf;
  WVertex *wv_begin,*wv_end;

  _vertexEdge.resize(_mesh->nbVertex());
  for(unsigned int i=0;i<_mesh->nbVertex();i++) {
    _vertexEdge[i].clear();
    _wvertex.push_back(winged->createVertex(_mesh->vertex(i)));
  }

  for(unsigned int i=0;i<_mesh->nbFace();i++) {
//    cout << "FACE " << i << endl;
//    Facet *f=soup->face(i);
    wf=winged->createFace();
    wf->normal(_mesh->normalFace(i));
    for(unsigned int j=0;j<_mesh->nbVertexFace(i);j++) {
      // On cherche et on crée éventuellement un nouveau sommet
      unsigned int v=_mesh->indexVertexCycle(i,j);

      wv_begin=_wvertex[v];


      // On cherche et on crée éventuellement une nouvelle arête
      unsigned int next=_mesh->indexVertexCycle(i,j+1);
      wv_end=_wvertex[next];

      if (wv_begin!=wv_end) {

//      cout << "Edge from soup = " << v << " and " << next << endl;
//      cout << "Edge from winged = " << wv_begin << " and " << wv_end << endl;

      we=this->findEdge(wv_begin,wv_end);
      if (we) {
//          cout << "edge already created =" << we << endl;
//          cout << "left = " << we->left() << " and right =" << we->right() << endl;
        if (we->begin()==wv_begin) {
          if ((we->begin()!=wv_begin) || (we->end()!=wv_end)) error("Edge found incoherent",__LINE__,__FILE__);
          if (we->left()!=NULL) error("left already set",__LINE__,__FILE__);
          we->left(wf);
        }
        else {
          if ((we->end()!=wv_begin) || (we->begin()!=wv_end)) error("Edge found incoherent",__LINE__,__FILE__);
          if (we->right()!=NULL) error("right already set",__LINE__,__FILE__);
          we->right(wf);
        }
      }
      else {
        we=winged->createEdge(wv_begin,wv_end);
        _vertexEdge[wv_begin->index()].push_back(we);
        _vertexEdge[wv_end->index()].push_back(we);
        we->left(wf);
//          cout << "edge JUST created =" << we << endl;
//          cout << "left = " << we->left() << " and right =" << we->right() << endl;
      }

      wv_begin->edge(we);
      wv_end->edge(we);
      }
      else
        error("bug",__LINE__,__FILE__);
    }
    wf->edge(we);

    unsigned int pred,succ,current0,current1;
    current0=_mesh->indexVertexCycle(i,0);
    current1=_mesh->indexVertexCycle(i,1);
    pred=_mesh->indexVertexCycle(i,-1);
    succ=_mesh->indexVertexCycle(i,2);;
    WEdge *currente,*prede,*succe;
    currente=this->findEdge(_wvertex[current0],_wvertex[current1]);
    prede=this->findEdge(_wvertex[pred],_wvertex[current0]);
    succe=this->findEdge(_wvertex[current1],_wvertex[succ]);

    for(unsigned int j=0;j<_mesh->nbVertexFace(i);j++) {
//  cout << "trouve les sommets ?" <<_v[current0]->point() << ","<< _v[current1]->point() << endl;
//    cout << "trouve l'arete ?" << currente << endl;
      if ((currente==succe) || (currente==prede))
        error("ouhla ",__LINE__,__FILE__);
      if (currente->left()==wf) {
          currente->succLeft(succe);
          currente->predLeft(prede);
      }
      else {
        if (currente->right()!=wf) error("No face for the edge !",__LINE__,__FILE__);
        currente->succRight(succe);
        currente->predRight(prede);
      }
      pred=current0;
      current0=current1;
      current1=succ;
      succ=_mesh->indexVertexCycle(i,j+3);
      prede=currente;
      currente=succe;
      succe=this->findEdge(_wvertex[current1],_wvertex[succ]);
      if (!succe) error("PAS DE SUCCESSEUR",__LINE__,__FILE__);
//      cout << "succ sommet = " << succ->point() << endl;
//      cout << "succ edge = " << succe << endl;
    }

    /*
    WEdge *currentver=wf->edge();
    for(unsigned int j=0;j<f->nbVertex();j++) {
      cout << "edge soup = " << f->vertex(j)->point() << "," << f->vertexSucc(j)->point() << endl;
      cout << "edge winged =" << currentver->begin()->point() << "," << currentver->begin()->point() << endl;
      if (currentver->left()==wf) currentver=currentver->succLeft(); else currentver=currentver->succRight();
    }
*/

  }
  /*
  cout << "NbVertex Winged = " << winged->nbVertex() << endl;
  cout << "NbFace Winged = " << winged->nbFace() << endl;
  cout << "NbEdge Winged = " << winged->nbEdge() << endl;


  cout << "Check Vertex Bound" << endl;
  winged->index();
  winged->checkVertexBound();
  */
//  winged->check();
  /*
  cout << "Recheck after correction" << endl;
  winged->checkVertexBound();
*/
  delete _mesh;
}

