#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "InPuts.h"



class Camera : public InPut::ScrollBar::Observer, public InPut::Cursor::Observer
{

	glm::mat4 m_view_matrix{};
	glm::mat4 m_projection_matrix{};

	// Initial position : on +Z
	glm::vec3 m_position = glm::vec3(0, 0, 5);
	// Initial horizontal angle : toward -Z
	float m_horizontal_angle = 3.14f;
	// Initial vertical angle : none
	float m_vertical_angle = 0.0f;
	// Initial Field of View
	float m_fov = 45.0f;

	float m_speed = 3.0f; // 3 units / second
	float m_mouse_speed = 0.005f;

	enum class Direction
	{
		UP, DOWN, RIGHT, LEFT
	};

	friend class KeyObserver;
	class KeyObserver : public InPut::KeyBoard::Key::Observer
	{

	private:

		Camera* m_P_cam;

		Direction m_direction;

		double m_time_last_pressed = 0;

	public:

		void set(Camera* cam, Direction dir)
		{
			m_P_cam = cam;
			m_direction = dir;
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
		return m_view_matrix;
	}
	const glm::mat4& getProjectionMatrix() const
	{
		return m_projection_matrix;
	}

	void init(GLFWwindow* window);
	void upDate(GLFWwindow* window);

	virtual void motionCallBack(GLFWwindow* p_win, double xpos, double ypos);
	virtual void scrollCallBack(double yoffset);

};
