#pragma once

#include <queue>
#include <algorithm>

#include <GL/glew.h>



//
// TODO: integrate GLuint loadBMP_custom(const char * imagepath, GLuint textureID); and GLuint loadDDS(const char * imagepath, GLuint textureID); into the class below.
//

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath, GLuint textureID);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath, GLuint textureID); 



class TEX
{
	
	static std::priority_queue<GLint, std::vector<GLint>, std::less<typename std::vector<GLint>::value_type>> FreeTextureUnitNumbers;
	static bool is_class_loaded;
	static void loadClass();

private:

	GLuint m_texture_id = 0;
	mutable GLint m_textureunitnumber = -1;

public:

	friend void swap(TEX& t1, TEX& t2);

	TEX() = default;
	~TEX();

	TEX(const TEX&) = delete;
	TEX& operator=(const TEX&) = delete;

	TEX(TEX&& tex);
	TEX& operator=(TEX&& T);

	operator GLuint() const
	{
		return m_textureunitnumber;
	}

	void loadBMP_custom(const char* const filepath) // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
	{
		::loadBMP_custom(filepath, m_texture_id);
	}
	void loadDDS(const char* const filepath)
	{
	}
	void unLoad()
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
		::loadDDS(filepath, m_texture_id);
	}

	void bind() const;
	void unBind() const;

};
