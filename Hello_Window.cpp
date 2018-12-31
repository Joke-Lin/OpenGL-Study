#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 设置主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 设置次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 设置为核心模式

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

	glViewport(100, 100, 100, 100); // 设置显示的区域 （清空函数会清空整个屏幕）

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.0f, 1.0f, 0.3f, 1.0f); // 设置清屏颜色 默认为黑色
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}