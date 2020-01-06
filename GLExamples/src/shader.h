#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>

class Shader
{
public:
	//Program ID
	unsigned int ID;
	//Constructor
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	
	//Functions
	void Use();
	void SetBool(const std::string& name, bool value)const;
	void SetInt(const std::string& name, int value)const;
	void SetFloat(const std::string& name, float value)const;
	void SetVec2(const std::string& name, float x, float y)const;
	void SetVec3(const std::string& name, float x, float y, float z)const;
	void SetVec4(const std::string& name, float x, float y, float z, float w)const;

private:
	//Utility Function
	void CheckCompileErrors(GLuint shader, std::string type);
};

#endif // !SHADER_H
