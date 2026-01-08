#version 330 core
in vec4 gl_FragCoord;
out vec4 FragColor;

in vec2 TexCoord;
in mat4 transform;

uniform vec2 u_resolution;

uniform vec2 u_mouse;


uniform sampler2D ourTexture;

void main() {
	FragColor = texture(ourTexture, TexCoord);
} 
