#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow* window);
void error_callback(int error, const char* description);

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow *window = glfwCreateWindow(640, 480, "OpenGL Lightning", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();


    /* loop while window on */
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void processInput(GLFWwindow *window) {

    // Escape for quitting:
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}