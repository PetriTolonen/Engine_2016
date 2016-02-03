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
};

uniform MATERIAL g_Material;

varying   vec2 g_vVSTexCoord;

varying   vec3 g_vViewVecES;
varying   vec3 g_vNormalES;	
varying   vec3 g_vLightVecES;

uniform float totalTime;	

void main()
{
	vec3 vNormal = normalize(g_vNormalES);
	vec3 vLight = normalize(g_vLightVecES);
	vec3 vView = normalize(g_vViewVecES);
	vec3 vHalf = normalize(vLight+vView);
	
	float NdotL = saturate(dot(vNormal), vLight));
	float NdotH = saturate(dot(vNormal), vHalf));
	
	float fDiffuse = NdotL;
	
	float fSpecular = pow(NdotH, g_Material.vSpecular.w);
	float SelfShadow = 4.0 * fDiffuse;
	
	gl_FragColor.rgba = vec4(0.15,0.15,0.15,0.15)*g_Material.vAmbient;
	gl_FragColor.rgba += g_Material.vDiffuse * fDiffuse;
	gl_FragColor += SelfShadow * vec3(0.15,0.15,0.15) * g_Material.vSpecular.xyz*fSpecular;
}