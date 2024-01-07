#ifndef __LAB_WORK_4_HPP__
#define __LAB_WORK_4_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include "common/camera.hpp"
#include <vector>
#include "common/models/triangle_mesh_model.hpp"

namespace M3D_ISICG
{
	

	class LabWork4 : public BaseLabWork
	{
	  public:
		LabWork4() : BaseLabWork() {}
		~LabWork4();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;
		
		void _initCamera();
		void updateMVPMatrix();
		void updateNormalMatrix();
		

	  private:
		// ================ Scene data.

		float fov = 90.f;
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;

		glm::mat4 MVPMatrix;
		glm::mat4 NormalMatrix;
		int		  uniformLocationMVPMatrix;
		int		  uniformLocationNormalMatrix;


		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_INDEX;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================ TriangleMeshModel
		// 
			TriangleMeshModel bunny;
		// 
		// ================

		Camera					 _camera;
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
