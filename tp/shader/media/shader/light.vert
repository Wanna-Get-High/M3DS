#version 120

varying vec4 couleur;
varying vec4 vertexEye;

varying vec3 L;
varying vec3 N;
varying vec3 V;

void main() {

    // tous les vecteur sont normalisé

    //-------------------------------------------------------------------------------//
    //                            Éclairement diffus                                 //
    //-------------------------------------------------------------------------------//
    // diffus=max(N·L,0.0)

    //diffuse = gl_FrontMaterial.diffuse
    vec4 diffuse = vec4 (0,0,1,0);

    // la direction d’éclairement L est donnée par le built in gl_LightSource[0].position
    // (de type vec4, il s’agit de la valeur affectée par glLightfv(GL_LIGHT0,GL_POSITION,...)
    // par le squelette; la coordonnée w étant assurée à 1 vous pouvez prendre
    L = gl_LightSource[0].position.xyz;

    // le vecteur N doit être donnée dans le repère observateur pour le calcul d’éclairement,
    // et doit donc subir le repère Eye
    N = gl_NormalMatrix*gl_Normal;

    // diffus=max(N·L,0.0)
    float diffus = max(dot(normalize(L),(N)),0.0);

     //couleur=diffus*gl_FrontMaterial.diffuse;


    //-------------------------------------------------------------------------------//
    //                            Éclairement spéculaire                             //
    //-------------------------------------------------------------------------------//
    // spec =(V · R)^s


    // V est le vecteur observation=Vertex Eye.  Il faut qu’il soit exprimé dans le repère Eye !
    vertexEye = gl_ModelViewMatrix*gl_Vertex;


    // Pour la couleur en sortie du vertex, il suffit d’affecter couleur avec la somme du diffus et du spéculaire.
    couleur = diffus*diffuse;

    gl_Position=gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
}
