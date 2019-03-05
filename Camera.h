// TODO: make the up vector of the camera adjustable
// TODO: dont let the camera turn around

#pragma once

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "InPuts.h"
#include "Math.h"
#include "Light.h"



class Camera : public PosDirObj // Inheritance is not for polimorfism in this case.
{

protected:

	float m_near = 1.0f;
	float m_far  = 100.0f;

	float m_fov = glm::radians(45.0f); // filed of view (the whole horizontal angle, not the half of it)

public:

	virtual ~Camera()
	{
	}
	Camera() = default;
	Camera(const DirectionalLight& LIGHT);

	float getNear() const
	{
		return m_near;
	}
	void setNear(const float nearr) // TODO: why do we get a syntax error, if the parameter name is not nearr but near?
	{
		m_near = nearr;
	}

	float getFar() const
	{
		return m_far;
	}
	void setFar(const float farr) // TODO: why do we get a syntax error, if the parameter name is not farr but far?
	{
		m_far = farr;
	}

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
glm::mat4 getPerspectiveProj(const Camera&, int win_width, int win_height);
std::vector<glm::vec3> getFrustum(const Camera& CAM, int win_width, int win_height);
glm::mat4 getOrthogonaleProj(const Camera& CAM, const float left, const float right, const float bottom, const float top);

class InPutObserverCamera final : public InPut::ScrollBar::Observer, public InPut::Cursor::Observer, public Camera
{

	enum class Direction
	{
		FORWARD, BACKWARD, RIGHT, LEFT
	};

	friend class KeyObserver;
	class KeyObserver final : public InPut::KeyBoard::Key::Observer
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

