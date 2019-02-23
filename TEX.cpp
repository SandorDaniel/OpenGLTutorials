//#define sd_debugger
#include "Debug.h"

#include <queue>
#include <utility>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

#include "TEX.h"



int getFormat(TexType type)
{
	switch (type)
	{
	case TexType::COLOR:
		return GL_RGB;
		break;
	case TexType::DEPTH:
		return GL_DEPTH_COMPONENT;
		break;
	}
}


int getAttachment(TexType type)
{
	switch (type)
	{
	case TexType::COLOR:
		return GL_COLOR_ATTACHMENT0;
		break;
	case TexType::DEPTH:
		return GL_DEPTH_ATTACHMENT;
		break;
	}
}


int getComponentCount(TexType type)
{
	switch (type)
	{
	case TexType::COLOR:
		return 3;
		break;
	case TexType::DEPTH:
		return 1;
		break;
	}
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



void BMPInRAM::load(const char* const filepath) // Load a .BMP file using our custom loader // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
{
	// TODO: make this code to be C++-like code

	printf("Reading image %s\n", filepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(filepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filepath);
		getchar();
		return;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    fclose(file); return; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    fclose(file); return; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	m_width = *(int*)&(header[0x12]);
	m_height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = m_width * m_height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);

	bytes.clear();
	bytes.insert(bytes.begin(), data, data + imageSize);
	
	delete[] data;
}


void DDSInRAM::load(const char* const filepath)
{
	// TODO: make this code to be C++-like code

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filepath); getchar();
		return;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	m_height = *(unsigned int*)&(header[8]);
	m_width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	mipMapCount = *(unsigned int*)&(header[24]);
	fourCC = *(unsigned int*)&(header[80]);


	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	bytes.clear();
	bytes.insert(bytes.begin(), buffer, buffer + bufsize);

	free(buffer);
}


void printImage(
	const std::string& PPM_FILE_NAME_WITH_EXTENSION,
	const std::vector<unsigned char>& TEXTURE_DATA,
	const GLsizei TEXT_WIDTH,
	const GLsizei TEXT_HEIGHT,
	const int COMPONENT_COUNT)
{
	// http://netpbm.sourceforge.net/doc/ppm.html

	std::ofstream out;
	out.open(PPM_FILE_NAME_WITH_EXTENSION);
	if (!out) {
		std::cerr << "Cannot open file.";
		exit(-1);
	}

	out << "P3" << std::endl;

	out << TEXT_WIDTH << " " << TEXT_HEIGHT << std::endl;

	out << 255 << std::endl;

	for (int i = TEXT_HEIGHT - 1; i >= 0; --i)
	{
		for (int j = 0; j < COMPONENT_COUNT * TEXT_WIDTH; ++j)
		{
			if (TEXTURE_DATA[i * COMPONENT_COUNT * TEXT_WIDTH + j] < 10)
			{
				out << ' ';
			}

			if (TEXTURE_DATA[i * COMPONENT_COUNT * TEXT_WIDTH + j] < 100)
			{
				out << ' ';
			}

			out << (int)(TEXTURE_DATA[i * COMPONENT_COUNT * TEXT_WIDTH + j]) << ' ';
		}
		out << std::endl;
	}

	out.close();
}
