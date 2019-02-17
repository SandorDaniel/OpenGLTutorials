// TODO: add aspects to lights if it is needed

#pragma once

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Math.h"



class Light // TODO: : public PosdirObj
{

public:

	static std::vector < Light* >    lights;

	static std::vector < GLfloat >   m_source_light_power;
	static std::vector < GLfloat >   m_source_light_ambient_power_coefficient;
	static std::vector < glm::vec4 > m_source_light_posdir;

	static std::vector < GLfloat >   m_positional_light_angle_in_radians;
	static std::vector < glm::vec3 > m_positional_source_light_dir;

	static std::vector < glm::vec3 > m_source_light_diffuse_col;
	static std::vector < glm::vec3 > m_source_light_specular_col;
	static std::vector < glm::vec3 > m_source_light_ambient_col;

	static GLint m_source_light_powerID;
	static GLint m_source_light_ambient_power_coefficientID;
	static GLint m_source_light_posdirID;

	static GLint m_positional_light_angle_in_radiansID;
	static GLint m_positional_source_light_dirID;

	static GLint m_source_light_diffuse_colID;
	static GLint m_source_light_specular_colID;
	static GLint m_source_light_ambient_colID;

	static void getUniformLocationsForAll(GLint m_programID)
	{
		m_source_light_powerID = glGetUniformLocation(m_programID, "source_light_power");
		m_source_light_ambient_power_coefficientID = glGetUniformLocation(m_programID, "source_light_ambient_power_coefficient");
		m_source_light_posdirID = glGetUniformLocation(m_programID, "source_light_posdir");
		m_positional_light_angle_in_radiansID = glGetUniformLocation(m_programID, "positional_light_angle_in_radians");
		m_positional_source_light_dirID = glGetUniformLocation(m_programID, "positional_source_light_dir");
		m_source_light_diffuse_colID = glGetUniformLocation(m_programID, "source_light_diffuse_col");
		m_source_light_specular_colID = glGetUniformLocation(m_programID, "source_light_specular_col");
		m_source_light_ambient_colID = glGetUniformLocation(m_programID, "source_light_ambient_col");
	}

	static void assignUniformsForAll()
	{
		glUniform1fv(m_source_light_powerID, m_source_light_power.size(), &m_source_light_power[0]);
		glUniform1fv(m_source_light_ambient_power_coefficientID, m_source_light_power.size(), &m_source_light_ambient_power_coefficient[0]);
		glUniform4fv(m_source_light_posdirID, m_source_light_power.size(), &m_source_light_posdir[0][0]);
		glUniform1fv(m_positional_light_angle_in_radiansID, m_source_light_power.size(), &m_positional_light_angle_in_radians[0]);
		glUniform3fv(m_positional_source_light_dirID, m_source_light_power.size(), &m_positional_source_light_dir[0][0]);
		glUniform3fv(m_source_light_diffuse_colID, m_source_light_power.size(), &m_source_light_diffuse_col[0][0]);
		glUniform3fv(m_source_light_specular_colID, m_source_light_power.size(), &m_source_light_specular_col[0][0]);
		glUniform3fv(m_source_light_ambient_colID, m_source_light_power.size(), &m_source_light_ambient_col[0][0]);
	}

private:

	int id;

public:

	Light()
	{
		id = m_source_light_power.size();

		lights.push_back(this);

		m_source_light_power.push_back(0);
		m_source_light_ambient_power_coefficient.push_back(0);
		m_source_light_posdir.push_back(glm::vec4(0.0f, 10.0f, 0.0f, 1.0f));

		m_positional_light_angle_in_radians.push_back(2 * glm::pi<float>());
		m_positional_source_light_dir.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

		m_source_light_diffuse_col.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		m_source_light_specular_col.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		m_source_light_ambient_col.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	~Light()
	{
		for (int i = id + 1; i < m_source_light_power.size(); ++i)
		{
			--(lights[i]->id);
		}

		m_source_light_power.erase(m_source_light_power.begin() + id);
		m_source_light_ambient_power_coefficient.erase(m_source_light_ambient_power_coefficient.begin() + id);
		m_source_light_posdir.erase(m_source_light_posdir.begin() + id);

		m_positional_light_angle_in_radians.erase(m_positional_light_angle_in_radians.begin() + id);
		m_positional_source_light_dir.erase(m_positional_source_light_dir.begin() + id);

		m_source_light_diffuse_col.erase(m_source_light_diffuse_col.begin() + id);
		m_source_light_specular_col.erase(m_source_light_specular_col.begin() + id);
		m_source_light_ambient_col.erase(m_source_light_ambient_col.begin() + id);
	}

	void setPower(const GLfloat a)
	{
		m_source_light_power[id] = a;
	}
	void setAmbientPowerCoefficient(const GLfloat a)
	{
		m_source_light_ambient_power_coefficient[id] = a;
	}
	void setPosDir(const glm::vec4& a)
	{
		m_source_light_posdir[id] = a;
	}
	void setAngle(const GLfloat a)
	{
		m_positional_light_angle_in_radians[id] = a;
	}
	void setDir(const glm::vec3& a)
	{
		m_positional_source_light_dir[id] = a;
	}
	void setDiffuseCol(const glm::vec3& a)
	{
		m_source_light_diffuse_col[id] = a;
	}
	void setSpecularCol(const glm::vec3& a)
	{
		m_source_light_specular_col[id] = a;
	}
	void setAmbientCol(const glm::vec3& a)
	{
		m_source_light_ambient_col[id] = a;
	}

};


