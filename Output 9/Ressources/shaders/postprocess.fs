#define blur 1
#define wave 2
#define baw 3
#define sepia 4
#define vortex 5
#define noise 6
#define bf3 7
#define nightvision 8
/** Les identifiants de chaque effet **/

#define PI 3.14159

uniform sampler2D fbo_texture;
uniform float u_offset;

uniform int effect;

varying vec2 f_texcoord;

const float sampleDist = 0.8;
float sampleStrength = 1.0;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))+5.0) * 43758.5453);
}

void main(void)
{
    bool postprocessed = false;

    float offset = u_offset;
#define offset_speed 1.0
    offset *= offset_speed;

    if(effect == blur)
    {
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
        float dist = sqrt(pow(dir.x,2) + pow(dir.y,2));
        dir = dir/dist;

        vec4 color = texture2D(fbo_texture,f_texcoord);
        vec4 sum = color;

        for (int i = 0; i < 10; i++)
            sum += texture2D( fbo_texture, f_texcoord + dir * samples[i] * sampleDist );

        sum *= 1.0/11.0;
        float t = dist * sampleStrength;
        t = clamp( t ,0.0,1.0);

        gl_FragColor = mix( color, sum, t );
        postprocessed = true;
    }
    if(effect == wave)
    {
        vec2 texcoord = f_texcoord;
        texcoord.x += sin(texcoord.y * 4*2*PI + offset) / 100.0;
        gl_FragColor = texture2D(fbo_texture, texcoord);
        postprocessed = true;
    }
    if(effect == baw || effect == sepia)
    {
        float depth = 0;
        float multiplier = 1;

        if(effect == sepia)
        {
            depth = 0.1;
            multiplier = 0.8;
        }
        vec4 color = texture2D(fbo_texture, f_texcoord)*multiplier;
        float average = (color.r + color.g + color.b)/3.0;
        color.r = average+2*depth;
        color.g = average+depth;
        color.b = average;
        gl_FragColor = color;
        postprocessed = true;
    }
    if(effect == vortex)
    {
        vec2 dir = 0.5 - f_texcoord;
        float dist = sqrt(pow(dir.x,2) + pow(dir.y*0.6,2))-0.4;
        if(dist < 0)
            dist =0;

        vec2 texcoord = f_texcoord;//-vec2(0.5,0.5);
        //1000*600 TEST
        float theta = 2*PI*dist*0.3;
        vec4 rotation = vec4(cos(theta), -sin(theta), sin(theta), cos(theta));
        texcoord.x = cos(theta)*(texcoord.x-0.5)-sin(theta)*(texcoord.y-0.5)+0.5;
        texcoord.y = sin(theta)*(texcoord.x-0.5)+cos(theta)*(texcoord.y-0.5)+0.5;
        //texcoord += vec2(0.5,0.5);
        gl_FragColor = texture2D(fbo_texture, texcoord);
        //gl_FragColor.r += dist;
        postprocessed = true;
    }
    if(effect == noise)
    {
        gl_FragColor = rand(f_texcoord*offset)*texture2D(fbo_texture, f_texcoord);
        postprocessed = true;
    }
    if(effect == bf3)
    {
#define strenght 0.06 //en % de l'ecran

        vec2 texcoord;
        if(sin(offset)>0.8&&abs(rand(f_texcoord*vec2(0,1)))<0.5)
        {
            texcoord = f_texcoord+(vec2(strenght,0)*rand(vec2(0,1)*f_texcoord*offset))*(sin(offset)*(1-2*rand(f_texcoord*vec2(0,1))));
        }
        else
        {
            texcoord = f_texcoord;
        }
        gl_FragColor = texture2D(fbo_texture, texcoord);
        if(sin(offset) >-0.5&& sin(offset)<0.8 && cos(offset)>0)
        {
            vec2 dir = vec2(0.01*sin(offset),0.01*0.6*cos(offset));
            gl_FragColor/=1.5;
            gl_FragColor.r += texture2D(fbo_texture, texcoord+dir).r*1.5;
            gl_FragColor.g += texture2D(fbo_texture, texcoord+dir*-2).g*1.5;
            gl_FragColor.b += texture2D(fbo_texture, texcoord+dir*3).b*1.5;
            gl_FragColor/=2.0;
        }
        postprocessed = true;
    }
    if(!postprocessed)
    {
        gl_FragColor = texture2D(fbo_texture, f_texcoord);
        postprocessed = true;
    }

}
