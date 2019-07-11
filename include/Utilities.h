#ifndef __UTILITIES_H_
#define __UTILITIES_H_

#include <glm/glm.hpp>

namespace NHTV
{

	// a basic vertex structure supporting position, colour and texture coordinate
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 colour;
		glm::vec2 texCoord;
	};

	const float PI = 3.14159265359f;
	const float	TWOPI = 6.28318530717958657692f;
	const float HALFPI = 1.57079632679489661923f;
	const float THREEHALFPI = 4.71238898038468985769f;

	const float	EPSILON = 0.00000000001f;
	const float	DEG2RAD = 0.01745329251994329577f;
	const float	RAD2DEG = 57.2957795130823208768f;

	//////////////////////////////////////////////////////////////////////////
	/// Reciprocal function
	//////////////////////////////////////////////////////////////////////////
	inline float Recipf(float x)
	{
		return (1.0f / x);
	}

	//////////////////////////////////////////////////////////////////////////
	/// Min & Max functions
	inline int  Max(int x, int y) { return (x > y) ? x : y; }
	inline int  Min(int x, int y) { return (x < y) ? x : y; }
	inline float  Maxf(float x, float y) { return (x > y) ? x : y; }
	inline float  Minf(float x, float y) { return (x < y) ? x : y; }

	//////////////////////////////////////////////////////////////////////////
	/// Clamp function (between 0.0f and 1.0f)
	//////////////////////////////////////////////////////////////////////////
	inline float  Clampf(float x)
	{
		return (x > 1.0f) ? 1.0f : ((x < 0.0f) ? 0.0f : x);
	}

	//////////////////////////////////////////////////////////////////////////
	/// Clamp function (between MIN and MAX)
	//////////////////////////////////////////////////////////////////////////
	inline float  Clampf(float x, float minx, float maxx)
	{
		return (x > maxx) ? maxx : ((x < minx) ? minx : x);
	}
	inline int  Clamp(int x, int minx, int maxx)
	{
		return (x > maxx) ? maxx : ((x < minx) ? minx : x);
	}

	//////////////////////////////////////////////////////////////////////////
	/// Linearly interpolate a float
	//////////////////////////////////////////////////////////////////////////
	inline float  Lerp(float fA, float fB, float fT)
	{
		return (fB - fA) * fT + fA;
	}

	inline float  SmoothStep(float fValue)
	{
		return (fValue * fValue * (3.0f - 2.0f * fValue));
	}

	// a utility class with static helper methods
	class Utility
	{
	public:

		// loads a texture in from memory into an OpenGL format
		static unsigned int	loadTexture(const char* a_pFilename, int& a_uiWidth, int& a_uiHeight, int& a_uiBPP);
		// creates a shader program using the specified shader code files and a list of input attributes and output attributes
		static unsigned int	loadShaderFromFile(unsigned int a_uiInputAttributeCount, const char** a_aszInputAttributes,
			unsigned int a_uiOutputAttributeCount, const char** a_aszOutputAttributes,
			const char* a_szVertexShader, const char* a_szPixelShader,
			const char* a_szGeometryShader = nullptr,
			const char* a_szTessellationControlShader = nullptr, const char* a_szTessellationEvaluationShader = nullptr);
		// creates a shader program using the specified shader code files and a list of input attributes and output attributes
		static unsigned int	loadShaderFromBuffer(unsigned int a_uiInputAttributeCount, const char** a_aszInputAttributes,
			unsigned int a_uiOutputAttributeCount, const char** a_aszOutputAttributes,
			const unsigned char* a_szVertexShader, const unsigned char* a_szPixelShader,
			const unsigned char* a_szGeometryShader = nullptr,
			const unsigned char* a_szTessellationControlShader = nullptr, const unsigned char* a_szTessellationEvaluationShader = nullptr);

		// helper function for loading shader code into memory
		static unsigned char*	fileToBuffer(const char* a_szPath);

		// utilities for timing; Get() updates timers and returns time since last Get call
		static void		resetTimer();
		static float	tickTimer();
		static float	getDeltaTime();
		static float	getTotalTime();
	private:
		static bool compileShader(const unsigned int a_shaderHandle, const unsigned char* a_shaderSrc);
	};
	
} //namespace NHTV
#endif // __UTILITIES_H_#pragma once
