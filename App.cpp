//#define sd_debugger
#include "Debug.h"

#include "App.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include <common/shader.hpp>
#include <common/objloader.hpp>

#include <vector>

#include "Camera.h"



void App::Init()
{
	// Read our .obj file
	std::vector< glm::vec3 > v_pos3;
	std::vector< glm::vec2 > v_tex2;
	std::vector< glm::vec3 > v_nor3; // Won't be used at the moment.
	bool res = loadOBJ("../tutorial07_model_loading/cube.obj", v_pos3, v_tex2, v_nor3);

	std::vector< glm::vec4 > v_pos{};
	for (auto v3 : v_pos3)
	{
		v_pos.push_back(glm::vec4(v3, 1.0f));
	}
	m_vbo_pos.Load(v_pos);

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	m_MVPID = glGetUniformLocation(m_programID, "MVP");

	m_vao.Bind(m_vbo_pos);

	m_vbo_tex.Load(v_tex2);

	m_vao.Bind(m_vbo_tex);

	// Load the texture using any two methods
	tex1.LoadBMP_custom("../tutorial05_textured_cube/uvtemplate.bmp");
	tex2.LoadDDS("../tutorial07_model_loading/uvmap.DDS");

	tex1.Bind();
	tex2.Bind();

	camera.init(window);
}


void App::Update()
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

	//glm::mat4 M = T * R * S;

	glm::mat4 M;

	// View transformation

	//glm::mat4 V = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 V = camera.getViewMatrix();

	// Projection transformation

	//glm::mat4 P = glm::perspective(glm::pi<float>() / 4.0f, static_cast<float>(width) / static_cast<float>(height), 5.0f, 100.0f);
	//glm::mat4 P = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 5.0f, 100.0f);
	glm::mat4 P = camera.getProjectionMatrix();

	// MVP

	m_MVP = P * V * M;
}


void App::Render() const
{
	m_vao.Enable();

	// Use our shader
	glUseProgram(m_programID);

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, &m_MVP[0][0]);

	tex2.Uniform(m_programID, "myTextureSampler"); // Two UV coordinatesfor each vertex. They were created with Blender.

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.GetElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	m_vao.Disable();
}


void App::Clean()
{
	glDeleteProgram(m_programID);
}
