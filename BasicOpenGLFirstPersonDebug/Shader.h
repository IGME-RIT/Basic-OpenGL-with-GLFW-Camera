/*
File: Shader.h
Author: Sanketh Bhat
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>




class Shader
{
	//OpenGL uses shader "programs"
	GLuint vprogram, fprogram; 

	//Default file names
	std::string  filenamev = "shaders/vShader.glsl";
	std::string  filenamef = "shaders/fShader.glsl";

public:
	GLuint program;
	Shader();
	Shader(std::string fnv, std::string fnf);
	~Shader();
	bool load();
	bool compile(GLenum shaderType);
	void use();
	void unload();
};

