#define sd_debugger
#include "Debug.h" // TODO rename to sdd

#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include <common/objloader.hpp>

#include "App.h"
#include "Shader.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "TEX.h"
#include "Camera.h"



void App::init()
{
	// Read our .obj file
	std::vector< glm::vec3 > v_pos;
	std::vector< glm::vec3 > v_nor;
	std::vector< glm::vec2 > v_tex;

	bool res = loadOBJ("../tutorial07_model_loading/cube.obj", v_pos, v_tex, v_nor);


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
	m_programID = LoadShaders(
		std::vector<const char*>{"SimpleVertexShader.vertexshader"},
		std::vector<const char*>{"SimpleFragmentShader.fragmentshader"});

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	m_MVPID = glGetUniformLocation(m_programID, "MVP");
	m_cam_posID = glGetUniformLocation(m_programID, "cam_pos");
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
	int win_width, win_height;
	glfwGetWindowSize(window, &win_width, &win_height);
	glm::mat4 MVP = getProj(m_camera, win_width, win_height) * getView(m_camera) * m_M;
	glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, &MVP[0][0]);
	
	glUniform3fv(m_cam_posID, 1, reinterpret_cast<GLfloat*>(&m_camera.getPos()));

	m_tex.setUniform(m_programID, "myTextureSampler"); // Two UV coordinatesfor each vertex. They were created with Blender.

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	m_vao.disAble();
}


void App::clean()
{
	glDeleteProgram(m_programID);
}
