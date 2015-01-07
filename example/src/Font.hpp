#ifndef _Font_hpp_
#define _Font_hpp_

#include "stb_truetype.h"

#include <string>
#include <vector>

class Font
{
    public:
        Font(const std::string& filename);
        ~Font();

        stbtt_aligned_quad getQuad(int& charIndex, float& x, float& y);
        unsigned int getTexture() const;

        void draw(float x, float y, const std::string& text);

    private:
        void loadFont(const std::string& filename);

    private:
        static constexpr unsigned int BITMAP_W = 512;
        static constexpr unsigned int BITMAP_H = 512;

        unsigned int texID;
        stbtt_packedchar pdata[256];
};

class Text
{
    public:
        Text();
        Text(Font& font, float x, float y, const std::string& text);

        void setText(Font& font, float x, float y, const std::string& text);
        void draw(const Font& font);

    private:
        std::vector<float> buffer;
        unsigned int vbo;
};


#endif /* _Font_hpp_ */
