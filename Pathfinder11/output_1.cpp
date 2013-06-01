#include "output.h"

double beginTime = 0;
unsigned int fps = 0;

void outPut::setScene()
{
    _status.running = _status.running && glfwGetWindowParam( GLFW_OPENED ); //On met à jour le statut d'execution
    if(!_status.running)
        return;
    static bool autoZfocus(true);
    static bool init(true);
    if (init)
    {
        glfwSetTime(0);
        TwAddVarRW(b_scene, "Auto Z focus", TW_TYPE_BOOLCPP, &autoZfocus, "group = 'Focus'");
        TwAddVarRO(b_scene, "FPS", TW_TYPE_INT16, &fps, "");
        init = false;
    }
    float mat[4*4];
    coords3d<float> focus3d;

    beginTime = glfwGetTime();

    focus3d = _scene3d.focus*(coords3d<float>::retournercoords3d(_reg.PIXEL_SIZE, _reg.PIXEL_SIZE, 1));
    if(autoZfocus)
    {
        focus3d.z = getVertex<float>((int)_scene3d.focus.x, (int)_scene3d.focus.y).z;
    }
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(_scene3d.zoom,_reg.WIDTH/_reg.HEIGHT,3,100000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(focus3d.x, focus3d.y, _scene3d.distance, focus3d.x, focus3d.y, focus3d.z, 0, 1, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.buffer);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

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
    float camPos[4] = {focus3d.x, focus3d.y,_scene3d.distance,0.0};
    _sLight.setPos(_scene3d.lightPos );

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    glTranslatef(focus3d.x, focus3d.y, focus3d.z);
    ConvertQuaternionToMatrix(_scene3d.orientation, mat);
    glMultMatrixf(mat);
    glTranslatef(-focus3d.x, -focus3d.y, -focus3d.z);

}
void outPut::drawScene()
{
    if(!_status.running)
        return;
    drawTerrain();
    drawLight();
    drawPoints();
    TwDraw();
}

void outPut::display()
{
    if(!_status.running)
        return;

    if(_reg.WIREFRAME)
    {
        glPushAttrib(GL_POLYGON_BIT);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUseProgram(_postProcess.getProgramID());
    glBindTexture(GL_TEXTURE_2D, frameBuffer.colors);
    glUniform1i(uid_pp_colors, /*GL_TEXTURE*/0);
    glUniform1f(uid_pp_offset, glfwGetTime() * 2*3.14159 * .75);
    glUniform1i(uid_pp_effect, _effect);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(-1, -1);
    glVertex2d(1, -1);
    glVertex2d(-1, 1);
    glVertex2d(1, 1);
    glEnd();

    glfwSwapBuffers();

    double loopTime = glfwGetTime() - beginTime;
    fps = 1.0/loopTime;
    if(fps >  _reg.MAX_FPS)
    {
        glfwSleep(1.0/(double)_reg.MAX_FPS-loopTime );
        fps = 1.0/(glfwGetTime() - beginTime);
    }
    if(_reg.WIREFRAME)
        glPopAttrib();
}

void outPut::drawResult(vector<Node>* list)
{
    if(!_status.running)
        return;
    static vector<Node>* lastList = NULL;

    static bool init(true);
    static int occurence(0);
    occurence++;

    static VA results;
    static int* attribs;
    static GLuint buf_result;
    static opShader shader;

    static GLuint uid_occurence = 0;

    if(init)
    {
        lastList = list;
        if(list == NULL)
            return;
    }
    else if(list == NULL)
    {
        list = lastList;
        init = true;
    }
#define DECALZ 1
#define LINEWIDTH 2
#define ATTRIB_ID 1
    if(init)
    {
        shader.delShader();
        shader.loadProgram("Ressources/shaders/result.vs","Ressources/shaders/result.fs");
        catchError("chargement des sources");
        glBindAttribLocation(shader.getProgramID(), ATTRIB_ID, "attrib");
        catchError("bindattrib");
        shader.linkProgram();
        catchError("link");

        uid_occurence = glGetUniformLocation(shader.getProgramID(), "occurence");
        catchError("getuniformlocation");

        glDeleteBuffers(1,&buf_result);
        delete results.verticesA;
        delete attribs;
        results.verticesA = new float[list->size()*P_SIZE];
        attribs = new int[list->size()];

        coords3d<float> vertex(0,0,0), normal(0,0,0);

        for(unsigned int i = 0; i < list->size(); i++)
        {
            vertex = getVertex<float>((*list)[i].x,(*list)[i].y);
            results.verticesA[i*P_SIZE] = vertex.x;
            results.verticesA[i*P_SIZE+1] = vertex.y;
            results.verticesA[i*P_SIZE+2] = vertex.z+DECALZ;

            attribs[i] = list->size()-i;
        }

        /* creation de nos VBO */
        glGenBuffers(1, &buf_result);
        /* on bind le buffer des positions de sommets */
        glBindBuffer(GL_ARRAY_BUFFER, buf_result);

        GLsizeiptr size_pos = (list->size()*P_SIZE*sizeof *results.verticesA),
        size_attrib = (list->size()*sizeof *attribs);
        /* on alloue un espace */
        glBufferData(GL_ARRAY_BUFFER, size_pos + size_attrib ,
                     NULL, GL_STREAM_DRAW);
        /* on specifie les donnees */
        glBufferSubData(GL_ARRAY_BUFFER, 0, size_pos, results.verticesA);
        glBufferSubData(GL_ARRAY_BUFFER, size_pos, size_attrib, attribs);

        /* plus aucun buffer n'est a utiliser */
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        init = false;
    }

    if(!_reg.ZTEST_RESULT)
        glDisable(GL_DEPTH_TEST);
    else
        glEnable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableVertexAttribArray(ATTRIB_ID);

    glBindBuffer(GL_ARRAY_BUFFER, buf_result);
    /* specification du buffer des positions de sommets */
    glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
    /* specification du buffer des attributs de sommets */
    glVertexAttribIPointer(ATTRIB_ID, 1, GL_INT, 0, BUFFER_OFFSET((list->size()*P_SIZE*sizeof *results.verticesA)));
    catchError("Envoi du tableau d'attributs du resultat");

    glUseProgram(shader.getProgramID());
    glUniform1i(uid_occurence, occurence);
    catchError("Envoi de l'uniform 'occurence'");

    glLineWidth(LINEWIDTH);
    glDrawArrays(GL_LINE_STRIP, 0, list->size());
    glLineWidth(1);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableVertexAttribArray(ATTRIB_ID);
    glUseProgram(0);
}

void outPut::drawLight()
{
    glEnable(GL_DEPTH_TEST);
    glUseProgram(_sNolight.getProgramID());
#define DL_SCALE 10
#define LINE_WIDTH 2
    glPointSize(5);
    glLineWidth(LINE_WIDTH);
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
}
