struct s_light
{
    vec4 position;
    vec4 ambient, diffuse, specular;
};

uniform s_light lighting;

varying vec3 normal, lightDir, eyeVec;

void main()
{
	normal = gl_NormalMatrix * mat3(gl_ProjectionMatrix) * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);

	lightDir = vec3((gl_ModelViewProjectionMatrix*lighting.position).xyz/* - vVertex*/);
	eyeVec = -vVertex;

	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}
