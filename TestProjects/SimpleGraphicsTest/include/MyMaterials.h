#pragma once

#include <graphics\Shader.h>
#include <graphics\Texture.h>

using namespace graphics;

struct SharedShaderValues
{
	slmath::mat4 matModel;
	slmath::mat4 matView;
	slmath::mat4 matProj;
	slmath::mat4 matModelView;
	slmath::mat4 matNormal;
	slmath::mat4 matModelViewProj;
	slmath::vec3 lightPos;
	slmath::vec3 camPos;

	float totalTime = 0.0f;
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
	GLint m_id;
};

class SimpleMaterialUniforms : public graphics::ShaderUniforms
{
public:
	slmath::vec4 vAmbient;
	slmath::vec4 vDiffuse;
	slmath::vec4 vSpecular;
	core::Ref<graphics::Texture> diffuseMap;

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
		m_diffuseMapLocation = glGetUniformLocation(shader->getProgram(), "difTexture");
	}

	virtual void bind(graphics::Shader * shader)
	{
		shader->bind();
		m_globalShaderUniforms->bind(shader);
		glUniform4fv(m_materialAmbientLoc, 1, &vAmbient.x);
		glUniform4fv(m_materialDiffuseLoc, 1, &vDiffuse.x);
		glUniform4fv(m_materialSpecularLoc, 1, &vSpecular.x);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->getTextureId());

		glUniform1i(m_diffuseMapLocation, 0);
	}

private:
	core::Ref<GlobalShaderUniforms> m_globalShaderUniforms;
	GLint m_materialAmbientLoc;
	GLint m_materialDiffuseLoc;
	GLint m_materialSpecularLoc;
	GLint m_diffuseMapLocation;

};

class SimpleMaterialWithTextureUniformsCube : public graphics::ShaderUniforms
{
public:
	slmath::vec4 vAmbient;
	slmath::vec4 vDiffuse;
	slmath::vec4 vSpecular;
	slmath::vec4 vGlossyness;
	core::Ref<graphics::Texture> diffuseMap;
	core::Ref<graphics::TextureCube> cubeMap;
	core::Ref<graphics::Texture> glossyMap;

	SimpleMaterialWithTextureUniformsCube(Shader* shader, const SharedShaderValues* sharedValues = 0) :
		ShaderUniforms(shader),
		m_globalShaderUniforms(new GlobalShaderUniforms(shader, sharedValues))
	{
	}

	virtual ~SimpleMaterialWithTextureUniformsCube()
	{
	}

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		m_globalShaderUniforms->getUniformLocations(shader);
		m_materialAmbientLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vAmbient");
		m_materialDiffuseLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vDiffuse");
		m_materialSpecularLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vSpecular");
		m_materialGlossynessLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vGlossyness");
		m_diffuseMapLocation = glGetUniformLocation(shader->getProgram(), "difTexture");
		m_cubeMapLocation = glGetUniformLocation(shader->getProgram(), "cubeTexture");
		m_glossyMapLocation = glGetUniformLocation(shader->getProgram(), "glossyTexture");
	}

	virtual void bind(graphics::Shader * shader)
	{
		shader->bind();
		m_globalShaderUniforms->bind(shader);
		glUniform4fv(m_materialAmbientLoc, 1, &vAmbient.x);
		glUniform4fv(m_materialDiffuseLoc, 1, &vDiffuse.x);
		glUniform4fv(m_materialSpecularLoc, 1, &vSpecular.x);
		glUniform4fv(m_materialGlossynessLoc, 1, &vGlossyness.x);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->getTextureId());

		glUniform1i(m_diffuseMapLocation, 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getTextureId());

		glUniform1i(m_cubeMapLocation, 1);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, glossyMap->getTextureId());

		glUniform1i(m_glossyMapLocation, 2);
	}

private:
	core::Ref<GlobalShaderUniforms> m_globalShaderUniforms;
	GLint m_materialAmbientLoc;
	GLint m_materialDiffuseLoc;
	GLint m_materialSpecularLoc;
	GLint m_materialGlossynessLoc;
	GLint m_diffuseMapLocation;
	GLint m_cubeMapLocation;
	GLint m_glossyMapLocation;

};