#include <GL/gl.h>

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

#include <td/core/img.h>
#include <cstring>
#include <iomanip>

int main()
{
    td::img i;

    std::ifstream in("example.png", std::ios_base::binary);
    if (!in) {
        std::cout << "cant open " << strerror(errno) << std::endl;
        return -1;
    }

    i.load_png(in);

    std::cout << i.width() << "x" << i.height() << " (depth " << static_cast<size_t>(i.depth()) << "; color type: " << i.ctype() << ")" << std::endl;

    uint32_t* pp = reinterpret_cast<uint32_t*>(i.data());
    for (size_t i = 0; i < 100; i++) {
        std::cout << std::setw(2) << std::setfill('0') << std::hex << pp[i] << std::endl;
    }

//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//
//        glClearColor(1.0, 0.0, 0.0, 1.0);
//
//        /* Render here */
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
    return 0;
}