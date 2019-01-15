#ifdef sd_debugger
#ifndef DEBUG_H
#define DEBUG_H


#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <map>
#include <exception>
#include <typeinfo>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/texture.hpp>

#include "App.h"
#include "Shader.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "TEX.h"

namespace sdd // stands for Sandor Daniels Debugger
{
	static std::ostream& dout = std::cout;
	static double before, after;
}

#endif
#endif
