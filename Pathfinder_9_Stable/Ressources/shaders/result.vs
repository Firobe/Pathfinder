uniform int occurence;
attribute int attrib;

#define SPEED 0.2
#define SPACE 4

void main()
{
	//gl_FrontColor = gl_Color;
	float intensity = int(mod(occurence*SPEED+attrib,SPACE))*(1.0/SPACE);
	gl_FrontColor.r = 0;
	gl_FrontColor.g = intensity;
	gl_FrontColor.b = intensity;
	
	vec4 position = gl_Vertex;
	//position.z*=occurence;
    gl_Position = gl_ModelViewProjectionMatrix* position;

	gl_ClipVertex = gl_ModelViewMatrix * gl_Position;
	
}