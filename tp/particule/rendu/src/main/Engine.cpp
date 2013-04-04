#include "Engine.h"
#include <cstdlib>

#include "ParticleList.h"
#include "Particle.h"

/**
@file
@author F. Aubert
@brief Basic particle engine

*/

using namespace std;
using namespace prog3d;



/** Intégration du mouvement
** - on parcourt l'ensemble des particules p : il faut mettre à jour la position et la vitesse de p durant l'intervalle de temps dt (passé en paramètre).
** - p->position() donne la position actuelle (de type Point3d), et p->position(unPoint3d) affecte la position de p
** - p->velocity() donne la vitesse actuelle (de type Vector3) et p->velocity(unVector3) affecte la vitesse de p
** - p->force() donne la somme des forces s'appliquant sur la particule p (de type Vector3 et calculée lors de la boucle principale).

** Il s'agit ici d'affecter la nouvelle position et la nouvelle vitesse en appliquant l'intégration d'Euler (cf cours).

**/
void Engine::euler(double dt) {
    for(unsigned int i=0; i<_particleList->size(); i++) {
        Particle *p=(*_particleList)[i];
        if (p->alive()) {

            Vector3 vitesse = p->velocity();
            p->position(p->position() + (vitesse*dt));
            p->velocity(vitesse + ((p->force()/p->mass())*dt));

        }
    }
}


/** Calcule les forces appliquées à chaque particule
** Pour ajouter une force il suffit de faire p->addForce(unVector3) (permet de cumuler avec, éventuellement, d'autres forces provenant d'autres procédures).
** Ici, la force est initialement nulle, et on se contente d'ajouter la force de gravité dans le sens des y négatifs.
** p->mass() donne la masse de la particule.

**/

void Engine::computeForce() {
    for(unsigned int i=0; i<_particleList->size(); i++) {
        Particle *p=(*_particleList)[i];
        if (p->alive()) {
            /* A COMPLETER : appliquer les forces subies */
            p->addForce(Vector3(0,-9.81,0)*p->mass());
        }
    }
}


/** Détermine s'il y a une collision avec les plans de la scène et modifie en conséquence la position et la vitesse de la particule

** - chaque plan de la scène est parcouru (tableau _planeList) : il faut déterminer la collision de chaque particule p avec ce plan
** simplement en testant si p est du coté négatif du plan. Si oui, la particule a traversé le plan et on corrige alors sa vitesse et sa position.

**/
void Engine::collisionPlane() {

    for(unsigned int j=0; j<_planeList.size(); j++) {
        Plane *plane = _planeList[j];

        for(unsigned int i=0; i<_particleList->size(); i++) {
            Particle *p = (*_particleList)[i];

            if (p->alive()) {
                Vector3 posCorrection(0,0,0); // correction en position à calculer si collision
                Vector3 velCorrection(0,0,0); // correction en vitesse à calculer si collision
                /* A COMPLETER  : détecter collision et corriger vitesse/position */
                /* on peut utiliser p->position(), p->velocity() (et les setters), et plane->point() et plane->normal() */
                /* p->radius() donne le rayon de la particule (exercice avec les sphère). */

                Vector3 point = ((p->position()-Vector3(0,p->radius(),0))-plane->point());

                // si il y a colision
                if( point.dot(plane->normal()) < 0 ) {

                    double epsilon = 0.1;

                    // xnew = xold + (1 + epsilon)(H - xOld)
                    // H = projection de xOld sur le plan
                    Vector3 xOld = p->position();
                    Vector3 projectionPlan = plane->project(xOld);

                    posCorrection = (1+epsilon) * (projectionPlan - (xOld - Vector3(0,p->radius(),0)));

                    // Vold - [(1-Ɛ)Vold.n]n
                    Vector3 vOld = p->velocity();
                    Vector3 n = plane->normal();

                    velCorrection = - ((1-epsilon) * (vOld.dot(n))) * n ;
                }

                // appliquer la correction éventuelle :
                p->addPositionCorrec(posCorrection);
                p->addVelocityCorrec(velCorrection);
            }
        }
    }
}

/** Calcul de l'impulsion entre 2 sphères
** - n = normale lors de la collision {\bf normalisée !}
** - restititution : > 0.9 superball ;  <0.1 ecrasement
**/
double Engine::computeImpulse(Particle *p1, Particle *p2,const Vector3 &n, double restitution) {
    double res;
    double v12n=(p2->velocity()-p1->velocity()).dot(n);
    double inv_mass=1.0/p1->mass()+1.0/p2->mass();
    res=-(1.0+restitution)*v12n/(inv_mass);
    return res;

}

/** Collisions entre toutes les sphères :
** - on teste toutes les paires de particules (double boucle, sans redondances) : s'il y a collision entre 2 sphères p1 et p2 on répond par impulsion.
** - utilisez p1->addVelocityCorrec(...), p2->addVelocityCorrec(...), etc pour tenir compte des corrections issues de cette collision qui seront à appliquer à p1 et p2
** - l'appel à computeImpulse vous donne le coefficient d'impulsion (cette méthode se trouve juste au dessus pour déterminer les paramètres).

**/
void Engine::interCollision() {
    for(unsigned int i=0; i<_particleList->size(); i++) {
        Particle *p1=(*_particleList)[i];
        if (p1->alive()) {
            for(unsigned int j=i+1; j<_particleList->size(); j++) {
                Particle *p2=(*_particleList)[j];
                if (p2->alive()) {
                    Vector3 posCorrectionP1(0,0,0); // correction en position de P1
                    Vector3 velCorrectionP1(0,0,0); // correction en vitesse de P1
                    Vector3 posCorrectionP2(0,0,0); // correction en position de P2
                    Vector3 velCorrectionP2(0,0,0); // correction en vitesse de P2

                    /* A COMPLETER */
                    Vector3 pos1 = p1->position();
                    Vector3 pos2 = p2->position();

                    double rad1 = p1->radius();
                    double rad2 = p2->radius();
                    double length = (pos1 - pos2).length();

                    if ( length < (rad1+rad2) ){

                        double d = length - rad1 - rad2;
                        Vector3 n = pos2 - pos1;
                        double epsilon = 0.1;
                        double mass1 = p1->mass();
                        double mass2 = p2->mass();

                        // p1new = p1old + (1+e) * m2 / (m1+m2) * D * N/N.N
                        // p2new = p2old - (1+e) * m1 / (m1+m2) * D * N/N.N
                        posCorrectionP1 = (1 + epsilon) * (mass2 / (mass1 + mass2)) * d * (n/n.dot(n));
                        posCorrectionP2 = - (1 + epsilon) * (mass1 / (mass1 + mass2)) * d * (n/n.dot(n));

                       //  v1new − v1old = − k / m1*N
                        //  v2new − v2old = k / m2*N
                        velCorrectionP1 = - (computeImpulse(p1,p2,n,epsilon)/mass1)*n;
                        velCorrectionP2 = (computeImpulse(p1,p2,n,epsilon)/mass2)*n;
                    }

                    // appliquer la correction éventuelle :
                    p1->addPositionCorrec(posCorrectionP1);
                    p1->addVelocityCorrec(velCorrectionP1);
                    p2->addPositionCorrec(posCorrectionP2);
                    p2->addVelocityCorrec(velCorrectionP2);
                }
            }
        }
    }


}

/** ************************************************************************************************************ */
/** ************************************************************************************************************ */
/** ************************************************************************************************************ */
/** ************************************************************************************************************ */


Engine::Engine() {
    //ctor
    _start=clock();
    _dt=0.01;

    _particleList=NULL;
    _windEnable=false;
    //  _boxList=NULL;
}

Engine::~Engine() {
    //dtor
}

void Engine::addPlane(Plane *p) {
    _planeList.push_back(p);
}

void Engine::particleList(ParticleList *l) {
    _particleList=l;
}

/*
void Engine::boxList(BoxList *b) {
  _boxList=b;
}
*/

void Engine::draw() {
    _particleList->draw();
    //  _boxList->draw();
}




void Engine::resetForce() {
    for(unsigned int i=0; i<_particleList->size(); i++) {
        Particle *p=(*_particleList)[i];
        if (p->alive()) {
            p->resetForce();
        }
    }
}





void Engine::updatePositionVelocity() {
    for(unsigned int i=0; i<_particleList->size(); i++) {
        Particle *p=(*_particleList)[i];
        if (p->alive()) {
            p->velocityCorrection();
            p->positionCorrection();
        }
    }
}


int Engine::nbParticle() {
    return _particleList->nbParticle();
}




/**
** Boucle principale (appelée à chaque tracé d'image) : résolution pour une durée dt
** - incrémentation du temps
** - résolution des collisions (détection/réponse par rapports aux plans, et aux autres sphères).
** - calcul des forces
** - intégration par euler
**/
void Engine::update() {
    // synchronisation très basique avec le "vrai temps"
    double elapsed;
    do {
        elapsed=double(clock()-_start)/CLOCKS_PER_SEC;
    } while (elapsed<_dt); // _dt est le pas de temps minimal : attente si pas atteint
    _start=clock();

    // réinitialise toutes les forces à 0
    resetForce();
    // résolution des collisions (affectation des corrections en vitesse et positions)
    // avec les plans de la scène :
    collisionPlane();
    // inter collisions entre toutes les particules :
    interCollision();
    // prise en compte des corrections :
    updatePositionVelocity();
    // calcul des forces appliquées à chaque particule
    computeForce();
    // calcul des positions/vitesses avec euler explicite
    euler(elapsed);
    // mise à jour des particules (naissances/morts)
    _particleList->updateLife();

}

void Engine::modeParticle(bool mode) {
    _modeParticle=mode;
    _particleList->modeParticle(mode);
}

void Engine::enableWind(const prog3d::Line &ray) {
    _wind=ray;
    _windEnable=true;
}

void Engine::disableWind() {
    _windEnable=false;
}



