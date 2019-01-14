#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "InPuts.h"



class Camera : public InPut::ScrollBar::Observer, public InPut::Cursor::Observer
{
	GLFWwindow* m_p_win = nullptr;

	glm::mat4 m_view_matrix{}; // Only for efficiency reasons.
	glm::mat4 m_projection_matrix{}; // Only for efficiency reasons.

	glm::vec3 m_position = glm::vec3(0, 0, 5);
	float m_horizontal_angle = 3.14f;
	float m_vertical_angle = 0.0f;
	float m_fov = 45.0f; // filed of view // TODO: tisztazni, hogy milyen szogrol van szo pontosan

	const float m_speed = 3.0f; // 3 units / second
	const float m_mouse_speed = 0.005f;

	enum class Direction
	{
		FORWARD, BACKWARD, RIGHT, LEFT
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

	Camera(GLFWwindow* p_win);

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
