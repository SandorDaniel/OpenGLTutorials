#pragma once

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
#include <algorithm>


#include <GL/glew.h>


#include <GLFW/glfw3.h>


#define GLM_META_PROG_HELPERS
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



namespace sdd // stands for Sandor Daniel's Debugger
{

	static std::ostream& dout = std::cout;


	class Timer final
	{

		double previous_time = 0;
	
	public:

		double operator()()
		{
			glFinish(); // glFinish does not return until the effects of all previously called GL commands are complete.

			double actual_time = glfwGetTime();
			double elapsed_time = actual_time - previous_time;
			previous_time = actual_time;

			return elapsed_time;
		}

	};


	static void debugSecondsPerFrame(double time_interval_in_seconds_for_avg_sfp)
	{     
		static double lastTime = glfwGetTime();
		static int nbFrames = 0;
		static Timer timer;
		static double highest_spf = 0;

		double spf = timer();
		if (spf > highest_spf)
		{
			highest_spf = spf;
		}

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= time_interval_in_seconds_for_avg_sfp) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			dout << "AVG: " << time_interval_in_seconds_for_avg_sfp / static_cast<double>(nbFrames) << "ms/frame" << std::endl;
			dout << "MAX: " << highest_spf                                                          << "ms/frame" << std::endl;
			nbFrames = 0;
			lastTime += time_interval_in_seconds_for_avg_sfp;
			highest_spf = 0;
		}
	}

	template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision>
	static std::ostream& operator<<(std::ostream& os, const TVec<CoordType, precision>& vec)
	{
		for (int i = 0; i < (TVec<CoordType, precision>::components) - 1; ++i)
		{
			os << vec[i] << ' ';
		}
		os << vec[TVec<CoordType, precision>::components - 1];

		return os;
	}


}

#endif

#endif
