#version 120

varying vec4 couleur;

varying vec3 L;
varying vec3 N;
varying vec3 V;

void main() {

    // tous les vecteur sont normalisé

    //-------------------------------------------------------------------------------//
    //                            Éclairement diffus                                 //
    //-------------------------------------------------------------------------------//
    // diffus=max(N·L,0.0)

    // la direction d’éclairement L est donnée par le built in gl_LightSource[0].position
    // (de type vec4, il s’agit de la valeur affectée par glLightfv(GL_LIGHT0,GL_POSITION,...)
    // par le squelette; la coordonnée w étant assurée à 1 vous pouvez prendre
    L = normalize(gl_LightSource[0].position.xyz);

    // le vecteur N doit être donnée dans le repère observateur pour le calcul d’éclairement,
    // et doit donc subir le repère Eye
    N = normalize(gl_NormalMatrix*gl_Normal);

    // diffus=max(N·L,0.0)
    float diffus = max(dot(L,N),0.0);

     //couleur=diffus*gl_FrontMaterial.diffuse;


    //-------------------------------------------------------------------------------//
    //                            Éclairement spéculaire                             //
    //-------------------------------------------------------------------------------//
    // spec =(V · R)^s


    // V est le vecteur observation=Vertex Eye.  Il faut qu’il soit exprimé dans le repère Eye !
    vec4 vertexEye = gl_ModelViewMatrix*gl_Vertex;

    // de signe contraire car dirigé vers l'utilisateur
    V = normalize(- vertexEye.xyz);

    // N doit toujours être orienté vers l'observateur
    if (dot(V,N)<0) N = -N;

    // R est donné par 2*(N.L)N−L.
    vec3 R = normalize(2*dot(N,L)*N - L);

    // Le coefficient de brillance s est donné par la constante prédéfinie gl_FrontMaterial.shininess
    // (affectée par l’application avec l’instruction glMaterial déjà faite dans le squelette)
    float S = gl_FrontMaterial.shininess;

    // la valeur du spéculaire a un sens si les vecteurs R et V vont dans la même direction (angle aigu) sinon l’éclairement est nul.
    // Donc lorsque les vecteurs R et V forment un angle obtus (V · R <0) il faut forcer un coefficient spec=0.
    // On peut obtenir cela en appliquant simplement en GLSL coef=max(dot(V,R),0.0;
    float coef = max(dot(V,R),0.0);

    // spec =(V · R)^s
    float spec = pow(coef,S);

    // Pour la couleur en sortie du vertex, il suffit d’affecter couleur avec la somme du diffus et du spéculaire.
    couleur = diffus*gl_FrontMaterial.diffuse + spec*gl_FrontMaterial.specular;

    gl_Position=gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
}
