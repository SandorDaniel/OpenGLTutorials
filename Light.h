// TODO: add aspects to lights if it is needed
// TODO: outlining

#pragma once

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Math.h"



class Light // TODO: : public PosdirObj
{

public:

	virtual ~Light()
	{
	}

	virtual void setPower(const GLfloat a)          = 0;
	virtual void setDir(const glm::vec3& a)         = 0;
	virtual void setDiffuseCol(const glm::vec3& a)  = 0;
	virtual void setSpecularCol(const glm::vec3& a) = 0;
	virtual void setAmbientCol(const glm::vec3& a)  = 0;

	virtual GLfloat   getPower()       const = 0;
	virtual glm::vec3 getDir()         const = 0;
	virtual glm::vec3 getDiffuseCol()  const = 0;
	virtual glm::vec3 getSpecularCol() const = 0;
	virtual glm::vec3 getAmbientCol()  const = 0;

};


class PositionalLight final : public Light
{

public:

	static std::vector<PositionalLight*> lights;

	static std::vector<GLfloat>          powers;
	static std::vector<glm::vec3>        positions;

	static std::vector<GLfloat>          angles_in_radians;
	static std::vector<glm::vec3>        dirs;

	static std::vector<glm::vec3>        diffuse_cols;
	static std::vector<glm::vec3>        specular_cols;
	static std::vector<glm::vec3>        ambient_cols;

	static GLint power_id;
	static GLint pos_id;

	static GLint angle_in_radians_id;
	static GLint dir_id;

	static GLint diffuse_col_id;
	static GLint specular_col_id;
	static GLint ambient_col_id;

	static void getUniformLocationsForAll(GLint m_programID)
	{
		power_id            = glGetUniformLocation(m_programID, "positional_light_power");
		pos_id              = glGetUniformLocation(m_programID, "positional_light_pos");
		angle_in_radians_id = glGetUniformLocation(m_programID, "positional_light_angle_in_radians");
		dir_id              = glGetUniformLocation(m_programID, "positional_light_dir");
		diffuse_col_id      = glGetUniformLocation(m_programID, "positional_light_diffuse_col");
		specular_col_id     = glGetUniformLocation(m_programID, "positional_light_specular_col");
		ambient_col_id      = glGetUniformLocation(m_programID, "positional_light_ambient_col");
	}
	static void assignUniformsForAll()
	{
		glUniform1fv(power_id,            powers.size(), &powers[0]);
		glUniform3fv(pos_id,              powers.size(), &positions[0][0]);
		glUniform1fv(angle_in_radians_id, powers.size(), &angles_in_radians[0]);
		glUniform3fv(dir_id,              powers.size(), &dirs[0][0]);
		glUniform3fv(diffuse_col_id,      powers.size(), &diffuse_cols[0][0]);
		glUniform3fv(specular_col_id,     powers.size(), &specular_cols[0][0]);
		glUniform3fv(ambient_col_id,      powers.size(), &ambient_cols[0][0]);
	}

private:

	int id;

public:

	PositionalLight()
	{
		id = powers.size();

		lights.push_back(this);

		powers.push_back(0);
		positions.push_back(glm::vec3(0.0f, 10.0f, 0.0f));

		angles_in_radians.push_back(2 * glm::pi<float>());
		dirs.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

		diffuse_cols.push_back (glm::vec3(1.0f, 1.0f, 1.0f));
		specular_cols.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		ambient_cols.push_back (glm::vec3(1.0f, 1.0f, 1.0f));
	}
	~PositionalLight()
	{
		for (int i = id + 1; i < powers.size(); ++i)
		{
			--(lights[i]->id);
		}

		powers.erase(powers.begin() + id);
		positions.erase(positions.begin() + id);

		angles_in_radians.erase(angles_in_radians.begin() + id);
		dirs.erase(dirs.begin() + id);

		diffuse_cols.erase (diffuse_cols.begin()  + id);
		specular_cols.erase(specular_cols.begin() + id);
		ambient_cols.erase (ambient_cols.begin()  + id);
	}

	void setPower(const GLfloat a)
	{
		powers[id] = a;
	}
	void setPos(const glm::vec3& a)
	{
		positions[id] = a;
	}
	void setAngleInRadians(const GLfloat a)
	{
		angles_in_radians[id] = a;
	}
	void setDir(const glm::vec3& a)
	{
		dirs[id] = a;
	}
	void setDiffuseCol(const glm::vec3& a)
	{
		diffuse_cols[id] = a;
	}
	void setSpecularCol(const glm::vec3& a)
	{
		specular_cols[id] = a;
	}
	void setAmbientCol(const glm::vec3& a)
	{
		ambient_cols[id] = a;
	}

	GLfloat   getPower() const
	{
		return powers[id];
	}
	glm::vec3 getPos() const
	{
		return positions[id];
	}
	GLfloat   getAngleInRadians() const
	{
		return angles_in_radians[id];
	}
	glm::vec3 getDir() const
	{
		return dirs[id];
	}
	glm::vec3 getDiffuseCol() const
	{
		return diffuse_cols[id];
	}
	glm::vec3 getSpecularCol() const
	{
		return specular_cols[id];
	}
	glm::vec3 getAmbientCol() const
	{
		return ambient_cols[id];
	}

};


class DirectionalLight final : public Light
{

public:

	static std::vector<DirectionalLight*> lights;

	static std::vector<GLfloat>           powers;
	static std::vector<glm::vec3>         dirs;

	static std::vector<glm::vec3>         diffuse_cols;
	static std::vector<glm::vec3>         specular_cols;
	static std::vector<glm::vec3>         ambient_cols;

	static GLint power_id;
	static GLint dir_id;

	static GLint diffuse_col_id;
	static GLint specular_col_id;
	static GLint ambient_col_id;

	static void getUniformLocationsForAll(GLint m_programID)
	{
		power_id =        glGetUniformLocation(m_programID, "directional_light_power");
		dir_id =          glGetUniformLocation(m_programID, "directional_light_dir");
		diffuse_col_id =  glGetUniformLocation(m_programID, "directional_light_diffuse_col");
		specular_col_id = glGetUniformLocation(m_programID, "directional_light_specular_col");
		ambient_col_id =  glGetUniformLocation(m_programID, "directional_light_ambient_col");
	}
	static void assignUniformsForAll()
	{
		glUniform1fv(power_id,        powers.size(), &powers[0]);
		glUniform3fv(dir_id,          powers.size(), &dirs[0][0]);
		glUniform3fv(diffuse_col_id,  powers.size(), &diffuse_cols[0][0]);
		glUniform3fv(specular_col_id, powers.size(), &specular_cols[0][0]);
		glUniform3fv(ambient_col_id,  powers.size(), &ambient_cols[0][0]);
	}

private:

	int id;

public:

	DirectionalLight()
	{
		id = powers.size();

		lights.push_back(this);

		powers.push_back(0);
		dirs.push_back(glm::vec3(0.0f, 10.0f, 0.0f));

		diffuse_cols.push_back (glm::vec3(1.0f, 1.0f, 1.0f));
		specular_cols.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		ambient_cols.push_back (glm::vec3(1.0f, 1.0f, 1.0f));
	}
	~DirectionalLight()
	{
		for (int i = id + 1; i < powers.size(); ++i)
		{
			--(lights[i]->id);
		}

		powers.erase(powers.begin() + id);
		dirs.erase(dirs.begin() + id);

		diffuse_cols.erase (diffuse_cols.begin()  + id);
		specular_cols.erase(specular_cols.begin() + id);
		ambient_cols.erase (ambient_cols.begin()  + id);
	}

	void setPower(const GLfloat a)
	{
		powers[id] = a;
	}
	void setDir(const glm::vec3& a)
	{
		dirs[id] = a;
	}
	void setDiffuseCol(const glm::vec3& a)
	{
		diffuse_cols[id] = a;
	}
	void setSpecularCol(const glm::vec3& a)
	{
		specular_cols[id] = a;
	}
	void setAmbientCol(const glm::vec3& a)
	{
		ambient_cols[id] = a;
	}

	GLfloat   getPower() const
	{
		return powers[id];
	}
	glm::vec3 getDir() const
	{
		return dirs[id];
	}
	glm::vec3 getDiffuseCol() const
	{
		return diffuse_cols[id];
	}
	glm::vec3 getSpecularCol() const
	{
		return specular_cols[id];
	}
	glm::vec3 getAmbientCol() const
	{
		return ambient_cols[id];
	}

};
