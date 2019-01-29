//#define sd_debugger
#include "Debug.h"

#include <queue>
#include <utility>

#include <GL/glew.h>

#include "TEX.h"



std::priority_queue<GLint, std::vector<GLint>> TEX::AspFreeTEX::FreeTextureUnitNumbers{};


bool TEX::AspFreeTEX::is_class_loaded = false;


void TEX::AspFreeTEX::loadClass()
{
	GLint MaxTextureUnitNumber;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextureUnitNumber); // A total number of textures that can be used, period (GL_MAX_TEXTURE_IMAGE_UNITS - the number of textures that can be accessed by the fragment shader)
	for (GLint i = 0; i < MaxTextureUnitNumber; ++i)
	{
		FreeTextureUnitNumbers.push(i);
	}

	is_class_loaded = true;
}


void swap(TEX::AspFreeTEX& t1, TEX::AspFreeTEX& t2)
{
	using std::swap;

	swap(t1.m_texture_id, t2.m_texture_id);
	swap(t1.m_textureunitnumber, t2.m_textureunitnumber);
}


TEX::AspFreeTEX::~AspFreeTEX()
{
	if (m_textureunitnumber >= 0)
	{
		glBindTextureUnit(GL_TEXTURE0 + m_textureunitnumber, 0);

		FreeTextureUnitNumbers.push(m_textureunitnumber);
		m_textureunitnumber = -1;
	}

	glDeleteTextures(1, &m_texture_id);
}


TEX::AspFreeTEX::AspFreeTEX()
{
	glGenTextures(1, &m_texture_id);

	// Binding is neccesarry because generated names do not initially correspond to an instance of an object,
	// objects with generated names are created by binding a generated name to the context.
	// We need an instance of an object in order to use its name with a DSA function.
	GLuint bound_tex; // We want to live every state to be the same...
	glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&bound_tex)); // TODO make casting more safety
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glBindTexture(GL_TEXTURE_2D, bound_tex);

	//// Instead of the above we can use this, if our OpenGL version number is at least 4.5:
	//glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
}


TEX::AspFreeTEX::AspFreeTEX(TEX::AspFreeTEX&& tex) :
	m_texture_id(tex.m_texture_id),
	m_textureunitnumber(tex.m_textureunitnumber)
{
	tex.m_texture_id = 0;
	tex.m_textureunitnumber = -1;
}


TEX::AspFreeTEX& TEX::AspFreeTEX::operator=(TEX::AspFreeTEX&& T)
{
	TEX::AspFreeTEX temp_tex((std::move(T)));

	swap(*this, temp_tex);

	return *this;
}


void TEX::AspFreeTEX::bind() const
{
	if (!is_class_loaded)
	{
		loadClass();
	}

	if (!FreeTextureUnitNumbers.empty())
	{
		m_textureunitnumber = FreeTextureUnitNumbers.top();
		FreeTextureUnitNumbers.pop();
	}
	else
	{
		throw; // TODO
	}

	// Bind our texture in Texture Unit TextureUnitNumber
	glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
}


void TEX::AspFreeTEX::unBind() const
{
	glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeTextureUnitNumbers.push(m_textureunitnumber);
	m_textureunitnumber = -1;
}


void swap(TEX& t1, TEX& t2)
{
	using std::swap;

	swap(t1.m_loading, t2.m_loading);
	swap(t1.m_binding, t2.m_binding);
	swap(t1.m_tex, t2.m_tex);
}


TEX& TEX::operator=(TEX&& xtex)
{
	TEX temp_xtex((std::move(xtex)));

	swap(*this, temp_xtex);

	return *this;
}



//
// TODO: make the code below to be a c++-like code, and integrate it to the class above
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>


GLuint loadBMP_custom(const char * imagepath, GLuint textureID) {

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
		getchar();
		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    fclose(file); return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    fclose(file); return 0; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);

	GLuint bound_tex; // We want to live every state to be the same...
	glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&bound_tex)); // TODO make casting more safety

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, bound_tex);
}

// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
// or do it yourself (just like loadBMP_custom and loadDDS)
//GLuint loadTGA_glfw(const char * imagepath){
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// Read the file, call glTexImage2D with the right parameters
//	glfwLoadTexture2D(imagepath, 0);
//
//	// Nice trilinear filtering.
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Return the ID of the texture we just created
//	return textureID;
//}



#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath, GLuint textureID) {

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	GLuint bound_tex; // We want to live every state to be the same...
	glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&bound_tex)); // TODO make casting more safety

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, bound_tex);
}