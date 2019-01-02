// 三角形内三原色板
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

const char *vertexShaderSource =	// 顶点着色器源代码 此处主要处理位置
"#version 330 core\n"	// 注意OpenGL4版本的GLSL不同
"layout (location = 0) in vec3 apos;\n"
"layout (location = 1) in vec3 acolor;\n"
"out vec3 ourcolor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(apos.x,apos.y,apos.z,1.0f);\n"
"	ourcolor = acolor;\n"
"}\n\0";

const char *fragmentShaderSource =	// 片段着色器源代码 此处主要设置颜色RGBA
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
	glViewport(0, 0, 800, 600); // 暂时未生效。。。
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	// 链接源代码
	glCompileShader(vertexShader); // 编译
	int isSuccess;
	char info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess); // 获取是否成功创建
	if (!isSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		cout << "Failed: " << info << endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	// 创建片段着色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		cout << "Failed: " << info << endl;
	}

	GLuint shaderProgram = glCreateProgram(); // 创建一个空程序
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // 链接着色器程序
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		cout << "Failed: " << info << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 2, // 第一个三角形
	};

	GLuint VAO, VBO, EBO; // 新建VAO以及VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO); // 绑定VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定VBO 默认加入上次绑定的VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 设置顶点属性指针 给着色器位置0的属性提供如何解析数据VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // 打开属性开关 3，4版本默认打开不同
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO
	glBindVertexArray(0); // 解绑VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 解绑EBO
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 使用线框模式 GL_LINE => GL_FILL 填充模式

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 0.5f); // 以何种颜色清空
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram); // 使用着色器
		glBindVertexArray(VAO); // 绑定VAO 其实不用每次绑定 因为这里没有VAO的切换
		// glDrawArrays(GL_LINE_LOOP,0,5); // 画三角形，从0开始取3个顶点 VAO画法
		glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(100);
	}

	glfwTerminate();
	return 0;
}
