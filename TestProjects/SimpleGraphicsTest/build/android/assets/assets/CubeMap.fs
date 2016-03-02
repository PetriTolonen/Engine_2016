precision mediump float;

#ifdef GL_FRAGMENT_PRECISION_HIGH
	#define NEED_HIGHP highp
#else
	#define NEED_HIGHP mediump
#endif

#define saturate(x) clamp(x,0.0,1.0)

struct MATERIAL
{
	vec4 vAmbient;
	vec4 vDiffuse;
	vec4 vSpecular;
	vec4 vGlossyness;
};

uniform sampler2D difTexture;
uniform samplerCube cubeTexture;
uniform sampler2D glossyTexture;
uniform MATERIAL g_Material;

varying   vec3 g_vViewVecES;
varying   vec3 g_vNormalES;	
varying   vec3 g_vLightVecES;

varying   vec3 g_vTexCoordsES;

void main()
{
	vec3 vNormal = normalize(g_vNormalES);
	vec3 vLight = normalize(g_vLightVecES);
	vec3 vView = normalize(g_vViewVecES);
	vec3 vHalf = normalize(vLight+vView);
	
	vec4 vEnvColor = textureCube(cubeTexture, reflect(vView, vNormal));
	
	float NdotL = saturate(dot(vNormal, vLight));
	float NdotH = saturate(dot(vNormal, vHalf));
	
	vec4 texColor = texture2D(difTexture, vec2(g_vTexCoordsES),1.0).rgba;
	vec4 glossyColor = texture2D(glossyTexture, vec2(g_vTexCoordsES),1.0).rgba;
	float fDiffuse = NdotL;	
	
	float SelfShadow = 4.0 * fDiffuse;
	float glossyFactor = g_Material.vGlossyness.x * glossyColor.x;
	vec4 color = (1.0-glossyFactor)*texColor + (glossyFactor)*vEnvColor;
	
	float fSpecular = pow(NdotH, g_Material.vSpecular.w)*glossyFactor;
	
	gl_FragColor.rgba = vec4(0.15, 0.15, 0.15, 1.0) * g_Material.vAmbient*color;
	gl_FragColor.rgba += g_Material.vDiffuse * color * fDiffuse;
	gl_FragColor.rgb += SelfShadow * vec3(0.15,0.15,0.15) * g_Material.vSpecular.xyz*fSpecular;
}