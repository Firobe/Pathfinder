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

varying vec3 normal, lightDir, eyeVec;

void main (void)
{
    vec4 sceneColor= vec4(0.1,0.1,0.1,1.0);
	vec4 final_color =
	(sceneColor * material.ambient) +
	(lighting.ambient * material.ambient);

	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);

	float lambertTerm = dot(N,L);

	if(lambertTerm > 0.0)
	{
		final_color += lighting.diffuse *
		               material.diffuse *
					   lambertTerm;

		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0),
		                 material.shininess );
		final_color += lighting.specular *
		               material.specular *
					   specular;
	}

	gl_FragColor = final_color;
}
