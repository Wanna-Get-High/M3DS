#version 120

varying vec4 couleur;

varying vec3 L;
varying vec3 N;
varying vec3 V;

void main() {

    //gl_FragColor = vec4(0.3*L + 0.3*N + 0.3*V);
    gl_FragColor = couleur;

}
