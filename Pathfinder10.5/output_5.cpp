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
    static GLuint buf_pos, buf_norm, buf_cost, buf_index;

    unsigned int nbVertices = _dimensions.x*_dimensions.y;
    unsigned int nbIndex = (_dimensions.x-1)*(_dimensions.y-1)*6;
    //nbIndex = nbVertices;
    /*Chaque vertex est associé à une maille qui 'coûte' 6 indices en mémoire :
         _
        |\| -> deux triangles, avec 3 indices par triangle. On enlève une ligne et une colonne parce que ceux sur les côtés n'entrainent pas une maille.
        +-
         */

    if(init || reinit)
    {
        gen_verticesMap();
        drawNormals(true);

        glDeleteBuffers(1,&buf_pos);
        glDeleteBuffers(1,&buf_norm);
        glDeleteBuffers(1,&buf_cost);
        glDeleteBuffers(1,&buf_index);


        delete terrain.verticesA;
        delete terrain.normalsA;
        delete terrain.costsA;
        delete terrain.indexA;

        terrain.verticesA = new float[nbVertices*P_SIZE];
        terrain.normalsA = new float[nbVertices*N_SIZE];
        terrain.costsA = new int[nbVertices*C_SIZE];
        terrain.indexA = new unsigned int[nbIndex];

///Remplissage des tableaux de sommets et de coûts
        for(unsigned int i = 0; i < _dimensions.x; i++)
        {
            for(unsigned int j = 0; j < _dimensions.y; j++)
            {
                coords3d<float> vertex(0,0,0), normal(0,0,0);

                vertex = getVertex<float>(i,j);
                normal = _scene3d.normalMap[i][j];

                int vertexPos = (i*_dimensions.y+j);

                terrain.verticesA[vertexPos*P_SIZE] = vertex.x;
                terrain.verticesA[vertexPos*P_SIZE+1] = vertex.y;
                terrain.verticesA[vertexPos*P_SIZE+2] = vertex.z;

                terrain.normalsA[vertexPos*P_SIZE] = normal.x;
                terrain.normalsA[vertexPos*P_SIZE+1] = normal.y;
                terrain.normalsA[vertexPos*P_SIZE+2] = normal.z;

                terrain.costsA[vertexPos*C_SIZE] = -1;
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
        glGenBuffers(1, &buf_cost);
        glGenBuffers(1, &buf_norm);
        glGenBuffers(1, &buf_index);

        /* construction du VBO de positions */
        glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
        glBufferData(GL_ARRAY_BUFFER, (nbVertices*P_SIZE*sizeof *terrain.verticesA),
                     NULL, GL_STREAM_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*P_SIZE*sizeof
                                             *terrain.verticesA), terrain.verticesA);

        /* construction du VBO de normales */
        glBindBuffer(GL_ARRAY_BUFFER, buf_norm);
        glBufferData(GL_ARRAY_BUFFER, (nbVertices*N_SIZE*sizeof *terrain.normalsA),
                     NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*N_SIZE*sizeof
                                             *terrain.normalsA), terrain.normalsA);

        /* construction du VBO de coûts */
        glBindBuffer(GL_ARRAY_BUFFER, buf_cost);
        glBufferData(GL_ARRAY_BUFFER, (nbVertices*C_SIZE*sizeof *terrain.costsA),
                     NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*C_SIZE*sizeof
                                             *terrain.costsA), terrain.costsA);


        /* construction du IBO */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndex * sizeof *terrain.indexA,
                     NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, nbIndex * sizeof
                        *terrain.indexA, terrain.indexA);

    //On debind les VBO+IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _maxValue = 1;

        cout<<"\n(re)initialisation de l'affichage du terrain reussie\n";
        init = false;
        _valueChanges.clear();
    }
    drawAxis();

    glBindBuffer(GL_ARRAY_BUFFER, buf_cost);
    for(unsigned int i = 0; i < _valueChanges.size(); ++i)
    {
        int newCost = _valueChanges[i].value;
        //newCost = 2;
        //std::cout << newCost << "/" << _maxValue << std::endl;
        glBufferSubData(GL_ARRAY_BUFFER, (_valueChanges[i].x*_dimensions.y + _valueChanges[i].y)*(sizeof newCost)*C_SIZE, (sizeof
                                             newCost), &newCost);
        //terrain.costsA[_valueChanges[i].x*_dimensions.y + _valueChanges[i].y] = newCost;
    }
    /*
    glBufferSubData(GL_ARRAY_BUFFER, 0, (nbVertices*C_SIZE*sizeof
                                        *terrain.costsA), terrain.costsA);*/
    catchError("application de changements de couleur");
    _valueChanges.clear();

    glEnable(GL_DEPTH_TEST);
    if(_reg.DRAW_NORMALS)
        drawNormals();

    glUseProgram(_sLight.getProgramID());
    //Envoi des uniforms
    glUniform1i(uid_maxCost, _maxValue);
    catchError("Envoi de la valeur max");
    glUniform3f(uid_defaultColor, _reg.UNIFORM_COLOR[0],_reg.UNIFORM_COLOR[1],_reg.UNIFORM_COLOR[2]);
    /* specification du buffer des positions de sommets */
    glEnableVertexAttribArray(aID_position);
    glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
    glVertexAttribPointer(aID_position, P_SIZE , GL_FLOAT, 0, 0, BUFFER_OFFSET(0) );
    /* specification du buffer des normales de sommets */
    glEnableVertexAttribArray(aID_normal);
    glBindBuffer(GL_ARRAY_BUFFER, buf_norm);
    glVertexAttribPointer(aID_normal, N_SIZE , GL_FLOAT, 0, 0, BUFFER_OFFSET(0) );
    /* specification du buffer des coûts de sommets */
    glEnableVertexAttribArray(aID_cost);
    glBindBuffer(GL_ARRAY_BUFFER, buf_cost);
    glVertexAttribIPointer(aID_cost, C_SIZE, GL_INT, 0, BUFFER_OFFSET(0));

    /*Spécification de l'index*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_index);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawElements(GL_TRIANGLES, nbIndex, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    glDisableVertexAttribArray(aID_position);
    glDisableVertexAttribArray(aID_normal);
    glDisableVertexAttribArray(aID_cost);

    //On debind les VBO+IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(aID_position);
    glDisableVertexAttribArray(aID_normal);
    glDisableVertexAttribArray(aID_cost);
    glUseProgram(0);
}

