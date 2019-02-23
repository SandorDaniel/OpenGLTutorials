#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>

#include "VBO.hpp"
#include "VAO.hpp"
#include "IBO.hpp"
#include "TEX.h"
#include "FBO.h"

#include "Camera.h"
#include "Light.h"



class App
{
	GLFWwindow* window = nullptr; // TODO: rename it properly

	VAO m_vao;
	VBO<glm::tvec3, float, glm::highp, 3> m_vbo_pos;
	//VBO<glm::tvec3, float, glm::highp, 3> m_vbo_nor;
	VBO<glm::tvec2, float, glm::highp, 2> m_vbo_tex;
	VBO<glm::tvec3, float, glm::highp, 3> m_vbo_tg;
	VBO<glm::tvec3, float, glm::highp, 3> m_vbo_btg;
	IBO<GLushort> m_ibo;
	
	TEX<TexType::COLOR> m_tex_diff;
	TEX<TexType::COLOR> m_tex_spec;
	TEX<TexType::COLOR> m_tex_norm;

	GLint m_programID;
	GLint m_MID;
	GLint m_VID;
	GLint m_PID;
	GLint m_cam_posID;
	GLint m_tex_diffID;
	GLint m_tex_specID;
	GLint m_tex_normID;
	GLint m_does_model_transformation_contain_nonuniform_scalingID;

	Light light;
	Light* p_light;

	glm::mat4 m_M = glm::mat4(); // glm::mat4() is the identity matrix
	bool m_does_m_M_contain_nonuniform_scaling = false;
	glm::mat4 m_M2 = glm::mat4();
	bool m_does_m_M2_contain_nonuniform_scaling = false;

	InPutObserverCamera m_camera;
	const float m_NEAR = 1.0f;
	const float m_FAR = 100.0f;

	TEX<TexType::COLOR> m_tex_col;
	TEX<TexType::DEPTH> m_tex_depth;
	FBO fbo;

public:

	App(GLFWwindow* window) : window(window)
	{  
	}

	void init();
	void upDate();
	void render() const;
	void clean();
	void lateUpDate();
};
