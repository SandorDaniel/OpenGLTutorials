#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "InPuts.h"
#include "Math.h"



class Camera : public PosDirObj // Inheritance is not for polimorfism in this case.
{

protected:

	float m_fov = 45.0f; // filed of view // TODO: tisztazni, hogy milyen szogrol van szo pontosan

public:

	float getFov() const
	{
		return m_fov;
	}
	void setFov(const float fov)
	{
		m_fov = fov;
	}

};


glm::mat4 getView(const Camera&);
glm::mat4 getPerspectiveProj(const Camera&, int win_width, int win_height, float near, float far);

class InPutObserverCamera final : public InPut::ScrollBar::Observer, public InPut::Cursor::Observer, public Camera
{

	enum class Direction
	{
		FORWARD, BACKWARD, RIGHT, LEFT
	};

	friend class KeyObserver;
	class KeyObserver : public InPut::KeyBoard::Key::Observer
	{

		InPutObserverCamera* m_p_cam;

		Direction m_direction;

		double m_time_last_pressed = 0;

	public:

		void set(InPutObserverCamera* cam, Direction dir)
		{
			m_p_cam = cam;
			m_direction = dir;
		}

		virtual void pressCallBack()
		{
			m_time_last_pressed = glfwGetTime();
		}
		virtual void releaseCallBack();

	};

private:

	const float m_speed = 3.0f; // 3 units / second
	const float m_mouse_speed = 0.005f;
	
	KeyObserver m_observer_up, m_observer_down, m_observer_right, m_observer_left;

public:

	InPutObserverCamera();

	void init(GLFWwindow* window);
	void upDate(GLFWwindow* window);

	virtual void motionCallBack(GLFWwindow* p_win, double xpos, double ypos);
	virtual void scrollCallBack(double yoffset);

};

