#include "App.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <common/shader.hpp>

#include <vector>


void App::Init()
{
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	std::vector<glm::vec3> g_vertex_buffer_data{
	   glm::vec3(0.0f, 0.0f, -1.0f),
	   glm::vec3(1.0f, 0.0f, -1.0f),
	   glm::vec3(0.5f,  1.0f, -1.0f),
	   glm::vec3(0.0f, 0.0f, -1.0f),
	   glm::vec3(1.0f, 0.0f, -1.0f),
	   glm::vec3(0.5f,  1.0f, -1.0f),
	};

	for (int i = 0; i < 3; ++i)
	{
		g_vertex_buffer_data[i] = glm::vec3(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f)) * glm::vec4(g_vertex_buffer_data[i], 1.0f));
	}

	vbo.Load(g_vertex_buffer_data);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDisableVertexAttribArray(0);
}


void App::Render()
{
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);

	// Use our shader
	glUseProgram(programID);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vbo.GetElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
}