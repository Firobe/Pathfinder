uniform vec2 resolution;
/*uniform Lumiere lumiere;

struct Lumiere
{
    vec3 position;
    vec4 couleurAmbiante;
    vec4 couleurDiffuse;
    vec4 couleurSpeculaire;
    float shininess ;
};*/

void main(void)
{
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0)-gl_Color;
	gl_FragColor = gl_Color;
}