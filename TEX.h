#pragma once

#include <queue>
#include <algorithm>
#include <vector>

// TODO: After making loadBMP_custom and loadDDS(...) functions to be C++-like codes, change the three headers below to the appropriate C++ headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "Aspects.hpp"



enum class TexType
{
	COLOR,
	DEPTH
};

int getFormat(TexType type);
int getAttachment(TexType type);
int getComponentNumber(TexType type);


template<TexType type>
class TEX
{

	class AspFreeTEX
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

		template<TexType type>
		friend void swap(typename TEX<type>::AspFreeTEX& t1, typename TEX<type>::AspFreeTEX& t2);

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

		// TODO: separation: mass storage -> RAM -> VRAM
		void loadBMP_custom(const char* const filepath) // Load a .BMP file using our custom loader // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
		{
			// TODO: make this code to be C++-like code

			printf("Reading image %s\n", filepath);

			// Data read from the header of the BMP file
			unsigned char header[54];
			unsigned int dataPos;
			unsigned int imageSize;
			// Actual RGB data
			unsigned char * data;

			// Open the file
			FILE * file = fopen(filepath, "rb");
			if (!file) {
				printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filepath);
				getchar();
				return;
			}

			// Read the header, i.e. the 54 first bytes

			// If less than 54 bytes are read, problem
			if (fread(header, 1, 54, file) != 54) {
				printf("Not a correct BMP file\n");
				fclose(file);
				return;
			}
			// A BMP files always begins with "BM"
			if (header[0] != 'B' || header[1] != 'M') {
				printf("Not a correct BMP file\n");
				fclose(file);
				return;
			}
			// Make sure this is a 24bpp file
			if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    fclose(file); return; }
			if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    fclose(file); return; }

			// Read the information about the image
			dataPos = *(int*)&(header[0x0A]);
			imageSize = *(int*)&(header[0x22]);
			m_width = *(int*)&(header[0x12]);
			m_height = *(int*)&(header[0x16]);

			// Some BMP files are misformatted, guess missing information
			if (imageSize == 0)    imageSize = m_width * m_height * 3; // 3 : one byte for each Red, Green and Blue component
			if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

			// Create a buffer
			data = new unsigned char[imageSize];

			// Read the actual data from the file into the buffer
			fread(data, 1, imageSize, file);

			// Everything is in memory now, the file can be closed.
			fclose(file);

			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex); // TODO make casting more safety

			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, m_texture_id);

			// Give the image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

			// OpenGL has now copied the data. Free our own version
			delete[] data;

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
		void loadDDS(const char* const filepath) // Load a .DDS file using GLFW's own loader
		{
			// TODO: make this code to be C++-like code

			const int FOURCC_DXT1 = 0x31545844; // Equivalent to "DXT1" in ASCII
			const int FOURCC_DXT3 = 0x33545844; // Equivalent to "DXT3" in ASCII
			const int FOURCC_DXT5 = 0x35545844; // Equivalent to "DXT5" in ASCII

			unsigned char header[124];

			FILE *fp;

			/* try to open the file */
			fp = fopen(filepath, "rb");
			if (fp == NULL) {
				printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filepath); getchar();
				return;
			}

			/* verify the type of file */
			char filecode[4];
			fread(filecode, 1, 4, fp);
			if (strncmp(filecode, "DDS ", 4) != 0) {
				fclose(fp);
				return;
			}

			/* get the surface desc */
			fread(&header, 124, 1, fp);

			m_height = *(unsigned int*)&(header[8]);
			m_width = *(unsigned int*)&(header[12]);
			unsigned int linearSize = *(unsigned int*)&(header[16]);
			unsigned int mipMapCount = *(unsigned int*)&(header[24]);
			unsigned int fourCC = *(unsigned int*)&(header[80]);


			unsigned char* buffer;
			unsigned int bufsize;
			/* how big is it going to be including all mipmaps? */
			bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
			buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
			fread(buffer, 1, bufsize, fp);
			/* close the file pointer */
			fclose(fp);

			unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
			unsigned int format;
			switch (fourCC)
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
				free(buffer);
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
			GLsizei width = m_width;
			GLsizei height = m_height;
			for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
			{
				unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
				glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
					0, size, buffer + offset);

				offset += size;
				width /= 2;
				height /= 2;

				// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
				if (width < 1) width = 1;
				if (height < 1) height = 1;

			}

			free(buffer);

			glBindTexture(GL_TEXTURE_2D, 0);

			glBindTexture(GL_TEXTURE_2D, bound_tex);
		}

		explicit operator std::vector<unsigned char>() const
		{
			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex);
			glBindTexture(GL_TEXTURE_2D, m_texture_id);

			unsigned char* const P_texture_data = new unsigned char[m_width * m_height * getComponentNumber(type)];
			glGetTexImage(GL_TEXTURE_2D, 0, getFormat(type), GL_UNSIGNED_BYTE, P_texture_data);

			glBindTexture(GL_TEXTURE_2D, 0); // TODO: Is this row neccesarry?
			glBindTexture(GL_TEXTURE_2D, bound_tex);

			return std::vector<unsigned char>(P_texture_data, P_texture_data + m_width * m_height * getComponentNumber(type));
		}

		GLsizei getWidth() const
		{
			return m_width;
		}
		GLsizei getHeight() const
		{
			return m_height;
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

	template<TexType type>
	friend void swap(typename TEX<type>::AspFreeTEX& t1, typename TEX<type>::AspFreeTEX& t2);

private:

	mutable TwoStatesManager m_loading;
	mutable TwoStatesManager m_binding;

	AspFreeTEX m_tex;

public:

	template<TexType type>
	friend void swap(TEX<type>& t1, TEX<type>& t2);

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

	void loadBMP_custom(const char* const filepath) // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
	{
		return (m_loading.turnOn(static_cast<std::function<void(AspFreeTEX&, const char* const)>>(&AspFreeTEX::loadBMP_custom)))(m_tex, filepath);
	}
	void loadDDS(const char* const filepath)
	{
		return (m_loading.turnOn(static_cast<std::function<void(AspFreeTEX&, const char* const)>>(&AspFreeTEX::loadDDS)))(m_tex, filepath);
	}

	explicit operator std::vector<unsigned char>() const
	{
		return (m_loading.checkOn(static_cast<std::function<std::vector<unsigned char>(const AspFreeTEX&)>>(&AspFreeTEX::operator std::vector<unsigned char>)))(m_tex);
	}

	GLsizei getWidth() const
	{
		return m_tex.getWidth();
	}
	GLsizei getHeight() const
	{
		return m_tex.getHeight();
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


void printImage(
	const std::string& PPM_FILE_NAME_WITH_EXTENSION,
	const std::vector<unsigned char>& TEXTURE_DATA,
	const GLsizei TEXT_WIDTH,
	const GLsizei TEXT_HEIGHT);


template<TexType type>
std::priority_queue<GLint, std::vector<GLint>> TEX<type>::AspFreeTEX::FreeTextureUnitNumbers{};


template<TexType type>
bool TEX<type>::AspFreeTEX::is_class_loaded = false;


template<TexType type>
void TEX<type>::AspFreeTEX::loadClass()
{
	GLint MaxTextureUnitNumber;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextureUnitNumber); // A total number of textures that can be used, period (GL_MAX_TEXTURE_IMAGE_UNITS - the number of textures that can be accessed by the fragment shader)
	for (GLint i = 0; i < MaxTextureUnitNumber; ++i)
	{
		FreeTextureUnitNumbers.push(i);
	}

	is_class_loaded = true;
}


template<TexType type>
void swap(typename TEX<type>::AspFreeTEX& t1, typename TEX<type>::AspFreeTEX& t2)
{
	using std::swap;

	swap(t1.m_texture_id, t2.m_texture_id);
	swap(t1.m_textureunitnumber, t2.m_textureunitnumber);
}


template<TexType type>
void swap(TEX<type>& t1, TEX<type>& t2)
{
	using std::swap;

	swap(t1.m_loading, t2.m_loading);
	swap(t1.m_binding, t2.m_binding);
	swap(t1.m_tex, t2.m_tex);
}


