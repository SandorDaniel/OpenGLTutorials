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


int getComponentNumber(TexType type)
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






void printImage(
	const std::string& PPM_FILE_NAME_WITH_EXTENSION,
	const std::vector<unsigned char>& TEXTURE_DATA,
	const GLsizei TEXT_WIDTH,
	const GLsizei TEXT_HEIGHT)
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
		for (int j = 0; j < 3 * TEXT_WIDTH; ++j)
		{
			if (TEXTURE_DATA[i * 3 * TEXT_WIDTH + j] < 10)
			{
				out << ' ';
			}

			if (TEXTURE_DATA[i * 3 * TEXT_WIDTH + j] < 100)
			{
				out << ' ';
			}

			out << (int)(TEXTURE_DATA[i * 3 * TEXT_WIDTH + j]) << ' ';
		}
		out << std::endl;
	}

	out.close();
}
