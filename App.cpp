#include "App.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include <common/shader.hpp>

#include <vector>


void App::Init()
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	const std::vector<glm::vec4> g_vertex_buffer_data = {
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

	m_vbo.Load(g_vertex_buffer_data);

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	m_MVPID = glGetUniformLocation(m_programID, "MVP");

	m_vao.Bind(m_vbo);
}


void App::Update()
{
	// Model transformations

	//glm::mat4 S = glm::scale(glm::mat4(), glm::vec3(2.0f, 1.0f, 1.0f));
	//glm::mat4 R = glm::rotate(glm::mat4(), glm::radians<float>(45), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 T = glm::translate(glm::mat4(), glm::vec3(2.0f, 2.0f, 0.0f));

	//glm::mat4 M = T * R * S;

	glm::mat4 M;

	// View transformation

	glm::mat4 V = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Projection transformation

	glm::mat4 P = glm::perspective(glm::pi<float>() / 4.0f, 1024.0f / 768.0f, 5.0f, 100.0f);
	//glm::mat4 P = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 5.0f, 100.0f);

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

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, m_vbo.GetElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	m_vao.Disable();
}