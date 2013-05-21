uniform vec2 resolution;

 
 #define PI 3.14
varying float theta;

void main()
{
	gl_FrontColor = gl_Color;
    gl_Position = ftransform();
	/*theta = sqrt(pow((gl_Position.x-resolution.x/2),2)+pow((gl_Position.y-resolution.y/2),2))/resolution.x*1*PI;
	rotation = mat2(vec2(cos(theta), -sin(theta)),vec2(sin(theta), cos(theta)));
	vec2 position = vec2(gl_Position.x,gl_Position.y);
	position *= rotation;
	gl_Position.x = position.x;
	gl_Position.y = position.y;*/
	
	gl_ClipVertex = gl_ModelViewMatrix * gl_Position;
	
}