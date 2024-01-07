#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;
		void cercle(Vec2f, int, float);

	  private:
		// ================ Scene data.
		std::vector<Vec2f>		  triangle_vertices;
		std::vector<unsigned int> indices;
		std::vector<Vec4f>		  color_points;

		std::vector<Vec2f> cercle_vertices;
		std::vector<unsigned int> cercle_indices;
		std::vector<Vec4f>		  cercle_color_points;

		int uniformLocation_uTranslationX = 0;
		int uniformLocation_uTranslationY = 0;
		int uniformLocation_uluminosity = 0;

		float _time = 0;
		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_INDEX;
		GLuint vboPos  = GL_INVALID_INDEX;
		GLuint vboCol  = GL_INVALID_INDEX;
		GLuint vao	   = GL_INVALID_INDEX;
		GLuint ebo	   = GL_INVALID_INDEX;

		GLuint vboPos_cercle = GL_INVALID_INDEX;
		GLuint vboCol_cercle = GL_INVALID_INDEX;
		GLuint vao_cercle	 = GL_INVALID_INDEX;
		GLuint ebo_cercle	 = GL_INVALID_INDEX;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		float luminosity = 0.f;
		BOOLEAN ex = FALSE;
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
