#include "Math.h"



glm::vec3 getDir(const float m_vertical_angle, const float m_horizontal_angle)
{
	return glm::vec3(
		cos(m_vertical_angle) * sin(m_horizontal_angle),
		sin(m_vertical_angle),
		cos(m_vertical_angle) * cos(m_horizontal_angle)
	);
}

glm::vec2 getVerticalAndHorizontalAngles(const glm::vec3& v)
{
	return glm::vec2(
		glm::atan(v.x / v.z),
		glm::asin(v.y / glm::length(v)));
}