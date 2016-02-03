#pragma once

#include <graphics\Shader.h>

using namespace graphics;

struct SharedShaderValues
{
	float totalTime = 0.0f;
	slmath::mat4 matModel;
	slmath::mat4 matView;
	slmath::mat4 matProj;
	slmath::mat4 matModelView;
	slmath::mat4 matNormal;
	slmath::mat4 matModelViewProj;
	slmath::vec3 lightPos;
	slmath::vec3 camPos;
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
	GLint m_ids[8];
};