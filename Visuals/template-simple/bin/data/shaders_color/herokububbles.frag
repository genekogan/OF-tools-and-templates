#version 120

uniform float time;
uniform vec2 mous;
uniform vec2 resolution;
uniform float fRadius;

void main(void)
{
	vec2 uv = -1.0 + 2.0*gl_FragCoord.xy / resolution.xy;
	uv.x *=  resolution.x / resolution.y;
	
	vec3 color = vec3(0.0);

    // bubbles
	for( float i=0.0; i<60.0; i++ )
	{
        // bubble seeds
		float pha = sin(i*5.13+1.0)*0.5 + 0.5;
		float siz = pow( sin(i*1.74+5.0)*0.5 + 0.5, 4.0 );
		float pox = sin(i*3.55+4.1) * resolution.x / resolution.y;
		
        	// buble size, position and color
		vec2  pos = vec2( pox+sin(time/30.+pha+siz), -1.0-fRadius + (2.0+2.0*fRadius)
						 *mod(pha+0.1*(time/5.)*(0.2+0.8*siz),1.0)) * vec2(1.0, 1.0);
		float dis = length( uv - pos );

		
        	// render
		if(dis < fRadius){
			vec3  col = mix( vec3(0.8, 0.2, 0.0), vec3(0.8,0.5,0.2), 0.5+0.5*sin(i*sin(time*pox*0.03)+1.9));
			color += col.xyz *(1.- smoothstep( fRadius*(0.65+0.20*sin(pox*time)), fRadius, dis )) * (1.0 - cos(pox*time));	
		}
	}

	gl_FragColor = vec4(color,1.0);
}