uniform int occurence;
attribute int attrib;

#define SPEED 0.8
#define SPACE 10

void main()
{
	//gl_FrontColor = gl_Color;
	float intensity = int(mod(occurence*SPEED+attrib,SPACE))*(1.0/SPACE);
	gl_FrontColor.r = intensity;
	gl_FrontColor.g = 0;
	gl_FrontColor.b = 0;
	
	vec4 position = gl_Vertex;
	//position.z*=occurence;
    gl_Position = gl_ModelViewProjectionMatrix* position;

	gl_ClipVertex = gl_ModelViewMatrix * gl_Position;
	
}