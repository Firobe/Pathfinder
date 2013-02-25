#include "functions.h"

outPut::outPut(Matrix array)
{
    _data = array;
    _normalMap = new coords3d*[_data.GetX()];
    for(int i = 0; i < _data.GetX(); i++)
        _normalMap[i] = new coords3d[_data.GetY()];

    _verticesMap = new coords3d*[_data.GetX()];
    for(int i = 0; i < _data.GetX(); i++)
        _verticesMap[i] = new coords3d[_data.GetY()];
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

    _reg.COLORS = uniform;
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
    _reg.WIREFRAME = _config.getBoolValue("WIREFRAME");
    _reg.DRAW_NORMALS = _config.getBoolValue("DRAW_NORMALS");

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    assert(glfwOpenWindow( _reg.WIDTH, _reg.HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW ));

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

    _reg.MULTIPLIER = 5;
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

void outPut::gen_verticesMap()
{
    for(int x = 0; x < _data.GetX(); x++)
    {
        for(int y = 0; y < _data.GetY(); y++)
        {
            _verticesMap[x][y].x = x*_reg.PIXEL_SIZE;
            _verticesMap[x][y].y = y*_reg.PIXEL_SIZE;
            _verticesMap[x][y].z = _data.GetValue(x,y,0)/25.5*_reg.MULTIPLIER;
        }
    }
    gen_normalMap();
}

void outPut::gen_normalMap()
{
    coords3d somme(0,0,0), contribution;
    coords3d vert_normal(0,0,1);
    vert_normal.normalize();

    for(int x = 0; x < _data.GetX(); x++)
    {
        _normalMap[x][0] = vert_normal;
        _normalMap[x][_data.GetY()-1] = vert_normal;
    }
    for(int y = 1; y < _data.GetY(); y++)
    {
        _normalMap[0][y] = vert_normal;
        _normalMap[_data.GetY()-1][y] = vert_normal;
    }

    for(int x = 1; x < _data.GetX()-1; x++)
    {
        for(int y = 1; y < _data.GetY()-1; y++)
        {
            somme = coords3d::retournercoords3d(0,0,0);
            /*       0,1
             -1,1  +--+--+
                   |\2|\*|
                   |1\|3\|
             -1,0  +--+--+ 1,0
                   |\4|\6|
                   |*\|5\|
             -1,-1 +--+--+ 1,-1
                     0,-1
            */
            somme+= coords3d::cross(_verticesMap[x][y]-_verticesMap[x-1][y], _verticesMap[x-1][y+1]-_verticesMap[x-1][y]); //1
            somme+= coords3d::cross(_verticesMap[x-1][y+1]-_verticesMap[x][y+1], _verticesMap[x][y]-_verticesMap[x][y+1]); //2
            somme+= coords3d::cross(_verticesMap[x+1][y]-_verticesMap[x][y], _verticesMap[x][y+1]-_verticesMap[x][y]); //3
            somme+= coords3d::cross(_verticesMap[x-1][y]-_verticesMap[x][y], _verticesMap[x][y-1]-_verticesMap[x][y]); //4
            somme+= coords3d::cross(_verticesMap[x+1][y-1]-_verticesMap[x][y-1], _verticesMap[x][y]-_verticesMap[x][y-1]); //5
            somme+= coords3d::cross(_verticesMap[x][y]-_verticesMap[x+1][y], _verticesMap[x+1][y-1]-_verticesMap[x+1][y]); //6


            somme.normalize();
            _normalMap[x][y] = somme;
        }
    }
}

void outPut::genList()
{
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
        for(int x = 0; x < _data.GetX(); x++)
        {
            for(int y = 0; y < _data.GetY(); y++)
            {

                glBegin(GL_LINES);
                glVertex3d(_verticesMap[x][y].x, _verticesMap[x][y].y,_verticesMap[x][y].z);
                glVertex3d(_verticesMap[x][y].x+_normalMap[x][y].x, _verticesMap[x][y].y+_normalMap[x][y].y,_verticesMap[x][y].z+_normalMap[x][y].z);
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
        glColor3ub(255,255,255);

        for(int x = 0; x < _data.GetX()-1; x++)
        {
            glBegin(GL_TRIANGLES);

            for(int y = 0; y < _data.GetY()-1; y++)
            {
                if(_reg.COLORS == colorized)
                glColor3ub(_data.GetValue(x,y,0), 255-_data.GetValue(x,y,0), 0);
                if (_reg.COLORS == real)
                glColor3ub(_data.GetValue(x,y,0), _data.GetValue(x,y,0), _data.GetValue(x,y,0));

                glNormal3d(_normalMap[x][y].x,_normalMap[x][y].y,_normalMap[x][y].z);
                glVertex3d(_verticesMap[x][y].x, _verticesMap[x][y].y,_verticesMap[x][y].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data.GetValue(x+1,y,0), 255-_data.GetValue(x+1,y,0), 0);
                if (_reg.COLORS == real)
                glColor3ub(_data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0));

                glNormal3d(_normalMap[x+1][y].x,_normalMap[x+1][y].y,_normalMap[x+1][y].z);
                glVertex3d(_verticesMap[x+1][y].x, _verticesMap[x+1][y].y,_verticesMap[x+1][y].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data.GetValue(x,y+1,0), 255-_data.GetValue(x,y+1,0), 0);
                if (_reg.COLORS == real)
                glColor3ub(_data.GetValue(x,y+1,0), _data.GetValue(x,y+1,0), _data.GetValue(x,y+1,0));

                glNormal3d (_normalMap[x][y+1].x,_normalMap[x][y+1].y,_normalMap[x][y+1].z);
                glVertex3d(_verticesMap[x][y+1].x, _verticesMap[x][y+1].y,_verticesMap[x][y+1].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data.GetValue(x+1,y+1,0), 255-_data.GetValue(x+1,y+1,0), 0);
                if (_reg.COLORS == real)
                glColor3ub(_data.GetValue(x+1,y+1,0), _data.GetValue(x+1,y+1,0), _data.GetValue(x+1,y+1,0));

                glNormal3d(_normalMap[x+1][y+1].x,_normalMap[x+1][y+1].y,_normalMap[x+1][y+1].z);
                glVertex3d(_verticesMap[x+1][y+1].x, _verticesMap[x+1][y+1].y,_verticesMap[x+1][y+1].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data.GetValue(x,y+1,0), 255-_data.GetValue(x,y+1,0), 0);
                if (_reg.COLORS == real)
                glColor3ub(_data.GetValue(x,y+1,0), _data.GetValue(x,y+1,0), _data.GetValue(x,y+1,0));

                glNormal3d (_normalMap[x][y+1].x,_normalMap[x][y+1].y,_normalMap[x][y+1].z);
                glVertex3d(_verticesMap[x][y+1].x, _verticesMap[x][y+1].y,_verticesMap[x][y+1].z);

                if(_reg.COLORS == colorized)
                glColor3ub(_data.GetValue(x+1,y,0), 255-_data.GetValue(x+1,y,0), 0);
                if (_reg.COLORS == real)
                glColor3ub(_data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0), _data.GetValue(x+1,y,0));

                glNormal3d(_normalMap[x+1][y].x,_normalMap[x+1][y].y,_normalMap[x+1][y].z);
                glVertex3d(_verticesMap[x+1][y].x, _verticesMap[x+1][y].y,_verticesMap[x+1][y].z);
            }
            glEnd();
        }
}
