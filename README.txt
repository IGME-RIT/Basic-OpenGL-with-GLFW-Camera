Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Debug Camera Tutorial!
Prerequesites: 3D Cameras

In this tutorial, we will be making a FirstPersonDebug class that utilizes the 
3D camera class that we made in the previous tutorial. Our FirstPersonDebug system
will have a "camera" and a "transform". We will make them both in our constructor
	camera = new Camera(fovX, width, height, near, far);
	transform = new Transform();
We will then delete them both in the destructor
	delete camera;
	delete transform;

Our transformation has a position and a rotation, both of these will be changed
depending on keyboard and mouse input from the user. We calculate mouse 
movement in relation to window size, so that larger windows with more pixels
dont mess with the speed of the camera.
	glm::vec2 mouseMovement = mousePosition - (glm::vec2(width,height) / 2.0f);
We use position, because the position is reset to 0 each frame, so any change
in position must be the movement

Next, we get the rotation around the x-axis (vertical rotation) based on
the mouse's Y movement:
	float pitch = transform->eulerRotation.x - (int)mouseMovement.y * .001f;

Next, we clamp the camera so it does not look vertically more than 90 degrees,
or less than -90 degrees, so that the camera can't flip upside down
	if (transform->eulerRotation.x < -glm::half_pi<GLfloat>())
		pitch = -glm::half_pi<GLfloat>();
	else if (transform->eulerRotation.x > glm::half_pi<GLfloat>())
		pitch = glm::half_pi<GLfloat>();

Next, we have to get our rotation around the Y axis (horizontal rotation),
in relation to the mouse's X movement
	float yaw = transform->eulerRotation.y - (int)mouseMovement.x * .001f;

To finish rotations, we set our transformation's euler rotation based on the
vertical and horizontal rotations that we just calculated:
	transform->eulerRotation = glm::vec3(pitch, yaw, 0);

After that we use keyboard input to move the transformation in relation to
the rotation, which can be done with MoveForward and MoveRight functions of the
Transform class

After that, we upate the transformation 
	transform->Update();

Then we give our transformation to the camera
	camera->UpdateView(glm::inverse(transform->GetWorldMatrix()));

Then we create a matrix with the camera and pass it to the shader.
In main.cpp, we call this the "camMatrix", but this type of matrix is
usually called the "view" matrix, and we will call it the view matrix in
future tutorials.

Just like how the world matrix is a translation, rotation, and scale of
an object in the world, the view matrix is the translation, rotation, and 
scale of the camera. In the vertex shader, gl_Position will be
camMatrix * worldMatrix * position.

This will give you a 3D orthographic camera. In future tutorials, we will
add a projection matrix, which will make a camera look like most 3D games:
	like Halo, Call of Duty, GTA, Crash Bandicoot, and Spyro
