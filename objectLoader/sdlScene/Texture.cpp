#include "Texture.h"
#include <iostream>

vector<Texture*>Texture::textures;

Texture::Texture(string in_filename, string in_name)
{
    imageData = NULL;

    std::cout << "loading tga"<<  loadTGA(in_filename) << std::endl;;

    name = in_name;

    textures.push_back(this);
}

Texture::~Texture()
{
    for(vector<Texture *>::iterator it = textures.begin(); it < textures.end(); it++)
        if( (*it) == this)
            textures.erase(it);

    if(imageData)
        delete imageData;
}

bool Texture::loadTGA(string filename)
{
    TGA_Header TGAheader;

    ifstream file(filename.data(), std::ios_base::binary);

    if(!file.is_open()){
        std::cout << "file "<< filename.data() << "is not open"<< std::endl;
        return false;
    }
    else
    {
        std::cout << "file " << filename.data() << " is open " << std::endl;
    }

    if(!file.read((char *)&TGAheader, sizeof(TGAheader))){
        std::cout << "file " << filename.data()<<" is not readed" << std::endl;
        return false;
    }

    if(TGAheader.ImageType != 2){
        std::cout << "type is not 2" << std::endl;
        return false;
    }

    width = TGAheader.ImageWidth;
    height = TGAheader.ImageHeight;
    bpp = TGAheader.PixelDepth;

    if((width <= 0) || 
            height <= 0 || 
            (bpp != 24 && bpp != 32))
    {
        return false;
    }

    GLuint type = GL_RGBA;
    
    if(bpp == 24)
        type = GL_RGB;
    
    GLuint bytesPerPixel = bpp / 8;
    GLuint imageSize = width * height  * bytesPerPixel;
    
    imageData = new GLubyte[imageSize];

    if(imageData == NULL)
        return false;

    if(!file.read((char *)imageData, imageSize))
    {
        delete imageData;

        return false;
    }

    // RGB --> RGB
    for ( GLuint i = 0; i < (int)imageSize; i += bytesPerPixel)
    {
        GLuint temp = imageData[i];
        imageData[i] = imageData[i + 2];
        imageData[i + 2] = temp;
    }

    createTexture(imageData, width,height, type);

    // Everything worked!!!
    return true;
}

bool Texture::createTexture(unsigned char *imageData, int width, int height, int type)
{
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

    return true;
}
