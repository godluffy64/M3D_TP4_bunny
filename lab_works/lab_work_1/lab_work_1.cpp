#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

using namespace std;

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1()
	{
		glDeleteProgram( program );
		glDeleteBuffers( 1, &VBO_id );
		glDisableVertexArrayAttrib( VAO_id, 0 );
		glDeleteVertexArrays( 1, &VAO_id );
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );

		GLuint vshader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fshader = glCreateShader( GL_FRAGMENT_SHADER );

		const GLchar * vSrc = vertexShaderStr.c_str();
		const GLchar * fSrc = fragmentShaderStr.c_str();

		glShaderSource( vshader, 1, &vSrc, NULL );
		glShaderSource( fshader, 1, &fSrc, NULL );

		glCompileShader( vshader );
		glCompileShader( fshader );

		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vshader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vshader, sizeof( log ), NULL, log );
			glDeleteShader( vshader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}

		// Check for compilation errors
		glGetShaderiv( fshader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fshader, sizeof( log ), NULL, log );
			glDeleteShader( fshader );
			std::cerr << "Error compiling fragment shader: " << log << std::endl;
			return false;
		}

		program = glCreateProgram();

		glAttachShader( program, vshader );
		glAttachShader( program, fshader );
		glLinkProgram( program );

		// Check if link is ok.
		GLint linked;
		glGetProgramiv( program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}

		glDeleteShader( vshader );
		glDeleteShader( fshader );

		triangle_vertices = { { -0.5, 0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 } };

		/* triangle.push_back( glm::vec2( -0.5 * 1280, 0.5 * 720 ) );
		triangle.push_back( glm::vec2( 0.5 * 1280, 0.5 * 720 ) );
		triangle.push_back( glm::vec2( 0.5 * 1280, -0.5 * 720 ) );*/

		glCreateBuffers( 1, &VBO_id );

		glNamedBufferData(
			VBO_id, triangle_vertices.size() * sizeof( glm::vec2 ), triangle_vertices.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &VAO_id );
		glEnableVertexArrayAttrib( VAO_id, 0 );
		glVertexArrayAttribFormat( VAO_id, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( VAO_id, 0, VBO_id, 0, sizeof( glm::vec2 ) );
		glVertexArrayAttribBinding( VAO_id, 0, 0 );
		glUseProgram( program );

		// printf( "%d %d", sizeof( triangle ), sizeof(float));
		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBindVertexArray( VAO_id );
		glDrawArrays( GL_TRIANGLES, 0, triangle_vertices.size() );
		glBindVertexArray( 0 );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event ) {}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
