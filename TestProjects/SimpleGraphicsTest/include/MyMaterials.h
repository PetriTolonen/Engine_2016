#pragma once

#include <graphics\Shader.h>

using namespace graphics;

struct SharedShaderValues
{
	float totalTime = 0.0f;
};

class GlobalShaderUniforms : public ShaderUniforms 
{
public:
	GlobalShaderUniforms(Shader* shader, const SharedShaderValues* sharedSharedValues = 0) :
		ShaderUniforms(shader), 
		m_sharedShaderValues(sharedSharedValues){}
	virtual ~GlobalShaderUniforms(){}
	virtual void getUniformLocations(Shader* shader);
	virtual void bind(Shader* shader);
private:
	const SharedShaderValues* m_sharedShaderValues;
	GLint m_id;
};