#include "functions.h"

void outPut::display()
{
    init_outPut();

    TwBar *camera;
    camera = TwNewBar("Camera");
    float orientation[4] = {0, 0, 0, 1}; // direction pointing to +x and +y
    double distance = _data.GetX()+_data.GetY(), zoom = 60;
    coords3d focus(_data.GetX()/2,_data.GetY()/2,0);
    TwAddVarRW(camera, "Orientation", TW_TYPE_QUAT4F, &orientation,"axisx=x axisy=y axisz=z");
    TwAddVarRW(camera, "Distance", TW_TYPE_DOUBLE, &distance,"");
    TwAddVarRW(camera, "Zoom", TW_TYPE_DOUBLE, &zoom,"min = 5 max = 200");
    TwAddVarRW(camera, "x", TW_TYPE_DOUBLE, &(focus.x), "group = 'Focus'");
    TwAddVarRW(camera, "y", TW_TYPE_DOUBLE, &(focus.y), "group = 'Focus'");
    TwAddVarRW(camera, "z", TW_TYPE_DOUBLE, &(focus.z), "group = 'Focus'");

    TwBar *reglages;
    reglages = TwNewBar("Reglages");
    bool running = true;
    char title[50] = "PathFinder output";
    TwAddVarRW(reglages, "Title", TW_TYPE_CSSTRING(sizeof(title)), title, "group='Window settings'  help='Changer le titre de la fenetre'");

    TwAddVarRW(reglages, "Running", TW_TYPE_BOOLCPP, &running, "group='Window settings' help='Fermer le programme' key=ESC");
    TwAddVarRO(reglages, "MAX_FPS", TW_TYPE_INT32, &(_reg.MAX_FPS), "group='Window settings'");

    bool recalculate(true);
    TwAddVarRW(reglages, "Recalculer", TW_TYPE_BOOLCPP, &recalculate, "group = 'Rendu'");
    TwAddVarRW(reglages, "Multiplicateur", TW_TYPE_DOUBLE, &(_reg.MULTIPLIER), "group = 'Rendu' min = 0 max = 10");
    TwType colorsType;
    colorsType = TwDefineEnum("ColorsType", NULL, 0);
    TwAddVarRW(reglages, "Couleurs", colorsType, &(_reg.COLORS), " enum='0 {Réelles}, 1 {Colorisé}, 2 {Uniforme}'");
    TwAddVarRW(reglages, "Rendu filaire", TW_TYPE_BOOLCPP, &(_reg.WIREFRAME), "group = 'Rendu'");
    TwAddVarRW(reglages, "Normales", TW_TYPE_BOOLCPP, &(_reg.DRAW_NORMALS), "group = 'Rendu'");
    TwAddVarRW(reglages, "Couleur unie", TW_TYPE_COLOR3F, &(_reg.UNIFORM_COLOR), "");

    float lightPos[4] = {-30,-30,30,0};
    TwAddVarRW(reglages, "x", TW_TYPE_FLOAT, &(lightPos[0]), "group = Lumiere" );
    TwAddVarRW(reglages, "y", TW_TYPE_FLOAT, &(lightPos[1]), "group = Lumiere" );
    TwAddVarRW(reglages, "z", TW_TYPE_FLOAT, &(lightPos[2]), "group = Lumiere" );
    TwAddVarRW(reglages, "Directionnelle", TW_TYPE_FLOAT, &(lightPos[3]), "group = Lumiere min=0 max =1" );


    // Main loop
    while( running && glfwGetWindowParam( GLFW_OPENED ) )
    {
        static double loopTime;
        static float mat[4*4];
        glfwSetTime(0);


        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective(zoom,_reg.WIDTH/_reg.HEIGHT,1,1000);


        gluLookAt(0, 0, distance, focus.x, focus.y, focus.z, 0, 1, 0);

        glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
        glPointSize(5);
        glColor3ub(255,255,0);
        glBegin(GL_POINT);
        glVertex3d(lightPos[0],lightPos[1], lightPos[2]);
        glEnd();

       glTranslatef(focus.x, focus.y, focus.z);
        ConvertQuaternionToMatrix(orientation, mat);
        glMultMatrixf(mat);
        glTranslatef(-focus.x, -focus.y, -focus.z);

        if(recalculate)
        {
            genList();
            recalculate = false;
        }

        glCallList(_dispListMap);

// OpenGL rendering goes here...
        TwDraw();
// Swap front and back rendering buffers
        glfwSwapBuffers();

        loopTime = glfwGetTime();
        if(1/loopTime >  _reg.MAX_FPS)
            glfwSleep(loopTime-1/_reg.MAX_FPS );

        glfwSetWindowTitle( title );
    }
}

void ConvertQuaternionToMatrix(const float *quat, float *mat)
{
    float yy2 = 2.0f * quat[1] * quat[1];
    float xy2 = 2.0f * quat[0] * quat[1];
    float xz2 = 2.0f * quat[0] * quat[2];
    float yz2 = 2.0f * quat[1] * quat[2];
    float zz2 = 2.0f * quat[2] * quat[2];
    float wz2 = 2.0f * quat[3] * quat[2];
    float wy2 = 2.0f * quat[3] * quat[1];
    float wx2 = 2.0f * quat[3] * quat[0];
    float xx2 = 2.0f * quat[0] * quat[0];
    mat[0*4+0] = - yy2 - zz2 + 1.0f;
    mat[0*4+1] = xy2 + wz2;
    mat[0*4+2] = xz2 - wy2;
    mat[0*4+3] = 0;
    mat[1*4+0] = xy2 - wz2;
    mat[1*4+1] = - xx2 - zz2 + 1.0f;
    mat[1*4+2] = yz2 + wx2;
    mat[1*4+3] = 0;
    mat[2*4+0] = xz2 + wy2;
    mat[2*4+1] = yz2 - wx2;
    mat[2*4+2] = - xx2 - yy2 + 1.0f;
    mat[2*4+3] = 0;
    mat[3*4+0] = mat[3*4+1] = mat[3*4+2] = 0;
    mat[3*4+3] = 1;
}
