#include "output.h"


vector<int*> outPut::choosePoints()
{
    glfwEnable( GLFW_KEY_REPEAT );

    _points.clear();

    TwBar *b_points = TwNewBar("Points stratégiques");
    coords<int> curPoint((float)_dimensions.x/2.0, (float)_dimensions.y/2.0);
    bool done(false), addCurrent(false), addRandom(false);
    coords3d<float> vertex(0,0,0);

    TwAddVarRW(b_points, "x", TW_TYPE_INT32, &(curPoint.x), "keyincr = RIGHT keydecr = LEFT" );
    TwAddVarRW(b_points, "y", TW_TYPE_INT32, &(curPoint.y), "keyincr = UP keydecr = DOWN" );
    TwAddVarRW(b_points, "Fini", TW_TYPE_BOOLCPP, &(done), "key = RETURN" );
    TwAddVarRW(b_points, "Ajouter courant", TW_TYPE_BOOLCPP, &(addCurrent), "key = SPACE" );
    TwAddVarRW(b_points, "Ajouter au hasard", TW_TYPE_BOOLCPP, &(addRandom), "key = 'r'" );
    TwAddVarRW(b_points, "Pente max", TW_TYPE_INT32, &(_status.maxDiff), "keyincr=o keydecr=l");

    while((!done || _points.size() < 2) && _status.running)
    {
    setScene();
    drawScene();
    glDisable(GL_DEPTH_TEST);
    glUseProgram(_sNolight.getProgramID());

    curPoint.x = clamp(curPoint.x, 1, _dimensions.x-1);
    curPoint.y = clamp(curPoint.y, 1, _dimensions.y-1);
    vertex = getVertex<float>(curPoint.x,curPoint.y);
    _scene3d.focus.x = curPoint.x;
    _scene3d.focus.y = curPoint.y;

    if(addRandom)
    {
        addCurrent = true;
        addRandom = false;
        curPoint.x = rand()%(_dimensions.x-1);
        curPoint.y = rand()%(_dimensions.y-1);
    }

    if(addCurrent)
    {
        _points.push_back(curPoint);
        addCurrent = false;
    }

    glBegin(GL_POINTS);
    glColor3ub(0,255,0);
    glVertex3d(vertex.x, vertex.y, vertex.z);
    glEnd();
    display();
    }
    unsigned int n = _points.size();
    std::cout << "nb de bombes : " << n << std::endl;
    vector<int*> bombList(n, NULL);
    for(unsigned int i = 0; i<n;i++)
    {
        bombList[i] = new int[n+2];
        for(unsigned int j = 0; j < n+2; ++j)
        {
            bombList[i][j] = 0;
        }
        bombList[i][n+2-2] = _points[i].x;
        bombList[i][n+2-1] = _points[i].y;
    }

    TwDeleteBar(b_points);

    TwDefine(" Scene/x  keyincr = RIGHT keydecr = LEFT");
    TwDefine(" Scene/y  keyincr = UP keydecr = DOWN");

    return bombList;
}

void outPut::drawPoints()
{
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_POINTS);
    for(unsigned int i=0;i<_points.size();i++)
    {
        glColor3ub(0,255,255);
        coords3d<float> vertex = getVertex<float>(_points[i].x,_points[i].y);
        glVertex3d(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
    glPopAttrib();
    return;
}

/*
Sortie : vector<int*> bombList

        _____n____
       /          \
       ____..._____
     / |0        0|
    |  |0        0|
n+2 |  |...    ...|
    |  |x0      xn|
     \ |y0      yn|
       ----...-----
*/
