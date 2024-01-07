#include "lab_work_3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "utils/random.hpp"
#include <iostream>

using namespace std;

namespace M3D_ISICG

{

	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3()
	{
		glDeleteProgram( program );

		glDeleteBuffers( 1, &_cube.ebo );
		glDeleteBuffers( 1, &_cube.vbo_col );
		glDeleteBuffers( 1, &_cube.vbo_pos );
		glDisableVertexArrayAttrib( _cube.vao, 0 );
		glDeleteVertexArrays( 1, &_cube.vao );
	}

	bool LabWork3::init()
	{

		std::cout << "Initializing lab work 3..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw3.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw3.frag" );

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

		//
		_time	   = 0;
		uniformLocation_umodel_matrix = glGetUniformLocation( program, "umodel_matrix" );
		uniformLocation_uView_matrix  = glGetUniformLocation( program, "uView_matrix" );
		
		uniformLocation_umodel_rotate = glGetUniformLocation( program, "umodel_rotate" );
		uniformLocation_uProjection_matrix = glGetUniformLocation( program, "uProjection_matrix" );
		uniformLocation_uModelViewProjection_matrix = glGetUniformLocation( program, "uModelViewProjection_matrix" );

		modelMatrix2 = glm::mat4( 1.f );
		modelMatrix2 = glm::translate( modelMatrix2, glm::vec3( 3.f, 0.f, 0.f ) );

		glEnable( GL_DEPTH_TEST );
		_createCube();
		_initCamera();
		glUseProgram( program );
		// glProgramUniform1f( program, uniformLocation, 0.1f );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork3::animate( const float p_deltaTime )
	{
		_time += p_deltaTime;
	}

	void LabWork3::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBindVertexArray( _cube.vao );

		_updateModelMatrixRotate();
		_updateModelViewProjectionMatrix(); 
		glDrawElements( GL_TRIANGLES, _cube.sommetIndices.size(), GL_UNSIGNED_INT, 0 );

		_UpdateModelMatrix2();
		glDrawElements( GL_TRIANGLES, _cube.sommetIndices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}
	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );
		if ( ImGui::SliderFloat( "fov", &fov, 0.f, 120.f ) )
		{
			_camera.setFovy( fov );
		}

		if ( ImGui::Button( "axe x" ) )
		{
			modelMatrix2 = glm::translate( glm::mat4( 1.f ), glm::vec3( 3.f, 0.f, 0.f ) );
		}

		if ( ImGui::Button( "axe y" ) )
		{
			modelMatrix2 = glm::translate( glm::mat4( 1.f ), glm::vec3( 0.f, 3.f, 0.f ) );
		}

		if ( ImGui::Button( "axe z" ) )
		{
			modelMatrix2 = glm::translate( glm::mat4( 1.f ), glm::vec3( 0.f, 0.f, 3.f ) );
		}

		ImGui::End();

		
	}
	void LabWork3::_createCube()
	{
		// vecteur position + indice

		_cube.sommetPos = { { -0.5f, 0.5f, 0.5f },	{ 0.5f, 0.5f, 0.5f },	{ 0.5f, -0.5f, 0.5f }, { -0.5f, -0.5f, 0.5f },
				{ -0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f } };

		_cube.sommetIndices = { 0, 1, 3,  1, 2, 3,  1, 5, 2,  5, 6, 2,  4, 5, 0,  5, 1, 0,  4, 5, 7,  5, 6, 7,  7, 6, 3,  6, 2, 3,  0, 4, 3,  4, 7, 3  };

		_cube.sommetCol = { getRandomVec3f(), getRandomVec3f(),getRandomVec3f(),getRandomVec3f(),getRandomVec3f(),getRandomVec3f(), getRandomVec3f(), getRandomVec3f() }; 
		
		// namespace M3D_ISICG

		// _cube vao

		glCreateVertexArrays(1,  &_cube.vao );

		// _cube vbo_pos

		glCreateBuffers( 1, &_cube.vbo_pos );
		glNamedBufferData(_cube.vbo_pos, _cube.sommetPos.size() * sizeof( Vec3f ), _cube.sommetPos.data(), GL_STATIC_DRAW );
		glEnableVertexArrayAttrib( _cube.vao, 0 );
		glVertexArrayAttribFormat( _cube.vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _cube.vao, 0, _cube.vbo_pos, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( _cube.vao, 0, 0 );

		// _cube vbo_Col

		glCreateBuffers( 1, &_cube.vbo_col );
		glNamedBufferData(_cube.vbo_col, _cube.sommetCol.size() * sizeof( Vec3f ), _cube.sommetCol.data(), GL_STATIC_DRAW );
		glEnableVertexArrayAttrib( _cube.vao, 1 );
		glVertexArrayAttribFormat( _cube.vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _cube.vao, 1, _cube.vbo_col, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( _cube.vao, 1, 1 );
		// _cube ebo

		glCreateBuffers( 1, &_cube.ebo );
		glNamedBufferData( _cube.ebo, _cube.sommetIndices.size() * sizeof( unsigned int ), _cube.sommetIndices.data(), GL_STATIC_DRAW );
		glVertexArrayElementBuffer( _cube.vao, _cube.ebo );

		//

		_cube.transf_object		 = glm::mat4( 1.0f );
		_cube.transf_object			  = glm::scale( _cube.transf_object, glm::vec3( 0.8f ) );
		


	}

	void LabWork3::_updateModelMatrixRotate() 
	{
		MatrixRotate = glm::rotate( _cube.transf_object, _time, glm::vec3( 0.f, 1.f, 1.f ) );
		glProgramUniformMatrix4fv(
			program, uniformLocation_umodel_rotate, 1, GL_FALSE, glm::value_ptr( MatrixRotate ) );
	}


	void LabWork3::_updateModelMatrix() 
	{
		
		glProgramUniformMatrix4fv(program, uniformLocation_umodel_matrix, 1, GL_FALSE, glm::value_ptr( _cube.transf_object ) );

	}


	void LabWork3::_updateViewMatrix()
	{
		

		glProgramUniformMatrix4fv(
			program, uniformLocation_uView_matrix, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
	}

	void LabWork3::_updateProjectionMatrix()
	{
		

		glProgramUniformMatrix4fv(program, uniformLocation_uProjection_matrix, 1, GL_FALSE, glm::value_ptr( _camera.getProjectionMatrix() ) );
	}

	void LabWork3::_updateModelViewProjectionMatrix() 
	{ 
		MVP = _camera.getProjectionMatrix() * _camera.getViewMatrix() * MatrixRotate * _cube.transf_object;
		glProgramUniformMatrix4fv( program, uniformLocation_uModelViewProjection_matrix,1 ,GL_FALSE , glm::value_ptr( MVP ) );


	}

	void LabWork3::_UpdateModelMatrix2() 
	{ 

		_cube.transf_object = glm::mat4( 1.0f );
		_cube.transf_object = glm::scale( _cube.transf_object, glm::vec3( 0.8f ) );
		MatrixRotate = glm::rotate( _cube.transf_object, _time, glm::vec3( 0.f, 1.f, 1.f ) );

		MVP = _camera.getProjectionMatrix() * _camera.getViewMatrix() * MatrixRotate * modelMatrix2;
		glProgramUniformMatrix4fv(program, uniformLocation_uModelViewProjection_matrix, 1, GL_FALSE, glm::value_ptr( MVP ) );
	}
	

	 void LabWork3::_initCamera() 
	 {
		 _camera.setPosition( Vec3f { 0.f, 1.f, 3.f } );
		 _camera.setScreenSize( 1280, 720 );
	 }

} // namespace M3D_ISICG
