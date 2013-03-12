#version 120

varying vec4 couleur;

varying vec3 L;
varying vec3 N;
varying vec3 V;

varying float diffus;

void main() {

    //gl_FragColor = couleur;

    // Q17
    vec3 R = normalize(2*dot(N,L)*N - L);

    float coef = max(dot(V,R),0.0);

   float spec = pow(coef,gl_FrontMaterial.shininess);

    gl_FragColor = diffus*gl_FrontMaterial.diffuse + spec*gl_FrontMaterial.specular;
}
