#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "InPuts.h"

#include <iostream>

void computeMatricesFromInputs(GLFWwindow*);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif
