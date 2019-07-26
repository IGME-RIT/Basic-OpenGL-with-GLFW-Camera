// NOTE: before proceeding further, it is recommended you learn about the programmable graphics pipeline.
// To know more about the programmable graphics pipeline please visit this website:
// http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-1:-The-Graphics-Pipeline.html

// The vertex shader is used to transform the attributes of vertices such as color, texture, position
// and the direction from the original color space to the display space. It allows the original objects
// to be distorted or reshaped in any manner


#version 400
// Vertex attribute for position, which is at location 0
layout(location=0) in vec3 position;
// Vertex attribute for color, which is at location 1
layout(location=1) in vec3 color;

uniform mat4 camMatrix;
uniform mat4 worldMatrix;



// The output color from vertex shader which will feed into fragment shader
out vec3 outColor;



void main()
{
 // gl_Position is inbuilt vertex shader output variable of type vec4.
	
	gl_Position  = camMatrix * worldMatrix * vec4(position,1);

	//gl_Position  = worldMatrix * vec4(position,1);

	// store the color in ourColor output variable
	outColor = color;


  
}
