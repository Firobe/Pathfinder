struct s_light
{
    vec4 position;
    vec4 ambient, diffuse, specular;
};

varying vec3 normal, lightDir, eyeVec;
uniform s_light lighting;


void main()
{
vec4 lightPos = gl_ModelViewMatrix*lighting.position;
	normal = gl_NormalMatrix * gl_Normal;

	vec4 vVertex = vec4(gl_ModelViewMatrix * gl_Vertex);

    if (lighting.position[3]== 0)
    {
        lightDir = vec4(lightPos);
    }
    else
    {
        lightDir = vec4(lighting.position - vVertex);
    }

	eyeVec = -vVertex;

	gl_Position = ftransform();
}
