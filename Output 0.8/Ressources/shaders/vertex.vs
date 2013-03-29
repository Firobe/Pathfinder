uniform vec2 resolution;

void main(void)
{
gl_FrontColor = gl_Color;
gl_Position = ftransform();
gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
}