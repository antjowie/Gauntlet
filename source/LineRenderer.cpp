#include "LineRenderer.h"
#include <GL/glew.h>
#include <glm/ext.hpp>
#include "Utilities.h"

using namespace NHTV;

LineRenderer::LineRenderer(unsigned int a_maxLines  /* = 16384 */)
	: m_maxLines(a_maxLines),
	m_lineCount(0),
	m_lines(new Line[a_maxLines])
{
	char* vsSource = "#version 330\n\
					 in vec4 Position; \
					 in vec4 Colour; \
					 out vec4 vColour; \
					 uniform mat4 ProjectionView; \
					 void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

	char* fsSource = "#version 330\n \
					 in vec4 vColour; \
					 out vec4 outColour; \
					 void main()	{ outColour = vColour; }";

	const char* aszInputs[] = { "Position", "Colour" };
	const char* aszOutputs[] = { "outColour" };

	m_programID = Utility::loadShaderFromBuffer(2, aszInputs, 1, aszOutputs, reinterpret_cast<unsigned char*>(vsSource), reinterpret_cast<unsigned char*>(fsSource) );

	createBuffers();
}

LineRenderer::~LineRenderer()
{
	delete[] m_lines;
	glDeleteBuffers(1, &m_lineVBO);
	glDeleteBuffers(1, &m_lineIBO);
	glDeleteVertexArrays(1, &m_lineVAO);
	glDeleteProgram(m_programID);
}

void LineRenderer::createBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxLines * sizeof(Line), m_lines, GL_DYNAMIC_DRAW);
	
	unsigned int uiIndexCount = m_maxLines * 2;
	unsigned int* auiIndices = new unsigned int[uiIndexCount];
	for (unsigned int i = 0; i < uiIndexCount; ++i)
	{
		auiIndices[i] = i;
	}
	glGenBuffers(1, &m_lineIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiIndexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	delete[] auiIndices;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glGenVertexArrays(1, &m_lineVAO);
	glBindVertexArray(m_lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(LineVertex), ((char*)0) + 16);

	glBindVertexArray(0);
}

void LineRenderer::addLine(const glm::vec4& a_rv0, const glm::vec4& a_rv1, const glm::vec4& a_colour)
{
	addLine(a_rv0, a_rv1, a_colour, a_colour);
}

void LineRenderer::addLine(const glm::vec4& a_rv0, const glm::vec4& a_rv1, const glm::vec4& a_colour0, const glm::vec4& a_colour1)
{
	if (m_lineCount < m_maxLines)
	{
		m_lines[m_lineCount].v0.position = a_rv0;
		m_lines[m_lineCount].v0.colour = a_colour0;
		m_lines[m_lineCount].v1.position = a_rv1;
		m_lines[m_lineCount].v1.colour = a_colour1;
		++m_lineCount;
	}
}

void LineRenderer::draw(const glm::mat4& a_projectionView)
{
	if( m_lineCount > 0 )
	{
		glUseProgram(m_programID);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "ProjectionView");
		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(a_projectionView));

		glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_lineCount * sizeof(Line), m_lines);
		glBindVertexArray(m_lineVAO);
		glDrawElements(GL_LINES, m_lineCount * 2, GL_UNSIGNED_INT, 0);
	}
}

void LineRenderer::clear()
{
	m_lineCount = 0;
}
