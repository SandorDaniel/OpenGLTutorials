#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>



glm::vec3 getDir(const float m_vertical_angle, const float m_horizontal_angle);
glm::vec2 getVerticalAndHorizontalAngles(const glm::vec3& v);


class PosDirObj
{

protected:

	glm::vec3 m_position = glm::vec3(0, 0, 5);

	float m_horizontal_angle = glm::pi<float>();
	float m_vertical_angle = 0.0f;

public:

	virtual ~PosDirObj()
	{
	}

	glm::vec3 getPos() const
	{
		return m_position;
	}
	float getHorizontalAngle() const
	{
		return m_horizontal_angle;
	}
	float getVerticalAngle() const
	{
		return m_vertical_angle;
	}

	void setPos(const glm::vec3& position)
	{
		m_position = position;
	}
	void setHorizontalAngle(const float horizontal_angle)
	{
		m_horizontal_angle = horizontal_angle;
	}
	void setVerticalAngle(const float vertical_angle)
	{
		m_vertical_angle = vertical_angle;
	}

};


template<class T>
glm::vec3 getDir(const T& T)
{
	return getDir(T.getVerticalAngle(), T.getHorizontalAngle());
}
template<class T>
glm::vec3 getRight(const T& T)
{
	return  glm::vec3(
		sin(T.getHorizontalAngle() - glm::pi<float>() / 2.0f),
		0,
		cos(T.getHorizontalAngle() - glm::pi<float>() / 2.0f)
	);
}