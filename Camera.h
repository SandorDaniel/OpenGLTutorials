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

	glm::vec3 m_position = glm::vec3(0, 0, 5);
	float m_horizontal_angle = glm::pi<float>();
	float m_vertical_angle = 0.0f;
	
	float m_fov = 45.0f; // filed of view // TODO: tisztazni, hogy milyen szogrol van szo pontosan

	const float m_speed = 3.0f; // 3 units / second
	const float m_mouse_speed = 0.005f;
	
	KeyObserver m_observer_up, m_observer_down, m_observer_right, m_observer_left;

public:

	Camera();

	glm::vec3 getPos() const
	{
		return m_position;
	}
	glm::vec3 getDir() const
	{
		return glm::vec3(
			cos(m_vertical_angle) * sin(m_horizontal_angle),
			sin(m_vertical_angle),
			cos(m_vertical_angle) * cos(m_horizontal_angle)
		);
	}
	glm::vec3 getRight() const
	{
		return  glm::vec3(
			sin(m_horizontal_angle - glm::pi<float>() / 2.0f),
			0,
			cos(m_horizontal_angle - glm::pi<float>() / 2.0f)
		);
	}
	float getFov() const
	{
		return m_fov;
	}

	void init(GLFWwindow* window);
	void upDate(GLFWwindow* window);

	virtual void motionCallBack(GLFWwindow* p_win, double xpos, double ypos);
	virtual void scrollCallBack(double yoffset);

};


glm::mat4 getView(const Camera&);
glm::mat4 getProj(const Camera&, int win_width, int win_height, float near, float far);
