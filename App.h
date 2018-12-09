#pragma once

#include <GL/glew.h>

class App
{
	GLuint vertexbuffer; // This will identify our vertex buffer
	GLuint programID;

public:

	void Init();
	void Render();
};