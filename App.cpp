//#define sd_debugger
#include "Debug.h"

#include "App.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include <common/shader.hpp>

#include <vector>

#include "Camera.h"



void App::Init()
{
	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	const std::vector<glm::vec4> g_position_buffer_data = {
		glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f), // triangle 1 : begin
		glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f), // triangle 1 : end
		glm::vec4(1.0f, 1.0f,-1.0f, 1.0f), // triangle 2 : begin
		glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f), // triangle 2 : end
		glm::vec4(1.0f,-1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f,-1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f,-1.0f, 1.0f, 1.0f)
	};

	m_vbo_pos.Load(g_position_buffer_data);

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	m_MVPID = glGetUniformLocation(m_programID, "MVP");

	m_vao.Bind(m_vbo_pos);

	const std::vector<glm::vec2> g_uv_buffer_data = {
		glm::vec2(0.000059f, 1.0f - 0.000004f),
		glm::vec2(0.000103f, 1.0f - 0.336048f),
		glm::vec2(0.335973f, 1.0f - 0.335903f),
		glm::vec2(1.000023f, 1.0f - 0.000013f),
		glm::vec2(0.667979f, 1.0f - 0.335851f),
		glm::vec2(0.999958f, 1.0f - 0.336064f),
		glm::vec2(0.667979f, 1.0f - 0.335851f),
		glm::vec2(0.336024f, 1.0f - 0.671877f),
		glm::vec2(0.667969f, 1.0f - 0.671889f),
		glm::vec2(1.000023f, 1.0f - 0.000013f),
		glm::vec2(0.668104f, 1.0f - 0.000013f),
		glm::vec2(0.667979f, 1.0f - 0.335851f),
		glm::vec2(0.000059f, 1.0f - 0.000004f),
		glm::vec2(0.335973f, 1.0f - 0.335903f),
		glm::vec2(0.336098f, 1.0f - 0.000071f),
		glm::vec2(0.667979f, 1.0f - 0.335851f),
		glm::vec2(0.335973f, 1.0f - 0.335903f),
		glm::vec2(0.336024f, 1.0f - 0.671877f),
		glm::vec2(1.000004f, 1.0f - 0.671847f),
		glm::vec2(0.999958f, 1.0f - 0.336064f),
		glm::vec2(0.667979f, 1.0f - 0.335851f),
		glm::vec2(0.668104f, 1.0f - 0.000013f),
		glm::vec2(0.335973f, 1.0f - 0.335903f),
		glm::vec2(0.667979f, 1.0f - 0.335851f),
		glm::vec2(0.335973f, 1.0f - 0.335903f),
		glm::vec2(0.668104f, 1.0f - 0.000013f),
		glm::vec2(0.336098f, 1.0f - 0.000071f),
		glm::vec2(0.000103f, 1.0f - 0.336048f),
		glm::vec2(0.000004f, 1.0f - 0.671870f),
		glm::vec2(0.336024f, 1.0f - 0.671877f),
		glm::vec2(0.000103f, 1.0f - 0.336048f),
		glm::vec2(0.336024f, 1.0f - 0.671877f),
		glm::vec2(0.335973f, 1.0f - 0.335903f),
		glm::vec2(0.667969f, 1.0f - 0.671889f),
		glm::vec2(1.000004f, 1.0f - 0.671847f),
		glm::vec2(0.667979f, 1.0f - 0.335851f)
	};

	m_vbo_tex.Load(g_uv_buffer_data);

	m_vao.Bind(m_vbo_tex);

	// Load the texture using any two methods
	tex1.LoadBMP_custom("../tutorial05_textured_cube/uvtemplate.bmp");
	tex2.LoadDDS("../tutorial05_textured_cube/uvtemplate.DDS");

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
