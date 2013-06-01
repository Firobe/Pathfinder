struct s_material
{
    vec4 ambient, diffuse, specular;
    float shininess;
};
struct s_light
{
    vec4 position;
    vec4 ambient, diffuse, specular;
};

uniform s_light lighting;
uniform s_material material;

varying vec3 N;
varying vec3 v;
void main (void)
{
vec3 L;
    if(lighting.position[3] == 0)
    {L = normalize((gl_ModelViewProjectionMatrix*lighting.position).xyz );}
    else
   { L = normalize((gl_ModelViewProjectionMatrix*lighting.position).xyz - v);}
   vec3 V = normalize(v); // we are in Eye Coordinates, so EyePos is (0,0,0)
   vec3 R = normalize(-reflect(L,N));

   //calculate Ambient Term:
   vec4 Iamb = lighting.ambient*material.ambient;
   //calculate Diffuse Term:
   vec4 Idiff = lighting.diffuse*gl_Color * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);

   // calculate Specular Term:
   /*vec4 Ispec = lighting.specular*material.specular
                * pow(max(dot(R,V),0.0), material.shininess);*/
    vec4 Ispec = vec4(1.0,1.0,1.0,1.0)*vec4(0.7,0.7,0.7,1.0)
                * pow(max(dot(R,V),0.0), 60.0);
               // Ispec *= 0.0;
   Ispec = clamp(Ispec, 0.0, 1.0);
   // write Total Color:
   vec4 sceneColor = vec4(0.0,0.0,0.0,1.0);
   gl_FragColor = sceneColor + Iamb + Idiff + Ispec;
  // gl_FragColor = gl_Color;
}
