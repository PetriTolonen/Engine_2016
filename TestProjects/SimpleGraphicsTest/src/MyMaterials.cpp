#include "MyMaterials.h"

void GlobalShaderUniforms::bind(Shader* shader)
{
	shader->bind();
	if (m_sharedShaderValues)
	{
		glUniform1f(m_id, m_sharedShaderValues->totalTime);
	}
}

void GlobalShaderUniforms::getUniformLocations(Shader* shader)
{
	m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
}