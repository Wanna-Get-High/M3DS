/**

  @author F. Aubert
  **/

#include "Entity.h"

using namespace prog3d;
using namespace std;

/**
@author F. Aubert
*/

// Mapping de la structure de données stockée dans obj vers buffers OpenGL
void Entity::initBuffer() {
    // On commence par tout mettre dans des tableaux "bas niveaux" pour pouvoir les passer à OpenGL
    int size_pts = this->nbVertex();
    int size_face = this->nbFace();

    float *pts;
    pts = new float[size_pts*3]; // *3 car on mettra pour chaque sommet les 3 coordonnées x,y,z

    float *normalPts;
    normalPts = new float[size_pts*3];

    unsigned int *indice;
    indice=new unsigned int[size_face*3]; // *3 car on doit mettre les 3 indices constituant chacun des triangles.

    // A Compléter : mapping dans les tableaux + initialisation des buffers OpenGL
    // this->nbVertex() donne le nombre de sommets de l'objet
    // this->nbFace() donne le nombre de face de l'objet (CU : l'objet doit être uniquement constitué de triangles)
    // this->vertex(i) donne les coordonnées du i-ieme sommet (de type Vector3 : this->vertex(i).x() pour x, this->vertex(i).y() pour y, this->vertex(i).z() pour z).
    // this->normalVertex(i) donne les coordonnées de la normale du i-ieme sommet (de type Vector3).
    // this->indexVertex(i,j) donne l'indice k du j-ieme sommet dans la face i (k étant l'indice dans le tableau des sommets : this->vertex(k) pour avoir ses coordonnées).

    // pour la luminosité on lui passe les normales
    for (int i = 0; i < size_pts; i++) {
        normalPts[i*3] = this->normalVertex(i).x();
        normalPts[i*3+1] = this->normalVertex(i).y();
        normalPts[i*3+2] = this->normalVertex(i).z();
    }

    // 1) initialiser les tableaux pts et indice
    for (int i = 0; i < size_pts; i++) {
        pts[i*3] = this->vertex(i).x();
        pts[i*3+1] = this->vertex(i).y();
        pts[i*3+2] = this->vertex(i).z();
    }

    for (int i=0; i < size_face; i++) {
        indice[i*3] = this->indexVertex(i,0);
        indice[i*3+1] = this->indexVertex(i,1);
        indice[i*3+2] = this->indexVertex(i,2);
    }

    // 2) créer les buffers openGL et recopier les tableaux pts et indice dans ces buffers
    glGenBuffers(1,&_vertexBuffer);
    glGenBuffers(1,&_indiceBuffer);

    // VERTEX
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    // le 3 correspond aux x, y, z
    glBufferData (GL_ARRAY_BUFFER, 3 * size_pts * sizeof(GLfloat), pts ,GL_STATIC_DRAW);

    // INDICES
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceBuffer);
    // le 3 correspond aux 3 indices qui forme le triangle
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, 3 * size_face * sizeof(uint), indice ,GL_STATIC_DRAW);

    // NORMAL_POINT
    glBindBuffer(GL_NORMAL_ARRAY, _normalBuffer);
    // le 3 correspond aux 3 indices qui forme le triangle
    glBufferData (GL_NORMAL_ARRAY, 3 * size_pts * sizeof(GLfloat), normalPts ,GL_STATIC_DRAW);

    // on supprime les tableaux de la mémoire centrale (tout est maintenant dans la mémoire OpenGL)
    delete[] pts;
    delete[] indice;
    delete[] normalPts;

}


void Entity::drawBuffer() {

    //------------------------------------------------//
    //                      Enable                    //
    //------------------------------------------------//
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
    glEnableClientState(GL_NORMAL_ARRAY);



    //------------------------------------------------//
    //                      Vertex                    //
    //------------------------------------------------//
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    // les attributs gl_Vertex seront lus depuis le buffer bufferVertex (i.e. le buffer courant) :
    // le 3 correspond à: x, y, z
    glVertexPointer(3 ,GL_FLOAT,0 ,0);

    //------------------------------------------------//
    //               normal Vertex                    //
    //------------------------------------------------//
    glBindBuffer(GL_NORMAL_ARRAY, _normalBuffer);
    // les attributs gl_Vertex seront lus depuis le buffer bufferVertex (i.e. le buffer courant) :
    // pas besion de préciser le 3 car tjr par 3
    glNormalPointer(GL_FLOAT,0 ,0);


    //------------------------------------------------//
    //                      indices                   //
    //------------------------------------------------//
    // on doit juste lui faire un bind apres la fction glDrawElements
    // va créer la figure avec les indices de  _indiceBuffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceBuffer);

    // commande de tracé (exécution du pipeline de tracé) avec 4 sommets :
    // type, taille, type, offset
    glDrawElements(GL_TRIANGLES,nbFace()*3,GL_UNSIGNED_INT,0);

    //------------------------------------------------//
    //                      Disable                   //
    //------------------------------------------------//
    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
    glDisableClientState(GL_NORMAL_ARRAY);
}

/** ********************************************************************** */
/** ********************************************************************** */

Entity::Entity() {  //ctor
}

Entity::~Entity() { //dtor
}

void Entity::read(string nom) {
    MeshObj::read(nom);
    this->triangulate(); // assure un modèle triangulaire (suppose des faces convexes)
    this->computeNormal();
    this->scaleInBoxMin(-2,2,-2,2,-2,2); // le modèle sera dans la boite x=[-2,2],y=[-2,2] etc (changement d'échelle)
}


