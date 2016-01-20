#ifdef GL_FRAGMENT_PRECISION_HIGH									
   precision highp float;										
#else															
   precision mediump float;										
#endif															
																
varying   vec4 g_vVSColor;                                      
varying   vec2 g_vVSTexCoord;									
uniform float totalTime;											
																
void main()														
{																
    float x = g_vVSTexCoord.x * 256.0;							
    float y = g_vVSTexCoord.y * 256.0;							
	 		float vv = 0.25 * (0.5 + (0.5*tan(x / 2.0 + totalTime))) + (0.5 + (0.5*sin(y / 2.0 - totalTime))) + (0.5 + (0.5 * sin((x + y) / 8.0 - totalTime))) + (0.5 + (0.5*sin(sqrt(float(x*x + y*y)) / 4.0 - totalTime))); 
    gl_FragColor = g_vVSColor/1.5 * vv;
}