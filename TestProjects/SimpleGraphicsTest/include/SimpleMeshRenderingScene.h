#include "scene.h"
#include <core/log.h>

#include <graphics\Shader.h>
#include "MyMaterials.h"

#include "teapot.h"
#include <graphics\Mesh.h>

graphics::Mesh* createTeapotMesh()
{
	graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);

	graphics::VertexArray* va[] =
	{
		new graphics::VertexArrayImpl<slmath::vec3>(
		graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),

		new graphics::VertexArrayImpl<slmath::vec3> (
		graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices),

		new graphics::VertexArrayImpl<slmath::vec3>(
		graphics::ATTRIB_UV, (slmath::vec3*)TeapotData::texCoords, TeapotData::numVertices)
	};

	graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));

	return new graphics::Mesh(ib, vb);
}

class SimpleMeshRenderingScene : public Scene
{
public:
	SimpleMeshRenderingScene()
	{

		LOG("SimpleMeshRenderingScene construct");
		checkOpenGL();

		m_count = 0.0f;

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS" , graphics::ATTRIB_NORMAL },
			{ "g_vTexCoord", graphics::ATTRIB_UV }
		};

		m_shader =
			new graphics::Shader("assets/Simple3d.vs", "assets/Simple3d.fs",
			attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));
		
		m_material = new GlobalShaderUniforms(m_shader, &m_sharedValues);
		m_mesh = createTeapotMesh();

		checkOpenGL();
	}


	virtual ~SimpleMeshRenderingScene()
	{
		glDeleteProgram(m_shader->getProgram());
		LOG("SimpleMeshRenderingScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime / 15.0f;
		m_totalTime += deltaTime;

		if (m_count > 1.0f)
			m_count = 0.0f;

		m_sharedValues.totalTime += deltaTime;

		float fAspect = (float)esContext->width / (float)esContext->height;
		m_matProjection = slmath::perspectiveFovRH(
			slmath::radians(45.0f),
			fAspect,
			5.0f,
			1000.0f);

		m_matView = slmath::lookAtRH(
			slmath::vec3(0.0f, 70.0f, 70.0f),
			slmath::vec3(0.0f, 15.0f, 0.0f),
			slmath::vec3(0.0f, 1.0f, 0.0f));

		m_matModel = slmath::rotationX(-3.1415f*0.5f);
		m_matModel = slmath::rotationY(0) * m_matModel;
		m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);
		checkOpenGL();
			
		// Clear the backbuffer and depth-buffer
		glClearColor(m_count, m_count, m_count, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		checkOpenGL();

		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;

		m_sharedValues.matModelViewProj = matModelViewProj;

		m_material->bind();
		checkOpenGL();

		m_mesh->render();
		checkOpenGL();
	}
private:
	float m_count;
	float m_totalTime;

	core::Ref<graphics::Shader> m_shader;

	SharedShaderValues m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;

	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;

	core::Ref<Mesh> m_mesh;
};


