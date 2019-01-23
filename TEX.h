#pragma once

#include <queue>
#include <algorithm>

#include <GL/glew.h>



class TEX
{
	
	static std::priority_queue<GLint, std::vector<GLint>, std::less<typename std::vector<GLint>::value_type>> FreeTextureUnitNumbers;
	static bool is_class_loaded;
	static void loadClass();

private:

	GLuint m_texture = 0;
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

	void loadBMP_custom(const char* const filepath); // TODO: a két függvényt regexpes estszétválasztással összevonni egybe
	void loadDDS(const char* const filepath);
	void unLoad()
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}

	void bind() const;
	void unBind() const;

};
