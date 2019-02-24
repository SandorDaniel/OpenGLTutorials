#pragma once

#include <queue>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <fstream>
#include <iostream>

// TODO: After making loadBMP_custom and loadDDS(...) functions to be C++-like codes, change the three headers below to the appropriate C++ headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "Aspects.hpp"



struct ColorTexData
{
	static constexpr int internal_format_id = GL_RGB;
	static constexpr int format_id = GL_BGR;
	static constexpr int type_id = GL_UNSIGNED_BYTE;
	static constexpr int attachment_id = GL_COLOR_ATTACHMENT0;
	static constexpr int component_count = 3;
	
	using type = GLubyte;

	static void setTextureSampling()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
};

struct DepthTexData
{
	static constexpr int internal_format_id = GL_DEPTH_COMPONENT16;
	static constexpr int format_id = GL_DEPTH_COMPONENT;
	static constexpr int type_id = GL_FLOAT;
	static constexpr int attachment_id = GL_DEPTH_ATTACHMENT;
	static constexpr int component_count = 1;

	using type = GLfloat;

	static void setTextureSampling()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
};


struct BMPInRAM
{
	std::vector<unsigned char> bytes;
	GLint m_width;
	GLint m_height;

	void load(const char* const filepath); // TODO changing unsigned char to GLbyte or something like this everywhere where it is appropriate
};

struct DDSInRAM
{
	std::vector<unsigned char> bytes;
	GLint m_width;
	GLint m_height;
	unsigned int fourCC;
	unsigned int mipMapCount;

	void load(const char* const filepath);
};


template<class TexType>
class TEX final
{

	class AspFreeTEX final
	{

		static std::priority_queue<GLint, std::vector<GLint>, std::less<typename std::vector<GLint>::value_type>> FreeTextureUnitNumbers;
		static bool is_class_loaded;
		static void loadClass();

	private:

		GLuint m_texture_id;
		mutable GLint m_textureunitnumber = -1;
		GLsizei m_width = 0;
		GLsizei m_height = 0;

	public:

		template<class TexType>
		friend void swap(typename TEX<TexType>::AspFreeTEX& t1, typename TEX<TexType>::AspFreeTEX& t2);

		AspFreeTEX()
		{
			glGenTextures(1, &m_texture_id);

			// Binding is neccesarry because generated names do not initially correspond to an instance of an object,
			// objects with generated names are created by binding a generated name to the context.
			// We need an instance of an object in order to use its name with a DSA function.
			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex); // TODO make casting more safety
			glBindTexture(GL_TEXTURE_2D, m_texture_id);
			glBindTexture(GL_TEXTURE_2D, bound_tex);

			//// Instead of the above we can use this, if our OpenGL version number is at least 4.5:
			//glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
		}
		~AspFreeTEX()
		{
			if (m_textureunitnumber >= 0)
			{
				glBindTextureUnit(GL_TEXTURE0 + m_textureunitnumber, 0);

				FreeTextureUnitNumbers.push(m_textureunitnumber);
				m_textureunitnumber = -1;
			}

			glDeleteTextures(1, &m_texture_id);
		}

		AspFreeTEX(const AspFreeTEX&) = delete;
		AspFreeTEX& operator=(const AspFreeTEX&) = delete;

		AspFreeTEX(AspFreeTEX&& tex) :
			m_texture_id(tex.m_texture_id),
			m_textureunitnumber(tex.m_textureunitnumber)
		{
			tex.m_texture_id = 0;
			tex.m_textureunitnumber = -1;
		}
		AspFreeTEX& operator=(AspFreeTEX&& T)
		{
			TEX::AspFreeTEX temp_tex((std::move(T)));

			swap(*this, temp_tex);

			return *this;
		}

		operator GLint() const
		{
			return m_textureunitnumber;
		}
		operator GLuint() const
		{
			return m_texture_id;
		}

		void loadBMP(const BMPInRAM& IMAGE)
		{
			static_assert(TexType::internal_format_id == TexType::internal_format_id, "Loading BMP file data into depth texture is pointless and so is not allowed.");

			// TODO: make this code to be C++-like code

			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex); // TODO make casting more safety

			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, m_texture_id);

			// Give the image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, 0, TexType::internal_format_id, IMAGE.m_width, IMAGE.m_height, 0, TexType::format_id, TexType::type_id, &IMAGE.bytes[0]);

			// Poor filtering, or ...
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

			// ... nice trilinear filtering ...
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			// ... which requires mipmaps. Generate them automatically.
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0); // TODO: Is this ro neccesarry?

			glBindTexture(GL_TEXTURE_2D, bound_tex);
		}
		void loadDDS(const DDSInRAM& IMAGE) // Load a .DDS file using GLFW's own loader
		{
			static_assert(TexType::internal_format_id == TexType::internal_format_id, "Loading BMP file data into depth texture is pointless and so is not allowed.");

			const int FOURCC_DXT1 = 0x31545844; // Equivalent to "DXT1" in ASCII
			const int FOURCC_DXT3 = 0x33545844; // Equivalent to "DXT3" in ASCII
			const int FOURCC_DXT5 = 0x35545844; // Equivalent to "DXT5" in ASCII

			unsigned int components = (IMAGE.fourCC == FOURCC_DXT1) ? 3 : 4;
			unsigned int format;
			switch (IMAGE.fourCC)
			{
			case FOURCC_DXT1:
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				break;
			case FOURCC_DXT3:
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				break;
			case FOURCC_DXT5:
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				break;
			default:
				return;
			}

			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex); // TODO make casting more safety

			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, m_texture_id);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;

			/* load the mipmaps */
			GLsizei width = IMAGE.m_width;
			GLsizei height = IMAGE.m_height;
			for (unsigned int level = 0; level < IMAGE.mipMapCount && (width || height); ++level)
			{
				unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
				glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
					0, size, &IMAGE.bytes[offset]);

				offset += size;
				width /= 2;
				height /= 2;

				// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
				if (width < 1) width = 1;
				if (height < 1) height = 1;
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			glBindTexture(GL_TEXTURE_2D, bound_tex);
		}
		void alloc(const GLsizei TEXT_WIDTH, const GLsizei TEXT_HEIGHT)
		{
			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex); // TODO make casting more safety

			glBindTexture(GL_TEXTURE_2D, m_texture_id);

			m_width = TEXT_WIDTH;
			m_height = TEXT_HEIGHT;

			TexType::setTextureSampling();
			glTexImage2D(GL_TEXTURE_2D, 0, TexType::internal_format_id, TEXT_WIDTH, TEXT_HEIGHT, 0, TexType::format_id, TexType::type_id, nullptr); // NULL - we are only allocating memory and not filling it
			glBindTexture(GL_TEXTURE_2D, 0);

			glBindTexture(GL_TEXTURE_2D, bound_tex);
		}

		explicit operator std::vector<typename TexType::type>() const
		{
			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex);
			glBindTexture(GL_TEXTURE_2D, m_texture_id);

			TexType::type* const P_texture_data = new TexType::type[m_width * m_height * TexType::component_count];
			glGetTexImage(GL_TEXTURE_2D, 0, TexType::format_id, TexType::type_id, P_texture_data);

			glBindTexture(GL_TEXTURE_2D, 0); // TODO: Is this row neccesarry?
			glBindTexture(GL_TEXTURE_2D, bound_tex);

			return std::vector<TexType::type>(P_texture_data, P_texture_data + m_width * m_height * TexType::component_count);
		}

		GLsizei getWidth() const
		{
			return m_width;
		}
		GLsizei getHeight() const
		{
			return m_height;
		}
		int getComponentCount() const
		{
			return TexType::component_count;
		}

		void bind() const
		{
			if (!is_class_loaded)
			{
				loadClass();
			}

			if (!FreeTextureUnitNumbers.empty())
			{
				m_textureunitnumber = FreeTextureUnitNumbers.top();
				FreeTextureUnitNumbers.pop();
			}
			else
			{
				throw; // TODO
			}

			// Bind our texture in Texture Unit TextureUnitNumber
			glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
			glBindTexture(GL_TEXTURE_2D, m_texture_id);
		}
		void unBind() const
		{
			glActiveTexture(GL_TEXTURE0 + m_textureunitnumber);
			glBindTexture(GL_TEXTURE_2D, 0);

			FreeTextureUnitNumbers.push(m_textureunitnumber);
			m_textureunitnumber = -1;
		}

	};

	template<class TexType>
	friend void swap(typename TEX<TexType>::AspFreeTEX& t1, typename TEX<TexType>::AspFreeTEX& t2);

private:

	mutable TwoStatesManager m_loading;
	mutable TwoStatesManager m_binding;

	AspFreeTEX m_tex;

public:

	template<class TexType>
	friend void swap(TEX<TexType>& t1, TEX<TexType>& t2);

	TEX() = default;
	~TEX() = default;

	TEX(const TEX&) = delete;
	TEX& operator=(const TEX&) = delete;

	TEX(TEX&& xtex) :
		m_loading(xtex.m_loading),
		m_binding(xtex.m_binding),
		m_tex(std::move(xtex.m_tex))
	{
	}
	TEX& operator=(TEX&& T)
	{
		TEX temp_xtex((std::move(xtex)));

		swap(*this, temp_xtex);

		return *this;
	}

	operator GLint() const
	{
		return (m_binding.checkOn(static_cast<std::function<GLint(const AspFreeTEX&)>>(&AspFreeTEX::operator GLint)))(m_tex);
	} // returns the number of texture channel wich it is bound to
	operator GLuint() const
	{
		return m_tex;
	} // returns the number of texture channel wich it is bound to

	void loadBMP(const BMPInRAM& IMAGE) // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
	{
		return (m_loading.turnOn(static_cast<std::function<void(AspFreeTEX&, const BMPInRAM&)>>(&AspFreeTEX::loadBMP)))(m_tex, IMAGE);
	}
	void loadDDS(const DDSInRAM& IMAGE)
	{
		return (m_loading.turnOn(static_cast<std::function<void(AspFreeTEX&, const DDSInRAM&)>>(&AspFreeTEX::loadDDS)))(m_tex, IMAGE);
	}
	void alloc(const GLsizei TEXT_WIDTH, const GLsizei TEXT_HEIGHT)
	{
		return (m_loading.turnOn(static_cast<std::function<void(AspFreeTEX&, const GLsizei, const GLsizei)>>(&AspFreeTEX::alloc)))(m_tex, TEXT_WIDTH, TEXT_HEIGHT);
	}

	explicit operator std::vector<typename TexType::type>() const
	{
		return (m_loading.checkOn(static_cast<std::function<std::vector<typename TexType::type>(const AspFreeTEX&)>>(&AspFreeTEX::operator std::vector<typename TexType::type>)))(m_tex);
	}

	GLsizei getWidth() const
	{
		return m_tex.getWidth();
	}
	GLsizei getHeight() const
	{
		return m_tex.getHeight();
	}
	int getComponentCount() const
	{
		return m_tex.getComponentCount();
	}

	void bind() const
	{
		auto func = m_binding.turnOn(static_cast<std::function<void(const AspFreeTEX&)>>(&AspFreeTEX::bind));
		return (m_loading.checkOn(func))(m_tex);
	}
	void unBind() const
	{
		std::function<void(const AspFreeTEX&)> func = m_binding.turnOff(static_cast<std::function<void(const AspFreeTEX&)>>(&AspFreeTEX::unBind));
		return (m_binding.checkOn(func))(m_tex);
	}

};


template<class TexType>
void printImage(
	const std::string& PPM_FILE_NAME_WITH_EXTENSION,
	const std::vector<typename TexType::type>& TEXTURE_DATA,
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
		for (int j = 0; j < TexType::component_count * TEXT_WIDTH; ++j)
		{
			if (TEXTURE_DATA[i * TexType::component_count * TEXT_WIDTH + j] < 10)
			{
				out << ' ';
			}

			if (TEXTURE_DATA[i * TexType::component_count * TEXT_WIDTH + j] < 100)
			{
				out << ' ';
			}

			out << (TexType::type_id == GL_UNSIGNED_BYTE ? static_cast<int>(TEXTURE_DATA[i * TexType::component_count * TEXT_WIDTH + j]) : TEXTURE_DATA[i * TexType::component_count * TEXT_WIDTH + j]) << ' ';
		}
		out << std::endl;
	}

	out.close();
}

template<class TexType>
void swap(typename TEX<TexType>::AspFreeTEX& t1, typename TEX<TexType>::AspFreeTEX& t2)
{
	using std::swap;

	swap(t1.m_texture_id, t2.m_texture_id);
	swap(t1.m_textureunitnumber, t2.m_textureunitnumber);
}

template<class TexType>
void swap(TEX<TexType>& t1, TEX<TexType>& t2)
{
	using std::swap;

	swap(t1.m_loading, t2.m_loading);
	swap(t1.m_binding, t2.m_binding);
	swap(t1.m_tex, t2.m_tex);
}


//
// Outline definitions
//


template<class TexType>
std::priority_queue<GLint, std::vector<GLint>> TEX<TexType>::AspFreeTEX::FreeTextureUnitNumbers{};


template<class TexType>
bool TEX<TexType>::AspFreeTEX::is_class_loaded = false;


template<class TexType>
void TEX<TexType>::AspFreeTEX::loadClass()
{
	GLint MaxTextureUnitNumber;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextureUnitNumber); // A total number of textures that can be used, period (GL_MAX_TEXTURE_IMAGE_UNITS - the number of textures that can be accessed by the fragment shader)
	for (GLint i = 0; i < MaxTextureUnitNumber; ++i)
	{
		FreeTextureUnitNumbers.push(i);
	}

	is_class_loaded = true;
}


