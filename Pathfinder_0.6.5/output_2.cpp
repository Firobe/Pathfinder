#include "output.h"
#include <string>
#define __WINDOW_TITLE "Pathfinder OutPut"

outPut::outPut(Matrix array)
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


    _scene3d.normalMap = new coords3d*[_dimensions.x];
    for(int i = 0; i < _dimensions.x; i++)
        _scene3d.normalMap[i] = new coords3d[_dimensions.y];

    _scene3d.verticesMap = new coords3d*[_dimensions.x];
    for(int i = 0; i < _dimensions.x; i++)
        _scene3d.verticesMap[i] = new coords3d[_dimensions.y];
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

    loadConfig();

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    assert(glfwOpenWindow( _reg.WIDTH, _reg.HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW ));
    std::string window_title = "";
    window_title = window_title + __WINDOW_TITLE +" compilé le "+__DATE__+ " à " + __TIME__;
    glfwSetWindowTitle(window_title.c_str() );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)_reg.WIDTH/_reg.HEIGHT,1,1000);

    glEnable(GL_DEPTH_TEST);
   /* glDisable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);*/
    /*glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);*/
    //glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    init_Tw();
    init_Bars();

    _reg.MULTIPLIER = 5;
    //_reg.UNIFORM_COLOR = {0.5,0.5,0.5};
    _reg.UNIFORM_COLOR[0]=_reg.UNIFORM_COLOR[1]=_reg.UNIFORM_COLOR[2]=0.5;
    _running = true;
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
    _reg.WIREFRAME = _config.getBoolValue("WIREFRAME");
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

void TW_CALL recalculate(void* clientData)
{
    static_cast<outPut *>(clientData)->genList();
    static_cast<outPut *>(clientData)->drawResult();
}

void outPut::init_Bars()
{
    b_scene = TwNewBar("Scene");
    _scene3d.distance = _dimensions.x+_dimensions.y;
    _scene3d.zoom = 60;
    _scene3d.focus = coords3d::retournercoords3d(_dimensions.x/2,_dimensions.y/2,0);
    //_scene3d.orientation = {0, 0, 0, 1}; // direction pointing to +x and +y
    _scene3d.orientation[0]=_scene3d.orientation[1]=_scene3d.orientation[2]=0;
    _scene3d.orientation[3]=1;
    TwAddVarRW(b_scene, "Orientation", TW_TYPE_QUAT4F, &(_scene3d.orientation),"axisx=x axisy=y axisz=z");
    TwAddVarRW(b_scene, "Distance", TW_TYPE_DOUBLE, &(_scene3d.distance),"");
    TwAddVarRW(b_scene, "Zoom", TW_TYPE_DOUBLE, &(_scene3d.zoom),"min = 5 max = 200");
    TwAddVarRW(b_scene, "x", TW_TYPE_DOUBLE, &(_scene3d.focus.x), "group = 'Focus'");
    TwAddVarRW(b_scene, "y", TW_TYPE_DOUBLE, &(_scene3d.focus.y), "group = 'Focus'");
    TwAddVarRW(b_scene, "z", TW_TYPE_DOUBLE, &(_scene3d.focus.z), "group = 'Focus'");
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
    TwAddVarRW(b_reglages, "En cours", TW_TYPE_BOOLCPP, &_running, "group='Window settings' help='Fermer le programme' key=ESC");
    TwAddVarRO(b_reglages, "MAX_FPS", TW_TYPE_INT32, &(_reg.MAX_FPS), "group='Window settings'");

    TwAddButton(b_reglages, "Recalculer", recalculate, this, " group = Rendu ");
    TwAddVarRW(b_reglages, "Multiplicateur", TW_TYPE_DOUBLE, &(_reg.MULTIPLIER), "group = 'Rendu' min = 0 max = 100");
    TwType colorsType;
    colorsType = TwDefineEnum("ColorsType", NULL, 0);
    TwAddVarRW(b_reglages, "Couleurs", colorsType, &(_reg.COLORS), " enum='0 {Réelles}, 1 {Colorisé}, 2 {Uniforme}'");
    TwAddVarRW(b_reglages, "Rendu filaire", TW_TYPE_BOOLCPP, &(_reg.WIREFRAME), "group = 'Rendu'");
    TwAddVarRW(b_reglages, "Normales", TW_TYPE_BOOLCPP, &(_reg.DRAW_NORMALS), "group = 'Rendu'");
    TwAddVarRW(b_reglages, "Couleur unie", TW_TYPE_COLOR3F, &(_reg.UNIFORM_COLOR), "colormode=hls group = 'Rendu'");
    TwAddVarRW(b_reglages, "Test de profondeur résultat", TW_TYPE_BOOLCPP, &(_reg.ZTEST_RESULT), "label='Z test resultat' group = 'Rendu'");

    TwSetTopBar(b_scene);
    TwDefine(" Reglages iconified=true ");
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
    coords3d somme(0,0,0), contribution;
    coords3d vert_normal(0,0,1);
    vert_normal.normalize();

    for(int x = 0; x < _dimensions.x; x++)
    {
        for(int y = 0; y < _dimensions.y; y++)
        {
            somme = coords3d::retournercoords3d(0,0,0);

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

            somme+= coords3d::cross(getVertex(x,y)-getVertex(x-1,y), getVertex(x-1,y+1)-getVertex(x-1,y)); //1
            somme+= coords3d::cross(getVertex(x-1,y+1)-getVertex(x,y+1), getVertex(x,y)-getVertex(x,y+1)); //2
            somme+= coords3d::cross(getVertex(x+1,y)-getVertex(x,y), getVertex(x,y+1)-getVertex(x,y)); //3
            somme+= coords3d::cross(getVertex(x-1,y)-getVertex(x,y), getVertex(x,y-1)-getVertex(x,y)); //4
            somme+= coords3d::cross(getVertex(x+1,y-1)-getVertex(x,y-1), getVertex(x,y)-getVertex(x,y-1)); //5
            somme+= coords3d::cross(getVertex(x,y)-getVertex(x+1,y), getVertex(x+1,y-1)-getVertex(x+1,y)); //6

            somme.normalize();
            _scene3d.normalMap[x][y] = somme;
        }
    }
}

coords3d outPut::getVertex(int x, int y)
{
    if(x < 0)
    x = 0;
    if(x > _dimensions.x-1)
    x = _dimensions.x-1;

    if(y < 0)
    y = 0;
    if(y > _dimensions.y-1)
    y = _dimensions.y-1;

    return _scene3d.verticesMap[x][y];
}

void outPut::genList()
{
    double beginTime = glfwGetTime();
    gen_verticesMap();
    glNewList(_dispListMap, GL_COMPILE);

    glDisable(GL_LIGHTING);
    drawAxis();
    if(_reg.DRAW_NORMALS)
    drawNormals();
    glEnable(GL_LIGHTING);

    if(_reg.WIREFRAME)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    drawTerrain();
    glEndList();
   // std::cout << "Temps de calcul : " << glfwGetTime()- beginTime << " secondes." << endl;
}

void outPut::drawAxis()
{
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


}

void outPut::drawNormals()
{
        glLineWidth(1);
        glColor3ub(255,0,0);
        for(int x = 0; x < _dimensions.x; x++)
        {
            for(int y = 0; y < _dimensions.y; y++)
            {

                glBegin(GL_LINES);
                glVertex3d(_scene3d.verticesMap[x][y].x, _scene3d.verticesMap[x][y].y,_scene3d.verticesMap[x][y].z);
                glVertex3d(_scene3d.verticesMap[x][y].x+_scene3d.normalMap[x][y].x, _scene3d.verticesMap[x][y].y+_scene3d.normalMap[x][y].y,_scene3d.verticesMap[x][y].z+_scene3d.normalMap[x][y].z);
                glEnd();
            }
            glEnd();
        }
}

void outPut::drawTerrain()
{
        glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
        glColor3ub(50, 50,50);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
        glColor3ub(100, 100,100);
        glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 30.0);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        glColor3f(_reg.UNIFORM_COLOR[0],_reg.UNIFORM_COLOR[1],_reg.UNIFORM_COLOR[2]);

        for(int x = 0; x < _dimensions.x-1; x++)
        {
            glBegin(GL_TRIANGLES);

            for(int y = 0; y < _dimensions.y-1; y++)
            {
                if(_reg.COLORS == colorized)
                glColor3ub(_data[x][y], 255-_data[x][y], 0);
                if (_reg.COLORS == real)
                glColor3ub(_data[x][y], _data[x][y], _data[x][y]);

                glNormal3d(_scene3d.normalMap[x][y].x,_scene3d.normalMap[x][y].y,_scene3d.normalMap[x][y].z);
                glVertex3d(_scene3d.verticesMap[x][y].x, _scene3d.verticesMap[x][y].y,_scene3d.verticesMap[x][y].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data[x+1][y], 255-_data[x+1][y], 0);
                if (_reg.COLORS == real)
                glColor3ub(_data[x+1][y], _data[x+1][y], _data[x+1][y]);

                glNormal3d(_scene3d.normalMap[x+1][y].x,_scene3d.normalMap[x+1][y].y,_scene3d.normalMap[x+1][y].z);
                glVertex3d(_scene3d.verticesMap[x+1][y].x, _scene3d.verticesMap[x+1][y].y,_scene3d.verticesMap[x+1][y].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data[x][y+1], 255-_data[x][y+1], 0);
                if (_reg.COLORS == real)
                glColor3ub(_data[x][y+1], _data[x][y+1], _data[x][y+1]);

                glNormal3d (_scene3d.normalMap[x][y+1].x,_scene3d.normalMap[x][y+1].y,_scene3d.normalMap[x][y+1].z);
                glVertex3d(_scene3d.verticesMap[x][y+1].x, _scene3d.verticesMap[x][y+1].y,_scene3d.verticesMap[x][y+1].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data[x+1][y+1], 255-_data[x+1][y+1], 0);
                if (_reg.COLORS == real)
                glColor3ub(_data[x+1][y+1], _data[x+1][y+1], _data[x+1][y+1]);

                glNormal3d(_scene3d.normalMap[x+1][y+1].x,_scene3d.normalMap[x+1][y+1].y,_scene3d.normalMap[x+1][y+1].z);
                glVertex3d(_scene3d.verticesMap[x+1][y+1].x, _scene3d.verticesMap[x+1][y+1].y,_scene3d.verticesMap[x+1][y+1].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data[x][y+1], 255-_data[x][y+1], 0);
                if (_reg.COLORS == real)
                glColor3ub(_data[x][y+1], _data[x][y+1], _data[x][y+1]);

                glNormal3d (_scene3d.normalMap[x][y+1].x,_scene3d.normalMap[x][y+1].y,_scene3d.normalMap[x][y+1].z);
                glVertex3d(_scene3d.verticesMap[x][y+1].x, _scene3d.verticesMap[x][y+1].y,_scene3d.verticesMap[x][y+1].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data[x+1][y], 255-_data[x+1][y], 0);
                if (_reg.COLORS == real)
                glColor3ub(_data[x+1][y], _data[x+1][y], _data[x+1][y]);

                glNormal3d(_scene3d.normalMap[x+1][y].x,_scene3d.normalMap[x+1][y].y,_scene3d.normalMap[x+1][y].z);
                glVertex3d(_scene3d.verticesMap[x+1][y].x, _scene3d.verticesMap[x+1][y].y,_scene3d.verticesMap[x+1][y].z);
            }
            glEnd();
        }
}
