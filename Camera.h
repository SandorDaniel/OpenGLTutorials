#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "InPuts.h"


class Camera : public InPut::ScrollBar::Observer, public InPut::Cursor::Observer
{

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 0, 5);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float FoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	enum class Direction
	{
		UP, DOWN, RIGHT, LEFT
	};

	friend class Obs;
	class Obs : public InPut::KeyBoard::Key::Observer
	{

	private:

		Camera* m_P_cam;

		Direction DIRECTION;

		double m_time_last_pressed = 0;

	public:

		void set(Camera* cam, Direction dir)
		{
			m_P_cam = cam;
			DIRECTION = dir;
		}

		virtual void pressCallBack()
		{
			m_time_last_pressed = glfwGetTime();
		}
		virtual void releaseCallBack();

	} m_observer_up, m_observer_down, m_observer_right, m_observer_left;

public:

	Camera();

	const glm::mat4& getViewMatrix() const
	{
		return ViewMatrix;
	}
	const glm::mat4& getProjectionMatrix() const
	{
		return ProjectionMatrix;
	}

	void init(GLFWwindow* window);
	void upDate(GLFWwindow* window);

	virtual void motionCallBack(GLFWwindow* p_win, double xpos, double ypos);
	virtual void scrollCallBack(double yoffset);

};
