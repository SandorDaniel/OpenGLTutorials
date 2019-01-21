#pragma once

#include <queue>

#include <GL/glew.h>



class TEX
{
	
	static std::priority_queue<GLint, std::vector<GLint>, std::less<typename std::vector<GLint>::value_type>> FreeTextureUnitNumbers;
	static bool is_class_loaded;
	static void loadClass();

	bool m_image_is_loaded = false;
	GLuint m_texture = 0;

	mutable bool m_is_bound = false;
	mutable GLint m_textureunitnumber = -1;

public:

	TEX() = default;
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





class X_TEX
{
	StrictOGLObjectBindingManager m_soobm;
	TEX m_tex;

public:

	X_TEX() = default;
	~X_TEX() = default;

	X_TEX(const X_TEX&) = delete;
	X_TEX& operator=(const X_TEX&) = delete;

	X_TEX(X_TEX&& tex);
	X_TEX& operator=(X_TEX&& T);

	operator GLuint() const;

	void loadBMP_custom(const char* const filepath); // TODO: a kï¿½t fï¿½ggvï¿½nyt regexpes estszï¿½tvï¿½lasztï¿½ssal ï¿½sszevonni egybe
	void loadDDS(const char* const filepath);

	void bind() const; // TODO: unBind()
	void unBind() const;

	void Clean();

};

