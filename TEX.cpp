//#define sd_debugger
#include "Debug.h"

#include <queue>

#include <GL/glew.h>

#include <common/texture.hpp>

#include "TEX.h"



std::priority_queue<GLint, std::vector<GLint>> TEX::FreeTextureUnitNumbers{};


bool TEX::is_class_loaded = false;


void TEX::loadClass()
{
	GLint MaxTextureUnitNumber;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextureUnitNumber); // A total number of textures that can be used, period (GL_MAX_TEXTURE_IMAGE_UNITS - the number of textures that can be accessed by the fragment shader)
	for (GLint i = 0; i < MaxTextureUnitNumber; ++i)
	{
		FreeTextureUnitNumbers.push(i);
	}

	is_class_loaded = true;
}


void swap(TEX& t1, TEX& t2)
{
	using std::swap;

	swap(t1.m_texture, t2.m_texture);
	swap(t1.m_textureunitnumber, t2.m_textureunitnumber);
}


TEX::~TEX()
{
	if (m_textureunitnumber >= 0)
	{
		unBind();
	}

	unLoad();
}


TEX::TEX(TEX&& tex) :
	m_texture(tex.m_texture),
	m_textureunitnumber(tex.m_textureunitnumber)
{
	tex.m_texture = 0;
	tex.m_textureunitnumber = -1;
}


TEX& TEX::operator=(TEX&& T)
{
	TEX temp_tex((std::move(T)));

	swap(*this, temp_tex);

	return *this;
}


void TEX::loadBMP_custom(const char* const filepath) // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
{
	m_texture = ::loadBMP_custom(filepath);
}


void TEX::loadDDS(const char* const filepath)
{
	m_texture = ::loadDDS(filepath);
}


void TEX::unLoad()
{
	glDeleteTextures(1, &m_texture);
}


void TEX::bind() const
{
	if (!is_class_loaded)
	{
		loadClass();
	}

	m_textureunitnumber = FreeTextureUnitNumbers.top();
	FreeTextureUnitNumbers.pop();

	// Bind our texture in Texture Unit TextureUnitNumber
	glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}


void TEX::unBind() const
{
	glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeTextureUnitNumbers.push(m_textureunitnumber);
	m_textureunitnumber = -1;
}


TEX::operator GLuint() const
{
	return m_textureunitnumber;
}
