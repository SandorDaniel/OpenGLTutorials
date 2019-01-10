//#define sd_debugger
#include "Debug.h"

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix()
{
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float FoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) // time independent method
{
	float newFoV = FoV - 5 * static_cast<float>(yoffset);
	if (20 < newFoV && newFoV < 80)
	{
		FoV = newFoV;
	}
}


GLFWwindow* Cursor::p_win = nullptr;
double Cursor::xpos = 0;
double Cursor::ypos = 0;

void cursor_motion_callback(GLFWwindow* window, double xpos, double ypos)
{
	Cursor::motion_callback(window, xpos, ypos);
}


std::map<int, KeyBoard::Key> KeyBoard::active_keys{};

void key_event_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyBoard::event_callback(window, key, scancode, action, mods);
}



void computeMatricesFromInputs(GLFWwindow* window){

	#pragma region Window

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	#pragma endregion

	#pragma region Cursor - Orientation

	// Get mouse position
	static bool is_call_back_method_set_up_for_mouse_motion = false;
	if (!is_call_back_method_set_up_for_mouse_motion)
	{
		glfwSetCursorPosCallback(window, cursor_motion_callback);
		is_call_back_method_set_up_for_mouse_motion = true;
	}

	// TODO: it may be skipped later 
	static bool is_mouse_inited = false;
	if (!is_mouse_inited)
	{
		Cursor::motion_callback(window, width / 2, height / 2);

		is_mouse_inited = true;
	}

	// Compute new orientation
	horizontalAngle += mouseSpeed * static_cast<float>(width / 2 - Cursor::getXPos());
	verticalAngle += mouseSpeed * static_cast<float>(height / 2 - Cursor::getYPos());
	
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	#pragma endregion

	#pragma region Keyboard - Position
	
	static bool is_call_back_method_set_up_for_keyboard_events = false;
	if (!is_call_back_method_set_up_for_keyboard_events)
	{
		glfwSetKeyCallback(window, key_event_callback);
		is_call_back_method_set_up_for_keyboard_events = true;
	}

	// Move forward
	position += direction * static_cast<float>(KeyBoard::getKey(GLFW_KEY_UP).getTimePressed()) * speed;
	// Move backward
	position -= direction * static_cast<float>(KeyBoard::getKey(GLFW_KEY_DOWN).getTimePressed()) * speed;
	// Strafe right
	position += right * static_cast<float>(KeyBoard::getKey(GLFW_KEY_RIGHT).getTimePressed()) * speed;
	// Strafe left
	position -= right * static_cast<float>(KeyBoard::getKey(GLFW_KEY_LEFT).getTimePressed()) * speed;

	#pragma endregion

	#pragma region ScrollBar - Angle

	static bool is_call_back_method_set_up_for_scrolling = false;
	if (!is_call_back_method_set_up_for_scrolling)
	{
		glfwSetScrollCallback(window, scroll_callback);
		is_call_back_method_set_up_for_scrolling = true;
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

}