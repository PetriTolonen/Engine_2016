#include "scene.h"
#include <core/log.h>

#include <graphics\Shader.h>
#include "MyMaterials.h"

#include "teapot.h"
#include <graphics\Mesh.h>

graphics::Mesh* createTeapotMeshBlinn()
{
	graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);

	graphics::VertexArray* va[] =
	{
		new graphics::VertexArrayImpl<slmath::vec3>(
		graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),

		new graphics::VertexArrayImpl<slmath::vec3>(
		graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices),

		new graphics::VertexArrayImpl<slmath::vec3>(
		graphics::ATTRIB_UV, (slmath::vec3*)TeapotData::texCoords, TeapotData::numVertices)
	};

	graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));

	return new graphics::Mesh(ib, vb);
}

class BlinnPhongScene : public Scene
{
public:
	BlinnPhongScene()
	{

		LOG("BlinnPhongScene construct");
		checkOpenGL();

		m_count = 0.0f;

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL },
			{ "g_vTexCoord", graphics::ATTRIB_UV }
		};

		m_shader =
			new graphics::Shader("assets/Blinn-phong.vs", "assets/Blinn-phong.fs",
			attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		SimpleMaterialUniforms* simpleMaterialUniforms = new SimpleMaterialUniforms(m_shader, &m_sharedValues);

		simpleMaterialUniforms->vAmbient = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vDiffuse = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vSpecular = slmath::vec4(1.0f, 1.0f, 1.0f, 0.5f);

		m_material = simpleMaterialUniforms;
		m_mesh = createTeapotMeshBlinn();

		checkOpenGL();
	}


	virtual ~BlinnPhongScene()
	{
		glDeleteProgram(m_shader->getProgram());
		LOG("BlinnPhongScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime / 15.0f;
		m_totalTime += deltaTime;

		if (m_count > 1.0f)
			m_count = 0.0f;

		//m_sharedValues.totalTime += deltaTime;

		float fAspect = (float)esContext->width / (float)esContext->height;
		m_matProjection = slmath::perspectiveFovRH(
			slmath::radians(45.0f),
			fAspect,
			5.0f,
			1000.0f);

		//cameraX = 50 * cos(m_totalTime);
		//cameraY = 50 * sin(m_totalTime);
		cameraY = 70;

		m_matView = slmath::lookAtRH(
			slmath::vec3(cameraX, cameraY, 70.0f),
			slmath::vec3(0.0f, 15.0f, 0.0f),
			slmath::vec3(0.0f, 1.0f, 0.0f));

		m_matModel = slmath::rotationX(-3.1415f*0.5f);
		m_matModel = slmath::rotationY(0) * m_matModel;
		m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel;

		// sharedShaderValues stuff
		m_sharedValues.matModel = m_matModel;
		m_sharedValues.matView = m_matView;
		m_sharedValues.matProj = m_matProjection;

		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;
		slmath::mat4 matNormal = slmath::transpose(slmath::inverse(matModelView));

		m_sharedValues.matModelView = matModelView;
		m_sharedValues.matNormal = matNormal;
		m_sharedValues.matModelViewProj = matModelViewProj;

		m_sharedValues.lightPos = slmath::vec3(0.0f, 70.0f, 70.0f);
		m_sharedValues.camPos = slmath::vec3(cameraX, cameraY, 70.0f);
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

		m_material->bind();
		checkOpenGL();

		m_mesh->render();
		checkOpenGL();
	}
private:
	float m_count;
	float m_totalTime;

	float cameraX = 0.0f;
	float cameraY = 0.0f;

	core::Ref<graphics::Shader> m_shader;

	SharedShaderValues m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;

	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;

	core::Ref<Mesh> m_mesh;
};


