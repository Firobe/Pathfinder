#include "functions.h"

outPut::outPut(Matrix array)
{
    _data = array;
}

outPut::~outPut()
{
    glDeleteLists(_dispListMap, 1);
    ///Terminer AntTweakBar
    TwTerminate();
    ///Terminer GLFW
    glfwTerminate();
}


void outPut::init_outPut()
{
    ///Initialisation OpenGL
    glfwInit();

    ///Initialisation Rude::Config
    assert(_config.load("config.ini"));
    _config.setSection("window");

    _reg.COLORS = colorized;
    if(_config.getBoolValue("AUTO"))
    {
        _config.setSection("render");
        assert(_reg.PIXEL_SIZE = _config.getIntValue("PIXEL_SIZE"));

        _reg.WIDTH = _data.GetX()*_reg.PIXEL_SIZE;
        _reg.HEIGHT = _data.GetY()*_reg.PIXEL_SIZE;
    }
    else
    {
        _config.setSection("window");
        _reg.HEIGHT = _config.getDoubleValue("HEIGHT");
        _reg.WIDTH = _config.getDoubleValue("WIDTH");
        _config.setSection("render");
        assert(_reg.PIXEL_SIZE = _config.getIntValue("PIXEL_SIZE"));
    }
    _config.setSection("render");
    assert(_reg.MAX_FPS = _config.getIntValue("MAX_FPS"));
    assert(_reg.WIREFRAME = _config.getBoolValue("WIREFRAME"));

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    assert(glfwOpenWindow( _reg.WIDTH, _reg.HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW ));

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)_reg.WIDTH/_reg.HEIGHT,1,1000);

    glDisable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    /*glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);*/

    init_Tw();

    _reg.MULTIPLIER = 1;
}

void outPut::init_Tw()
{
    ///Initialisation AntTweakBar
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(_reg.WIDTH, _reg.HEIGHT);
    // redirection des callbacks GFLW sut AntTweakBar
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
    glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);
}

void outPut::display()
{
    init_outPut();

    _dispListMap = glGenLists(1);

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

    // Main loop
    while( running && glfwGetWindowParam( GLFW_OPENED ) )
    {
        static double loopTime;
        static float mat[4*4];
        glfwSetTime(0);

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective(zoom,_reg.WIDTH/_reg.HEIGHT,1,1000);


        gluLookAt(0, 0, distance, focus.x, focus.y, focus.z, 0, 1, 0);

       glTranslatef(focus.x, focus.y, focus.z);
        ConvertQuaternionToMatrix(orientation, mat);
        glMultMatrixf(mat);
        glTranslatef(-focus.x, -focus.y, -focus.z);

        glClear(GL_COLOR_BUFFER_BIT);

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

void TW_CALL outPut::genList()
{
    int pixelSize(_reg.PIXEL_SIZE);
    glNewList(_dispListMap, GL_COMPILE);
    glPointSize(pixelSize);

    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3ub(255, 0,0);
    glVertex3d(0,0,0);
    glVertex3d(50,0,0);

    glColor3ub(0, 255,0);
    glVertex3d(0,0,0);
    glVertex3d(0,50,0);


    glColor3ub(0,0,255);
    glVertex3d(0,0,0);
    glVertex3d(0,0,50);
    glEnd();


    if(_reg.WIREFRAME)
    {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1);
    }
    else
    {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    unsigned int x, y;

    if(_reg.COLORS == real)
    {

        for(x = 0; x < _data.GetX()-1; x++)
        {
            glBegin(GL_TRIANGLE_STRIP);
            glColor3ub(_data.GetValue(x,0,0), _data.GetValue(x,0,0), _data.GetValue(x,0,0));
            glVertex3d(x*pixelSize+pixelSize/2.0, 0,_data.GetValue(x,0,0)/25.5*_reg.MULTIPLIER);

            for(y = 0; y < _data.GetY()-2; y++)
            {
                glColor3ub(_data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0));
                glVertex3d((x+1)*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0,_data.GetValue(x+1,y,0)/25.5*_reg.MULTIPLIER);

                glColor3ub(_data.GetValue(x,y+1,0), _data.GetValue(x,y+1,0), _data.GetValue(x,y+1,0));
                glVertex3d(x*pixelSize+pixelSize/2.0, (y+1)*pixelSize+pixelSize/2.0,_data.GetValue(x,y+1,0)/25.5*_reg.MULTIPLIER);
            }

            glColor3ub(_data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0));
            glVertex3d((x+1)*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0,_data.GetValue(x+1,y,0)/25.5*_reg.MULTIPLIER);
        glEnd();
        }
    }
    else if (_reg.COLORS == colorized)
    {
        for(x = 0; x < _data.GetX()-1; x++)
        {
        glBegin(GL_TRIANGLE_STRIP);
            glColor3ub(_data.GetValue(x,0,0), 255-_data.GetValue(x,0,0),0);
            glVertex3d(x*pixelSize+pixelSize/2.0, 0,_data.GetValue(x,0,0)/25.5*_reg.MULTIPLIER);

            for(y = 0; y < _data.GetY()-2; y++)
            {
                glColor3ub(_data.GetValue(x+1,y,0), 255-_data.GetValue(x+1,y,0),0);
                glVertex3d((x+1)*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0,_data.GetValue(x+1,y,0)/25.5*_reg.MULTIPLIER);

                glColor3ub(_data.GetValue(x,y+1,0), 255-_data.GetValue(x,y+1,0),0);
                glVertex3d(x*pixelSize+pixelSize/2.0, (y+1)*pixelSize+pixelSize/2.0,_data.GetValue(x,y+1,0)/25.5*_reg.MULTIPLIER);
            }

            glColor3ub(_data.GetValue(x+1,y,0), 255-_data.GetValue(x+1,y,0), 0);
            glVertex3d((x+1)*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0,_data.GetValue(x+1,y,0)/25.5*_reg.MULTIPLIER);
        glEnd();
        }
    }
    else
    {
        glColor3ub(255, 255,255);
        for(x = 0; x < _data.GetX()-1; x++)
        {
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3d(x*pixelSize+pixelSize/2.0, 0,_data.GetValue(x,0,0)/25.5*_reg.MULTIPLIER);

            for(y = 0; y < _data.GetY()-2; y++)
            {
                glVertex3d((x+1)*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0,_data.GetValue(x+1,y,0)/25.5*_reg.MULTIPLIER);
                glVertex3d(x*pixelSize+pixelSize/2.0, (y+1)*pixelSize+pixelSize/2.0,_data.GetValue(x,y+1,0)/25.5*_reg.MULTIPLIER);
            }
            glVertex3d((x+1)*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0,_data.GetValue(x+1,y,0)/25.5*_reg.MULTIPLIER);
        glEnd();
        }
    }
    glEndList();
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
