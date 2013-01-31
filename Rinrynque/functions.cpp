#include "functions.h"

outPut::outPut()
{

}

outPut::~outPut()
{

}

void outPut::openHeightMap(char carte[])
{
    FILE* f_heightMap = NULL;

    f_heightMap = fopen(carte,"r");

    assert(f_heightMap != NULL);

    int x,y;
    fscanf(f_heightMap, "%d\n", &x);
    fscanf(f_heightMap, "%d\n", &y);

    for(int i = 0; i < x; i++)
    {
        _heightMap.push_back(std::vector<int>(y));
        /*for(int j = 0; j < y; j++)
        {
            _heightMap[i].push_back(0);
            //std::cout << _heightMap[i][j] << ";";
        }*/
        // std::cout << std::endl;
    }

    // std::cout << "----------------" << std::endl;

    for(int i = y-1; i >=0; i--)
    {
        for(int j = 0; j < x; j++)
        {
            fscanf(f_heightMap, "%d*", &(_heightMap[j][i]));
            //         std::cout << _heightMap[i][j] << ";";
        }
        //     std::cout << std::endl;
    }
   // return _heightMap;
}

void outPut::display()
{
    ///Initialisation Rude::Config
    rude::Config config;
    assert(config.load("config.ini"));
    config.setSection("window");

    ///Initialisation OpenGL
    glfwInit();

    if(config.getBoolValue("AUTO"))
    {
        config.setSection("render");
        assert(o_registry["PIXEL_SIZE"] = config.getIntValue("PIXEL_SIZE"));

        o_registry["WIDTH"] = _heightMap.size()*o_registry["PIXEL_SIZE"];
        o_registry["HEIGHT"] = _heightMap[0].size()*o_registry["PIXEL_SIZE"];
    }
    else
    {
        config.setSection("window");
        o_registry["HEIGHT"] = config.getDoubleValue("HEIGHT");
        o_registry["WIDTH"] = config.getDoubleValue("WIDTH");
        o_registry["PIXEL_SIZE"] = o_registry["HEIGHT"]/_heightMap.size();
    }
    config.setSection("render");
    assert(o_registry["MAX_FPS"] = (int)config.getDoubleValue("MAX_FPS"));

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    assert(glfwOpenWindow( o_registry["WIDTH"], o_registry["HEIGHT"], 0,0,0,0,0,0, GLFW_WINDOW ));

    gluOrtho2D(0, o_registry["WIDTH"], o_registry["HEIGHT"], 0);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);   // Autoriser la transparence
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(o_registry["PIXEL_SIZE"]);

    //glPointSize(2);

    ///Initialisation AntTweakBar
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(o_registry["WIDTH"], o_registry["HEIGHT"]);
    // redirection des callbacks GFLW sut AntTweakBar
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
    glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);
    TwBar *reglages;
    reglages = TwNewBar("Reglages");
    bool running = true;
    char title[50] = "PathFinder output";
    TwAddVarRW(reglages, "Title", TW_TYPE_CSSTRING(sizeof(title)), title, "group='Window settings'  help='Changer le titre de la fenetre'");

    TwAddVarRW(reglages, "Running", TW_TYPE_BOOLCPP, &running, "group='Window settings' help='Fermer le programme' key=ESC");
    TwAddVarRO(reglages, "MAX_FPS", TW_TYPE_INT32, &(o_registry["MAX_FPS"]), "group='Window settings'");

    int HmX = _heightMap.size(), HmY = _heightMap[0].size();
    TwAddVarRO(reglages, "HmX", TW_TYPE_INT32, &HmX, "group='OutPut'");
    TwAddVarRO(reglages, "HmY", TW_TYPE_INT32, &HmY, "group='OutPut'");


    // Main loop
    while( running && glfwGetWindowParam( GLFW_OPENED ) )
    {
        static double loopTime;
        static int pixelSize = o_registry["PIXEL_SIZE"];
        glfwSetTime(0);

// OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
        for(unsigned int x = 0; x < _heightMap.size(); x++)
        {
            for(int y = 0; y < _heightMap[0].size(); y++)
            {
                // std::cout << i << std::endl << j << std::endl;
                glColor3d(_heightMap[x][y], 255-_heightMap[x][y],0);
                glVertex2d(x*pixelSize+pixelSize/2.0, y*pixelSize+pixelSize/2.0);
            }
        }
        glEnd();
        TwDraw();
// Swap front and back rendering buffers
        glfwSwapBuffers();

        loopTime = glfwGetTime();
        if(1/loopTime > o_registry["MAX_FPS"] )
            glfwSleep(loopTime-1/o_registry["MAX_FPS"] );

        glfwSetWindowTitle( title );
    }

    ///Terminer AntTweakBar
    TwTerminate();
    ///Terminer GLFW
    glfwTerminate();
}
