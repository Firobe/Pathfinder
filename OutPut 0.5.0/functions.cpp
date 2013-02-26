#include "functions.h"

bool outPut::getStatus()
{
    return _running && glfwGetWindowParam( GLFW_OPENED );
}

    void outPut::setScene()
    {
        static bool autoZfocus(true);
        static bool init = true;
        if (init)
        {
            glfwSetTime(0);
            TwAddVarRW(b_scene, "Auto Z focus", TW_TYPE_BOOLCPP, &autoZfocus, "");
            init = false;
        }
        static float mat[4*4];
        static coords3d focus3d;

        focus3d = _scene3d.focus*(coords3d::retournercoords3d(_reg.PIXEL_SIZE, _reg.PIXEL_SIZE, 1));
        if(autoZfocus)
        {focus3d.z = getVertex((int)_scene3d.focus.x, (int)_scene3d.focus.y).z;}

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective(_scene3d.zoom,_reg.WIDTH/_reg.HEIGHT,1,1000);


        gluLookAt(0, 0, _scene3d.distance, focus3d.x, focus3d.y, focus3d.z, 0, 1, 0);

        if(!_scene3d.lightPos[3])
        {_scene3d.lightPos[0] = -_scene3d.lightDir[0];  _scene3d.lightPos[1] = -_scene3d.lightDir[1];  _scene3d.lightPos[2] = -_scene3d.lightDir[2];}
        else
        {_scene3d.lightPos[0] = _scene3d.lightDir[0];  _scene3d.lightPos[1] = _scene3d.lightDir[1];  _scene3d.lightPos[2] = _scene3d.lightDir[2];}
        glLightfv(GL_LIGHT0,GL_POSITION,_scene3d.lightPos);


        glTranslatef(focus3d.x, focus3d.y, focus3d.z);
        ConvertQuaternionToMatrix(_scene3d.orientation, mat);
        glMultMatrixf(mat);
        glTranslatef(-focus3d.x, -focus3d.y, -focus3d.z);

    }
    void outPut::drawScene()
    {
        drawLight();

        glCallList(_dispListMap);
        TwDraw();
    }

void outPut::display()
{
        static double loopTime;
        static double beginTime = 0;
        static int fps = 0;

        glfwSwapBuffers();

        loopTime = glfwGetTime() - beginTime;
        fps = 1/loopTime;
        if(fps >  _reg.MAX_FPS)
            glfwSleep(loopTime-1/_reg.MAX_FPS );
        static float mat[4*4];
        static coords3d focus3d;

        beginTime = glfwGetTime();
}

void outPut::drawResult(const vector<Node>* list)
{
    static bool init(true);
    static GLuint dl_path = glGenLists(1);
    #define decalZ 1
    if(init)
    {
        glNewList(dl_path, GL_COMPILE);
        //glDisable(GL_LIGHTING);
        glColor3ub(10,213,55);

        glLineWidth(4);
        glBegin(GL_LINE_STRIP);

        coords3d vertex(0,0,0);

        for(int i = 0; i < list->size(); i++)
        {
            vertex = getVertex((*list)[i].x,(*list)[i].y);
            glVertex3d(vertex.x, vertex.y, vertex.z+decalZ);
        }
        glEnd();
        glColor3ub(255,255,255);
        //glEnable(GL_LIGHTING);
        glEndList();
        init = false;
    }
    glCallList(dl_path);
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
