#include "scene.h"
#include <core/log.h>

#include <graphics\Shader.h>
#include "MyMaterials.h"

class FullScreenQuadShaderScene : public Scene
{
public:
	FullScreenQuadShaderScene()
	{

		LOG("FullScreenQuadShaderScene construct");
		checkOpenGL();

		m_count = 0.0f;

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 },
			{ "g_vTexCoord", 2 }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

		m_shader = new graphics::Shader("assets/FullScreenQuad.vs", "assets/PlasmaEffect.fs", attributes, numAttributes);

		m_material = new GlobalShaderUniforms(m_shader, &m_sharedValues);

		checkOpenGL();
	}


	virtual ~FullScreenQuadShaderScene()
	{
		glDeleteProgram(m_shader->getProgram());
		LOG("FullScreenQuadShaderScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime / 15.0f;

		if (m_count > 1.0f)
			m_count = 0.0f;

		m_sharedValues.totalTime += deltaTime;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);
		checkOpenGL();

		float fSize = 0.95f;
		float VertexPositions[] =
		{
			-fSize, +fSize, 0.0f, 1.0f,
			-fSize, -fSize, 0.0f, 1.0f,
			+fSize, -fSize, 0.0f, 1.0f,
			-fSize, +fSize, 0.0f, 1.0f,
			+fSize, -fSize, 0.0f, 1.0f,
			+fSize, +fSize, 0.0f, 1.0f,
			-fSize, +fSize, 0.0f, 1.0f
		};

		float VertexColors[] = { 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
		};

		float TexCoord[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		// Clear the backbuffer and depth-buffer
		glClearColor(m_count, m_count, m_count, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		m_material->bind();
		m_material->getUniformLocations(m_shader);
		checkOpenGL();

		// Draw the colored triangle
		glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, VertexPositions);
		checkOpenGL();

		glEnableVertexAttribArray(0);
		checkOpenGL();

		glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray(1);
		checkOpenGL();

		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, TexCoord);
		checkOpenGL();
		glEnableVertexAttribArray(2);
		checkOpenGL();

		/*glUniform1f(glGetUniformLocation(m_shader->getProgram(), "totalTime"), m_countTime);
		checkOpenGL();*/
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		checkOpenGL();

		glDisableVertexAttribArray(0);
		checkOpenGL();
		glDisableVertexAttribArray(1);
		checkOpenGL();
	}
private:
	float m_count;

	core::Ref<graphics::Shader> m_shader;

	SharedShaderValues m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;
};


