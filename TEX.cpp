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

	swap(t1.m_image_is_loaded, t2.m_image_is_loaded);
	swap(t1.m_texture, t2.m_texture);
	swap(t1.m_textureunitnumber, t2.m_textureunitnumber);
}


TEX::TEX(TEX&& tex) : 
	m_image_is_loaded(tex.m_image_is_loaded),
	m_texture(tex.m_texture),
	m_textureunitnumber(tex.m_textureunitnumber)
{
	tex.m_image_is_loaded = false;
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
}


void TEX::unBind() const
{
	glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeTextureUnitNumbers.push(m_textureunitnumber);
	m_textureunitnumber = -1;
}


void TEX::Clean()
{
	if (m_image_is_loaded)
	{
		glDeleteTextures(1, &m_texture);
		m_image_is_loaded = false;
	}
}


TEX::operator GLuint() const
{
	return m_textureunitnumber;
}
