#include "Font.hpp"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <GL/glew.h>

Font::Font(const std::string& filename)
    : texID(0)
{
    loadFont(filename);
}

Font::~Font()
{
}

stbtt_aligned_quad Font::getQuad(int& charIndex, float& x, float& y)
{
    stbtt_aligned_quad q;
    stbtt_GetPackedQuad(pdata, BITMAP_W, BITMAP_H,
                        charIndex - 32,
                        &x, &y, &q,
                        1); //1=opengl & d3d10+,0=d3d9

    return q;
}

unsigned int Font::getTexture() const
{
    return texID;
}

void Font::draw(float x, float y, const std::string& text)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glBegin(GL_QUADS);
    for (int c : text)
    {
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(pdata, BITMAP_W, BITMAP_H,
                            c - 32,
                            &x, &y, &q,
                            1); //1=opengl & d3d10+,0=d3d9

        glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
        glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
        glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
        glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Font::loadFont(const std::string& filename)
{
    unsigned char ttf_buffer[1<<20];
    unsigned char temp_bitmap[BITMAP_H][BITMAP_W];

    fread(ttf_buffer, 1, 1 << 20, fopen(filename.c_str(), "rb"));

    stbtt_pack_context pc;
    stbtt_pack_range pr[1];
    stbtt_PackBegin(&pc, temp_bitmap[0], BITMAP_W, BITMAP_H, 0, 1, nullptr);

    pr[0].chardata_for_range = pdata;
    pr[0].first_unicode_char_in_range = 32;
    pr[0].num_chars_in_range = 95;
    pr[0].font_size = 20.0f;

    stbtt_PackSetOversampling(&pc, 2, 2);
    stbtt_PackFontRanges(&pc, ttf_buffer, 0, pr, 1);
    stbtt_PackEnd(&pc);

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, BITMAP_W, BITMAP_H, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Text::Text()
    : vbo(0)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

Text::Text(Font& font, float x, float y, const std::string& text)
{
    setText(font, x, y, text);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void Text::setText(Font& font, float x, float y, const std::string& text)
{
    buffer.clear();

    for (int c : text)
    {
        stbtt_aligned_quad q = font.getQuad(c, x, y);

        buffer.push_back(q.s0); buffer.push_back(q.t0);
        buffer.push_back(q.x0); buffer.push_back(q.y0);

        buffer.push_back(q.s1); buffer.push_back(q.t0);
        buffer.push_back(q.x1); buffer.push_back(q.y0);

        buffer.push_back(q.s1); buffer.push_back(q.t1);
        buffer.push_back(q.x1); buffer.push_back(q.y1);

        buffer.push_back(q.s0); buffer.push_back(q.t1);
        buffer.push_back(q.x0); buffer.push_back(q.y1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * buffer.size(),
                 (GLchar*)&buffer[0],
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Text::draw(const Font& font)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font.getTexture());

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 2, nullptr);
    glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, (GLvoid*)(sizeof(float) * 2));
    glDrawArrays(GL_QUADS, 0, buffer.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);
}
