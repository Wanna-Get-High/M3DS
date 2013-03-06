/**

  @author F. Aubert
  **/

#include "Square.h"

#include <iostream>

using namespace std;


/**
  Initialisation des buffers pour le carré
*/
void Square::initBuffer() {
    // TODO
    // float vertex[]={x0,y0,z0,x1,y1,z1,x2,y2,z2, etc}

    // initialisation des données des vertex pour chaque sommet du carré
    //                   x    y    z
    float vertex[] = {  0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0,
                        1.0, 1.0, 0.0};

    // 1 correspond au nombre de buffer voulu
    glGenBuffers(1, &_bufferVertex);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferVertex);
    glBufferData (GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertex ,GL_STATIC_DRAW);

    // initialisation des données des couleur pour chaque sommet du carré
    //                       r    g    b
    float vertexColors[] = {1.0, 0.0, 0.0,
                            1.0, 1.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0 };

    glGenBuffers(1, &_bufferColor);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferColor);
    glBufferData (GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertexColors ,GL_STATIC_DRAW);

    // pour la première texture

    // initialisation des données des couleur pour chaque sommet du carré
    //                           u    v
    float vertexTexture[] = {   0.0, 0.0,
                                1.0, 0.0,
                                0.0, 1.0,
                                1.0, 1.0};

    glGenBuffers(1, &_bufferTexCoord[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferTexCoord[0]);
    glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertexTexture ,GL_STATIC_DRAW);

    glGenBuffers(1, &_bufferTexCoord[1]);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferTexCoord[1]);
    glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertexTexture ,GL_STATIC_DRAW);

    glGenBuffers(1, &_bufferTexCoord[2]);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferTexCoord[2]);
    glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertexTexture ,GL_STATIC_DRAW);
}

/**
  Tracé par buffer du carré
*/
void Square::drawBuffer() {
    //------------------------------------------------//
    //                      Enable                    //
    //------------------------------------------------//
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);


    //------------------------------------------------//
    //                      les points                //
    //------------------------------------------------//
    // le buffer courant des données des vertex
    glBindBuffer(GL_ARRAY_BUFFER, _bufferVertex);
    // les attributs gl_Vertex seront lus depuis le buffer bufferVertex (i.e. le buffer courant) :
    // le 3 correspond a: x, y, z
    glVertexPointer (3 ,GL_FLOAT,0 ,0);

    //------------------------------------------------//
    //                      Color                     //
    //------------------------------------------------//
    // le buffer courant des données des couleurs
    glBindBuffer (GL_ARRAY_BUFFER, _bufferColor);
    // le 3 correspond a: r, g, b
    glColorPointer (3 ,GL_FLOAT,0 ,0);


    //------------------------------------------------//
    //                      Texture0                  //
    //------------------------------------------------//
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // on bind l'image texture au carré
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_idTexture[0]);

    // le buffer courant des données de la première texture
    glBindBuffer (GL_ARRAY_BUFFER, _bufferTexCoord[0]);
    // le 2 correspond au coordonnées de texture: u, v
    glTexCoordPointer(2 ,GL_FLOAT,0 ,0);

    //------------------------------------------------//
    //                      Texture1                  //
    //------------------------------------------------//

    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // on bind l'image texture au carré
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,_idTexture[1]);

    // le buffer courant des données de la première texture
    glBindBuffer (GL_ARRAY_BUFFER, _bufferTexCoord[1]);
    // le 2 correspond au coordonnées de texture: u, v
    glTexCoordPointer(2 ,GL_FLOAT,0 ,0);

    //------------------------------------------------//
    //                      Texture2                  //
    //------------------------------------------------//

    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // on bind l'image texture au carré
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,_idTexture[2]);

    // le buffer courant des données de la première texture
    glBindBuffer (GL_ARRAY_BUFFER, _bufferTexCoord[2]);
    // le 2 correspond au coordonnées de texture: u, v
    glTexCoordPointer(2 ,GL_FLOAT,0 ,0);



    // commande de tracé (exécution du pipeline de tracé) avec 4 sommets :
    glDrawArrays (GL_TRIANGLE_STRIP,0 ,4);

    //------------------------------------------------//
    //                      Disable                   //
    //------------------------------------------------//
    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


Square::Square() {
    _idTexture.resize(3);
    _bufferTexCoord.resize(3);
}

Square::~Square(){
}

void Square::texture(unsigned int unite,GLuint id) {
    _idTexture[unite]=id;
}



