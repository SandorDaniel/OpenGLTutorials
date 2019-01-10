//Altalanos TODO :
// - kodredundancia - vizsgalat ill.csokkentes(template, inheritance, class, function)
// - helyes kulso eroforras kezeles
// - muveleti ill. memoria hatekonysag optimalizalas
// - forditasi fuggoseg csokkentes
// - robosztussag(helyes hivasi sorrend-, elemszamegyezes- kikenyszerites; learning path)
// - kivetelbiztossag
// - szalbiztossag, vagy mi...
// - eliminating C-style and function-like conversions
// - eliminaiting magic numbers
// - nullptr instead of 0 and NULL
// - intialization for all variables
// - inheritance chencking, final, virtual destructor, virtual/pure virtual functions, protected members

//#define sd_debugger
#include "Debug.h"

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "App.h"


int main( void )
{
	GLFWwindow* window;

	// Initialise GLFW
	if( !glfwInit() )
	{
		std::cerr << "Failed to initialize GLFW\n" << std::endl;
		std::cin.get();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	const int WIDTH = 1024, HEIGHT = 768;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Tutorial 0 - Keyboard and Mouse", NULL, NULL);
	if( window == NULL ){
		std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	//glfwPollEvents(); // TODO: Is it really necessary?
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	App app(window);

	app.Init();
	 
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
	glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera ...
	glCullFace(GL_BACK); // ... ie let the GPU check if the camera is behind => 2 times less triangles on average
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Dark blue background

	do{

		app.Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		app.Render();

		// Swap buffers
		glfwSwapBuffers(window);
		KeyBoard::reSet();
		Cursor::reSetPosition();
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	#ifdef sd_debugger
	std::cin.get();
	#endif

	return 0;
}

