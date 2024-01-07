#include "lab_work_4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "utils/random.hpp"
#include <iostream>

using namespace std;

namespace M3D_ISICG

{

	const std::string LabWork4::_shaderFolder = "src/lab_works/lab_work_4/shaders/";

	LabWork4::~LabWork4()
	{
		glDeleteProgram( program );

	}

	bool LabWork4::init()
	{

		std::cout << "Initializing lab work 4..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		const std::string vertexShaderStr	= readFile( _shaderFolder + "mesh.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "mesh.frag" );

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

		//	TP4
		uniformLocationMVPMatrix = glGetUniformLocation( program, "uMVPMatrix" );
		uniformLocationNormalMatrix = glGetUniformLocation( program, "uNormalMatrix" );
		bunny.load( "bunny", "data/model/bunny/bunny.obj" );
		
		glEnable( GL_DEPTH_TEST );
		_initCamera();



		glUseProgram( program );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork4::animate( const float p_deltaTime )
	{

	}

	void LabWork4::render()
	{
		updateNormalMatrix();
		updateMVPMatrix(); 
		bunny.render(program);
	}

	void LabWork4::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			
		}
	}
	void LabWork4::displayUI()
	{
		ImGui::Begin( "Settings lab work 4" );
		if ( ImGui::SliderFloat( "fov", &fov, 0.f, 120.f ) )
		{
			_camera.setFovy( fov );
		}

		ImGui::End();

		
	}


	 void LabWork4::_initCamera() 
	 {
		 _camera.setPosition( Vec3f { 0.f, 1.f, 3.f } );
		 _camera.setScreenSize( 1280, 720 );
	 }

	void LabWork4::updateMVPMatrix() 
	{ 
		MVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix();
		 glProgramUniformMatrix4fv( program, uniformLocationMVPMatrix, 1, GL_FALSE, glm::value_ptr( MVPMatrix ) );
	}

	void LabWork4::updateNormalMatrix()
	{
		 NormalMatrix = glm::transpose( glm::inverse( _camera.getViewMatrix() ) );
		 glProgramUniformMatrix4fv( program, uniformLocationNormalMatrix, 1, GL_FALSE, glm::value_ptr( NormalMatrix ) );
	}



} // namespace M3D_ISICG
