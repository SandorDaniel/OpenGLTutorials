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
	std::vector<glm::vec3> original_v_pos_cilinder{};
	std::vector<glm::vec3> original_v_nor_cilinder{};
	std::vector<glm::vec2> original_v_tex_cilinder{};

	bool res = loadOBJ(
		"../tutorial13_normal_mapping/cylinder.obj", 
		original_v_pos_cilinder, 
		original_v_tex_cilinder, 
		original_v_nor_cilinder);

	std::vector<glm::vec3> compressed_v_pos_cilinder{};
	std::vector<glm::vec2> compressed_v_tex_cilinder{};
	std::vector<glm::vec3> compressed_v_nor_cilinder{};
	std::vector<GLushort> v_ind_cilinder{};

	indexVBO(
		original_v_pos_cilinder,
		original_v_tex_cilinder,
		original_v_nor_cilinder,
		v_ind_cilinder,
		compressed_v_pos_cilinder,
		compressed_v_tex_cilinder,
		compressed_v_nor_cilinder);

	std::vector<glm::vec3> compressed_v_tg_cilinder{};
	std::vector<glm::vec3> compressed_v_btg_cilinder{};

	computeTangentBasis(
		compressed_v_pos_cilinder,
		compressed_v_tex_cilinder,
		v_ind_cilinder,
		compressed_v_tg_cilinder,
		compressed_v_btg_cilinder);

	std::vector<glm::vec3> v_pos_plane
	{
		glm::vec3(-2.0f, -2.0f, 0.0f),
		glm::vec3(+2.0f, -2.0f, 0.0f),
		glm::vec3(+2.0f, +2.0f, 0.0f),
		glm::vec3(-2.0f, +2.0f, 0.0f)
	};
	std::vector<glm::vec2> v_tex_plane
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};
	std::vector<GLushort> v_ind_plane
	{
		0, 1, 2, 0, 2, 3
	};
	std::vector<glm::vec3> v_tg_plane{};
	std::vector<glm::vec3> v_btg_plane{};

	computeTangentBasis(
		v_pos_plane,
		v_tex_plane,
		v_ind_plane,
		v_tg_plane,
		v_btg_plane);

	DDSInRAM dds_image_matdiff_wall;
	dds_image_matdiff_wall.load("../tutorial13_normal_mapping/diffuse.DDS");

	DDSInRAM dds_image_matspec_wall;
	dds_image_matspec_wall.load("../tutorial13_normal_mapping/specular.DDS");

	BMPInRAM bmp_image_nor_wall;
	bmp_image_nor_wall.load("../tutorial13_normal_mapping/normal.bmp");

	#pragma endregion

	#pragma region OBJ Loading (RAM -> VRAM)

	m_vbo_pos_cilinder.load(compressed_v_pos_cilinder);
	m_vao_cilinder.attach(m_vbo_pos_cilinder);

	//m_vbo_nor.load(compressed_v_nor);
	//m_vao.attach(m_vbo_nor);

	m_vbo_tex_cilinder.load(compressed_v_tex_cilinder);
	m_vao_cilinder.attach(m_vbo_tex_cilinder);

	m_ibo_cilinder.load(v_ind_cilinder);
	m_vao_cilinder.attach(m_ibo_cilinder);

	m_vbo_tg_cilinder.load(compressed_v_tg_cilinder);
	m_vao_cilinder.attach(m_vbo_tg_cilinder);

	m_vbo_btg_cilinder.load(compressed_v_btg_cilinder);
	m_vao_cilinder.attach(m_vbo_btg_cilinder);

	m_vbo_pos_plane.load(v_pos_plane);
	m_vao_plane.attach(m_vbo_pos_plane);

	m_vbo_tex_plane.load(v_tex_plane);
	m_vao_plane.attach(m_vbo_tex_plane);

	m_ibo_plane.load(v_ind_plane);
	m_vao_plane.attach(m_ibo_plane);

	m_vbo_tg_plane.load(v_tg_plane);
	m_vao_plane.attach(m_vbo_tg_plane);

	m_vbo_btg_plane.load(v_btg_plane);
	m_vao_plane.attach(m_vbo_btg_plane);

	m_tex_matdiff_wall.loadDDS(dds_image_matdiff_wall);
	
	m_tex_matspec_wall.loadDDS(dds_image_matspec_wall);
	
	m_tex_nor_wall.loadBMP(bmp_image_nor_wall);

	#pragma endregion

	#pragma region CAMERA SetUp (window, events)

	m_camera.init(window);
	m_camera.setNear(1.0f);
	m_camera.setFar(100.0f);
	m_camera.setFov(glm::pi<float>() / 4.0f);

	#pragma endregion

	#pragma region GPU-Side-Program SetUp (loading and compiling sahders, setting up uniform variables)

	// Create and compile our GLSL program from the shaders
	m_program_nor_matlight_shadow_mapped_id = LoadShaders(
		std::vector<const char*>{"SimpleVertexShader.vertexshader"},
		std::vector<const char*>{"SimpleFragmentShader.fragmentshader"});

	// Get a handle for our uniform variables
	// Only during the initialisation
	m_M_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "M");
	m_V_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "V");
	m_P_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "P");
	m_cam_pos_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "cam_pos");
	m_tex_matdiff_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "tex_matdiff_sampler");
	m_tex_matspec_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "tex_matspec_sampler");
	m_tex_norm_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "tex_norm_sampler");
	m_does_model_transformation_contain_nonuniform_scaling_nor_matlight_shadow_mapped_id = glGetUniformLocation(m_program_nor_matlight_shadow_mapped_id, "is_model_nonuniform_scaled");
	Light::getUniformLocationsForAll(m_program_nor_matlight_shadow_mapped_id);

	#pragma endregion

	#pragma region FBO

	m_tex_col.alloc(100, 100);
	m_tex_depth.alloc(100, 100);

	fbo.attach(m_tex_col);
	fbo.attach(m_tex_depth);

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
	
	m_M_horizontal_cilinder = 
		glm::rotate(glm::mat4(), glm::radians<float>(-90), glm::vec3(0.0f, 0.0f, 1.0f));
	m_does_m_M_horizontal_cilinder_contain_nonuniform_scaling_horizontal_cilinder = false;

	m_M_vertical_cilinder = 
		glm::scale(glm::mat4(), glm::vec3(0.5f, 1.0f, 0.5f)) * 
		glm::translate(glm::mat4(), glm::vec3(3.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians<float>(-180), glm::vec3(0.0f, 1.0f, 0.0f));
	m_does_m_M_vertical_cilinder_contain_nonuniform_scaling_horizontal_cilinder = true;

	m_M_vertical_plane =
		glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -2.0f));
	m_does_m_M_vertical_plane_contain_nonuniform_scaling = false;

	m_M_horizontal_plane =
		glm::translate(glm::mat4(), glm::vec3(0.0f, -2.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians<float>(-90), glm::vec3(1.0f, 0.0f, 0.0f));
	m_does_m_M_horizontal_plane_contain_nonuniform_scaling = false;

	#pragma endregion

	#pragma region LIGHTS UpDate

	light_positional.setPower(0000.0f);
	light_positional.setPosDir(glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
	light_positional.setAngle(glm::pi<float>() / 30.0f);
	light_positional.setDir(glm::vec3(-1.0f, -1.0f, -1.0f));
	light_positional.setDiffuseCol(100.0f * glm::vec3(1.0f, 1.0f, 1.0f));
	light_positional.setSpecularCol(1.0f * glm::vec3(1.0f, 1.0f, 1.0f));
	light_positional.setAmbientCol(100000.0f * glm::vec3(1.0f, 1.0f, 1.0f));

	light_directional.setPower(1.0f);
	light_directional.setPosDir(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	light_directional.setAngle(glm::pi<float>() / 30.0f);
	light_directional.setDir(glm::vec3(-1.0f, -1.0f, 1.0f));
	light_directional.setDiffuseCol (  1.0f * glm::vec3(1.0f, 1.0f, 1.0f));
	light_directional.setSpecularCol(  1.0f * glm::vec3(1.0f, 1.0f, 1.0f));
	light_directional.setAmbientCol (000.0f * glm::vec3(1.0f, 1.0f, 1.0f));

	#pragma endregion
}


void App::render() const
{
	//fbo.bind(GL_DRAW_FRAMEBUFFER);

	// Use our shader
	glUseProgram(m_program_nor_matlight_shadow_mapped_id);

	m_tex_matdiff_wall.bind();
	m_tex_matspec_wall.bind();
	m_tex_nor_wall.bind();

	m_vao_cilinder.bind();

	// Set our "myTextureSampler" sampler to use Texture Unit TextureUnitNumber
	glUniform1i(m_tex_matdiff_nor_matlight_shadow_mapped_id, m_tex_matdiff_wall); // DSA version: glProgramUniform1i(m_programID, m_tex_diffID, m_tex);
	glUniform1i(m_tex_matspec_nor_matlight_shadow_mapped_id, m_tex_matspec_wall);
	glUniform1i(m_tex_norm_nor_matlight_shadow_mapped_id, m_tex_nor_wall);
	
	glUniform3fv(m_cam_pos_nor_matlight_shadow_mapped_id, 1, &m_camera.getPos()[0]); // DSA version: glProgramUniform3fv(m_programID, m_cam_pos_id, 1, reinterpret_cast<GLfloat*>(&m_camera.getPos()));

	int win_width, win_height;
	glfwGetWindowSize(window, &win_width, &win_height);

	Light::assignUniformsForAll();

	glm::mat4 V = getView(m_camera);
	glUniformMatrix4fv(m_V_nor_matlight_shadow_mapped_id, 1, GL_FALSE, &V[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_VID, 1, GL_FALSE, &V[0][0]);
	glm::mat4 P = getPerspectiveProj(m_camera, win_width, win_height);
	glUniformMatrix4fv(m_P_nor_matlight_shadow_mapped_id, 1, GL_FALSE, &P[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_PID, 1, GL_FALSE, &P[0][0]);

	glUniform1i(m_does_model_transformation_contain_nonuniform_scaling_nor_matlight_shadow_mapped_id, m_does_m_M_horizontal_cilinder_contain_nonuniform_scaling_horizontal_cilinder ? 1 : 0); // DSA version: glProgramUniform1i(m_programID, m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M_contain_nonuniform_scaling ? 1 : 0);
	glUniformMatrix4fv(m_M_nor_matlight_shadow_mapped_id, 1, GL_FALSE, &m_M_horizontal_cilinder[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_MID, 1, GL_FALSE, &m_M[0][0]);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_ibo_cilinder.getElementCount(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	glUniform1i(m_does_model_transformation_contain_nonuniform_scaling_nor_matlight_shadow_mapped_id, m_does_m_M_vertical_cilinder_contain_nonuniform_scaling_horizontal_cilinder ? 1 : 0); // DSA version: glProgramUniform1i(m_programID, m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M2_contain_nonuniform_scaling ? 1 : 0);
	glUniformMatrix4fv(m_M_nor_matlight_shadow_mapped_id, 1, GL_FALSE, &m_M_vertical_cilinder[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_MID, 1, GL_FALSE, &m_M2[0][0]);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, m_vbo_pos.getElementCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_ibo_cilinder.getElementCount(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	m_vao_cilinder.unBind();

	m_vao_plane.bind();

	glUniform1i(m_does_model_transformation_contain_nonuniform_scaling_nor_matlight_shadow_mapped_id, m_does_m_M_horizontal_plane_contain_nonuniform_scaling ? 1 : 0); // DSA version: glProgramUniform1i(m_programID, m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M2_contain_nonuniform_scaling ? 1 : 0);
	glUniformMatrix4fv(m_M_nor_matlight_shadow_mapped_id, 1, GL_FALSE, &m_M_horizontal_plane[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_MID, 1, GL_FALSE, &m_M[0][0]);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_ibo_plane.getElementCount(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	glUniform1i(m_does_model_transformation_contain_nonuniform_scaling_nor_matlight_shadow_mapped_id, m_does_m_M_horizontal_plane_contain_nonuniform_scaling ? 1 : 0); // DSA version: glProgramUniform1i(m_programID, m_does_model_transformation_contain_nonuniform_scalingID, m_does_m_M2_contain_nonuniform_scaling ? 1 : 0);
	glUniformMatrix4fv(m_M_nor_matlight_shadow_mapped_id, 1, GL_FALSE, &m_M_vertical_plane[0][0]); // DSA version: glProgramUniformMatrix4fv(m_programID, m_MID, 1, GL_FALSE, &m_M2[0][0]);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_ibo_plane.getElementCount(),    // count
		GL_UNSIGNED_SHORT,   // type of indices
		(void*)0           // element array buffer offset
	);

	m_vao_plane.unBind();

	m_tex_matdiff_wall.unBind();
	m_tex_matspec_wall.unBind();
	m_tex_nor_wall.unBind();
	
	//fbo.unBind();
}


void App::afterScreen()
{
	printImage(
		"printedTex.ppm",
		static_cast<std::vector<unsigned char>>(m_tex_col),
		m_tex_col.getWidth(),
		m_tex_col.getHeight(),
		m_tex_col.getComponentCount());

	printImage(
		"printedDepthTex.ppm",
		static_cast<std::vector<unsigned char>>(m_tex_depth),
		m_tex_depth.getWidth(),
		m_tex_depth.getHeight(),
		m_tex_depth.getComponentCount());
}


void App::clean()
{
	glDeleteProgram(m_program_nor_matlight_shadow_mapped_id);
}
