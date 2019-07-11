
#pragma once
#include <unordered_map>
#include <string>

#include "glm/vec4.hpp"

class Text
{
public:

	// Every text will be drawn after everything has been drawn
	// so we have to keep the draw call somewhere
	static void draw(const std::string &text, float x, float y, const bool drawFromCenter, const glm::vec4 colour = glm::vec4(1.f));
	static void display();

	static void initialize();

private:

	struct DrawCall
	{
		DrawCall(const std::string & text, const float x, const float y,const glm::vec4 colour);

		const std::string text;
		const float x;
		const float y;
		glm::vec4 colour;
	};

	// This map converts characters to indexes
	static std::unordered_map<char, unsigned int>	m_indexes;
	static std::vector<DrawCall>					m_drawCalls;
	
	static unsigned int m_spriteId;
	static unsigned int	m_width;

};
