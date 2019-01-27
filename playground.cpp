// TODO: correct lights
// TODO: https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/
// TODO: https://www.khronos.org/opengl/wiki/Common_Mistakes
// TODO: adding aspects to IBO and (depend on IBO) to VAO if it is neccesarry. 

// TODO: FRAGMENT SHADERS LIBRARY (FSL)
//       In order to avoid fragment shader's code-redundancy (so in order to make our fragment shaders more maintainable)
//       write separate header files for every reused (or willing to be reused) functionality and encapsulate and hide their implementation
//       in separate functions.
//       If some functionalities are different only in some types (including type-qualifiers like uniform, in, ...) then ad-hoc generalize
//       them with macro directives.
//       Treat global vaiables as the interface of the shader.
//       Shader interface - API interface
//       (its the responsibility of API to adapt shader interface as an interface of the implementation or a part of the implementation of a feature
//        of the api)

//TODO general regenerate:
// - Mini API for specific but not concrete shaders ie ShaderAdaptors or something lie this...
// - kodredundancia - vizsgalat ill.csokkentes(template, inheritance, class, function)
// - helyes kulso eroforras kezeles
// - muveleti ill. memoria hatekonysag optimalizalas
// - forditasi fuggoseg csokkentes
// - robosztussag(helyes hivasi sorrend-, elemszamegyezes- kikenyszerites; learning path)
// - kivetelbiztossag (eg.: copy-swap idiom https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom, https://stackoverflow.com/questions/6687388/why-do-some-people-use-swap-for-move-assignments / Kerrek SB's answer)
// - szalbiztossag, vagy mi...
// - eliminating C-style and function-like conversions
// - eliminaiting magic numbers
// - nullptr instead of 0 and NULL
// - intialization for all variables
// - inheritance chencking, final, virtual destructor, virtual/pure virtual functions, protected members
// - explicit initialization (prefer uniform)
// - function(Type*) -> function(Type&)
// - user-defined literlas for phisycal values, and find out if separate types for separate phisycal dimensions are reasonable.
// - find out which is better, store vertex datas in vec3s or in vec4s
// - change c-like string to std::string if it is possible

//#define sd_debugger
#include "Debug.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <GL/glew.h>

#ifdef _WIN32
#include <windows.h>
#include "wglext.h" // TODO...
#endif

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
	window = glfwCreateWindow(WIDTH, HEIGHT, "My Program", NULL, NULL);
	if( window == NULL ){
		std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	{
		char* title = new char[strlen("MyProgram - OGL version ") + strlen(reinterpret_cast<const char*>(glGetString(GL_VERSION))) + 1];
		title[0] = '\0';

		strcat(title, "MyProgram - OGL version ");
		strcat(title, reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		glfwSetWindowTitle(window, title);
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}

	#pragma region VSYNC
		#ifdef _WIN32
		{
			//https://stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl

			// this is pointer to function which returns pointer to string with list of all wgl extensions
			PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT = nullptr;

			// determine pointer to wglGetExtensionsStringEXT function
			wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

			if (strstr(wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") == nullptr)
			{
				// string was not found ie WGL_EXT_swap_control is not supported
				throw; // TODO
			}

			// Declare function pointer
			PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

			// Extension is supported, init pointer
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

			// Enabling VSync
			wglSwapIntervalEXT(1);
		}
		#endif
	#pragma endregion

	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	//glfwPollEvents(); // TODO: Is it really necessary?
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	App app(window);

	app.init();
	 
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
	
	glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera ...
	glCullFace(GL_BACK); // ... ie let the GPU check if the camera is behind => 2 times less triangles on average
	
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Dark blue background

	do{
		#ifdef sd_debugger
		sdd::debugSecondsPerFrame(1.0);
		#endif

		app.upDate();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		app.render();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	app.clean();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	#ifdef sd_debugger
	std::cin.get();
	#endif

	return 0;
}

