#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <stdio.h>
#include <glm/ext.hpp>
#include <iostream>
#include <SOIL.h>

#include "Utilities.h"

using namespace NHTV;

static double s_prevTime = 0;
static float s_totalTime = 0;
static float s_deltaTime = 0;

void Utility::resetTimer()
{
	s_prevTime = glfwGetTime();
	s_totalTime = 0;
	s_deltaTime = 0;
}

float Utility::tickTimer()
{
	double currentTime = glfwGetTime();
	s_deltaTime = (float)(currentTime - s_prevTime);
	s_totalTime += s_deltaTime;
	s_prevTime = currentTime;
	return s_deltaTime;
}

float Utility::getDeltaTime()
{
	return s_deltaTime;
}

float Utility::getTotalTime()
{
	return s_totalTime;
}

// loads a texture into OpenGL memory
unsigned int Utility::loadTexture(const char* a_pFilename, int& a_iWidth, int& a_iHeight, int& a_iBPP)
{
	unsigned int uiTextureID = 0;
	if (a_pFilename != nullptr)
	{
		unsigned char* pImageData = SOIL_load_image(a_pFilename, &a_iWidth, &a_iHeight, &a_iBPP, SOIL_LOAD_AUTO);
		if (pImageData)
		{
			uiTextureID = SOIL_create_OGL_texture(pImageData,
				a_iWidth,
				a_iHeight,
				a_iBPP,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS |
				SOIL_FLAG_INVERT_Y |
				SOIL_FLAG_NTSC_SAFE_RGB |
				SOIL_FLAG_COMPRESS_TO_DXT);
			SOIL_free_image_data(pImageData);
		}
	}
	if (0 == uiTextureID)
	{
		std::cout << "SOIL loading error: " << SOIL_last_result() << std::endl;
	}
	return uiTextureID;
}

unsigned int Utility::loadShaderFromFile(unsigned int a_uiInputAttributeCount, const char** a_aszInputAttributes,
		unsigned int a_uiOutputAttributeCount, const char** a_aszOutputAttributes,
		const char* a_szVertexShader, const char* a_szPixelShader,
		const char* a_szGeometryShader /* = nullptr */,
		const char* a_szTessellationControlShader /* = nullptr */, const char* a_szTessellationEvaluationShader /* = nullptr */)
{
	// load files into char buffers
	const unsigned char* vsSource = fileToBuffer(a_szVertexShader);
	const unsigned char* fsSource = fileToBuffer(a_szPixelShader);
	const unsigned char* gsSource = a_szGeometryShader == nullptr ? nullptr : fileToBuffer(a_szGeometryShader);
	const unsigned char* tcsSource = a_szTessellationControlShader == nullptr ? nullptr : fileToBuffer(a_szTessellationControlShader);
	const unsigned char* tesSource = a_szTessellationEvaluationShader == nullptr ? nullptr : fileToBuffer(a_szTessellationEvaluationShader);
	
	unsigned int uiProgramHandle = loadShaderFromBuffer(a_uiInputAttributeCount, a_aszInputAttributes, a_uiOutputAttributeCount, a_aszOutputAttributes,
		vsSource, fsSource, gsSource, tcsSource, tesSource);

	delete vsSource;
	delete fsSource;
	if (gsSource != nullptr)
	{
		delete gsSource;
	}
	if (tcsSource != nullptr)
	{
		delete tcsSource;
	}
	if (tesSource != nullptr)
	{
		delete tesSource;
	}
	return uiProgramHandle;

}

// loads only a vertex and pixel shader from text files
unsigned int Utility::loadShaderFromBuffer(unsigned int a_uiInputAttributeCount, const char** a_aszInputAttributes,
	unsigned int a_uiOutputAttributeCount, const char** a_aszOutputAttributes,
	const unsigned char* a_szVertexShader, const unsigned char* a_szPixelShader,
	const unsigned char* a_szGeometryShader /* = nullptr */,
	const unsigned char* a_szTessellationControlShader /* = nullptr */, const unsigned char* a_szTessellationEvaluationShader /* = nullptr */)
{
	int iSuccess;
	char acLog[256];
	// load files into char buffers
	const unsigned char* vsSource = a_szVertexShader;
	const unsigned char* fsSource = a_szPixelShader;
	const unsigned char* gsSource = a_szGeometryShader == nullptr ? nullptr : a_szGeometryShader;
	const unsigned char* tcsSource = a_szTessellationControlShader == nullptr ? nullptr : a_szTessellationControlShader;
	const unsigned char* tesSource = a_szTessellationEvaluationShader == nullptr ? nullptr : a_szTessellationEvaluationShader;

	// must have vertex and pixel
	if (vsSource == nullptr || fsSource == nullptr)
	{
		return 0;
	}
	// create 2 shader handles
	unsigned int vsHandle = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int gsHandle = 0;
	unsigned int tcsHandle = 0;
	unsigned int tesHandle = 0;

	// compile vertex shader and log errors
	if (!compileShader(vsHandle, vsSource))
	{
		glGetShaderInfoLog(vsHandle, sizeof(acLog), 0, acLog);
		std::cout << "Error: Failed to compile vertex shader!" << std::endl;
		std::cout << acLog << std::endl;
		return 0;
	}
	
	// compile pixel shader and log errors
	if (!compileShader(fsHandle, fsSource))
	{
		glGetShaderInfoLog(fsHandle, sizeof(acLog), 0, acLog);
		std::cout << "Error: Failed to compile fragment shader!" << std::endl;
		std::cout << acLog << std::endl;
		return 0;
	}

	if (gsSource != nullptr)
	{
		gsHandle = glCreateShader(GL_GEOMETRY_SHADER);
		// compile geometry shader and log errors
		if (!compileShader(gsHandle, gsSource))
		{
			glGetShaderInfoLog(gsHandle, sizeof(acLog), 0, acLog);
			std::cout << "Error: Failed to compile geometry shader!" << std::endl;
			std::cout << acLog << std::endl;
			return 0;
		}
	}

	if (tesSource != nullptr && tcsSource != nullptr)
	{
		tesHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		tcsHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		// compile tessellation control shader and log errors
		if (!compileShader(tcsHandle, tcsSource))
		{
			glGetShaderInfoLog(tcsHandle, sizeof(acLog), 0, acLog);
			std::cout << "Error: Failed to compile tessellation control shader!" << std::endl;
			std::cout << acLog << std::endl;
			return 0;
		}
		// compile tessellation evaluation shader and log errors
		if (!compileShader(tesHandle, tesSource))
		{
			glGetShaderInfoLog(tesHandle, sizeof(acLog), 0, acLog);
			std::cout << "Error: Failed to compile tessellation evaluation shader!" << std::endl;
			std::cout << acLog << std::endl;
			return 0;
		}
	}
	// create a shader program and attach the shaders to it
	unsigned int uiProgramHandle = glCreateProgram();
	glAttachShader(uiProgramHandle, vsHandle);
	glAttachShader(uiProgramHandle, fsHandle);

	if (gsHandle != 0)
	{
		glAttachShader(uiProgramHandle, gsHandle);
	}

	if (tcsHandle != 0 && tesHandle != 0)
	{
		glAttachShader(uiProgramHandle, tcsHandle);
		glAttachShader(uiProgramHandle, tesHandle);
	}

	// specify vertex input attributes
	for (unsigned int i = 0; i < a_uiInputAttributeCount; ++i)
	{
		glBindAttribLocation(uiProgramHandle, i, a_aszInputAttributes[i]);
	}
	// specify pixel shader outputs
	for (unsigned int i = 0; i < a_uiOutputAttributeCount; ++i)
	{
		glBindFragDataLocation(uiProgramHandle, i, a_aszOutputAttributes[i]);
	}
	// link the program together and log errors

	glLinkProgram(uiProgramHandle);
	glGetProgramiv(uiProgramHandle, GL_LINK_STATUS, &iSuccess);
	glGetProgramInfoLog(uiProgramHandle, sizeof(acLog), 0, acLog);

	if (iSuccess == GL_FALSE)
	{
		std::cout << "Error: Failed to link shader program!" << std::endl;
		std::cout << acLog << std::endl;
		return 0;
	}

	//Now that shaders have been linked to a program they can be deleted
	//otherwise we have an interesting memory leak
	glDeleteShader(vsHandle);
	glDeleteShader(fsHandle);
	if (gsSource != nullptr)
	{
		glDeleteShader(gsHandle);
	}
	if (tcsSource != nullptr)
	{
		glDeleteShader(tcsHandle);
	}
	if (tesSource != nullptr)
	{
		glDeleteShader(tesHandle);
	}
	glUseProgram(uiProgramHandle);
	return uiProgramHandle;
}

bool Utility::compileShader(const unsigned int a_shaderHandle, const unsigned char* a_shaderSrc )
{
	int iSuccess;
	glShaderSource(a_shaderHandle, 1, (const char**)&a_shaderSrc, 0);
	glCompileShader(a_shaderHandle);

	glGetShaderiv(a_shaderHandle, GL_COMPILE_STATUS, &iSuccess);
	return (iSuccess == GL_TRUE);
}

unsigned char* Utility::fileToBuffer(const char* a_sPath)
{

	// open file for text reading
	FILE* pFile = fopen(a_sPath, "rb");
	if (pFile == nullptr)
	{
		std::cout << "Error: Unable to open file " << a_sPath << " for reading!" << std::endl;
		return nullptr;
	}
	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	// allocate buffer and read file contents
	unsigned char* acBuffer = new unsigned char[uiLength + 1];
	memset(acBuffer, 0, uiLength + 1);
	fread(acBuffer, sizeof(unsigned char), uiLength, pFile);
	fclose(pFile);

	return acBuffer;
}

