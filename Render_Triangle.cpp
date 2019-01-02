// ����������ԭɫ��
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

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

const char *vertexShaderSource =	// ������ɫ��Դ���� �˴���Ҫ����λ��
"#version 330 core\n"	// ע��OpenGL4�汾��GLSL��ͬ
"layout (location = 0) in vec3 apos;\n"
"layout (location = 1) in vec3 acolor;\n"
"out vec3 ourcolor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(apos.x,apos.y,apos.z,1.0f);\n"
"	ourcolor = acolor;\n"
"}\n\0";

const char *fragmentShaderSource =	// Ƭ����ɫ��Դ���� �˴���Ҫ������ɫRGBA
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourcolor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourcolor,1.0f);\n"
"}\n\0";


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // �������汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ���ôΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ���ò����ں�ģʽ

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
	glViewport(0, 0, 800, 600); // ��ʱδ��Ч������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // ����������ɫ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	// ����Դ����
	glCompileShader(vertexShader); // ����
	int isSuccess;
	char info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess); // ��ȡ�Ƿ�ɹ�����
	if (!isSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		cout << "Failed: " << info << endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	// ����Ƭ����ɫ��
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		cout << "Failed: " << info << endl;
	}

	GLuint shaderProgram = glCreateProgram(); // ����һ���ճ���
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // ������ɫ������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		cout << "Failed: " << info << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};

	unsigned int indices[] = { // ע��������0��ʼ! 
		0, 1, 2, // ��һ��������
	};

	GLuint VAO, VBO, EBO; // �½�VAO�Լ�VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO); // ��VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ��VBO Ĭ�ϼ����ϴΰ󶨵�VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// ���ö�������ָ�� ����ɫ��λ��0�������ṩ��ν�������VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // �����Կ��� 3��4�汾Ĭ�ϴ򿪲�ͬ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // ���VBO
	glBindVertexArray(0); // ���VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // ���EBO
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // ʹ���߿�ģʽ GL_LINE => GL_FILL ���ģʽ

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 0.5f); // �Ժ�����ɫ���
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram); // ʹ����ɫ��
		glBindVertexArray(VAO); // ��VAO ��ʵ����ÿ�ΰ� ��Ϊ����û��VAO���л�
		// glDrawArrays(GL_LINE_LOOP,0,5); // �������Σ���0��ʼȡ3������ VAO����
		glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(100);
	}

	glfwTerminate();
	return 0;
}
