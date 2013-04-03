#include "shaders.h"

void opShader::loadProgram(const char* vsFile, const char* fsFile)
{
    vs = glCreateShader(GL_VERTEX_SHADER);
    assert(vs != 0);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fs != 0);
    program = glCreateProgram();
    loadVs(vsFile);
    loadFs(fsFile);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
}

void opShader::linkProgram()
{
    glLinkProgram(program);

    /* verification du succes du 'linkage' */
    GLint link_status = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if(link_status != GL_TRUE)
    {
        GLsizei logsize = 0;
        char *log = NULL;
        /* erreur a la compilation recuperation du log d'erreur */
        /* on recupere la taille du message d'erreur */
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);
        /* on alloue un espace memoire dans lequel OpenGL ecrira le
        message */
        log = new char[logsize + 1];
        assert (log != NULL);

        /* initialisation du contenu */
        memset(log, '\0', logsize + 1);
        glGetProgramInfoLog(program, logsize, &logsize, log);
        std::cerr << "\nImpossible de linker le program : " << log;
        /* ne pas oublier de liberer la memoire et notre shader */
        delete log;
        assert(false);
    }
    std::cout << "\nLink du shader reussi\n";
}

void opShader::loadVs(const char*filename)
{
    vs = loadShader(GL_VERTEX_SHADER, filename);
}


void opShader::loadFs(const char*filename)
{
    fs = loadShader(GL_FRAGMENT_SHADER, filename);
}

opShader::opShader()
{

}

void opShader::delShader()
{
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}

opShader::~opShader()
{
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}

GLchar* opShader::loadSource(const char*filename)
{
    GLchar *src = NULL; /* code source de notre shader */
    FILE *fp = NULL; /* fichier */
    long size; /* taille du fichier */
    long i; /* compteur */
    /* on ouvre le fichier */
    fp = fopen(filename, "r");
    /* on verifie si l'ouverture a echoue */
    if(fp == NULL)
    {
        std::cerr << "impossible d'ouvrir le fichier " << filename << std::endl;
        return NULL;
    }
    /* on recupere la longueur du fichier */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    /* on se replace au debut du fichier */
    rewind(fp);
    /* on alloue de la memoire pour y placer notre code source */
    src = new GLchar[size+1]; /* +1 pour le caractere de fin de chaine '\0' */
    if(src == NULL)
    {
        fclose(fp);
        fprintf(stderr, "erreur d'allocation de memoire!\n");
        return NULL;
    }
    /* lecture du fichier */
    for(i=0; i<size; i++)
        src[i] = fgetc(fp);
    /* on place le dernier caractere a '\0' */
    src[size] = '\0';
    fclose(fp);

    std::cout << "\nChargement des sources du shader " << filename << " reussie !";
    return src;
}

GLuint opShader::loadShader(GLenum type, const char*filename)
{
    GLuint shader = 0;
    /* creation d'un shader de sommet */
    shader = glCreateShader(type);
    if(shader == 0)
    {
        std::cerr << "Impossible de creer le shader\n";
        return 0;
    }
    /* chargement du code source */
    char *src = NULL;
    src = loadSource(filename);
    if(src == NULL)
    {
        /* theoriquement, la fonction LoadSource a deja affiche un
        message d'erreur, nous nous contenterons de supprimer notre shader
        et de retourner 0 */
        glDeleteShader(shader);
        return 0;
    }
    /* assignation du code source */
    glShaderSource(shader, 1, (const GLchar**)&src, NULL);
    /* compilation du shader */
    glCompileShader(shader);
    /* liberation de la memoire du code source */
    delete src;
    src = NULL;

    /* verification du succes de la compilation */
    GLint compile_status = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if(compile_status != GL_TRUE)
    {
        GLsizei logsize = 0;
        char *log = NULL;
        /* erreur a la compilation recuperation du log d'erreur */
        /* on recupere la taille du message d'erreur */
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
        /* on alloue un espace memoire dans lequel OpenGL ecrira le
        message */
        log = new char[logsize + 1];
        assert (log != NULL);

        /* initialisation du contenu */
        memset(log, '\0', logsize + 1);
        glGetShaderInfoLog(shader, logsize, &logsize, log);
        std::cerr << "\nImpossible de compiler le shader." << filename << " : " << log;
        /* ne pas oublier de liberer la memoire et notre shader */
        delete log;
        glDeleteShader(shader);
        return 0;
    }
    std::cout << "\nCompilation du " << ((type == GL_VERTEX_SHADER)?"vertex":"fragment") << " shader reussi.";
    return shader;
}
