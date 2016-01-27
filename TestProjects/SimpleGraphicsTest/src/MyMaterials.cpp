#include "MyMaterials.h"

void GlobalShaderUniforms::bind(Shader* shader)
{
	shader->bind();
	if (m_sharedShaderValues)
	{
		glUniformMatrix4fv(m_idMat, 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);
		glUniform1f(m_id, m_sharedShaderValues->totalTime);
	}
}

void GlobalShaderUniforms::getUniformLocations(Shader* shader)
{
	m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
	m_idMat = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
}