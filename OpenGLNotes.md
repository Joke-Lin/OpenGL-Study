# OpenGL Notes

## 1、配置环境 GLFW & GLAD

下载GLFW源码包，使用CMake生成VS2017项目，使用VS2017编译生成**glfw32.lib**(在src目录下)，在[GLAD在线服务](http://glad.dav1d.de/) 设置语言，版本以及**Core**模式，并且保证生成**加载器**(Generate a loader)，下载其zip文件。

## 2、第一个窗口

```c++
int main()
{
    glfwInit(); // 初始化 glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // iOS 配置
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // 退出
        return -1;
    }
    glfwMakeContextCurrent(window); // 设置当前线程主上下文
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // 加载OpenGL函数指针
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600); // 设置视口 左下角，高，宽
    // 渲染循环
    while(!glfwWindowShouldClose(window))
    {
        // 输入
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

        // 渲染指令
        ...

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}
```

## 3、Hello Triangle

- 顶点数组对象：Vertex Array Object，VAO

- 顶点缓冲对象：Vertex Buffer Object，VBO

- 索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，IBO

- 图形渲染管线：数据 →顶点着色器→图元装配→几何着色器→光栅化→片段着色器→测试与混合

- OpenGL着色器语言：OpenGL Shading Language, GLSL

- 标准化设备坐标：Normalized Device Coordinates, NDC，中心为坐标原点且范围为（-1，

  > 在OpenGL中，任何事物都在3D空间中，屏幕和窗口却是2D像素素组，图形渲染管线将3D坐标转换为2D像素，2D坐标和2D像素也是不同的，坐标是可以任意位置（浮点数），而2D像素只能说整数，也就是坐标的近似值，收到屏幕和窗口的限制

### VBO

顶点缓冲对象只是一个数据数组，可以存任何你想存的东西，数据本身并没有意义，在绘制开始之前将整个批次复制到缓冲区中，而这将位于**图形硬件内存上**。这对于绘图来说效率更高，因为尽管CPU和GPU之间的总线非常宽，但是当绘制操作停止以将OpenGL命令从CPU发送到GPU时，会产生绘图性能的瓶颈。为了避免这种情况，我们尽可能地在图形硬件上保留尽可能多的数据和处理。

通过[glDrawArrays（）](http://www.opengl.org/sdk/docs/man4/xhtml/glDrawArrays.xml) 可以采取不同的渲染模式：

![](C:\Users\23113\source\repos\IMAGE_Game\OpenGLStudy\src\rend_mode.jpg)

你可以为一个对象（比如三角形）创建许多的VBO管理不同的数据，比如三个顶点的颜色和数据，那么如何有效的管理这些数据（否则你就要一次次重复复杂的工作了，不断的绑定VBO，链接顶点属性。。。）这样就需要VAO的帮助

### VAO

> 在OpenGl核心模式下必须采用VAO模式

**VAO**本身就是一个容器（可以容纳许多的对象，比如VBO），VAO会在存储绑定之后（通过```glBindVertexArray（VAO）;```) 所有的VBO，也就是说它存储的是一个状态，每次切换状态只需要绑定不同的VAO即可。

### 总结

OpenGl画图需要数据（来自VBO，VAO），通过定义顶点属性指针告诉OpenGl如何解析数据，然后传递给顶点和片段着色器，最后在窗口上展现，这是一个复杂的过程

