#version 120

uniform sampler2DRect texture;
uniform int pixelsPerRow;
uniform vec2 resolution;

void main(void)
{
	vec2 p = gl_FragCoord.st/resolution.xy;
	float pixelSize = 1.0 / float(pixelsPerRow);
	
	float dx = mod(p.x, pixelSize) - pixelSize*0.5;
	float dy = mod(p.y, pixelSize) - pixelSize*0.5;
	
	p.x -= dx;
	p.y -= dy;
	vec3 col = texture2DRect(texture, p*resolution.xy).rgb;
	float bright = 0.3333*(col.r+col.g+col.b);
	
	float dist = sqrt(dx*dx + dy*dy);
	float rad = bright * pixelSize * 0.00008;
	float m = step(dist, rad);

	vec3 col2 = mix(vec3(0.0), vec3(1.0), m);
	gl_FragColor = vec4(col2, 1.0);
}

