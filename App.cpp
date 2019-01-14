//#define sd_debugger
#include "Debug.h"

#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include <common/shader.hpp>
#include <common/objloader.hpp>

#include "VBO.hpp"
#include "VAO.hpp"
#include "TEX.h"
#include "App.h"
#include "Camera.h"



void App::init()
{
	// Read our .obj file
	std::vector< glm::vec3 > v_pos3;
	std::vector< glm::vec3 > v_nor3;
	std::vector< glm::vec2 > v_tex;

	bool res = loadOBJ("../tutorial07_model_loading/cube.obj", v_pos3, v_tex, v_nor3);

	std::vector< glm::vec4 > v_pos{};
	for (auto v3 : v_pos3)
	{
		v_pos.push_back(glm::vec4(v3, 1.0f));
	}

	std::vector< glm::vec4 > v_nor{};
	for (auto v3 : v_nor3)
	{
		v_nor.push_back(glm::vec4(v3, 1.0f));
	}

	m_vbo_pos.load(v_pos);
	m_vao.bind(m_vbo_pos);

	m_vbo_nor.load(v_nor);
	m_vao.bind(m_vbo_nor);

	m_vbo_tex.load(v_tex);
	m_vao.bind(m_vbo_tex);

	// Load the texture using any two methods
	//tex.LoadBMP_custom("../tutorial05_textured_cube/uvtemplate.bmp");
	m_tex.loadDDS("../tutorial07_model_loading/uvmap.DDS");

	m_tex.bind();

	m_camera.init(window);

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	m_MVPID = glGetUniformLocation(m_programID, "MVP");
}


void App::upDate()
{
	// Input update

	for (auto it : InPut::KeyBoard::getActiveKeys())
	{
		if(glfwGetKey(window, it.first) == GLFW_PRESS)
		{
			it.second.release();
			it.second.press();
		}
	}

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	//camera.upDate(window);

	// Model transformations

	//glm::mat4 S = glm::scale(glm::mat4(), glm::vec3(2.0f, 1.0f, 1.0f));
	//glm::mat4 R = glm::rotate(glm::mat4(), glm::radians<float>(45), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 T = glm::translate(glm::mat4(), glm::vec3(2.0f, 2.0f, 0.0f));

	//M = T * R * S;

	m_M;
}


void App::render() const
{
	m_vao.enAble();

	// Use our shader
	glUseProgram(m_programID);

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glm::mat4 MVP = m_camera.getProjectionMatrix() * m_camera.getViewMatrix() * m_M;
	glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, &MVP[0][0]);

	m_tex.setUniform(m_programID, "myTextureSampler"); // Two UV coordinatesfor each vertex. They were created with Blender.

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	m_vao.disAble();
}


void App::clean()
{
	glDeleteProgram(m_programID);
}
