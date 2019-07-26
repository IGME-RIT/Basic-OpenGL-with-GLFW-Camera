/*
File: Shader.cpp
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


//This loads GLSL shaders for our use
//Feel free to just copy the header and source files and use them
//Make sure you know whats going on in the file, but no need to remember how to replicate it.
//Only handles vertex shaders and fragment(pixel) shaders.
#include "Shader.h"



Shader::Shader()
{
}

//Overloaded constructor, can be used when we want to use shaders other than the default ones.
Shader::Shader(std::string fnv, std::string fnf)
{
	filenamev = fnv;
	filenamef = fnf;
}


Shader::~Shader()
{
}

bool Shader::load()
{
	//Storing our compliation results for debuggi ng
	bool resultV = compile(GL_VERTEX_SHADER);
	bool resultF = compile(GL_FRAGMENT_SHADER);

	//If either shader failed to comple, return false
	if (resultV == false || resultF == false)
		return false;

	//Attaching shaders to the program
	program = glCreateProgram();				//Creating a base OpenGL program
	glAttachShader(program, vprogram);			//Attaching the compiled vertex shader
	glAttachShader(program, fprogram);			//Attaching the compiled frag shader
	glLinkProgram(program);						//Links shader program object so we can access it later

	//Debugging Program
	GLint checkStatus = 0;

	//This line returns a parameter from a shader program object, specified by the second parameter
	glGetProgramiv(program, GL_LINK_STATUS, &checkStatus);	//Check if the shader program linked correctly, stores result in the last parameter. Second 

	//If it's false, it didn't link properly so we have to debug it
	if (checkStatus == GL_FALSE)
	{
		GLint length = 0;
		//Same line as above, except this time we want the log data length so we can output it
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar* logData = new GLchar[length + 1];

		//This line stores the information log from the shader program into a specified variable.
		glGetProgramInfoLog(program, length, 0, logData);
		std::cout << logData << "\n";

		//Since we failed to create the shader program, delete any created objects
		glDeleteProgram(program);
		delete[] logData;

		//Print an error messavge and return
		std::cout << "Error linking program file\n";
		return false;
	}

	return true;
}

bool Shader::compile(GLenum shaderType)
{
	std::ifstream inFile;
	GLint checkStatus;
	GLuint vIndex;
	GLuint fIndex;

	//Find the GLSL file, second paramter specifies to open the file in binary format
	if (shaderType == GL_VERTEX_SHADER)
		inFile.open(filenamev, std::ios::binary);
	else if (shaderType == GL_FRAGMENT_SHADER)
		inFile.open(filenamef, std::ios::binary);

	//If we managed to open the file, continue
	if (inFile.is_open())
	{
		//Getting binary file length
		inFile.seekg(0, std::ios::end);
		int length = (int)inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		//Reading contents
		char* fileCont = new char[length + 1];
		inFile.read(fileCont, length);

		//Null pointer
		fileCont[length] = '\0';

		inFile.close();


		//Handling vertex shader compilation
		if (shaderType == GL_VERTEX_SHADER)
		{
			//Create a vertex shader program and store it
			vprogram = vIndex = glCreateShader(GL_VERTEX_SHADER);

			//Replace the defualt shader program with the shader file we read
			glShaderSource(vIndex, 1, &fileCont, 0);
			glCompileShader(vIndex);

			//Similar debug check as above
			glGetShaderiv(vIndex, GL_COMPILE_STATUS, &checkStatus);

			if (checkStatus == 0)
			{
				GLint length = 0;
				glGetShaderiv(vIndex, GL_INFO_LOG_LENGTH, &length);

				GLchar* logData = new GLchar[length + 1];
				glGetShaderInfoLog(vIndex, length, 0, logData);
				std::cout << logData << "\n";
				glDeleteShader(vIndex);
				delete[] logData;


			}
			else
			{
				delete[] fileCont;
				return true;
			}


		}
		//Handling fragment shader compilation, same as above but using fprogram instead of vprogram
		else if (shaderType == GL_FRAGMENT_SHADER)
		{
			fprogram = fIndex = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(fIndex, 1, &fileCont, 0);
			glCompileShader(fIndex);
			glGetShaderiv(fIndex, GL_COMPILE_STATUS, &checkStatus);
			if (checkStatus == 0)
			{
				GLint length = 0;
				glGetShaderiv(fIndex, GL_INFO_LOG_LENGTH, &length);

				GLchar* logData = new GLchar[length + 1];
				glGetShaderInfoLog(fIndex, length, 0, logData);
				std::cout << logData << "\n";
				glDeleteShader(fIndex);
				delete[] logData;


			}
			else
			{
				delete[] fileCont;
				return true;
			}

		}


	}

	std::cout << "Error opening shader file\n";
	return false;


}

void Shader::use()
{
	//Specifies what shader program to use while rendering
	glUseProgram(program);
}

//Delete created shader programs
void Shader::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram);

	program = 0;
	vprogram = 0;
	fprogram = 0;
}
