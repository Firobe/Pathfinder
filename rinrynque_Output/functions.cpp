#include "functions.h"

bool outPut::getStatus()
{
    return _running && glfwGetWindowParam( GLFW_OPENED );
}

void outPut::display()
{
    init_outPut();

    genList();
    int fps = 0;
    TwAddVarRO(b_scene, "FPS", TW_TYPE_INT8, &fps, " label='Nombre de FPS' ");
    glfwSetTime(0);
    // Main loop
    while( getStatus()  )
    {
        //_close = false;
        static double loopTime;
        static double beginTime = 0;
        beginTime = glfwGetTime();
        static float mat[4*4];


        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective(_scene3d.zoom,_reg.WIDTH/_reg.HEIGHT,1,1000);


        gluLookAt(0, 0, _scene3d.distance, _scene3d.focus.x, _scene3d.focus.y, _scene3d.focus.z, 0, 1, 0);

        if(!_scene3d.lightPos[3])
        {_scene3d.lightPos[0] = -_scene3d.lightDir[0];  _scene3d.lightPos[1] = -_scene3d.lightDir[1];  _scene3d.lightPos[2] = -_scene3d.lightDir[2];}
        else
        {_scene3d.lightPos[0] = _scene3d.lightDir[0];  _scene3d.lightPos[1] = _scene3d.lightDir[1];  _scene3d.lightPos[2] = _scene3d.lightDir[2];}
        glLightfv(GL_LIGHT0,GL_POSITION,_scene3d.lightPos);


        glTranslatef(_scene3d.focus.x, _scene3d.focus.y, _scene3d.focus.z);
        ConvertQuaternionToMatrix(_scene3d.orientation, mat);
        glMultMatrixf(mat);
        glTranslatef(-_scene3d.focus.x, -_scene3d.focus.y, -_scene3d.focus.z);

        drawLight();

        glCallList(_dispListMap);

// OpenGL rendering goes here...
        TwDraw();
// Swap front and back rendering buffers
        glfwSwapBuffers();

        loopTime = glfwGetTime() - beginTime;
        fps = 1/loopTime;
        if(fps >  _reg.MAX_FPS)
            glfwSleep(loopTime-1/_reg.MAX_FPS );
    }
}

void outPut::drawLight()
{
    #define DL_SCALE 10
        glDisable(GL_LIGHTING);
        glPointSize(5);
        glColor3ub(255,255,0);
        if(_scene3d.lightPos[3])
        {
            glBegin(GL_POINTS);
            glVertex3d(_scene3d.lightPos[0],_scene3d.lightPos[1],_scene3d.lightPos[2]);
            glEnd();
        }
        else
        {
            glBegin(GL_LINES);
            glVertex3d(_scene3d.lightPos[0]*DL_SCALE,_scene3d.lightPos[1]*DL_SCALE,_scene3d.lightPos[2]*DL_SCALE);
            glVertex3d(0,0,0);
            glEnd();
        }

        glEnable(GL_LIGHTING);
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
