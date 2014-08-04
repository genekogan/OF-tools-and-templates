#version 120

uniform sampler2DRect texture;
uniform vec2 resolution;
uniform float radius;
uniform float radTwist;

void main(void) {
    vec2 p = -1.0 + 2.0 * gl_FragCoord.st / resolution.xy;
    float r = sqrt(dot(p,p));

	p.x = mod(p.x + r * radTwist, 1.0);
    float a = atan(p.y,p.x);

    vec2 uv;
	uv.x = (a + 3.14159265359)/6.28318530718;
	uv.y = r / sqrt(radius);
    vec3 col = texture2DRect(texture, uv*resolution.xy).rgb;
    gl_FragColor = vec4(col, 1.0);
}
