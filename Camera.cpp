//#define sd_debugger
#include "Debug.h"

#include <limits>
#include <exception>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "InPuts.h"
#include "Camera.h"




Camera::Camera(const Light& LIGHT)
{
	const glm::vec2 HOR_VER = getVerticalAndHorizontalAngles(LIGHT.getDir());
	setHorizontalAngle(HOR_VER.x);
	setVerticalAngle(HOR_VER.y);
}


glm::mat4 getView(const Camera& CAM)
{
	glm::vec3 position = CAM.getPos();
	glm::vec3 direction = getDir(CAM);
	glm::vec3 right = getRight(CAM);
	glm::vec3 up = glm::cross(right, direction);

	return glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
}


glm::mat4 getPerspectiveProj(const Camera& CAM)
{
	if (!CAM.getWin())
	{
		throw; // TODO
	}

	int win_width, win_height;
	glfwGetWindowSize(CAM.getWin(), &win_width, &win_height);

	return glm::perspective(
		CAM.getFov(),
		static_cast<float>(win_width) / static_cast<float>(win_height),
		CAM.getNear(), CAM.getFar());
}


std::vector<glm::vec3> getFrustum(const Camera& CAM)
{
	if (!CAM.getWin())
	{
		throw; // TODO
	}

	int win_width, win_height;
	glfwGetWindowSize(CAM.getWin(), &win_width, &win_height);

	float near = CAM.getNear();
	float far  = CAM.getFar();

	float half_far_width  = far  * glm::tan(CAM.getFov() / 2.0f);
	float half_near_width = near * glm::tan(CAM.getFov() / 2.0f);

	float half_far_height  = (half_far_width  * win_height) / win_width;
	float half_near_height = (half_near_width * win_height) / win_width;

	glm::mat4 inv_V = glm::inverse(getView(CAM));

	return std::vector<glm::vec3>{
		glm::vec3(inv_V * glm::vec4(-half_near_width, -half_near_height, -near, 1.0f)),
		glm::vec3(inv_V * glm::vec4(+half_near_width, -half_near_height, -near, 1.0f)),
		glm::vec3(inv_V * glm::vec4(+half_near_width, +half_near_height, -near, 1.0f)),
		glm::vec3(inv_V * glm::vec4(-half_near_width, +half_near_height, -near, 1.0f)),
		glm::vec3(inv_V * glm::vec4(-half_far_width,  -half_far_height,  -far,  1.0f)),
		glm::vec3(inv_V * glm::vec4(+half_far_width,  -half_far_height,  -far,  1.0f)),
		glm::vec3(inv_V * glm::vec4(+half_far_width,  +half_far_height,  -far,  1.0f)),
		glm::vec3(inv_V * glm::vec4(-half_far_width,  +half_far_height,  -far,  1.0f))
	};
}


// TODO: set parameters according to camera data
glm::mat4 getOrthogonaleProj(const Camera& CAM, const float left, const float right, const float bottom, const float top) // TODO find out if float for left, right, bottom and top is appropriate
{
	if (!CAM.getWin())
	{
		throw; // TODO
	}

	int win_width, win_height;
	glfwGetWindowSize(CAM.getWin(), &win_width, &win_height);

	if (right - left > win_width || top - bottom > win_height)
	{
		throw; // TODO
	}

	return glm::ortho(
		left, right,
		bottom, top,
		CAM.getNear(), CAM.getFar());
}


void InPutObserverCamera::KeyObserver::releaseCallBack()
{
	glm::vec3 direction = getDir(*m_p_cam);
	glm::vec3 right = getRight(*m_p_cam);
	glm::vec3 up = glm::cross(right, direction);

	switch (m_direction)
	{
	case Direction::FORWARD:
		m_p_cam->setPos(
			m_p_cam->getPos()
			+ static_cast<float>(glfwGetTime() - m_time_last_pressed <= std::numeric_limits<float>::max() ?
				glfwGetTime() - m_time_last_pressed :
				throw std::domain_error("Camera.cpp: elapsed time is to big to be represented as a float"))
			* m_p_cam->m_speed * direction);
		break;
	case Direction::BACKWARD:
		m_p_cam->setPos(
			m_p_cam->getPos()
			- static_cast<float>(glfwGetTime() - m_time_last_pressed <= std::numeric_limits<float>::max() ?
				glfwGetTime() - m_time_last_pressed :
				throw std::domain_error("Camera.cpp: elapsed time is to big to be represented as a float"))
			* m_p_cam->m_speed * direction);
		break;
	case Direction::RIGHT:
		m_p_cam->setPos(
			m_p_cam->getPos()
			+ static_cast<float>(glfwGetTime() - m_time_last_pressed <= std::numeric_limits<float>::max() ?
				glfwGetTime() - m_time_last_pressed :
				throw std::domain_error("Camera.cpp: elapsed time is to big to be represented as a float"))
			* m_p_cam->m_speed * right);
		break;
	case Direction::LEFT:
		m_p_cam->setPos(
			m_p_cam->getPos()
			- static_cast<float>(glfwGetTime() - m_time_last_pressed <= std::numeric_limits<float>::max() ? 
				glfwGetTime() - m_time_last_pressed : 
				throw std::domain_error("Camera.cpp: elapsed time is to big to be represented as a float")) 
			* m_p_cam->m_speed * right);
		break;
	}
}


InPutObserverCamera::InPutObserverCamera() // TODO: parametrize methods
{
	m_observer_up.set(this, Direction::FORWARD);
	m_observer_down.set(this, Direction::BACKWARD);
	m_observer_right.set(this, Direction::RIGHT);
	m_observer_left.set(this, Direction::LEFT);
}

void InPutObserverCamera::init(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glfwSetCursorPosCallback(window, InPut::Cursor::motionCallback);
	InPut::Cursor::regist(*this);
	InPut::Cursor::motionCallback(window, width / 2, height / 2);

	glfwSetMouseButtonCallback(window, InPut::MouseButtons::actionCallback);
	InPut::MouseButtons::regist(*this);

	glfwSetKeyCallback(window, InPut::KeyBoard::press_or_release_callback);
	InPut::KeyBoard::getKey(GLFW_KEY_UP).regist(m_observer_up);
	InPut::KeyBoard::getKey(GLFW_KEY_DOWN).regist(m_observer_down);
	InPut::KeyBoard::getKey(GLFW_KEY_RIGHT).regist(m_observer_right);
	InPut::KeyBoard::getKey(GLFW_KEY_LEFT).regist(m_observer_left);

	glfwSetScrollCallback(window, InPut::ScrollBar::scrollCallBack);
	InPut::ScrollBar::regist(*this);
}

void InPutObserverCamera::upDate(GLFWwindow* window)
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
	//	sin(horizontalAngle - glm::pi<float>() / 2.0f),
	//	0,
	//	cos(horizontalAngle - glm::pi<float>() / 2.0f)
	//);

	//// Up vector
	//glm::vec3 up = glm::cross(right, direction);

	//#pragma endregion

	//#pragma region Keyboard - Position
	//const glm::mat4& getViewMatrix() const
	//{
	//	return ViewMatrix;
	//}
	//const glm::mat4& getPerspectiveProjectionMatrix() const
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

void InPutObserverCamera::motionCallBack(GLFWwindow* p_win, double xpos, double ypos)
{
	if (m_direction_is_under_controll)
	{
		int width, height;
		glfwGetWindowSize(p_win, &width, &height);

		// Compute new orientation
		setHorizontalAngle(getHorizontalAngle() + m_mouse_speed * static_cast<float>(width / 2 - xpos));
		setVerticalAngle(getVerticalAngle() + m_mouse_speed * static_cast<float>(height / 2 - ypos));

		glfwSetCursorPos(p_win, width / 2, height / 2); // Enable unlimited movement, if camera direction is under control.
	}
}

void InPutObserverCamera::pressCallBack(GLFWwindow* p_win)
{
	glfwSetInputMode(p_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int width, height;
	glfwGetWindowSize(p_win, &width, &height);
	glfwSetCursorPos(p_win, width / 2, height / 2); // Don't let the camera direction be changed with Dirac-delta

	m_direction_is_under_controll = true;
}

void InPutObserverCamera::releaseCallBack(GLFWwindow* p_win)
{
	glfwSetInputMode(p_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	m_direction_is_under_controll = false;
};

void InPutObserverCamera::scrollCallBack(double yoffset)
{
	float newFoV = getFov() - (glm::pi<float>() / 36.0f) * static_cast<float>(yoffset <= std::numeric_limits<float>::max() ? yoffset : throw std::domain_error("Camera.cpp: scrollbars yoffset is to big to be represented as a float"));
	if (glm::pi<float>() / 9.0f < newFoV && newFoV < (4 * glm::pi<float>()) / 9.0f)
	{
		setFov(newFoV);
	}
}
