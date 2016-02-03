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
	GlobalShaderUniforms(Shader* shader, const SharedShaderValues* sharedValues = 0) :
		ShaderUniforms(shader), 
		m_sharedShaderValues(sharedValues){}
	virtual ~GlobalShaderUniforms(){}
	virtual void getUniformLocations(Shader* shader);
	virtual void bind(Shader* shader);
private:
	const SharedShaderValues* m_sharedShaderValues;
	GLint m_ids[8];
};

class SimpleMaterialUniforms : public graphics::ShaderUniforms
{
public:
	slmath::vec4 vAmbient;
	slmath::vec4 vDiffuse;
	slmath::vec4 vSpecular;

	SimpleMaterialUniforms(Shader* shader, const SharedShaderValues* sharedValues = 0) :
		ShaderUniforms(shader),
		m_globalShaderUniforms( new GlobalShaderUniforms(shader, sharedValues))
	{
	}

	virtual ~SimpleMaterialUniforms()
	{
	}

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		m_globalShaderUniforms->getUniformLocations(shader);
		m_materialAmbientLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vAmbient");
		m_materialDiffuseLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vDiffuse");
		m_materialSpecularLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vSpecular");
	}

	virtual void bind(graphics::Shader * shader)
	{
		shader->bind();
		m_globalShaderUniforms->bind(shader);
		glUniform4fv(m_materialAmbientLoc, 1, &vAmbient.x);
		glUniform4fv(m_materialDiffuseLoc, 1, &vDiffuse.x);
		glUniform4fv(m_materialSpecularLoc, 1, &vSpecular.x);
	}

private:
	core::Ref<GlobalShaderUniforms> m_globalShaderUniforms;
	GLint m_materialAmbientLoc;
	GLint m_materialDiffuseLoc;
	GLint m_materialSpecularLoc;

};