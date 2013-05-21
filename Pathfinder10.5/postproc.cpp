#include "postproc.h"

fbo::fbo()
{
}

void fbo::genFBO(unsigned int screen_width,unsigned int screen_height)
{
    /* init_resources */
    /* Create back-buffer, used for post-processing */

    /* Texture */
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &colors);
    glBindTexture(GL_TEXTURE_2D, colors);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Depth buffer */
    glGenRenderbuffers(1, &depths);
    glBindRenderbuffer(GL_RENDERBUFFER, depths);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width, screen_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    /* Framebuffer to link everything together */
    glGenFramebuffers(1, &buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colors, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depths);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "Erreur au glCheckFramebufferStatus: %p", status);
        assert(false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

fbo::~fbo()
{
    glDeleteRenderbuffers(1, &depths);
    glDeleteTextures(1, &colors);
    glDeleteFramebuffers(1, &buffer);
}
