uniform mat4 g_matModelViewProj;

attribute vec4 g_vPositionOS;
attribute vec3 g_vNormalOS;

attribute vec2 g_vTexCoord;	

varying   vec2 g_vVSTexCoord;

varying   vec3 g_vVSNormalColor;

void main()
{
	vec4 vPositionCS = g_matModelViewProj * g_vPositionOS;
	
	gl_Position = vPositionCS;
	
		 g_vVSTexCoord = g_vTexCoord;

	g_vVSNormalColor = g_vNormalOS;		 
}