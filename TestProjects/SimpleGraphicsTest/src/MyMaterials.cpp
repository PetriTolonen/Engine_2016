#include "MyMaterials.h"

void GlobalShaderUniforms::getUniformLocations(Shader* shader)
{
	m_ids[0] = glGetUniformLocation(shader->getProgram(), "g_matModel");
	m_ids[1] = glGetUniformLocation(shader->getProgram(), "g_matView");
	m_ids[2] = glGetUniformLocation(shader->getProgram(), "g_matProj");
	m_ids[3] = glGetUniformLocation(shader->getProgram(), "g_matModelView");
	m_ids[4] = glGetUniformLocation(shader->getProgram(), "g_matNormal");
	m_ids[5] = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
	m_ids[6] = glGetUniformLocation(shader->getProgram(), "g_lightPos");
	m_ids[7] = glGetUniformLocation(shader->getProgram(), "g_camPos");

	m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
}

void GlobalShaderUniforms::bind(Shader* shader)
{
	shader->bind();
	if (m_sharedShaderValues)
	{
		glUniformMatrix4fv(m_ids[0], 1, GL_FALSE, &m_sharedShaderValues->matModel[0][0]);
		glUniformMatrix4fv(m_ids[1], 1, GL_FALSE, &m_sharedShaderValues->matView[0][0]);
		glUniformMatrix4fv(m_ids[2], 1, GL_FALSE, &m_sharedShaderValues->matProj[0][0]);
		glUniformMatrix4fv(m_ids[3], 1, GL_FALSE, &m_sharedShaderValues->matModelView[0][0]);
		glUniformMatrix4fv(m_ids[4], 1, GL_FALSE, &m_sharedShaderValues->matNormal[0][0]);
		glUniformMatrix4fv(m_ids[5], 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);

		glUniform3f(m_ids[6], m_sharedShaderValues->lightPos.x, m_sharedShaderValues->lightPos.y, m_sharedShaderValues->lightPos.z);
		glUniform3f(m_ids[7], m_sharedShaderValues->camPos.x, m_sharedShaderValues->camPos.y, m_sharedShaderValues->camPos.z);

		glUniform1f(m_id, m_sharedShaderValues->totalTime);
	}
}
