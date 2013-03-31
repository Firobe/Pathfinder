varying vec3 N;
varying vec3 v; 

#define DIRECTIONNAL 
  
void main (void)  
{  
vec3 L;
vec3 E;
	if(gl_LightSource[0].position[3] == 0)
   {L = normalize(gl_LightSource[0].position.xyz);
   E = vec3(1,1,1);}
   else
   {L = normalize(gl_LightSource[0].position.xyz - v);
   }   
   E = normalize(-v);// we are in Eye Coordinates, so EyePos is (0,0,0)  
  
   vec3 R = normalize(-reflect(L,N));  
 
   //calculate Ambient Term:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient;    

   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
	//vec4 Ispec = 0.77391 * pow(max(dot(R,E),0.0),0.3*89.6);
   Ispec = clamp(Ispec, 0.0, 1.0); 
   // write Total Color:  
   gl_FragColor = gl_Color * (gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec);
	/*gl_FragColor.r = gl_FragCoord.w*150.0;
	gl_FragColor.g = 1-gl_FragCoord.w*150.0;
	gl_FragColor.b = 1-gl_FragCoord.w*150.0;*/
}