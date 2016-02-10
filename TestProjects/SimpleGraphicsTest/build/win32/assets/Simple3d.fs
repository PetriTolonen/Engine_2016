precision mediump float;

#ifdef GL_FRAGMENT_PRECISION_HIGH
	#define NEED_HIGHP highp
#else
	#define NEED_HIGHP mediump
#endif

varying   vec2 g_vVSTexCoord;
varying   vec3 g_vVSNormalColor;	
uniform float totalTime;	

void main()
{
 float x = g_vVSTexCoord.x * 256.0;							
    float y = g_vVSTexCoord.y * 256.0;								
	float vv = 0.25 * sin(0.5 + (0.5*sin(x / 7.0 + totalTime))) + (0.5 + (0.5*sin(y / 5.0 - totalTime))) + (0.5 + (0.5 * sin((x + y) / 6.0 - totalTime))) + (0.5 + (0.5*sin(sqrt(float(x*x + y*y)) / 4.0 - totalTime))); 		
    vv = vv/2.5;
	if(vv < 0.9)
	gl_FragColor = vec4(vv, vv, vv, 1.0);
	else
	discard;
}