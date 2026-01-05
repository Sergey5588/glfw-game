#version 430 core
out vec4 FragColor;

layout(location = 100) uniform vec2 u_resolution;

vec2 i_mult(vec2 z1, vec2 z2) {
	return vec2(z1.x*z2.x - z1.y*z2.y, z1.x*z2.y + z1.y*z2.x);
}
vec2 mandelbrot(vec2 z, vec2 c) {
	return i_mult(z,z)+c;
}

void main() {	
	vec2 z = vec2(0);
	vec2 uv = gl_FragCoord.xy/u_resolution;
	uv-=vec2(0.5);
	uv*=3.0f;
	vec2 c = uv;
	for(int i =0; i < 40; i++) {
		z = mandelbrot(z,c);
		if(length(z)>2.0f) {
			FragColor = vec4(0,0,0,1);
			return;
		}
			
	}
    FragColor = vec4(1);
	return;
} 
