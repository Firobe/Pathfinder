//#define blur
//#define wave
#define baw
#define sepia
//#define noeffect
/** !! Ne definir qu'un seul effet **/

uniform sampler2D fbo_texture;
uniform float offset;
varying vec2 f_texcoord;
 
const float sampleDist = 0.8;
float sampleStrength = 1.0; 

void main(void)
{
#ifdef blur
	sampleStrength*= sin(offset)+1;
    float samples[10];
    samples[0] = -0.08;
    samples[1] = -0.05;
    samples[2] = -0.03;
    samples[3] = -0.02;
    samples[4] = -0.01;
    samples[5] =  0.01;
    samples[6] =  0.02;
    samples[7] =  0.03;
    samples[8] =  0.05;
    samples[9] =  0.08;

    vec2 dir = 0.5 - f_texcoord; 
    float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
    dir = dir/dist; 

    vec4 color = texture2D(fbo_texture,f_texcoord); 
    vec4 sum = color;

    for (int i = 0; i < 10; i++)
        sum += texture2D( fbo_texture, f_texcoord + dir * samples[i] * sampleDist );

    sum *= 1.0/11.0;
    float t = dist * sampleStrength;
    t = clamp( t ,0.0,1.0);

    gl_FragColor = mix( color, sum, t );
#endif
#ifdef wave
  vec2 texcoord = f_texcoord;
  texcoord.x += sin(texcoord.y * 4*2*3.14159 + offset) / 100;
  gl_FragColor = texture2D(fbo_texture, texcoord);
#endif
#ifdef noeffect
  gl_FragColor = texture2D(fbo_texture, f_texcoord);
#endif
#ifdef baw
#ifndef sepia
#define depth 0
#define multiplier 1
#endif
#ifdef sepia
#define depth 0.1
#define multiplier 0.8
#endif
vec4 color = texture2D(fbo_texture, f_texcoord)*multiplier;
float average = (color.r + color.g + color.b)/3.0;
color.r = average+2*depth;
color.g = average+depth;
color.b = average;
 gl_FragColor = color;
#endif
}
