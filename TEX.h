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

	bool m_image_is_loaded = false;
	GLuint m_texture = 0;

	mutable GLint m_textureunitnumber = -1;

public:

	friend void swap(TEX& t1, TEX& t2);

	TEX() = default;
	~TEX() { Clean(); }

	TEX(const TEX&) = delete;
	TEX& operator=(const TEX&) = delete;

	TEX(TEX&& tex);
	TEX& operator=(TEX&& T);

	operator GLuint() const;

	void loadBMP_custom(const char* const filepath); // TODO: a két függvényt regexpes estszétválasztással összevonni egybe
	void loadDDS(const char* const filepath);
	void unLoad();

	void bind() const; // TODO: unBind()
	void unBind() const;

	void Clean();
};
