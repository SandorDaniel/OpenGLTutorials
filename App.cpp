//#define sd_debugger
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
#include "TangentSpace.hpp"



void App::init()
{
	#pragma region OBJ Loading and Compressing (Mass Storage -> RAM)

	// Read our .obj file
	std::vector< glm::vec3 > original_v_pos;
	std::vector< glm::vec3 > original_v_nor;
	std::vector< glm::vec2 > original_v_tex;

	bool res = loadOBJ(
		"../tutorial13_normal_mapping/cylinder.obj", 
		original_v_pos, 
		original_v_tex, 
		original_v_nor);

	std::vector<glm::vec3> compressed_v_pos{};
	std::vector<glm::vec2> compressed_v_tex{};
	std::vector<glm::vec3> compressed_v_nor{};
	std::vector<GLushort> indices{};

	indexVBO(
		original_v_pos,
		original_v_tex,
		original_v_nor,
		indices,
		compressed_v_pos,
		compressed_v_tex,
		compressed_v_nor);

	std::vector<glm::vec3> compressed_v_tg{};
	std::vector<glm::vec3> compressed_v_btg{};

	computeTangentBasis(
		compressed_v_pos,
		compressed_v_tex,
		indices,
		compressed_v_tg,
		compressed_v_btg);

	#pragma endregion

	#pragma region OBJ Loading (RAM -> VRAM)

	m_vbo_pos.load(compressed_v_pos);
	m_vao.attach(m_vbo_pos);

	//m_vbo_nor.load(compressed_v_nor);
	//m_vao.attach(m_vbo_nor);

	m_vbo_tex.load(compressed_v_tex);
	m_vao.attach(m_vbo_tex);

	m_ibo.load(indices);
	m_vao.attach(m_ibo);

	m_vbo_tg.load(compressed_v_tg);
	m_vao.attach(m_vbo_tg);

	m_vbo_btg.load(compressed_v_btg);
	m_vao.attach(m_vbo_btg);

	m_tex_diff.loadDDS("../tutorial13_normal_mapping/diffuse.DDS");
	m_tex_spec.loadDDS("../tutorial13_normal_mapping/specular.DDS");
	m_tex_norm.loadBMP_custom("../tutorial13_normal_mapping/normal.bmp");

	#pragma endregion

	#pragma region CAMERA SetUp (window, events)

	m_camera.init(window);

	#pragma endregion

	#pragma region GPU-Side-Program SetUp (loading and compiling sahders, setting up uniform variables)

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders(
		std::vector<const char*>{"SimpleVertexShader.vertexshader"},
		std::vector<const char*>{"SimpleFragmentShader.fragmentshader"});

	// Get a handle for our uniform variables
	// Only during the initialisation
	m_MID = glGetUniformLocation(m_programID, "M");
	m_VID = glGetUniformLocation(m_programID, "V");
	m_PID = glGetUniformLocation(m_programID, "P");
	m_cam_posID = glGetUniformLocation(m_programID, "cam_pos");
	m_tex_diffID = glGetUniformLocation(m_programID, "my_tex_diff_sampler");
	m_tex_specID = glGetUniformLocation(m_programID, "my_tex_spec_sampler");
	m_tex_normID = glGetUniformLocation(m_programID, "my_tex_norm_sampler");
	m_does_model_transformation_contain_nonuniform_scalingID = glGetUniformLocation(m_programID, "is_model_nonuniform_scaled");
	Light::getUniformLocationsForAll(m_programID);

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

	m_M = glm::rotate(glm::mat4(), glm::radians<float>(20), glm::vec3(0.0f, 0.1f, 0.0f));
	m_does_m_M_contain_nonuniform_scaling = false;

	m_M2 = glm::scale(glm::mat4(), glm::vec3(0.5f, 1.0f, 0.5f)) * glm::translate(glm::mat4(), glm::vec3(3.0f, 0.0f, 0.0f));
	m_does_m_M2_contain_nonuniform_scaling = true;

	#pragma endregion

	#pragma region LIGHTS UpDate

	light = new Light();
	light->setPower(300.0f);
	light->setAmbientPowerCoefficient(50.0f);
	light->setPosDir(glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
	light->setAngle(glm::pi<float>() / 30.0f);
	light->setDir(glm::vec3(-1.0f, -1.0f, -1.0f));
	light->setDiffuseCol(glm::vec3(1.0f, 1.0f, 1.0f));
	light->setSpecularCol(0.1f * glm::vec3(1.0f, 1.0f, 1.0f));
	light->setAmbientCol(glm::vec3(1.0f, 1.0f, 1.0f));
	delete light;

	light2.setPower(300.0f);
	light2.setAmbientPowerCoefficient(50.0f);
	light2.setPosDir(glm::vec4(5.0f, 5.0f, -5.0f, 1.0f));
	light2.setAngle(glm::pi<float>() / 30.0f);
	light2.setDir(glm::vec3(-1.0f, -1.0f, 1.0f));
	light2.setDiffuseCol(glm::vec3(1.0f, 1.0f, 1.0f));
	light2.setSpecularCol(0.1f * glm::vec3(1.0f, 1.0f, 1.0f));
	light2.setAmbientCol(glm::vec3(1.0f, 1.0f, 1.0f));

	#pragma endregion
}


void App::render() const
{
	m_vao.bind();
	m_tex_diff.bind();
	m_tex_spec.bind();
	m_tex_norm.bind();

	// Use our shader
	glUseProgram(m_programID);

	// Set our "myTextureSampler" sampler to use Texture Unit TextureUnitNumber
	glUniform1i(m_tex_diffID, m_tex_diff); // DSA version: glProgramUniform1i(m_programID, m_tex_diffID, m_tex);
	glUniform1i(m_tex_specID, m_tex_spec);
	glUniform1i(m_tex_normID, m_tex_norm);
	
	glUniform3fv(m_cam_posID, 1, &m_camera.getPos()[0]); // DSA version: glProgramUniform3fv(m_programID, m_cam_posID, 1, reinterpret_cast<GLfloat*>(&m_camera.getPos()));

	int win_width, win_height;
	glfwGetWindowSize(window, &win_width, &win_height);

	Light::assignUniformsForAll();

	glm::mat4 V = getView(m_camera);
	glUniformMatrix4fv(m_VID, 1, GL_FALSE, &V[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_VID, 1, GL_FALSE, &V[0][0]);
	glm::mat4 P = getProj(m_camera, win_width, win_height, m_NEAR, m_FAR);
	glUniformMatrix4fv(m_PID, 1, GL_FALSE, &P[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_PID, 1, GL_FALSE, &P[0][0]);

	glUniform1i(m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M_contain_nonuniform_scaling ? 1 : 0); // DSA version: glProgramUniform1i(m_programID, m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M_contain_nonuniform_scaling ? 1 : 0);
	glUniformMatrix4fv(m_MID, 1, GL_FALSE, &m_M[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_MID, 1, GL_FALSE, &m_M[0][0]);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_ibo.getElementCount(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	glUniform1i(m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M2_contain_nonuniform_scaling ? 1 : 0); // DSA version: glProgramUniform1i(m_programID, m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M2_contain_nonuniform_scaling ? 1 : 0);
	glUniformMatrix4fv(m_MID, 1, GL_FALSE, &m_M2[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_MID, 1, GL_FALSE, &m_M2[0][0]);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_ibo.getElementCount(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	m_tex_diff.unBind();
	m_tex_spec.unBind();
	m_tex_norm.unBind();
	m_vao.unBind();
}


void App::clean()
{
	glDeleteProgram(m_programID);
}
