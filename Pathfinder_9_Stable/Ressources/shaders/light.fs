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
	vec4 final_color =
	(gl_FrontLightModelProduct.sceneColor * material.ambient) +
	(lighting.ambient * material.ambient);

	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);

	float lambertTerm = dot(N,L);

	if(lambertTerm > 0.0)
	{
		final_color += lighting.diffuse *
		               gl_Color *
					   lambertTerm;

		vec3 V = normalize(eyeVec);

		vec3 R = reflect(-L, N);
		/*float specular = pow( max(dot(R, E), 0.0),
		                 material.shininess );*/
        float specular = pow(max(dot(R,V),0.0), material.shininess);
		                 //specular = 0.0;
		final_color += lighting.specular *
		               material.specular *
					   specular;
	}

	gl_FragColor = final_color;
}

