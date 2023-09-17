Texture LoadTexture2D(const char *path, float anisotropy, bool gamma)
{
    Texture texture;
    glGenTextures(1, &texture.ID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internal_format;
        GLenum data_format;

        if (nrComponents == 1)
            internal_format = data_format = GL_RED;
        else if (nrComponents == 3)
        {
            internal_format = gamma ? GL_SRGB : GL_RGB;
            data_format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internal_format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
            data_format = GL_RGBA;
        }
        texture.width = width;
        texture.height = height;
        glBindTexture(GL_TEXTURE_2D, texture.ID);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (anisotropy != 0)
        {
            GLfloat value, max_anisotropy = anisotropy;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &value);
            value = (value > max_anisotropy) ? max_anisotropy : value;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, value);
        }
    }
    else
        printf("failed loading texture\n");
    stbi_image_free(data);
    return texture;
}

SDL_Surface *LoadSDLImage(char* filename)
{
	// Read data
	int32_t width, height, bytesPerPixel;
	void* data = stbi_load(filename, &width, &height, &bytesPerPixel, 0);

	// Calculate pitch
	int pitch;
    pitch = width * bytesPerPixel;
    pitch = (pitch + 3) & ~3;

    // Setup relevance bitmask
	int32_t Rmask, Gmask, Bmask, Amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    Rmask = 0x000000FF;
    Gmask = 0x0000FF00;
    Bmask = 0x00FF0000;
    Amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
#else
    int s = (bytesPerPixel == 4) ? 0 : 8;
    Rmask = 0xFF000000 >> s;
    Gmask = 0x00FF0000 >> s;
    Bmask = 0x0000FF00 >> s;
    Amask = 0x000000FF >> s;
#endif
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, width, height, bytesPerPixel*8, pitch, Rmask, Gmask,
                             Bmask, Amask);
	if (!surface)
	{
		// NOTE: Should free stbi_load 'data' variable here
		return NULL;
	}
	return surface;
}