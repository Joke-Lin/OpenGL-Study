/* 顶点着色器
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 atexCoord;

uniform mat4 transform;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
	gl_Position = transform*vec4(aPos,1.0f);
	texCoord = atexCoord;
};
*/

/* 片段着色器
#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture1;

void main()
{
   FragColor = texture(texture1,texCoord);
};
*/

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>

#include <std_image.h>
#include <shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 设置主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 设置次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 设置采用内核模式

	GLFWwindow* window = glfwCreateWindow(800, 600, "myFirstOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Fail to initialize GLAD" << endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	Shader shaderProgram("vertexShader.vert", "fragmentShader.frag");
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		// positions           // texture coords
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left
	};

	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	Shader myShader("vertexShader.vert", "fragmentShader.frag");
	// 开辟空间
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 绑定VAO
	glBindVertexArray(VAO);
	// 绑定VBO EBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 设置顶点数据解析
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 配置纹理
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // 采用默认重复和过滤
	//// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("monster.jpg", &width, &height, &channels, 0); // 自己的图片就行
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else cout << "Failed to load img" << endl;
	stbi_image_free(data);
	myShader.use();
	myShader.setInt("texture1", 0); // 链接纹理位置

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glm::mat4 transform = glm::mat4(1.0f);
		float time = glfwGetTime();
		transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
		time = abs(sin(time));
		transform = glm::scale(transform, glm::vec3(time, time, time));

		myShader.use();
		GLuint transformLoc = glGetUniformLocation(myShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(50);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}
