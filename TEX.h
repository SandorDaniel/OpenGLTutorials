#pragma once

#include <queue>
#include <algorithm>
#include <vector>

#include <GL/glew.h>

#include "Aspects.hpp"



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

		friend void swap(TEX::AspFreeTEX& t1, TEX::AspFreeTEX& t2);

		AspFreeTEX();
		~AspFreeTEX();

		AspFreeTEX(const AspFreeTEX&) = delete;
		AspFreeTEX& operator=(const AspFreeTEX&) = delete;

		AspFreeTEX(AspFreeTEX&& tex);
		AspFreeTEX& operator=(AspFreeTEX&& T);

		operator GLuint() const
		{
			return m_textureunitnumber;
		}

		// TODO: separation: mass storage -> RAM -> VRAM
		void loadBMP_custom(const char* const filepath); // Load a .BMP file using our custom loader // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
		void loadDDS(const char* const filepath); // Load a .DDS file using GLFW's own loader

		explicit operator std::vector<unsigned char>() const
		{
			GLint bound_tex; // We want to live every state to be the same...
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_tex);
			glBindTexture(GL_TEXTURE_2D, m_texture_id);

			unsigned char* const P_texture_data = new unsigned char[m_width * m_height * 3];
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, P_texture_data);

			glBindTexture(GL_TEXTURE_2D, 0); // TODO: Is this row neccesarry?
			glBindTexture(GL_TEXTURE_2D, bound_tex);

			return std::vector<unsigned char>(P_texture_data, P_texture_data + m_width * m_height * 3);
		}

		GLsizei getWidth() const
		{
			return m_width;
		}
		GLsizei getHeight() const
		{
			return m_height;
		}

		void bind() const;
		void unBind() const;

	};

	friend void swap(TEX::AspFreeTEX& t1, TEX::AspFreeTEX& t2);

private:

	mutable TwoStatesManager m_loading;
	mutable TwoStatesManager m_binding;

	AspFreeTEX m_tex;

public:

	friend void swap(TEX& t1, TEX& t2);

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
	TEX& operator=(TEX&& T);

	operator GLuint() const
	{
		return (m_binding.checkOn(static_cast<std::function<GLuint(const AspFreeTEX&)>>(&AspFreeTEX::operator GLuint)))(m_tex);
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