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
#include <common/vboindexer.hpp>

#include "App.h"
#include "Shader.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "TEX.h"
#include "Camera.h"



void App::init()
{
	#pragma region OBJ Loading and Compressing (Mass Storage -> RAM)

	// Read our .obj file
	std::vector< glm::vec3 > original_v_pos;
	std::vector< glm::vec3 > original_v_nor;
	std::vector< glm::vec2 > original_v_tex;

	bool res = loadOBJ(
		"../tutorial08_basic_shading/suzanne.obj", 
		original_v_pos, 
		original_v_tex, 
		original_v_nor);

	std::vector<glm::vec3> compressed_v_pos{};
	std::vector<glm::vec2> compressed_v_tex{};
	std::vector<glm::vec3> compressed_v_nor{};

	indexVBO(
		original_v_pos,
		original_v_tex,
		original_v_nor,
		m_indices,
		compressed_v_pos,
		compressed_v_tex,
		compressed_v_nor);

	#pragma endregion

	#pragma region OBJ Loading (RAM -> VRAM)

	m_vbo_pos.load(compressed_v_pos);
	m_vao.attach(m_vbo_pos);

	m_vbo_nor.load(compressed_v_nor);
	m_vao.attach(m_vbo_nor);

	m_vbo_tex.load(compressed_v_tex);
	m_vao.attach(m_vbo_tex);


	glGenBuffers(1, &m_elementbufferID);

	glBindVertexArray(m_vao); // Make the new array active, creating it if necessary.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbufferID); // Can not bind an IBO to courrent context, if there is no VAO bounded to the current context.

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLushort), &m_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0); // !!!VAO (container object) has to get unbound before IBO (regular object)!!! (???WHY???)

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	true ? m_tex.loadDDS("../tutorial08_basic_shading/uvmap.DDS") : m_tex.loadBMP_custom("../tutorial05_textured_cube/uvtemplate.bmp"); // Load the texture using any two methods

	#pragma endregion

	#pragma region CAMERA SetUp (window, events)

	m_camera.init(window);

	#pragma endregion

	#pragma region GPU-Side-Program SetUp (loading and compiling sahders, setting up uniform variables)

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders(
		std::vector<const char*>{"SimpleVertexShader.vertexshader"},
		std::vector<const char*>{"SimpleFragmentShader.fragmentshader"});

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	m_MVPID = glGetUniformLocation(m_programID, "MVP");
	m_cam_posID = glGetUniformLocation(m_programID, "cam_pos");
	// Get a handle for our "myTextureSampler" uniform
	m_textureID = glGetUniformLocation(m_programID, "myTextureSampler");

	#pragma endregion
}


void App::upDate()
{
	#pragma region INPUT UpDate

	for (auto it : InPut::KeyBoard::getActiveKeys())
	{
		if (glfwGetKey(window, it.first) == GLFW_PRESS)
		{
			it.second.release();
			it.second.press();
		}
	}

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	#pragma endregion

	//camera.upDate(window);

	#pragma region MODEL-TRANSFORMATIONS UpDate

	//glm::mat4 S = glm::scale(glm::mat4(), glm::vec3(2.0f, 1.0f, 1.0f));
	//glm::mat4 R = glm::rotate(glm::mat4(), glm::radians<float>(45), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 T = glm::translate(glm::mat4(), glm::vec3(2.0f, 2.0f, 0.0f));

	//M = T * R * S;

	m_M;
	m_M2 = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 0.0f));

	#pragma endregion
}


void App::render() const
{
	m_vao.bind();
	m_tex.bind();

	// Use our shader
	glUseProgram(m_programID);

	// Set our "myTextureSampler" sampler to use Texture Unit TextureUnitNumber
	glUniform1i(m_textureID, m_tex);
	
	glUniform3fv(m_cam_posID, 1, reinterpret_cast<GLfloat*>(&m_camera.getPos()));

	int win_width, win_height;
	glfwGetWindowSize(window, &win_width, &win_height);

	glm::mat4 MVP = getProj(m_camera, win_width, win_height) * getView(m_camera) * m_M;
	glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, &MVP[0][0]);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	MVP = getProj(m_camera, win_width, win_height) * getView(m_camera) * m_M2;
	glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, &MVP[0][0]);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	m_tex.unBind();
	m_vao.unBind();
}


void App::clean()
{
	glDeleteProgram(m_programID);
}
