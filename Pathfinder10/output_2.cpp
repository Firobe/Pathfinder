#include "output.h"
#include <string>
#define __WINDOW_TITLE "Pathfinder OutPut"

void outPut::reInit()
{
    ///Terminer AntTweakBar
    TwTerminate();
    ///Terminer GLFW
    glfwTerminate();
    init_outPut();
}

outPut::outPut(Matrix& array)
{
    _dimensions.x = array.GetX();
    _dimensions.y = array.GetY();

    _data = new int*[_dimensions.x];
    for(int i = 0; i < _dimensions.x; i++)
        _data[i] = new int[_dimensions.y];

    for(int i = 0; i < _dimensions.x; i++)
    {
        for (int j = 0 ; j < _dimensions.y; j++)
        {
            _data[i][j] = array.GetValue(i,j,0);
        }
    }


    _scene3d.normalMap = new coords3d<float>*[_dimensions.x];
    for(int i = 0; i < _dimensions.x; i++)
        _scene3d.normalMap[i] = new coords3d<float>[_dimensions.y];

    _scene3d.verticesMap = new coords3d<float>*[_dimensions.x];
    for(int i = 0; i < _dimensions.x; i++)
        _scene3d.verticesMap[i] = new coords3d<float>[_dimensions.y];

    _valueChanges.clear();
    //_valueChanges.allocate(_dimensions.x*_dimensions.y);
    _maxValue = 1;
}

outPut::~outPut()
{
    ///Terminer AntTweakBar
    TwTerminate();
    glDeleteProgram(_sLight.getProgramID());
    glDeleteProgram(_postProcess.getProgramID());
    glDeleteProgram(_sNolight.getProgramID());
    ///Terminer GLFW
    glfwTerminate();
}


void outPut::init_outPut()
{
    ///Initialisation OpenGL
    glfwInit();

    loadConfig();

    glfwOpenWindowHint(GLFW_OPENGL_CORE_PROFILE, GL_TRUE);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    assert(glfwOpenWindow( _reg.WIDTH, _reg.HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW ));
    std::string window_title = "";
    window_title = window_title + __WINDOW_TITLE +" compilé le "+__DATE__+ " à " + __TIME__;
    glfwSetWindowTitle(window_title.c_str() );

    glewInit();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)_reg.WIDTH/_reg.HEIGHT,1,1000);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glClearColor(119.0/255.0, 181.0/255.0, 254.0/255.0,1.0);

    _sNolight.loadProgram("Ressources/shaders/simple.vs","Ressources/shaders/simple.fs");
    _sNolight.linkProgram();
    _sLight.loadProgram("Ressources/shaders/light.vs","Ressources/shaders/light.fs");
    glBindAttribLocation(_sLight.getProgramID(), aID_position, "a_position");
    glBindAttribLocation(_sLight.getProgramID(), aID_normal, "a_normal");
    glBindAttribLocation(_sLight.getProgramID(), aID_cost, "a_cost");
    _sLight.linkProgram();
    uid_maxCost = glGetUniformLocation(_sLight.getProgramID(), "maxCost");
    uid_defaultColor = glGetUniformLocation(_sLight.getProgramID(), "defaultColor");

    {
        //float lightPos[4] = {200.0,200.0,50.0,1.0};
        float lightD[4] = {0.0f,0.0f,0.0f,0.0f};
        float lightA[4] = {1.0f,1.0f,1.0f,1.0f};
        float lightS[4] = {1.0f,1.0f,1.0f,1.0f};

        float materialD[4] = {0.23f, 0.23f, 0.3f, 1.0f };
        float materialA[4] = {0.27f, 0.27f, 0.27f, 1.0f };
        float materialS[4] = {0.7f, 0.7f, 0.7f, 1.0f };


    _sLight.initCommunication();
//    _sLight.setLightPos(lightPos);
    _sLight.setLightMaterial(lightD, lightA,lightS);
    _sLight.setMaterial(materialD, materialA,materialS,200.0);

    }

    _postProcess.loadProgram("Ressources/shaders/postprocess.vs","Ressources/shaders/postprocess.fs");
    //glBindAttribLocation(_postProcess.getProgramID(), aID_UV, "uv_fbo");
    _postProcess.linkProgram();
    uid_pp_colors = glGetUniformLocation(_postProcess.getProgramID(), "fbo_texture");
    uid_pp_offset = glGetUniformLocation(_postProcess.getProgramID(), "u_offset");
    uid_pp_effect = glGetUniformLocation(_postProcess.getProgramID(), "effect");

    frameBuffer.genFBO(_reg.WIDTH, _reg.HEIGHT);

    GLuint idUniformResolution = glGetUniformLocation(_sNolight.getProgramID(), "resolution");
    glUniform2f(idUniformResolution, (float)_reg.WIDTH, (float)_reg.HEIGHT);
    idUniformResolution = glGetUniformLocation(_sLight.getProgramID(), "resolution");
    glUniform2f(idUniformResolution, (float)_reg.WIDTH, (float)_reg.HEIGHT);

    glUseProgram(_sNolight.getProgramID());

    init_Tw();
    init_Bars();

    _reg.MULTIPLIER = 5;
    //_reg.UNIFORM_COLOR = {0.5,0.5,0.5};
    _reg.UNIFORM_COLOR[0]=_reg.UNIFORM_COLOR[1]=_reg.UNIFORM_COLOR[2]=0.5;

    _status.running = true;
    _status.maxDiff = 50;
    _valueChanges.reserve(_dimensions.x*_dimensions.y);

    _reg.WIREFRAME = false;
}

void outPut::loadConfig()
{
    ///Initialisation Rude::Config
    assert(_config.load("config.ini"));
    _config.setSection("window");

    _reg.COLORS = uniform;
    if(_config.getBoolValue("AUTO"))
    {
        _config.setSection("render");
        assert(_reg.PIXEL_SIZE = _config.getIntValue("PIXEL_SIZE"));

        _reg.WIDTH = _dimensions.x*_reg.PIXEL_SIZE;
        _reg.HEIGHT = _dimensions.y*_reg.PIXEL_SIZE;
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
    //_reg.WIREFRAME = _config.getBoolValue("WIREFRAME");
    _reg.DRAW_NORMALS = _config.getBoolValue("DRAW_NORMALS");
    _reg.ZTEST_RESULT = _config.getBoolValue("ZTEST_RESULT");
}

void outPut::init_Tw()
{
    ///Initialisation AntTweakBar
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(_reg.WIDTH, _reg.HEIGHT);
    // redirection des callbacks GFLW sur AntTweakBar
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
    glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);
}

void TW_CALL recalculate(void* outputObject)
{
    static_cast<outPut *>(outputObject)->drawTerrain(true);
    static_cast<outPut *>(outputObject)->drawResult();
}

void TW_CALL cbCenterView(void* outputObject)
{
    static_cast<outPut *>(outputObject)->centerView();
}

void outPut::centerView()
{
    _scene3d.focus.x = (int)(_dimensions.x/2.0);
    _scene3d.focus.y = (int)(_dimensions.y/2.0);
}

void outPut::init_Bars()
{
    b_scene = TwNewBar("Scene");
    _scene3d.distance = _dimensions.x+_dimensions.y;
    _scene3d.zoom = 60;
    _scene3d.focus = coords3d<float>::retournercoords3d(_dimensions.x/2,_dimensions.y/2,0);
    //_scene3d.orientation = {0, 0, 0, 1}; // direction pointing to +x and +y
    _scene3d.orientation[0]=_scene3d.orientation[1]=_scene3d.orientation[2]=0;
    _scene3d.orientation[3]=1;
    TwAddVarRW(b_scene, "Orientation", TW_TYPE_QUAT4F, &(_scene3d.orientation),"axisx=x axisy=y axisz=z");
    TwAddVarRW(b_scene, "Distance", TW_TYPE_DOUBLE, &(_scene3d.distance),"");
    TwAddVarRW(b_scene, "Zoom", TW_TYPE_DOUBLE, &(_scene3d.zoom),"min = 5 max = 200");
    TwAddVarRW(b_scene, "x", TW_TYPE_FLOAT, &(_scene3d.focus.x), "group = 'Focus' ");
    TwAddVarRW(b_scene, "y", TW_TYPE_FLOAT, &(_scene3d.focus.y), "group = 'Focus' ");
    TwAddVarRW(b_scene, "z", TW_TYPE_FLOAT, &(_scene3d.focus.z), "group = 'Focus'");
    TwAddButton(b_scene, "Centrer", cbCenterView, this, " group = 'Focus' ");

    TwAddVarRW(b_scene, "Test de profondeur résultat", TW_TYPE_BOOLCPP,
               &(_reg.ZTEST_RESULT), "label='Z test resultat'");

    //_scene3d.lightDir = {1,1,-1};
    _scene3d.lightDir[0]=_scene3d.lightDir[1]=1;
    _scene3d.lightDir[2]=-1;
    //_scene3d.lightPos = {-1,-1,1,0};
    _scene3d.lightPos[0]=_scene3d.lightPos[1]=-1;
    _scene3d.lightPos[2]=1;
    _scene3d.lightPos[3]=0;
    TwAddVarRW(b_scene, "Direction", TW_TYPE_DIR3F, &(_scene3d.lightDir), "group = Lumiere" );
    TwAddVarRW(b_scene, "Directionnelle", TW_TYPE_FLOAT, &(_scene3d.lightPos[3]), "group = Lumiere min=0 max =1" );

    b_reglages = TwNewBar("Reglages");
    TwAddVarRO(b_reglages, "MAX_FPS", TW_TYPE_INT32, &(_reg.MAX_FPS), "group='Window settings'");
    TwAddButton(b_reglages, "Recalculer", recalculate, this, " group = Rendu ");
    TwAddVarRW(b_reglages, "Multiplicateur", TW_TYPE_DOUBLE, &(_reg.MULTIPLIER), "group = 'Rendu' min = 0 max = 100");
    TwType colorsType;
    colorsType = TwDefineEnum("ColorsType", NULL, 0);
    TwAddVarRW(b_reglages, "Couleurs", colorsType, &(_reg.COLORS), " enum='0 {Réelles}, 1 {Colorisé}, 2 {Uniforme}'");
    TwAddVarRW(b_reglages, "Filaire", TW_TYPE_BOOLCPP, &(_reg.WIREFRAME), " group = Rendu ");
    TwAddVarRW(b_reglages, "Normales", TW_TYPE_BOOLCPP, &(_reg.DRAW_NORMALS), "group = 'Rendu'");
    TwAddVarRW(b_reglages, "Couleur unie", TW_TYPE_COLOR3F, &(_reg.UNIFORM_COLOR), "colormode=hls group = 'Rendu'");
    b_postprocess = TwNewBar("Post-processing");
    _effect = 0;
    TwAddVarRW(b_postprocess, "Effet", TW_TYPE_INT32, &(_effect),"max=20 keyincr = p keydecr = m");

    b_pathfind = TwNewBar("Pathfind");
    TwAddVarRW(b_pathfind, "En cours", TW_TYPE_BOOLCPP, &(_status.running), "help='Fermer le programme' key=ESC");
    TwAddVarRW(b_pathfind, "Pause", TW_TYPE_BOOLCPP, &(_status.pause), "key=t");

    TwSetTopBar(b_scene);
    TwDefine(" Reglages iconified=true ");
    TwDefine(" Pathfind iconified=true ");
    TwDefine(" Post-processing iconified=true ");
}

void outPut::gen_verticesMap()
{
    for(int x = 0; x < _dimensions.x; x++)
    {
        for(int y = 0; y < _dimensions.y; y++)
        {
            _scene3d.verticesMap[x][y].x = x*_reg.PIXEL_SIZE;
            _scene3d.verticesMap[x][y].y = y*_reg.PIXEL_SIZE;
            _scene3d.verticesMap[x][y].z = _data[x][y]/25.5*_reg.MULTIPLIER;
        }
    }
    gen_normalMap();
}

void outPut::gen_normalMap()
{
    coords3d<float> somme(0,0,0), contribution;
    coords3d<float> vert_normal(0,0,1);
    vert_normal.normalize();

    for(int x = 0; x < _dimensions.x; x++)
    {
        for(int y = 0; y < _dimensions.y; y++)
        {
            somme = coords3d<float>::retournercoords3d(0,0,0);

            /*        0,1
              -1,1  +--+--+
                    |\2|\*|
                    |1\|3\|
              -1,0  +--+--+ 1,0
                    |\4|\6|
                    |*\|5\|
                    +--+--+ 1,-1
                      0,-1
            */

            somme+= coords3d<float>::cross(getVertex<float>(x,y)-getVertex<float>(x-1,y), getVertex<float>(x-1,y+1)-getVertex<float>(x-1,y)); //1
            somme+= coords3d<float>::cross(getVertex<float>(x-1,y+1)-getVertex<float>(x,y+1), getVertex<float>(x,y)-getVertex<float>(x,y+1)); //2
            somme+= coords3d<float>::cross(getVertex<float>(x+1,y)-getVertex<float>(x,y), getVertex<float>(x,y+1)-getVertex<float>(x,y)); //3
            somme+= coords3d<float>::cross(getVertex<float>(x-1,y)-getVertex<float>(x,y), getVertex<float>(x,y-1)-getVertex<float>(x,y)); //4
            somme+= coords3d<float>::cross(getVertex<float>(x+1,y-1)-getVertex<float>(x,y-1), getVertex<float>(x,y)-getVertex<float>(x,y-1)); //5
            somme+= coords3d<float>::cross(getVertex<float>(x,y)-getVertex<float>(x+1,y), getVertex<float>(x+1,y-1)-getVertex<float>(x+1,y)); //6

            somme.normalize();
            _scene3d.normalMap[x][y] = somme;
        }
    }
}

void outPut::drawAxis()
{
    glEnable(GL_DEPTH_TEST);
    glUseProgram(_sNolight.getProgramID());
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
    glLineWidth(1);
}
