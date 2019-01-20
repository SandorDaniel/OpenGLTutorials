//#define sd_debugger
#include "Debug.h"

#include <queue>

#include <GL/glew.h>

#include <common/texture.hpp>

#include "TEX.h"



std::priority_queue<GLint, std::vector<GLint>, TEX::Less> TEX::FreeTextureUnitNumbers{};


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


TEX::TEX(TEX&& tex) : m_image_is_loaded(tex.m_image_is_loaded), m_texture(tex.m_texture), m_is_bound(tex.m_is_bound), m_textureunitnumber(tex.m_textureunitnumber)
{
	tex.m_image_is_loaded = false;
	tex.m_is_bound = false;
}


TEX& TEX::operator=(TEX&& T)
{
	if (this == &T)
	{
		return *this;
	}

	Clean();

	m_image_is_loaded = T.m_image_is_loaded;
	m_texture = T.m_texture;
	m_is_bound = T.m_is_bound;
	m_textureunitnumber = T.m_textureunitnumber;

	T.m_image_is_loaded = false;
	T.m_is_bound = false;

	return *this;
}


void TEX::loadBMP_custom(const char* const filepath) // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
{
	m_texture = ::loadBMP_custom(filepath);
	m_image_is_loaded = true;
}


void TEX::loadDDS(const char* const filepath)
{
	m_texture = ::loadDDS(filepath);
	m_image_is_loaded = true;
}


void TEX::bind() const
{
	if (!m_image_is_loaded)
	{
		throw; // TODO...
	}

	if (!is_class_loaded)
	{
		loadClass();
	}

	m_textureunitnumber = FreeTextureUnitNumbers.top();
	FreeTextureUnitNumbers.pop();

	// Bind our texture in Texture Unit TextureUnitNumber
	glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	m_is_bound = true;
}


void TEX::unBind() const
{
	if (m_is_bound)
	{
		glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
		glBindTexture(GL_TEXTURE_2D, 0);

		FreeTextureUnitNumbers.push(m_textureunitnumber);
		m_textureunitnumber = -1;

		m_is_bound = false;
	}
}


void TEX::Clean()
{
	if (m_is_bound)
	{
		unBind();
	}

	if (m_image_is_loaded)
	{
		glDeleteTextures(1, &m_texture);
		m_image_is_loaded = false;
	}
}


TEX::operator GLuint() const
{
	if (!m_is_bound)
	{
		throw;
	}

	return m_textureunitnumber;
}
