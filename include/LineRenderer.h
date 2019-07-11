#ifndef __LINE_RENDERER_H__
#define __LINE_RENDERER_H__

#include <glm/glm.hpp>

namespace NHTV
{
	class LineRenderer
	{
	public:
		LineRenderer(unsigned int a_uiMaxLines = 16384 );
		~LineRenderer();

		// removes all Lines
		void		clear();
		// draws current Gizmo buffers, either using a combined (projection * view) matrix, or separate matrices
		void		draw(const glm::mat4& a_projectionView);
		void		draw(const glm::mat4& a_view, const glm::mat4& a_projection);
		// Adds a single line
		void		addLine(const glm::vec4& a_rv0, const glm::vec4& a_rv1,
			const glm::vec4& a_colour);
		// Adds a single line
		void		addLine(const glm::vec4& a_rv0, const glm::vec4& a_rv1,
			const glm::vec4& a_colour0, const glm::vec4& a_colour1);
	private:
		void		createBuffers();

		struct LineVertex
		{
			glm::vec4 position;
			glm::vec4 colour;
		};

		struct Line
		{
			LineVertex v0;
			LineVertex v1;
		};

		unsigned int	m_programID;
		unsigned int	m_vertexShader;
		unsigned int	m_fragmentShader;
		// line data
		unsigned int	m_maxLines;
		unsigned int	m_lineCount;
		Line*		m_lines;

		unsigned int	m_lineVAO;
		unsigned int 	m_lineVBO;
		unsigned int 	m_lineIBO;

	};

	inline void LineRenderer::draw(const glm::mat4& a_view, const glm::mat4& a_projection)
	{
		draw(a_projection * a_view);
	}
} //end namespace NHTV

#endif //__LINE_RENDERER_H__