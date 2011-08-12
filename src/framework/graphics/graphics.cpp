/* The MIT License
 *
 * Copyright (c) 2010 OTClient, https://github.com/edubart/otclient
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <global.h>
#include <graphics/graphics.h>

Graphics g_graphics;

void Graphics::init()
{
    // setup opengl
    glEnable(GL_ALPHA_TEST); // enable alpha by default
    glAlphaFunc(GL_GREATER, 0.0f); // default alpha mode
    glDisable(GL_DEPTH_TEST); // we are rendering 2D only, we don't need it
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    logInfo("GPU ", glGetString(GL_RENDERER));
    logInfo("OpenGL ", glGetString(GL_VERSION));
}

void Graphics::terminate()
{
    m_bindedTexture.reset();
}

bool Graphics::isExtensionSupported(const char *extension)
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;
    where = (GLubyte *)strchr(extension, ' ');

    if(where || *extension == '\0')
        return 0;

    extensions = glGetString(GL_EXTENSIONS);

    start = extensions;
    while(true) {
        where = (GLubyte *) strstr((const char *)start, extension);
        if(!where)
            break;

        terminator = where + strlen(extension);

        if(where == start || *(where - 1) == ' ')
            if(*terminator == ' ' || *terminator == '\0')
                return 1;

        start = terminator;
    }
    return 0;
}

void Graphics::resize(const Size& size)
{
    m_screenSize = size;
    restoreViewport();
}

void Graphics::restoreViewport()
{
    disableDrawing();

    const int& width = m_screenSize.width();
    const int& height = m_screenSize.height();

    // resize gl viewport
    glViewport(0, 0, width, height);

    /*
     0,0---------0,w
     |           |
     |           |
     |           |
     h,0---------h,w
     */
    // setup view region like above
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, width, height, 0.0f);

    // back to model view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Graphics::beginRender()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
}

void Graphics::endRender()
{
    disableDrawing();
}

void Graphics::disableDrawing()
{
    if(m_drawMode != DRAW_NONE) {
        glEnd();
        m_drawMode = DRAW_NONE;

        m_bindedTexture.reset();
        glColor4ubv(Color::white.rgbaPtr());
    }
}

void Graphics::drawTexturedRect(const Rect& screenCoords, const TexturePtr& texture, const Rect& textureCoords, const Color& color)
{
    if(screenCoords.isEmpty() || textureCoords.isEmpty())
        return;

    // rect correction for opengl
    int right = screenCoords.right() + 1;
    int bottom = screenCoords.bottom() + 1;
    int top = screenCoords.top();
    int left = screenCoords.left();
    const Size& textureSize = texture->getGlSize();

    float textureRight = 0.0f;
    float textureBottom = 1.0f;
    float textureTop = 0.0f;
    float textureLeft = 1.0f;

    if(!textureCoords.isEmpty()) {
        textureRight = (float)(textureCoords.right() + 1) / textureSize.width();
        textureBottom = (float)(textureCoords.bottom() + 1) / textureSize.height();
        textureTop = (float)textureCoords.top() / textureSize.height();
        textureLeft = (float)textureCoords.left() / textureSize.width();
    }

    bindTexture(texture, color);
    glTexCoord2f(textureLeft,  textureTop);    glVertex2i(left,  top);
    glTexCoord2f(textureLeft,  textureBottom); glVertex2i(left,  bottom);
    glTexCoord2f(textureRight, textureBottom); glVertex2i(right, bottom);
    glTexCoord2f(textureRight, textureTop);    glVertex2i(right, top);
}

void Graphics::drawRepeatedTexturedRect(const Rect& screenCoords, const TexturePtr& texture, const Rect& textureCoords, const Color& color)
{
    if(screenCoords.isEmpty() || textureCoords.isEmpty())
        return;

    // render many repeated texture rects
    Rect virtualScreenCoords(0,0,screenCoords.size());
    for(int y = 0; y <= virtualScreenCoords.height(); y += textureCoords.height()) {
        for(int x = 0; x <= virtualScreenCoords.width(); x += textureCoords.width()) {
            Rect partialCoords(x, y, textureCoords.size());
            Rect partialTextureCoords = textureCoords;

            // partialCoords to screenCoords bottomRight
            if(partialCoords.bottom() > virtualScreenCoords.bottom()) {
                partialTextureCoords.setBottom(partialTextureCoords.bottom() + (virtualScreenCoords.bottom() - partialCoords.bottom()));
                partialCoords.setBottom(virtualScreenCoords.bottom());
            }
            if(partialCoords.right() > virtualScreenCoords.right()) {
                partialTextureCoords.setRight(partialTextureCoords.right() + (virtualScreenCoords.right() - partialCoords.right()));
                partialCoords.setRight(virtualScreenCoords.right());
            }

            partialCoords.translate(screenCoords.topLeft());
            drawTexturedRect(partialCoords, texture, partialTextureCoords, color);
        }
    }
}

void Graphics::drawFilledRect(const Rect& screenCoords, const Color& color)
{
    if(screenCoords.isEmpty())
        return;

    // rect correction for opengl
    int right = screenCoords.right() + 1;
    int bottom = screenCoords.bottom() + 1;
    int top = screenCoords.top();
    int left = screenCoords.left();

    bindColor(color);
    glVertex2i(left,  top);
    glVertex2i(left,  bottom);
    glVertex2i(right, bottom);
    glVertex2i(right, top);
}


void Graphics::drawBoundingRect(const Rect& screenCoords, const Color& color, int innerLineWidth)
{
    if(2 * innerLineWidth > screenCoords.height() || screenCoords.isEmpty())
        return;

    // rect correction for opengl
    int right = screenCoords.right()+1;
    int bottom = screenCoords.bottom()+1;
    int top = screenCoords.top();
    int left = screenCoords.left();

    bindColor(color);

    // top line
    glVertex2i(left,  top);
    glVertex2i(left,  top + innerLineWidth);
    glVertex2i(right, top + innerLineWidth);
    glVertex2i(right, top);

    // left
    glVertex2i(left, screenCoords.top() + innerLineWidth);
    glVertex2i(left, bottom - innerLineWidth);
    glVertex2i(left + innerLineWidth, bottom - innerLineWidth);
    glVertex2i(left + innerLineWidth, screenCoords.top() + innerLineWidth);

    // bottom line
    glVertex2i(left,  bottom);
    glVertex2i(left,  bottom - innerLineWidth);
    glVertex2i(right, bottom - innerLineWidth);
    glVertex2i(right, bottom);

    // right line
    glVertex2i(right                 , top + innerLineWidth);
    glVertex2i(right                 , bottom - innerLineWidth);
    glVertex2i(right - innerLineWidth, bottom - innerLineWidth);
    glVertex2i(right - innerLineWidth, top + innerLineWidth);
}

void Graphics::bindColor(const Color& color)
{
    // switch drawing to colored quads
    if(m_drawMode != DRAW_COLOR_QUADS || m_bindedColor != color) {
        if(m_drawMode != DRAW_NONE)
            glEnd();
        glDisable(GL_TEXTURE_2D);
        if(m_bindedColor != color) {
            glColor4ubv(color.rgbaPtr());
            m_bindedColor = color;
        }
        glBegin(GL_QUADS);
        m_drawMode = DRAW_COLOR_QUADS;
    }
}

void Graphics::bindTexture(const TexturePtr& texture, const Color& color)
{
    // switch drawing to textured quads
    if(m_drawMode != DRAW_TEXTURE_QUADS || m_bindedTexture != texture || m_bindedColor != color) {
        if(m_drawMode != DRAW_NONE)
            glEnd();
        glEnable(GL_TEXTURE_2D);
        if(m_bindedTexture != texture) {
            glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
            m_bindedTexture = texture;
        }
        if(m_bindedColor != color) {
            glColor4ubv(color.rgbaPtr());
            m_bindedColor = color;
        }
        glBegin(GL_QUADS);
        m_drawMode = DRAW_TEXTURE_QUADS;
    }
}
