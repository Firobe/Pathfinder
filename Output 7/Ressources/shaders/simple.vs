void main()
{
	gl_FrontColor = gl_Color;
    gl_Position = ftransform();
	
	gl_ClipVertex = gl_ModelViewMatrix * gl_Position;
	
}