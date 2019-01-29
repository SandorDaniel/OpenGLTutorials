#pragma once

#include <queue>
#include <algorithm>

#include <GL/glew.h>



//
// TODO: integrate GLuint loadBMP_custom(const char * imagepath, GLuint textureID); and GLuint loadDDS(const char * imagepath, GLuint textureID); into the class below.
//

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath, GLuint textureID);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath, GLuint textureID); 



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

		void loadBMP_custom(const char* const filepath) // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
		{
			::loadBMP_custom(filepath, m_texture_id);
		}
		void loadDDS(const char* const filepath)
		{
			::loadDDS(filepath, m_texture_id);
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
