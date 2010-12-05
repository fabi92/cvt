#version 120
varying vec4 vtx_Color;
varying vec2 vtx_Index;
uniform sampler2D TexFont;
uniform float Scale;

void main()
{
	vec4 c = vtx_Color;
	c.a *= texture2D( TexFont, gl_PointCoord * Scale + vtx_Index * Scale ).r;
	gl_FragColor = c;
}