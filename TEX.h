#pragma once

#include <queue>

#include <GL/glew.h>



class TEX
{
	class Less
	{

	public:

		bool operator()(const GLint a, const GLint b) const { return a > b; }

	};

	static std::priority_queue<GLint, std::vector<GLint>, Less> FreeTextureUnitNumbers;
	static bool is_class_loaded;
	static void loadClass();

	bool m_image_is_loaded = false;
	GLuint m_texture = 0;

	mutable bool m_is_bound = false;
	mutable GLint m_textureunitnumber = -1;

public:

	TEX() { }
	~TEX() { Clean(); }

	TEX(const TEX&) = delete;
	TEX& operator=(const TEX&) = delete;

	TEX(TEX&& tex);
	TEX& operator=(TEX&& T);

	operator GLuint() const;

	void loadBMP_custom(const char* const filepath); // TODO: a két függvényt regexpes estszétválasztással összevonni egybe
	void loadDDS(const char* const filepath);

	void bind() const; // TODO: unBind()
	void unBind() const;

	void Clean();

};