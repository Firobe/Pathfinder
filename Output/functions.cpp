#include "functions.h"

outPut::outPut(Matrix array)
{
    _data = array;
}

outPut::~outPut()
{
    glDeleteLists(_dispListMap, 1);
}

void outPut::display()
{
    ///Initialisation OpenGL
    glfwInit();

    ///Initialisation Rude::Config
    assert(_config.load("config.ini"));
    _config.setSection("window");

    if(_config.getBoolValue("AUTO"))
    {
        _config.setSection("render");
        assert(o_registry["PIXEL_SIZE"] = _config.getIntValue("PIXEL_SIZE"));

        o_registry["WIDTH"] = _data.GetX()*o_registry["PIXEL_SIZE"];
        o_registry["HEIGHT"] = _data.GetY()*o_registry["PIXEL_SIZE"];
    }
    else
    {
        _config.setSection("window");
        o_registry["HEIGHT"] = _config.getDoubleValue("HEIGHT");
        o_registry["WIDTH"] = _config.getDoubleValue("WIDTH");
        o_registry["PIXEL_SIZE"] = o_registry["HEIGHT"]/_data.GetY();
    }
    _config.setSection("render");
    assert(o_registry["MAX_FPS"] = (int)_config.getDoubleValue("MAX_FPS"));

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    assert(glfwOpenWindow( o_registry["WIDTH"], o_registry["HEIGHT"], 0,0,0,0,0,0, GLFW_WINDOW ));

    gluOrtho2D(0, o_registry["WIDTH"], 0, o_registry["HEIGHT"]);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);   // Autoriser la transparence
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);

    ///Initialisation AntTweakBar
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(o_registry["WIDTH"], o_registry["HEIGHT"]);
    // redirection des callbacks GFLW sut AntTweakBar
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
    glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);

    _dispListMap = glGenLists(1);
    genList();

    TwBar *reglages;
    reglages = TwNewBar("Reglages");
    bool running = true;
    char title[50] = "PathFinder output";
    TwAddVarRW(reglages, "Title", TW_TYPE_CSSTRING(sizeof(title)), title, "group='Window settings'  help='Changer le titre de la fenetre'");

    TwAddVarRW(reglages, "Running", TW_TYPE_BOOLCPP, &running, "group='Window settings' help='Fermer le programme' key=ESC");
    TwAddVarRO(reglages, "MAX_FPS", TW_TYPE_INT32, &(o_registry["MAX_FPS"]), "group='Window settings'");

    int HmX = _data.GetX(), HmY = _data.GetY();
    TwAddVarRO(reglages, "HmX", TW_TYPE_INT32, &HmX, "group='OutPut'");
    TwAddVarRO(reglages, "HmY", TW_TYPE_INT32, &HmY, "group='OutPut'");

    // Main loop
    while( running && glfwGetWindowParam( GLFW_OPENED ) )
    {
        static double loopTime;
        static int pixelSize = o_registry["PIXEL_SIZE"];
        static int max_FPS = o_registry["MAX_FPS"];
        glfwSetTime(0);

// OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);
        glCallList(_dispListMap);

        glColor3d(0, 0,0);
        glPointSize(pixelSize/3);
        glBegin(GL_POINTS);
        for(unsigned int x = 0; x < _data.GetX(); x++)
        {
            for(unsigned int y = 0; y < _data.GetY(); y++)
            {
                for(unsigned int i = 1; i < 9; i++)
                {
                    if(_data.GetValue(x,y,i,3))
                        glVertex2d(x*pixelSize+pixelSize/2.0+(decalXY[i][0])*pixelSize/3.0, y*pixelSize+pixelSize/2.0+(decalXY[i][1])*pixelSize/3.0);
                }
            }
        }
        glEnd();

        TwDraw();
// Swap front and back rendering buffers
        glfwSwapBuffers();

        loopTime = glfwGetTime();
        if(1/loopTime >  max_FPS)
            glfwSleep(loopTime-1/max_FPS );

        glfwSetWindowTitle( title );
    }
    ///Terminer AntTweakBar
    TwTerminate();
    ///Terminer GLFW
    glfwTerminate();
}

void outPut::genList()
{
    int pixelSize = o_registry["PIXEL_SIZE"];
    glNewList(_dispListMap, GL_COMPILE);
    glPointSize(pixelSize);
    glBegin(GL_POINTS);

    FILE* out = NULL;
    out = fopen("out.txt","w+");
    assert(out != NULL);

    for(unsigned int x = 0; x < _data.GetX(); x++)
    {
        for(int y = 0; y < _data.GetY(); y++)
        {
            // std::cout << i << std::endl << j << std::endl;
            glColor3ub(_data.GetValue(x,y,0,0), 255-_data.GetValue(x,y,0,0),0);
            glVertex2d(x*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0);
            fprintf(out, "%d|", _data.GetValue(x,y,0,0));
        }
        fprintf(out, "\n");
    }
    glEnd();

    glBegin(GL_POINTS);
    glPointSize(pixelSize/3.0);
    glColor3ub(0,0,255);
    for(unsigned int x = 0; x < _data.GetX(); x++)
    {
        for(int y = 0; y < _data.GetY(); y++)
        {
            glVertex2d(x*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0);
        }
    }
    glEnd();
    glEndList();
    fclose(out);
}
