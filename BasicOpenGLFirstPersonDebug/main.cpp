#include <iostream>
#include <vector>

// We are using the glew32s.lib
// Thus we have a define statement
// If we do not want to use the static library, we can include glew32.lib in the project properties
// If we do use the non static glew32.lib, we need to include the glew32.dll in the solution folder
// The glew32.dll can be found here $(SolutionDir)\..\External Libraries\GLEW\bin\Release\Win32
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//GLM provides new data types and math helper functions for any vector math we want to do.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "FirstPersonDebug.h"
#include "Shader.h"
//Now we want to draw multiple entities
//Each entity will need a set of vertices, 
//a vertex buffer object(VBO),
//a vertex array object(VAO)
//We use a struct as it simplifies the example.

//We've swapped from simple GLfloats to GLM's vec3, a three dimensional vector.
struct shape {
	std::vector<glm::vec3> vertices;
	GLuint VAO;
	GLuint VBO;

	//3D Vectors
	///Make changes here and see what happens!
	glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);					//The scale (size) of the shape, initlaized to 1,1,1,
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);			//Rotation in radians
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);			//Position initalized to 0
	glm::mat4 worldMatrix;								//The world matrix for this object.


};




// Variables for the Height and width of the window
const GLint WIDTH = 800, HEIGHT = 600;

glm::vec2 mousePosition = glm::vec2();

void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
	mousePosition = glm::vec2(mouseX, mouseY);
}


int main()
{
	//Initializes the glfw
	glfwInit();

	// Setting the required options for GLFW

	// Setting the OpenGL version, in this case 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Setting the Profile for the OpenGL.
	// Option of choosing between Compat and Core.
	// We choose core, because we won't be using any deprecated function from the previous versions of OpenGL
	// In other words, no backward compatibility
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Setting the forward compatibility of the application to true
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// We don't want the window to resize as of now.
	// Therefore we will set the resizeable window hint to false.
	// To make is resizeable change the value to GL_TRUE.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create the window object
	// The first and second parameters passed in are WIDTH and HEIGHT of the window we want to create
	// The third parameter is the title of the window we want to create

	// NOTE: Fourth paramter is called monitor of type GLFWmonitor, used for the fullscreen mode.
	//		 Fifth paramter is called share of type GLFWwindow, here we can use the context of another window to create this window
	// Since we won't be using any of these two features for the current tutorial we will pass nullptr in those fields
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shaders Tutorial", nullptr, nullptr);

	// We call the function glfwGetFramebufferSize to query the actual size of the window and store it in the variables.
	// This is useful for the high density screens and getting the window size when the window has resized.
	// Therefore we will be using these variables when creating the viewport for the window
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// Check if the window creation was successful by checking if the window object is a null pointer or not
	if (window == nullptr)
	{
		// If the window returns a null pointer, meaning the window creation was not successful
		// we print out the messsage and terminate the glfw using glfwTerminate()
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		// Since the application was not able to create a window we exit the program returning EXIT_FAILURE
		return EXIT_FAILURE;
	}

	// Creating a window does not make it the current context in the windows.
	// As a results if the window is not made the current context we wouldn't be able the perform the operations we want on it
	// So we make the created window to current context using the function glfwMakeContextCurrent() and passing in the Window object
	glfwMakeContextCurrent(window);

	// Enable GLEW, setting glewExperimental to true.
	// This allows GLEW take the modern approach to retrive function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup OpenGL function pointers
	if (GLEW_OK != glewInit())
	{
		// If the initalization is not successful, print out the message and exit the program with return value EXIT_FAILURE
		std::cout << "Failed to initialize GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	// Setting up the viewport
	// First the parameters are used to set the top left coordinates
	// The next two parameters specify the height and the width of the viewport.
	// We use the variables screenWidth and screenHeight, in which we stored the value of width and height of the window,
	glViewport(0, 0, screenWidth, screenHeight);


	
	glfwSetCursorPosCallback(window, mouseMoveCallback);



	//Initalizing the first entity - a simple triangle from the last example
	shape triangle;
	triangle.vertices = {
		// POSITION					//COLOR				
		glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.5f, 0.0f),  	glm::vec3(0.0f, 0.0f, 1.0f)
	};

	//Standard steps to prep something to be rendered
	glGenVertexArrays(1, &triangle.VAO);																					//Generating VAO
	glGenBuffers(1, &triangle.VBO);																							//Generating VBO
	glBindVertexArray(triangle.VAO);																						//Binding the VAO so we can manipulate it
	glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO);																		    //Binding the VBO so we can manipulate it

	//Defining VBO data
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glm::vec3)*triangle.vertices.size(), //The data size is the size of one vec3 multiplied by the number of vertices
		&triangle.vertices[0],					  //We want to start at the first index of the vector, so we point to it
		GL_STATIC_DRAW);

	//Defining our vertex data

	//Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);

	//New Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (GLvoid *)(sizeof(glm::vec3)));
	// Enable the generic vertex attribute array created above using the the function glEnableVertexAttributeArray,
	// passing in the index of the vertex attribute array



	//Unbinding both our OpenGL objects
	glBindBuffer(0, triangle.VBO);
	glBindVertexArray(0);

	//Creating, initalizing and using our shader(s)

	Shader basicShader;
	basicShader.load();
	basicShader.use();

	FirstPersonDebug* camera = new FirstPersonDebug(90, WIDTH, HEIGHT, 0.1f, 100.f);
	camera->transform->position.z = 2.f;
	
	GLuint worldULoc = glGetUniformLocation(basicShader.program, (char*)"worldMatrix");
	GLuint camULoc = glGetUniformLocation(basicShader.program, (char*)"camMatrix");

	// This is the game loop, the game logic and render part goes in here.
	// It checks if the created window is still open, and keeps performing the specified operations until the window is closed
	while (!glfwWindowShouldClose(window))

	{	// Calculate delta time.
		float dt = glfwGetTime();
		// Reset the timer.
		glfwSetTime(0);


		// Checking for events/inputs
		glfwPollEvents();

		// Specifies the RGBA values which will be used by glClear to clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Clears contents of a screen to a preset value, previously selected, by passing in what buffer we want to clear
		// In this case we are clearing the color buffer. Thus setting a background color to the color specified previously in glClearColor
		glClear(GL_COLOR_BUFFER_BIT);



		camera->Update(window, WIDTH, HEIGHT, mousePosition, dt);
		glUniformMatrix4fv(camULoc, 1, GL_FALSE, &camera->GetCameraMatrix()[0][0]);

		//WORLD MATRIX CALCULATION
		//We want to create a 4x4 matrix for our triangle
		//This matrix will have 3 main components
		//Translation matrix - tells us where to move our vertices
		//Rotation matrix -	tells us how to rotate the model
		//Scale matrix - tells us the size of the model
		//World Matrix = Translation Matrix* Rotation Matrix *Scale Matrix  ***IN ORDER***
		//With matrix multiplication, order matters.
		//Otherwise we manipulate vertices incorrectly
		
		triangle.rotation.y += 0.01f;

		triangle.worldMatrix =
			glm::translate(triangle.position) *													//creates a translation matrix taking our position as the parameter
			glm::yawPitchRoll(triangle.rotation.y, triangle.rotation.x, triangle.rotation.z)*    //creates a rotation matrix based on a yaw(y rotation), pitch (x rotation) and roll (z rotation)
			glm::scale(triangle.scale);															//creates a scale matrix taking our scale vector


		glUniformMatrix4fv(worldULoc, 1, GL_FALSE, &triangle.worldMatrix[0][0]);

		// Draw OpenGL stuff
		//Drawing our triangle by binding it's VAO
		glBindVertexArray(triangle.VAO);
		glDrawArrays(GL_TRIANGLES, 0, triangle.vertices.size());
		glBindVertexArray(0);	//Unbind after we're finished drawing this entity.



		// Swaps the front and back buffers of the specified window
		glfwSwapBuffers(window);
	}

	// Delete the vertex array object, passing in the number of the vertex arrays objects stored in the the array (VAO)
	glDeleteVertexArrays(1, &triangle.VAO);


	// Delete the number of buffer objects passed in the array buffer.
	glDeleteBuffers(1, &triangle.VBO);




	// Terminate all the stuff related to GLFW and exit the program using the return value EXIT_SUCCESS
	glfwTerminate();

	return EXIT_SUCCESS;
}