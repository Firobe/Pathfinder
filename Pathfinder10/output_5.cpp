#include "output.h"

void outPut::drawNormals(bool reinit)
{
    static VA normals;
    static GLuint buf_pos;
    unsigned int nbVertices = _dimensions.x*_dimensions.y*2;

    if(reinit)
    {
        glDeleteBuffers(1,&buf_pos);
        delete normals.verticesA;

        normals.verticesA = new float[nbVertices*P_SIZE];

#define NORM_SCALE 2.5
///Remplissage des tableaux de sommets et de couleurs
        for(unsigned int i = 0; i < _dimensions.x; i++)
        {
            for(unsigned int j = 0; j < _dimensions.y; j++)
            {
                coords3d<float> vertex(0,0,0), normal(0,0,0);

                vertex = getVertex<float>(i,j);
                normal = _scene3d.normalMap[i][j];
                normals.verticesA[(i*_dimensions.y+j)*P_SIZE*2] = vertex.x;
                normals.verticesA[(i*_dimensions.y+j)*P_SIZE*2+1] = vertex.y;
                normals.verticesA[(i*_dimensions.y+j)*P_SIZE*2+2] = vertex.z;

                normals.verticesA[(i*_dimensions.y+j)*P_SIZE*2+3] = vertex.x+normal.x*NORM_SCALE;
                normals.verticesA[(i*_dimensions.y+j)*P_SIZE*2+4] = vertex.y+normal.y*NORM_SCALE;
                normals.verticesA[(i*_dimensions.y+j)*P_SIZE*2+5] = vertex.z+normal.z*NORM_SCALE;
            }
        }
        /* creation de nos VBO+IBO */
        glGenBuffers(1, &buf_pos);

        /* construction du VBO de positions */
        glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
        glBufferData(GL_ARRAY_BUFFER, (nbVertices*P_SIZE*sizeof *normals.verticesA),
                     NULL, GL_STREAM_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*P_SIZE*sizeof
                                             *normals.verticesA), normals.verticesA);

    //On debind le VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

        cout<<"\n(re)initialisation de l'affichage des normales reussie\n";
    }
    glUseProgram(_sNolight.getProgramID());
    /* specification du buffer des positions de sommets */
    glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
    glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));

    glColor3ub(255,0,255);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINES, 0, nbVertices);

    glDisableClientState(GL_VERTEX_ARRAY);

    //On debind le VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void outPut::drawTerrain(bool reinit)
{
    static bool lastwfStatus(!_reg.WIREFRAME);
    if(lastwfStatus != _reg.WIREFRAME)
    {
        if(_reg.WIREFRAME)
            {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }}
    lastwfStatus = _reg.WIREFRAME;

    static bool init(true);
    static VA terrain;
    static GLuint buf_pos, buf_col, buf_norm, buf_index;

    unsigned int nbVertices = _dimensions.x*_dimensions.y;
    unsigned int nbIndex = (_dimensions.x-1)*(_dimensions.y-1)*6;
    //nbIndex = nbVertices;
    /*Chaque vertex est associ� � une maille qui 'co�te' 6 indices en m�moire :
         _
        |\| -> deux triangles, avec 3 indices par triangle. On enl�ve une ligne et une colonne parce que ceux sur les c�t�s n'entrainent pas une maille.
        +-
         */

    if(init || reinit)
    {
        gen_verticesMap();
        drawNormals(true);

        glDeleteBuffers(1,&buf_pos);
        glDeleteBuffers(1,&buf_col);
        glDeleteBuffers(1,&buf_norm);
        glDeleteBuffers(1,&buf_index);


        delete terrain.verticesA;
        delete terrain.colorsA;
        delete terrain.indexA;
        delete terrain.normalsA;

        terrain.verticesA = new float[nbVertices*P_SIZE];
        terrain.colorsA = new float[nbVertices*C_SIZE];
        terrain.normalsA = new float[nbVertices*N_SIZE];
        terrain.indexA = new unsigned int[nbIndex];

///Remplissage des tableaux de sommets et de couleurs
        for(unsigned int i = 0; i < _dimensions.x; i++)
        {
            for(unsigned int j = 0; j < _dimensions.y; j++)
            {
                coords3d<float> vertex(0,0,0), normal(0,0,0);

                vertex = getVertex<float>(i,j);
                normal = _scene3d.normalMap[i][j];
                terrain.verticesA[(i*_dimensions.y+j)*P_SIZE] = vertex.x;
                terrain.verticesA[(i*_dimensions.y+j)*P_SIZE+1] = vertex.y;
                terrain.verticesA[(i*_dimensions.y+j)*P_SIZE+2] = vertex.z;

                terrain.normalsA[(i*_dimensions.y+j)*P_SIZE] = normal.x;
                terrain.normalsA[(i*_dimensions.y+j)*P_SIZE+1] = normal.y;
                terrain.normalsA[(i*_dimensions.y+j)*P_SIZE+2] = normal.z;

                float color[3] = {_reg.UNIFORM_COLOR[0],_reg.UNIFORM_COLOR[1],_reg.UNIFORM_COLOR[2]};
                if(_reg.COLORS == colorized)
                {
                    color[0] = _data[i][j]/255.0;
                    color[1] = 1-_data[i][j]/255.0;
                    color[2] = 0;
                }
                else if(_reg.COLORS == real)
                {
                    color[0] = _data[i][j]/255.0;
                    color[1] = _data[i][j]/255.0;
                    color[2] = _data[i][j]/255.0;
                }

                terrain.colorsA[(i*_dimensions.y+j)*C_SIZE] = color[0];
                terrain.colorsA[(i*_dimensions.y+j)*C_SIZE+1] = color[1];
                terrain.colorsA[(i*_dimensions.y+j)*C_SIZE+2] = color[2];
            }
        }
///Remplissage de l'index
        for(unsigned int i = 0; i < _dimensions.x-1; i++)
        {
            for(unsigned int j = 0; j < _dimensions.y-1; j++)
            {
                terrain.indexA[((i*(_dimensions.y-1))+j)*6] = i*_dimensions.y+j;
                terrain.indexA[((i*(_dimensions.y-1))+j)*6+2] = i*_dimensions.y+j+1;
                terrain.indexA[((i*(_dimensions.y-1))+j)*6+1] = (i+1)*_dimensions.y+j;
                terrain.indexA[((i*(_dimensions.y-1))+j)*6+3] = i*_dimensions.y+j+1;
                terrain.indexA[((i*(_dimensions.y-1))+j)*6+4] = (i+1)*_dimensions.y+j;
                terrain.indexA[((i*(_dimensions.y-1))+j)*6+5] = (i+1)*_dimensions.y+j+1;
                //cout << "\nindice max : " << ((i*(_dimensions.y-1))+j)*6+5 << " / " << nbIndex;

            }
        }

        /* creation de nos VBO+IBO */
        glGenBuffers(1, &buf_pos);
        glGenBuffers(1, &buf_col);
        glGenBuffers(1, &buf_norm);
        glGenBuffers(1, &buf_index);

        /* construction du VBO de positions */
        glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
        glBufferData(GL_ARRAY_BUFFER, (nbVertices*P_SIZE*sizeof *terrain.verticesA),
                     NULL, GL_STREAM_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*P_SIZE*sizeof
                                             *terrain.verticesA), terrain.verticesA);

        /* construction du VBO de couleurs */
        glBindBuffer(GL_ARRAY_BUFFER, buf_col);
        glBufferData(GL_ARRAY_BUFFER, (nbVertices*C_SIZE*sizeof *terrain.colorsA),
                     NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*C_SIZE*sizeof
                                             *terrain.colorsA), terrain.colorsA);

        /* construction du VBO de normales */
        glBindBuffer(GL_ARRAY_BUFFER, buf_norm);
        glBufferData(GL_ARRAY_BUFFER, (nbVertices*N_SIZE*sizeof *terrain.normalsA),
                     NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*N_SIZE*sizeof
                                             *terrain.normalsA), terrain.normalsA);

        /* construction du IBO */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndex * sizeof *terrain.indexA,
                     NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, nbIndex * sizeof
                        *terrain.indexA, terrain.indexA);

    //On debind les VBO+IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

        cout<<"\n(re)initialisation de l'affichage du terrain reussie\n";
        init = false;
    }
    drawAxis();

    glBindBuffer(GL_ARRAY_BUFFER, buf_col);
    for(unsigned int i = 0; i < _valueChanges.size(); ++i)
    {
        float couleur[3];
        couleur[0] = (float)_valueChanges[i].value/400/*(float)_maxValue*/;
        couleur[1] = 1.0-couleur[0];
        couleur[2] = 0.0f;
        glBufferSubData(GL_ARRAY_BUFFER, (_valueChanges[i].x*_dimensions.y + _valueChanges[i].y)*(C_SIZE* sizeof *couleur), (C_SIZE*sizeof
                                             *couleur), couleur);
    }
    _valueChanges.clear();

    glEnable(GL_DEPTH_TEST);
    if(_reg.DRAW_NORMALS)
        drawNormals();

    glUseProgram(_sLight.getProgramID());
    /* specification du buffer des positions de sommets */
    glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
    glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
    /* specification du buffer des couleurs de sommets */
    glBindBuffer(GL_ARRAY_BUFFER, buf_col);
    glColorPointer(C_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
    /* specification du buffer des normales de sommets */
    glBindBuffer(GL_ARRAY_BUFFER, buf_norm);
    glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(0));

    /*Sp�cification de l'index*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_index);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glDrawElements(GL_TRIANGLES, nbIndex, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    //On debind les VBO+IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

