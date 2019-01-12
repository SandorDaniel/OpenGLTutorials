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

public:

	friend class ObserverUP;
	class ObserverUP : public InPut::KeyBoard::Key::Observer
	{

		Camera* const m_P_cam = nullptr;

		double m_time_last_pressed = 0;

	public:

		ObserverUP(Camera* p_cam) : m_P_cam(p_cam) 
		{ 
		}

		virtual void pressCallBack()
		{
			m_time_last_pressed = glfwGetTime();
		}

		virtual void releaseCallBack()
		{
			glm::vec3 direction(
				cos(m_P_cam->verticalAngle) * sin(m_P_cam->horizontalAngle),
				sin(m_P_cam->verticalAngle),
				cos(m_P_cam->verticalAngle) * cos(m_P_cam->horizontalAngle)
			);

			// Right vector
			glm::vec3 right = glm::vec3(
				sin(m_P_cam->horizontalAngle - 3.14f / 2.0f),
				0,
				cos(m_P_cam->horizontalAngle - 3.14f / 2.0f)
			);

			// Up vector
			glm::vec3 up = glm::cross(right, direction);

			m_P_cam->position += static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->speed * direction;

			// Camera matrix
			m_P_cam->ViewMatrix = glm::lookAt(
				m_P_cam->position,           // Camera is here
				m_P_cam->position + direction, // and looks here : at the same position, plus "direction"
				up                  // Head is up (set to 0,-1,0 to look upside-down)
			);
		}

	} m_observer_up = this;

	friend class ObserverDOWN;
	class ObserverDOWN : public InPut::KeyBoard::Key::Observer
	{

		Camera* const m_P_cam = nullptr;

		double m_time_last_pressed = 0;

	public:

		ObserverDOWN(Camera* p_cam) : m_P_cam(p_cam)
		{
		}

		virtual void pressCallBack()
		{
			m_time_last_pressed = glfwGetTime();
		}

		virtual void releaseCallBack()
		{
			glm::vec3 direction(
				cos(m_P_cam->verticalAngle) * sin(m_P_cam->horizontalAngle),
				sin(m_P_cam->verticalAngle),
				cos(m_P_cam->verticalAngle) * cos(m_P_cam->horizontalAngle)
			);

			// Right vector
			glm::vec3 right = glm::vec3(
				sin(m_P_cam->horizontalAngle - 3.14f / 2.0f),
				0,
				cos(m_P_cam->horizontalAngle - 3.14f / 2.0f)
			);

			// Up vector
			glm::vec3 up = glm::cross(right, direction);

			m_P_cam->position -= static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->speed * direction;

			// Camera matrix
			m_P_cam->ViewMatrix = glm::lookAt(
				m_P_cam->position,           // Camera is here
				m_P_cam->position + direction, // and looks here : at the same position, plus "direction"
				up                  // Head is up (set to 0,-1,0 to look upside-down)
			);
		}

	} m_observer_down = this;

	friend class ObserverRIGHT;
	class ObserverRIGHT : public InPut::KeyBoard::Key::Observer
	{

		Camera* const m_P_cam = nullptr;

		double m_time_last_pressed = 0;

	public:

		ObserverRIGHT(Camera* p_cam) : m_P_cam(p_cam)
		{
		}

		virtual void pressCallBack()
		{
			m_time_last_pressed = glfwGetTime();
		}

		virtual void releaseCallBack()
		{
			// Direction : Spherical coordinates to Cartesian coordinates conversion
			glm::vec3 direction(
				cos(m_P_cam->verticalAngle) * sin(m_P_cam->horizontalAngle),
				sin(m_P_cam->verticalAngle),
				cos(m_P_cam->verticalAngle) * cos(m_P_cam->horizontalAngle)
			);

			// Right vector
			glm::vec3 right = glm::vec3(
				sin(m_P_cam->horizontalAngle - 3.14f / 2.0f),
				0,
				cos(m_P_cam->horizontalAngle - 3.14f / 2.0f)
			);

			// Up vector
			glm::vec3 up = glm::cross(right, direction);

			m_P_cam->position += static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->speed * right;

			// Camera matrix
			m_P_cam->ViewMatrix = glm::lookAt(
				m_P_cam->position,           // Camera is here
				m_P_cam->position + direction, // and looks here : at the same position, plus "direction"
				up                  // Head is up (set to 0,-1,0 to look upside-down)
			);
		}

	} m_observer_right = this;

	friend class ObserverLEFT;
	class ObserverLEFT : public InPut::KeyBoard::Key::Observer
	{

		Camera* const m_P_cam = nullptr;

		double m_time_last_pressed = 0;

	public:

		ObserverLEFT(Camera* p_cam) : m_P_cam(p_cam)
		{
		}

		virtual void pressCallBack()
		{
			m_time_last_pressed = glfwGetTime();
		}

		virtual void releaseCallBack()
		{
			// Direction : Spherical coordinates to Cartesian coordinates conversion
			glm::vec3 direction(
				cos(m_P_cam->verticalAngle) * sin(m_P_cam->horizontalAngle),
				sin(m_P_cam->verticalAngle),
				cos(m_P_cam->verticalAngle) * cos(m_P_cam->horizontalAngle)
			);

			// Right vector
			glm::vec3 right = glm::vec3(
				sin(m_P_cam->horizontalAngle - 3.14f / 2.0f),
				0,
				cos(m_P_cam->horizontalAngle - 3.14f / 2.0f)
			);

			// Up vector
			glm::vec3 up = glm::cross(right, direction);

			m_P_cam->position -= static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->speed * right;

			// Camera matrix
			m_P_cam->ViewMatrix = glm::lookAt(
				m_P_cam->position,           // Camera is here
				m_P_cam->position + direction, // and looks here : at the same position, plus "direction"
				up                  // Head is up (set to 0,-1,0 to look upside-down)
			);
		}

	} m_observer_left = this;

	{

	void upDate(GLFWwindow* window) {

		#pragma region Window

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		#pragma endregion

		#pragma region Cursor - Orientation

		// Get mouse position
		static bool is_call_back_method_set_up_for_mouse_motion = false;
		if (!is_call_back_method_set_up_for_mouse_motion)
		{
			glfwSetCursorPosCallback(window, InPut::Cursor::motionCallback);
			is_call_back_method_set_up_for_mouse_motion = true;
		}

		// TODO: it may be skipped later 
		static bool is_mouse_inited = false;
		if (!is_mouse_inited)
		{
			InPut::Cursor::motionCallback(window, width / 2, height / 2);
			InPut::Cursor::regist(*this);
			is_mouse_inited = true;
		}


		//// Compute new orientation
		//horizontalAngle += mouseSpeed * static_cast<float>(width / 2 - InPut::Cursor::getXPos());
		//verticalAngle += mouseSpeed * static_cast<float>(height / 2 - InPut::Cursor::getYPos());

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		#pragma endregion

		#pragma region Keyboard - Position
	const glm::mat4& getViewMatrix() const
	{
		return ViewMatrix;
	}
	const glm::mat4& getProjectionMatrix() const
	{
		return ProjectionMatrix;
	}

		static bool is_call_back_method_set_up_for_keyboard_events = false;
		if (!is_call_back_method_set_up_for_keyboard_events)
		{
			glfwSetKeyCallback(window, InPut::KeyBoard::press_or_release_callback);
			InPut::KeyBoard::getKey(GLFW_KEY_UP).regist(m_observer_up);
			InPut::KeyBoard::getKey(GLFW_KEY_DOWN).regist(m_observer_down);
			InPut::KeyBoard::getKey(GLFW_KEY_RIGHT).regist(m_observer_right);
			InPut::KeyBoard::getKey(GLFW_KEY_LEFT).regist(m_observer_left);
			is_call_back_method_set_up_for_keyboard_events = true;
		}

		// Move forward
		position += direction * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_UP).getTimePressed()) * speed;
		// Move backward
		position -= direction * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_DOWN).getTimePressed()) * speed;
		// Strafe right
		position += right * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_RIGHT).getTimePressed()) * speed;
		// Strafe left
		position -= right * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_LEFT).getTimePressed()) * speed;

		#pragma endregion

		#pragma region ScrollBar - Angle

		static bool is_call_back_method_set_up_for_scrolling = false;
		if (!is_call_back_method_set_up_for_scrolling)
		{
			glfwSetScrollCallback(window, InPut::ScrollBar::scrollCallBack);
			InPut::ScrollBar::regist(*this);
			is_call_back_method_set_up_for_scrolling = true;
		}

		#pragma endregion

		// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	}

	virtual void motionCallBack(GLFWwindow* p_win, double xpos, double ypos)
	{
		int width, height;
		glfwGetWindowSize(p_win, &width, &height);

		// Compute new orientation
		horizontalAngle += mouseSpeed * static_cast<float>(width / 2 - InPut::Cursor::getXPos());
		verticalAngle += mouseSpeed * static_cast<float>(height / 2 - InPut::Cursor::getYPos());

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}

	virtual void scrollCallBack(double yoffset)
	{
		float newFoV = FoV - 5 * static_cast<float>(yoffset);
		if (20 < newFoV && newFoV < 80)
		{
			FoV = newFoV;
		}
		
		// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	}

	void init(GLFWwindow* window)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		glfwSetCursorPosCallback(window, InPut::Cursor::motionCallback);
		InPut::Cursor::regist(*this);
		InPut::Cursor::motionCallback(window, width / 2, height / 2);

		glfwSetKeyCallback(window, InPut::KeyBoard::press_or_release_callback);
		InPut::KeyBoard::getKey(GLFW_KEY_UP).regist(m_observer_up);
		InPut::KeyBoard::getKey(GLFW_KEY_DOWN).regist(m_observer_down);
		InPut::KeyBoard::getKey(GLFW_KEY_RIGHT).regist(m_observer_right);
		InPut::KeyBoard::getKey(GLFW_KEY_LEFT).regist(m_observer_left);

		glfwSetScrollCallback(window, InPut::ScrollBar::scrollCallBack);
		InPut::ScrollBar::regist(*this);

		scrollCallBack(0);
	}

};