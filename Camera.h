#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "InPuts.h"



class Camera : public InPut::ScrollBar::Observer, public InPut::Cursor::Observer
{

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

	};

private:

	GLFWwindow* m_p_win = nullptr;

	glm::vec3 m_position = glm::vec3(0, 0, 5);
	float m_horizontal_angle = 3.14f;
	float m_vertical_angle = 0.0f;
	float m_fov = 45.0f; // filed of view // TODO: tisztazni, hogy milyen szogrol van szo pontosan

	const float m_speed = 3.0f; // 3 units / second
	const float m_mouse_speed = 0.005f;
	
	KeyObserver m_observer_up, m_observer_down, m_observer_right, m_observer_left;

public:

	Camera(GLFWwindow* p_win);

	const glm::mat4 getViewMatrix() const
	{
		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(m_vertical_angle) * sin(m_horizontal_angle),
			sin(m_vertical_angle),
			cos(m_vertical_angle) * cos(m_horizontal_angle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(m_horizontal_angle - 3.14f / 2.0f),
			0,
			cos(m_horizontal_angle - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Camera matrix
		return glm::lookAt(
			m_position,           // Camera is here
			m_position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}
	const glm::mat4 getProjectionMatrix() const
	{
		// Projection matrix : 45digrees Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		int width, height;
		glfwGetWindowSize(m_p_win, &width, &height);
		
		return glm::perspective(glm::radians(m_fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
	}
	
	const glm::vec3& getPos() const
	{
		return m_position;
	}
	const glm::vec3& getDir() const
	{
		return glm::vec3(
			cos(m_vertical_angle) * sin(m_horizontal_angle),
			sin(m_vertical_angle),
			cos(m_vertical_angle) * cos(m_horizontal_angle));
	}

	void init(GLFWwindow* window);
	void upDate(GLFWwindow* window);

	virtual void motionCallBack(GLFWwindow* p_win, double xpos, double ypos);
	virtual void scrollCallBack(double yoffset);

};
