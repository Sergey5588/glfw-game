#include <complex.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include "linmath.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define rad(n) n*M_PI/180.0
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLuint compile_shader(const GLchar* source, GLenum type);

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char* message, const void* userParam);
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int g_width = 800;
int g_height = 600;

double m_x, m_y;

vec3 cameraPos   = {0.0f, 0.0f,  3.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp    = {0.0f, 1.0f,  0.0f};

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float lastX = 400, lastY = 300;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
int main(void) {

	const char vertexShaderSource[] = {
		#embed "shader.vert"
		, 0
	};
	const char fragmentShaderSource[] = {
		#embed "shader.frag"
		, 0
	};
	const GLchar* vertSourcePtr = vertexShaderSource;
	const GLchar* fragSourcePtr = fragmentShaderSource;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(g_width, g_height, "glfw-game", NULL, NULL);
	if(window == NULL) {
		fprintf(stderr, "Failed to open window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to init GLAD");
		return -1;
	}
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Makes errors easier to trace
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	//OpenGL setup
	glViewport(0,0,800,800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);  
	glEnable(GL_DEPTH_TEST);  

	GLuint vertexShader, fragmentShader;
	vertexShader = compile_shader(vertexShaderSource, GL_VERTEX_SHADER);
	fragmentShader = compile_shader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 
	//color 
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
//	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);  

	GLuint texture;
	glGenTextures(1, &texture);  
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int img_width, img_height, nrChannels;
	unsigned char *img_cat = stbi_load("cat.png", &img_width, &img_height, &nrChannels, 0);
	if(img_cat) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_cat);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		fprintf(stderr, "failed to open cat.png");
	}
	stbi_image_free(img_cat);
	
	mat4x4 model = {
		{1.0f, 0.0f,0.0f,0.0f},
		{0.0f, 1.0f,0.0f,0.0f},
		{0.0f, 0.0f,1.0f,0.0f},
		{0.0f, 0.0f,0.0f,1.0f},
	};
	//scaling -> rotation ->translation
	
//	mat4x4_rotate(model,model, 0.0,0.0,1.0, M_PI/2);
	
	mat4x4 view = {
		{1.0f, 0.0f,0.0f,0.0f},
		{0.0f, 1.0f,0.0f,0.0f},
		{0.0f, 0.0f,1.0f,0.0f},
		{0.0f, 0.0f,0.0f,1.0f},
	};
	mat4x4_translate(view, 0,0,-3.0f);
	mat4x4 proj = {0};
	mat4x4_perspective(proj, rad(45.0f), (float)g_width/(float)g_height, 0.1f, 100.0f);
	vec3 cubePositions[] = {
		{ 0.0f,  0.0f,  0.0f},
		{ 2.0f,  5.0f, -15.0f}, 
		{-1.5f, -2.2f, -2.5f},  
		{-3.8f, -2.0f, -12.3f},  
		{ 2.4f, -0.4f, -3.5f},  
		{-1.7f,  3.0f, -7.5f},  
		{ 1.3f, -2.0f, -2.5f},  
		{ 1.5f,  2.0f, -2.5f}, 
		{ 1.5f,  0.2f, -1.5f}, 
		{-1.3f,  1.0f, -1.5f}  
	};
	while(!glfwWindowShouldClose(window)) {
		//input
		processInput(window);
		//render

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
//		mat4x4_rotate_Y(model, model, 0.05f);
		vec3 pPf = {0};
		vec3_add(pPf, cameraPos, cameraFront);
		mat4x4_look_at(view,cameraPos, pPf,cameraUp);

		mat4x4_perspective(proj, rad(45.0f), (float)g_width/(float)g_height, 0.1f, 100.0f);

		glClearColor(0.2f,0.3f,0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
//		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &proj[0][0]);

		glUniform2f(glGetUniformLocation(shaderProgram, "u_resolution"), (float)g_width, (float)g_height);
		glUniform2f(glGetUniformLocation(shaderProgram, "u_mouse"), (float)m_x, (float)m_y);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		for(int i = 0; i < 10; i++) {
			mat4x4 test_model = {
				{1.0f, 0.0f,0.0f,0.0f},
				{0.0f, 1.0f,0.0f,0.0f},
				{0.0f, 0.0f,1.0f,0.0f},
				{0.0f, 0.0f,0.0f,1.0f},
			};
			mat4x4_translate(test_model, cubePositions[i][0],cubePositions[i][1],cubePositions[i][2]);
			float ang = 20.0f * i;
			mat4x4_rotate(test_model, test_model, 1.0f, 0.3f, 0.5f, rad(ang));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &test_model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
//		glDrawArrays(GL_TRIANGLES, 0, 36);
		//check and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	g_width = width;
	g_height = height;
	glViewport(0,0,width,height);
	
}
void processInput(GLFWwindow *window) {
	const float cameraSpeed = 5.0f*deltaTime; // adjust accordingly
	glfwGetCursorPos(window, &m_x, &m_y);
	vec3 mul = {0};
	vec3_scale(mul, cameraFront, cameraSpeed);
	vec3 cross_mul = {0};
	vec3_mul_cross(cross_mul, cameraFront, cameraUp);
	vec3_norm(cross_mul, cross_mul);
	vec3_scale(cross_mul, cross_mul, cameraSpeed);
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		vec3_add(cameraPos, cameraPos, mul); 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		vec3_sub(cameraPos, cameraPos, mul); 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		vec3_sub(cameraPos, cameraPos, cross_mul);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		vec3_add(cameraPos, cameraPos, cross_mul);
}
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char* message, const void* userParam) {
//	return;
    // Filter out non-essential notifications
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    // Output the debug message
	printf("[OpenGL] %s\n", message);
    // You can expand this to log source, type, and severity[citation:1]
}
GLuint compile_shader(const GLchar* source, GLenum type) {
	GLuint shader = glCreateShader(type);
	
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "Shader compilation error: %s \n", infoLog);
	}
	return shader;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	m_x = xpos;
	m_y = ypos;

	if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction = {0};
    direction[0] = cos(rad(yaw)) * cos(rad(pitch));
    direction[1] = sin(rad(pitch));
    direction[2] = sin(rad(yaw)) * cos(rad(pitch));

	vec3_norm(cameraFront, direction);
}
