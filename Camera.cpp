//#define sd_debugger
#include "Debug.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "InPuts.h"
#include "Camera.h"



void Camera::KeyObserver::releaseCallBack()
{
	glm::vec3 direction = m_P_cam->getDir();
	glm::vec3 right = m_P_cam->getRight();
	glm::vec3 up = glm::cross(right, direction);

	switch (m_direction)
	{
	case Direction::FORWARD:
		m_P_cam->m_position += static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->m_speed * direction;
		break;
	case Direction::BACKWARD:
		m_P_cam->m_position -= static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->m_speed * direction;
		break;
	case Direction::RIGHT:
		m_P_cam->m_position += static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->m_speed * right;
		break;
	case Direction::LEFT:
		m_P_cam->m_position -= static_cast<float>(glfwGetTime() - m_time_last_pressed) * m_P_cam->m_speed * right;
		break;
	}
}


Camera::Camera(GLFWwindow* window) : m_p_win(window) // TODO: parametrize methods
{
	m_observer_up.set(this, Direction::FORWARD);
	m_observer_down.set(this, Direction::BACKWARD);
	m_observer_right.set(this, Direction::RIGHT);
	m_observer_left.set(this, Direction::LEFT);
}

void Camera::init(GLFWwindow* window)
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
}

void Camera::upDate(GLFWwindow* window)
{
	////#pragma region Time

	//// glfwGetTime is called only once, the first time this function is called
	//static double lastTime = glfwGetTime();

	//// Compute time difference between current and last frame
	//double currentTime = glfwGetTime();
	//float deltaTime = float(currentTime - lastTime);

	////#pragma endregion

	//#pragma region Window

	//int width, height;
	//glfwGetWindowSize(window, &width, &height);

	//#pragma endregion

	//#pragma region Cursor - Orientation

	//// Get mouse position
	//static bool is_call_back_method_set_up_for_mouse_motion = false;
	//if (!is_call_back_method_set_up_for_mouse_motion)
	//{
	//	glfwSetCursorPosCallback(window, InPut::Cursor::motionCallback);
	//	is_call_back_method_set_up_for_mouse_motion = true;
	//}

	//// TODO: it may be skipped later 
	//static bool is_mouse_inited = false;
	//if (!is_mouse_inited)
	//{
	//	InPut::Cursor::motionCallback(window, width / 2, height / 2);
	//	InPut::Cursor::regist(*this);
	//	is_mouse_inited = true;
	//}

	////// Compute new orientation
	////horizontalAngle += mouseSpeed * static_cast<float>(width / 2 - InPut::Cursor::getXPos());
	////verticalAngle += mouseSpeed * static_cast<float>(height / 2 - InPut::Cursor::getYPos());

	//// Direction : Spherical coordinates to Cartesian coordinates conversion
	//glm::vec3 direction(
	//	cos(verticalAngle) * sin(horizontalAngle),
	//	sin(verticalAngle),
	//	cos(verticalAngle) * cos(horizontalAngle)
	//);

	//// Right vector
	//glm::vec3 right = glm::vec3(
	//	sin(horizontalAngle - 3.14f / 2.0f),
	//	0,
	//	cos(horizontalAngle - 3.14f / 2.0f)
	//);

	//// Up vector
	//glm::vec3 up = glm::cross(right, direction);

	//#pragma endregion

	//#pragma region Keyboard - Position
	//const glm::mat4& getViewMatrix() const
	//{
	//	return ViewMatrix;
	//}
	//const glm::mat4& getProjectionMatrix() const
	//{
	//	return ProjectionMatrix;
	//}

	//static bool is_call_back_method_set_up_for_keyboard_events = false;
	//if (!is_call_back_method_set_up_for_keyboard_events)
	//{
	//	glfwSetKeyCallback(window, InPut::KeyBoard::press_or_release_callback);
	//	InPut::KeyBoard::getKey(GLFW_KEY_UP).regist(m_observer_up);
	//	InPut::KeyBoard::getKey(GLFW_KEY_DOWN).regist(m_observer_down);
	//	InPut::KeyBoard::getKey(GLFW_KEY_RIGHT).regist(m_observer_right);
	//	InPut::KeyBoard::getKey(GLFW_KEY_LEFT).regist(m_observer_left);
	//	is_call_back_method_set_up_for_keyboard_events = true;
	//}

	//// Move forward
	//position += direction * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_UP).getTimePressed()) * speed;
	//// Move backward
	//position -= direction * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_DOWN).getTimePressed()) * speed;
	//// Strafe right
	//position += right * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_RIGHT).getTimePressed()) * speed;
	//// Strafe left
	//position -= right * static_cast<float>(InPut::KeyBoard::getKey(GLFW_KEY_LEFT).getTimePressed()) * speed;

	//#pragma endregion

	//#pragma region ScrollBar - Angle

	//static bool is_call_back_method_set_up_for_scrolling = false;
	//if (!is_call_back_method_set_up_for_scrolling)
	//{
	//	glfwSetScrollCallback(window, InPut::ScrollBar::scrollCallBack);
	//	InPut::ScrollBar::regist(*this);
	//	is_call_back_method_set_up_for_scrolling = true;
	//}

	//#pragma endregion

	//// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	//// Camera matrix
	//ViewMatrix = glm::lookAt(
	//	position,           // Camera is here
	//	position + direction, // and looks here : at the same position, plus "direction"
	//	up                  // Head is up (set to 0,-1,0 to look upside-down)
	//);

}

void Camera::motionCallBack(GLFWwindow* p_win, double xpos, double ypos)
{
	int width, height;
	glfwGetWindowSize(p_win, &width, &height);

	// Compute new orientation
	m_horizontal_angle += m_mouse_speed * static_cast<float>(width / 2 - xpos);
	m_vertical_angle += m_mouse_speed * static_cast<float>(height / 2 - ypos);
}

void Camera::scrollCallBack(double yoffset)
{
	float newFoV = m_fov - 5 * static_cast<float>(yoffset);
	if (20 < newFoV && newFoV < 80)
	{
		m_fov = newFoV;
	}
}
