uniform mat 4 g_matView;
uniform mat4 g_matModelView;
uniform mat4 g_matModelViewProj;
uniform mat4 g_matNormal;
uniform vec3 g_lightPos;

attribute vec4 g_vPositionOS;
attribute vec3 g_vNormalOS;
attribute vec2 g_vTexCoord;	

varying   vec2 g_vTexCoordES;
varying   vec3 g_vNormalES;
varying   vec3 g_vViewVecES;
varying   vec3 g_vLightVecES;

void main()
{
	vec4 vPositionES = g_matModelView     * g_vPositionOS;
	vec4 vPositionCS = g_matModelViewProj * g_vPositionOS;
	vec3 vLightPosES = (g_matView * vec4(g_lightPos, 1.0)).xyx;
	
	vec3 vNormalES = (g_matNormal * vec4(g_vNormalOS, 0.0)).xyz;
	
	gl_Position = vPositionCS;
	g_vNormalES = normalize(vNormalES.xyz);
	g_vViewVecES = normalize(-vPositionES.xyz);
	g_vLightVecES = normalize(vLightPosES - vPositionES.xyz);
}