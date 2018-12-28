#include "App.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include <common/shader.hpp>

#include <vector>


void App::Init()
{
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

	m_vbo_pos.Load(g_position_buffer_data);

	const std::vector<glm::vec4> g_color_buffer_data = {
		glm::vec4(0.583f,  0.771f,  0.014f, 1.0f),
		glm::vec4(0.609f,  0.115f,  0.436f, 1.0f),
		glm::vec4(0.327f,  0.483f,  0.844f, 1.0f),
		glm::vec4(0.822f,  0.569f,  0.201f, 1.0f),
		glm::vec4(0.435f,  0.602f,  0.223f, 1.0f),
		glm::vec4(0.310f,  0.747f,  0.185f, 1.0f),
		glm::vec4(0.597f,  0.770f,  0.761f, 1.0f),
		glm::vec4(0.559f,  0.436f,  0.730f, 1.0f),
		glm::vec4(0.359f,  0.583f,  0.152f, 1.0f),
		glm::vec4(0.483f,  0.596f,  0.789f, 1.0f),
		glm::vec4(0.559f,  0.861f,  0.639f, 1.0f),
		glm::vec4(0.195f,  0.548f,  0.859f, 1.0f),
		glm::vec4(0.014f,  0.184f,  0.576f, 1.0f),
		glm::vec4(0.771f,  0.328f,  0.970f, 1.0f),
		glm::vec4(0.406f,  0.615f,  0.116f, 1.0f),
		glm::vec4(0.676f,  0.977f,  0.133f, 1.0f),
		glm::vec4(0.971f,  0.572f,  0.833f, 1.0f),
		glm::vec4(0.140f,  0.616f,  0.489f, 1.0f),
		glm::vec4(0.997f,  0.513f,  0.064f, 1.0f),
		glm::vec4(0.945f,  0.719f,  0.592f, 1.0f),
		glm::vec4(0.543f,  0.021f,  0.978f, 1.0f),
		glm::vec4(0.279f,  0.317f,  0.505f, 1.0f),
		glm::vec4(0.167f,  0.620f,  0.077f, 1.0f),
		glm::vec4(0.347f,  0.857f,  0.137f, 1.0f),
		glm::vec4(0.055f,  0.953f,  0.042f, 1.0f),
		glm::vec4(0.714f,  0.505f,  0.345f, 1.0f),
		glm::vec4(0.783f,  0.290f,  0.734f, 1.0f),
		glm::vec4(0.722f,  0.645f,  0.174f, 1.0f),
		glm::vec4(0.302f,  0.455f,  0.848f, 1.0f),
		glm::vec4(0.225f,  0.587f,  0.040f, 1.0f),
		glm::vec4(0.517f,  0.713f,  0.338f, 1.0f),
		glm::vec4(0.053f,  0.959f,  0.120f, 1.0f),
		glm::vec4(0.393f,  0.621f,  0.362f, 1.0f),
		glm::vec4(0.673f,  0.211f,  0.457f, 1.0f),
		glm::vec4(0.820f,  0.883f,  0.371f, 1.0f),
		glm::vec4(0.982f,  0.099f,  0.879f, 1.0f)
	};

	m_vbo_col.Load(g_color_buffer_data);

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	m_MVPID = glGetUniformLocation(m_programID, "MVP");

	m_vao.Bind(m_vbo_pos);

	m_vao.Bind(m_vbo_col);
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
	glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.GetElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	m_vao.Disable();
}