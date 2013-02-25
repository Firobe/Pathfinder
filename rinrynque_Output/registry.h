#ifndef REGISTRY_H_INCLUDED
#define REGISTRY_H_INCLUDED
enum colors_types{real, colorized, uniform};
struct registry
{
    double MULTIPLIER;
    int PIXEL_SIZE;
    int WIDTH;
    int HEIGHT;
    int MAX_FPS;
    colors_types COLORS;
    bool WIREFRAME;
    bool DRAW_NORMALS;

};

#endif // REGISTRY_H_INCLUDED
