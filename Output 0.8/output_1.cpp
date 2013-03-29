#include "output.h"

double beginTime = 0;
unsigned int fps = 0;

void outPut::setScene()
{
    static bool autoZfocus(true);
    static bool init(true);
    if (init)
    {
        glfwSetTime(0);
        TwAddVarRW(b_scene, "Auto Z focus", TW_TYPE_BOOLCPP, &autoZfocus, "");
        TwAddVarRO(b_scene, "FPS", TW_TYPE_INT16, &fps, "");
        init = false;
    }
    float mat[4*4];
    coords3d focus3d;

    beginTime = glfwGetTime();

    focus3d = _scene3d.focus*(coords3d::retournercoords3d(_reg.PIXEL_SIZE, _reg.PIXEL_SIZE, 1));
    if(autoZfocus)
    {
        focus3d.z = getVertex((int)_scene3d.focus.x, (int)_scene3d.focus.y).z;
    }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(_scene3d.zoom,_reg.WIDTH/_reg.HEIGHT,3,100000);


    gluLookAt(focus3d.x, focus3d.y, _scene3d.distance, focus3d.x, focus3d.y, focus3d.z, 0, 1, 0);

    if(!_scene3d.lightPos[3])
    {
        _scene3d.lightPos[0] = -_scene3d.lightDir[0];
        _scene3d.lightPos[1] = -_scene3d.lightDir[1];
        _scene3d.lightPos[2] = -_scene3d.lightDir[2];
    }
    else
    {
        _scene3d.lightPos[0] = _scene3d.lightDir[0];
        _scene3d.lightPos[1] = _scene3d.lightDir[1];
        _scene3d.lightPos[2] = _scene3d.lightDir[2];
    }
    glLightfv(GL_LIGHT0,GL_POSITION,_scene3d.lightPos);


    glTranslatef(focus3d.x, focus3d.y, focus3d.z);
    ConvertQuaternionToMatrix(_scene3d.orientation, mat);
    glMultMatrixf(mat);
    glTranslatef(-focus3d.x, -focus3d.y, -focus3d.z);

}
void outPut::drawScene()
{
    drawLight();
    drawTerrain();
    TwDraw();
}

void outPut::display()
{
    //glfwSwapInterval(1);
    glfwSwapBuffers();

    double loopTime = glfwGetTime() - beginTime;
    fps = 1.0/loopTime;
    if(fps >  _reg.MAX_FPS)
        {
            glfwSleep(1.0/(double)_reg.MAX_FPS-loopTime );
            fps = 1.0/(glfwGetTime() - beginTime);
        //cout << "\nDodo de " << 1.0/(double)_reg.MAX_FPS-loopTime <<"s";
        }
}

void outPut::drawResult(vector<Node>* list)
{
    static bool init(true);
    static unsigned int occurence(0);
    static VA results;
    static GLuint buf_pos, buf_col;
    occurence++;
    static vector<Node>* lastList = NULL;
    if(init)
    {
        lastList = list;
        if(list == NULL)
            return;
        std::reverse(list->begin(), list->end());
    }
    else if(list == NULL)
    {
        list = lastList;
        std::reverse(list->begin(), list->end());
        init = true;
    }
#define decalZ 1
#define lineWidth 2
    if(init)
    {
        results.verticesA = new float[list->size()*P_SIZE];
        results.colorsA = new float[list->size()*C_SIZE];

        coords3d vertex(0,0,0), normal(0,0,0);

        for(unsigned int i = 0; i < list->size(); i++)
        {
            vertex = getVertex((*list)[i].x,(*list)[i].y);
            normal = _scene3d.normalMap[(*list)[i].x][(*list)[i].y];
            results.verticesA[i*P_SIZE] = vertex.x;
            results.verticesA[i*P_SIZE+1] = vertex.y;
            results.verticesA[i*P_SIZE+2] = vertex.z;
        }
        /* creation de nos VBO */
            glGenBuffers(1, &buf_pos);
            glGenBuffers(1, &buf_col);
            /* on bind le buffer des positions de sommets */
            glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
            /* on alloue un espace */
            glBufferData(GL_ARRAY_BUFFER, (list->size()*P_SIZE*sizeof *results.verticesA),
                         NULL, GL_STREAM_DRAW);
            /* on specifie les donnees */
            glBufferSubData(GL_ARRAY_BUFFER, 0, (list->size()*P_SIZE*sizeof
                                                 *results.verticesA), results.verticesA);
            /* on bind le buffer des positions de sommets */
            glBindBuffer(GL_ARRAY_BUFFER, buf_col);
            /* on alloue un espace */
            glBufferData(GL_ARRAY_BUFFER, (list->size()*C_SIZE*sizeof *results.colorsA),
                         NULL, GL_STREAM_DRAW);
            /* on specifie les donnees */
            glBufferSubData(GL_ARRAY_BUFFER, 0, (list->size()*C_SIZE*sizeof
                                                 *results.colorsA), results.colorsA);
            /* plus aucun buffer n'est a utiliser */
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        init = false;
    }
#define SPACE 5
#define SPEED 0.5

    for(unsigned int i = 0; i < list->size(); i++)
    {
        results.colorsA[i*C_SIZE] = 0;
        results.colorsA[i*C_SIZE+1] = (int)(occurence*SPEED+i)%(SPACE)*(255/SPACE);
        results.colorsA[i*C_SIZE+2] = 0;
    }
    GLvoid *col_vbo = NULL;
    glBindBuffer(GL_ARRAY_BUFFER, buf_col);
    col_vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    assert(col_vbo != NULL);
    /* transfert */
    memcpy(col_vbo, results.colorsA, list->size() * P_SIZE * sizeof *(results.colorsA));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    col_vbo = NULL;


    if(!_reg.ZTEST_RESULT)
        glDisable(GL_DEPTH_TEST);

    /* specification du buffer des positions de sommets */
    glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
    glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
    /* specification du buffer des couleurs de sommets */
    glBindBuffer(GL_ARRAY_BUFFER, buf_col);
    glColorPointer(C_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDisable(GL_LIGHTING);
    glLineWidth(lineWidth);
    glDrawArrays(GL_LINE_STRIP, 0, list->size());
    glEnable(GL_LIGHTING);
    glLineWidth(1);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    if(!_reg.ZTEST_RESULT)
        glEnable(GL_DEPTH_TEST);
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
