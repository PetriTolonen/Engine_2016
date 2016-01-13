#include "scene.h"
#include <core/log.h>

class FullScreenQuadScene : public Scene
{
public:
	FullScreenQuadScene()
	{
		
		LOG("FullScreenQuadScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_hShaderProgram = 0;

		const char* g_strVSProgram =
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			"attribute vec2 g_vTexCoord;										\n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVSTexCoord;										\n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"	 g_vVSTexCoord = g_vTexCoord;									\n"
			"}																	 \n";


		const char* g_strFSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVSTexCoord;										\n"
			"uniform float g_vTime;											\n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    float x = g_vVSTexCoord.x * 256.0;								\n"
			"    float y = g_vVSTexCoord.y * 256.0;									\n"
			"	 		float vv = 0.25 * (0.5 + (0.5*sin(x / 2.0 + g_vTime))) + (0.5 + (0.5*sin(y / 8.0 - g_vTime))) + (0.5 + (0.5 * sin((x + y) / 6.0 - g_vTime))) + (0.5 + (0.5*sin(sqrt(float(x*x + y*y)) / 4.0 - g_vTime))); \n"
			"    gl_FragColor = g_vVSColor/1.5 * vv;										\n"
			/*"    gl_FragColor = vec4(0.5)*(g_vVSTexCoord.x+g_vVSTexCoord.y);       \n"*/
			"}																	 \n";

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{"g_vVertex", 0 },
			{"g_vColor",  1 },
			{"g_vTexCoord", 2}
		};

		FrmCompileShaderProgram(g_strVSProgram,g_strFSProgram,&m_hShaderProgram,attributes,sizeof(attributes)/sizeof(FRM_SHADER_ATTRIBUTE) );

		checkOpenGL();
	}


    virtual ~FullScreenQuadScene()
	{
		glDeleteProgram( m_hShaderProgram );
		LOG("FullScreenQuadScene destruct");
	}


    virtual void update( graphics::ESContext* esContext, float deltaTime )
	{
		m_count += deltaTime / 15.0f;

		if( m_count > 1.0f )
			m_count = 0.0f;

		m_countTime += (deltaTime / 2.0f);

		if (m_countTime > 10000.0f)
		{
			m_countTime = 0.0f;
		}
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport( 0, 0, esContext->width, esContext->height );
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

		float VertexColors[] = {1.0f, 0.0f, 0.0f, 1.0f,
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
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		checkOpenGL();

		// Set the shader program and the texture
		glUseProgram( m_hShaderProgram );
		checkOpenGL();

		// Draw the colored triangle
		glVertexAttribPointer( 0, 4, GL_FLOAT, 0, 0, VertexPositions );
		checkOpenGL();

		glEnableVertexAttribArray( 0 );
		checkOpenGL();

		glVertexAttribPointer( 1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray( 1 );
		checkOpenGL();

		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, TexCoord);
		checkOpenGL();
		glEnableVertexAttribArray(2);
		checkOpenGL();

		glUniform1f(glGetUniformLocation(m_hShaderProgram, "g_vTime"), m_countTime);
		checkOpenGL();

		glDrawArrays( GL_TRIANGLES, 0, 6 );
		checkOpenGL();

		glDisableVertexAttribArray( 0 );
		checkOpenGL();
		glDisableVertexAttribArray( 1 );
		checkOpenGL();
	}
private:
	float m_count;
	float m_countTime;
	
	GLuint       m_hShaderProgram;

};


