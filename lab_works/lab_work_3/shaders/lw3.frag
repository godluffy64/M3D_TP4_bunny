#version 450

layout( location = 0 ) out vec4 fragColor;


in vec4 color;
uniform float uluminosity;
void main()
{
	fragColor = color;
}
