#version 120

uniform sampler2DRect texture;
uniform float brt;
uniform int rad;

//const vec2 texOffset = vec2(1.0, 1.0);

void main(void) {
  int i = 0;
  int j = 0;
  vec4 sum = vec4(0.0);
  
  for( i=-rad;i<rad;i++) {
    for( j=-rad;j<rad;j++) {
        sum += texture2DRect( texture, gl_FragCoord.st + vec2(j,i))*brt;
    }
  }

  gl_FragColor = sum*sum+ vec4(texture2DRect( texture, gl_FragCoord.st).rgb, 1.0);
}
