#version 430 core
in vec4 gl_FragCoord;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

layout(location = 100) uniform vec2 u_resolution;

layout(location = 101) uniform vec2 u_mouse;


uniform sampler2D ourTexture;

void main() {
	FragColor = texture(ourTexture, TexCoord);
} 
