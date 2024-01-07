#include "lab_work_2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "utils/random.hpp"
#include <iostream>

using namespace std;

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		glDeleteProgram( program );
		glDeleteBuffers( 1, &vboPos );
		glDeleteBuffers( 1, &vboCol );
		glDeleteBuffers( 1, &ebo );
		glDisableVertexArrayAttrib( vao, 0 );
		glDeleteVertexArrays( 1, &vao );
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 2..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw2.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );

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

		triangle_vertices = { { -0.5, 0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 }, { -0.5, -0.5 } };
		indices			  = { 0, 1, 2, 3, 2, 0 };
		color_points
			= { { 1.f, 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f, 1.f }, { 0.f, 0.f, 1.f, 1.f }, { 1.f, 0.f, 1.f, 1.f } };

		glCreateVertexArrays( 1, &vao );

		// vboPos

		glCreateBuffers( 1, &vboPos );
		glNamedBufferData(
			vboPos, triangle_vertices.size() * sizeof( glm::vec2 ), triangle_vertices.data(), GL_STATIC_DRAW );
		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao, 0, vboPos, 0, sizeof( glm::vec2 ) );
		glVertexArrayAttribBinding( vao, 0, 0 );

		// vboCol

		glCreateBuffers( 1, &vboCol );
		glNamedBufferData( vboCol, color_points.size() * sizeof( glm::vec4 ), color_points.data(), GL_STATIC_DRAW );
		glEnableVertexArrayAttrib( vao, 1 );
		glVertexArrayAttribFormat( vao, 1, 4, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao, 1, vboCol, 0, sizeof( glm::vec4 ) );
		glVertexArrayAttribBinding( vao, 1, 1 );

		// ebo

		glCreateBuffers( 1, &ebo );
		glNamedBufferData( ebo, indices.size() * sizeof( unsigned int ), indices.data(), GL_STATIC_DRAW );
		glVertexArrayElementBuffer( vao, ebo );

		cercle( { 0.1f, 0.f }, 64, 0.5f );

		//
		_time	   = 0;
		luminosity = 1;
		glUseProgram( program );
		uniformLocation_uTranslationX = glGetUniformLocation( program, "uTranslationX" );
		uniformLocation_uTranslationY = glGetUniformLocation( program, "uTranslationY" );
		uniformLocation_uluminosity	  = glGetUniformLocation( program, "uluminosity" );
		glProgramUniform1f( program, uniformLocation_uluminosity, luminosity );
		// glProgramUniform1f( program, uniformLocation, 0.1f );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::animate( const float p_deltaTime )
	{
		glProgramUniform1f( program, uniformLocation_uTranslationX, glm::sin( _time ) / 2 );
		glProgramUniform1f( program, uniformLocation_uTranslationY, glm::cos( _time ) / 2 );
		_time += p_deltaTime;
	}

	void LabWork2::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		
		
		if ( !ex ) 
		{
			glBindVertexArray( vao );
			glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
		}
		else 
		{
			glBindVertexArray( vao_cercle );
			glDrawElements( GL_TRIANGLES, cercle_indices.size(), GL_UNSIGNED_INT, 0 );
		}
		
		
		glBindVertexArray( 0 );
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 2" );
		if ( ImGui::SliderFloat( "luminosite", &luminosity, 0.f, 1.f ) )
		{
			glProgramUniform1f( program, uniformLocation_uluminosity, luminosity );
		}

		if ( ImGui::ColorEdit3( "couleur de fond", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		if ( ImGui::Button( "cercle/rectangle" ) ) ex = ~ex;

		ImGui::End();

		
	}


	void LabWork2::cercle(Vec2f centre,int nombre_entiers, float rayon ) 
	{
		cercle_vertices		= { centre };
		float incr			= (( 2 * M_PI /float( nombre_entiers) ));
		float angle =  0;

		for ( int i = 1; i <= nombre_entiers; i++ ) 
		{
			cercle_vertices.push_back( { centre.x + rayon * glm::cos( angle ), centre.y +  rayon * glm::sin( angle ) } );
			cercle_color_points.push_back( { getRandomVec3f(), 1.f } );
			cercle_indices.push_back( 0 );
			cercle_indices.push_back( i - 1 );
			cercle_indices.push_back( i  );
			angle += incr;
			
		}
		// on ajoute le dernier triangle "manuelement" pour que les couleurs entre le premier triangle dessiné et le dernier se confondent.
		cercle_indices.push_back( 0 );	
		cercle_indices.push_back( nombre_entiers);
		cercle_indices.push_back(1);
		cercle_color_points.push_back( { getRandomVec3f(), 1.f } );


		glCreateVertexArrays( 1, &vao_cercle );


		glCreateBuffers( 1, &vboPos_cercle );
		glNamedBufferData(vboPos_cercle, cercle_vertices.size() * sizeof( Vec2f ), cercle_vertices.data(), GL_STATIC_DRAW );
		glEnableVertexArrayAttrib( vao_cercle, 0 );
		glVertexArrayAttribFormat( vao_cercle, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao_cercle, 0, vboPos_cercle, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( vao_cercle, 0, 0 );

		glCreateBuffers( 1, &vboCol_cercle );
		glNamedBufferData(vboCol_cercle, cercle_color_points.size() * sizeof( Vec4f ), cercle_color_points.data(), GL_STATIC_DRAW );
		glEnableVertexArrayAttrib( vao_cercle, 1 );
		glVertexArrayAttribFormat( vao_cercle, 1, 4, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao_cercle, 1, vboCol_cercle, 0, sizeof( Vec4f ) );
		glVertexArrayAttribBinding( vao_cercle, 1, 1 );

		glCreateBuffers( 1, &ebo_cercle );
		glNamedBufferData( ebo_cercle, cercle_indices.size() * sizeof( unsigned int ), cercle_indices.data(), GL_STATIC_DRAW );
		glVertexArrayElementBuffer( vao_cercle, ebo_cercle );
	}

} // namespace M3D_ISICG
