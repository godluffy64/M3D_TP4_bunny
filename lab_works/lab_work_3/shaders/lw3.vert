#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexColor;

out vec4 color;

uniform mat4 umodel_rotate;

uniform mat4 umodel_matrix;
uniform mat4 uView_matrix;
uniform mat4 uProjection_matrix;
uniform mat4 uModelViewProjection_matrix;


void main()
{
	gl_Position = uModelViewProjection_matrix * vec4(aVertexPosition,  1.f);
	color = vec4(aVertexColor, 1.f);
}