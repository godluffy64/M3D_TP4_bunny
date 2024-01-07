#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include "common/camera.hpp"
#include <vector>

namespace M3D_ISICG
{
	// Mesh structure

	struct Mesh
	{
		std::vector<Vec3f>		  sommetPos;
		std::vector<Vec3f>		  sommetCol;
		std::vector<unsigned int> sommetIndices;
		glm::mat4				  transf_object;
		GLuint					  vbo_pos;
		GLuint					  vbo_col;
		GLuint					  vao;
		GLuint					  ebo;
		Mesh() 
		{ 
			vao = GL_INVALID_INDEX;
			ebo = GL_INVALID_INDEX;
			vbo_col = GL_INVALID_INDEX;
			vbo_pos = GL_INVALID_INDEX;

		}
	};

	

	class LabWork3 : public BaseLabWork
	{
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;
		
		void _createCube();
		
		void _updateModelMatrix();
		void _updateModelMatrixRotate();
		void _updateViewMatrix();
		void _updateProjectionMatrix();
		void _updateModelViewProjectionMatrix();
		void _initCamera();
		void _UpdateModelMatrix2();
		

	  private:
		// ================ Scene data.

		int uniformLocation_umodel_matrix;
		int uniformLocation_umodel_rotate;
		int uniformLocation_uView_matrix;
		int uniformLocation_uProjection_matrix;
		int uniformLocation_uModelViewProjection_matrix;

		float _time;
		float fov = 90.f;
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;

		glm::mat4 MatrixRotate;
		glm::mat4 MVP;
		glm::mat4 modelMatrix2;

		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_INDEX;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================ Mesh
		
		Mesh _cube;
		// ================

		Camera					 _camera;
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
