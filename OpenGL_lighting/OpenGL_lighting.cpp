#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"


void processInput(GLFWwindow* window);
void error_callback(int error, const char* description);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);

// constant positions
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float lastX = 320.0f;
float lastY = 240.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Lightning", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);


    Shader modelShader("lightningShader.vert", "lightningShader.frag");
    Model backpack("C:\\Users\\guilh\\Downloads\\backpack\\backpack.obj");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };

    /* loop while window on */
    while (!glfwWindowShouldClose(window))
    {
        // delta time math:
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // input:
        processInput(window);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // matrices math:
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)320 / (float)240, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // activate shader and set uniforms:
        //lightingShader.use();
        //lightingShader.setMat4("projection", projection);
        //lightingShader.setMat4("view", view);
        //lightingShader.setMat4("model", model);
        //
        //// material stuff:
        //glm::vec3 ambientColor = glm::vec3(1.0f, 0.5f, 0.31f);
        //glm::vec3 diffuseColor = glm::vec3(1.0f, 0.5f, 0.31f);
        //glm::vec3 specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
        //lightingShader.setVec3("material.ambient", ambientColor);
        //lightingShader.setVec3("material.diffuse", diffuseColor);
        //lightingShader.setVec3("material.specular", specularColor);
        //lightingShader.setFloat("material.shininess", 32.0f);
        // directional light:
        glm::vec3 dirLightDirection = glm::vec3(-0.5f, -1.0f, -0.3f);
        glm::vec3 dirLightAmbient   = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 dirLightDiffuse   = glm::vec3(0.4f, 0.4f, 0.4f);
        glm::vec3 dirLightSpecular  = glm::vec3(0.1f, 0.1f, 0.f);
        modelShader.setVec3("dLight.direction", dirLightDirection);
        modelShader.setVec3("dLight.ambient", dirLightAmbient);
        modelShader.setVec3("dLight.diffuse", dirLightDiffuse);
        modelShader.setVec3("dLight.specular", dirLightSpecular);
        glm::vec3 poLightDiffuse  = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 poLightSpecular = glm::vec3(0.1f, 0.1f, 0.1f);
        // point light 1:
        modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        modelShader.setVec3("pointLights[0].ambient", dirLightAmbient);
        modelShader.setVec3("pointLights[0].diffuse", poLightDiffuse);
        modelShader.setVec3("pointLights[0].specular", poLightSpecular);
        modelShader.setFloat("pointLights[0].constant", 1.0f);
        modelShader.setFloat("pointLights[0].linear", 0.09f);
        modelShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2:
        modelShader.setVec3("pointLights[1].position", pointLightPositions[0]);
        modelShader.setVec3("pointLights[1].ambient", dirLightAmbient);
        modelShader.setVec3("pointLights[1].diffuse", poLightDiffuse);
        modelShader.setVec3("pointLights[1].specular", poLightSpecular);
        modelShader.setFloat("pointLights[1].constant", 1.0f);
        modelShader.setFloat("pointLights[1].linear", 0.09f);
        modelShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3:
        modelShader.setVec3("pointLights[2].position", pointLightPositions[0]);
        modelShader.setVec3("pointLights[2].ambient", dirLightAmbient);
        modelShader.setVec3("pointLights[2].diffuse", poLightDiffuse);
        modelShader.setVec3("pointLights[2].specular", poLightSpecular);
        modelShader.setFloat("pointLights[2].constant", 1.0f);
        modelShader.setFloat("pointLights[2].linear", 0.09f);
        modelShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4:
        modelShader.setVec3("pointLights[3].position", pointLightPositions[0]);
        modelShader.setVec3("pointLights[3].ambient", dirLightAmbient);
        modelShader.setVec3("pointLights[3].diffuse", poLightDiffuse);
        modelShader.setVec3("pointLights[3].specular", poLightSpecular);
        modelShader.setFloat("pointLights[3].constant", 1.0f);
        modelShader.setFloat("pointLights[3].linear", 0.09f);
        modelShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spot light:
        glm::vec3 spotLightAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 spotLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        modelShader.setVec3("sLight.position", camera.Position);
        modelShader.setVec3("sLight.direction", camera.Front);
        modelShader.setVec3("sLight.ambient", spotLightAmbient);
        modelShader.setVec3("sLight.diffuse", spotLightDiffuse);
        modelShader.setVec3("sLight.specular", spotLightDiffuse);
        modelShader.setFloat("sLight.constant", 1.0f);
        modelShader.setFloat("sLight.linear", 0.09f);
        modelShader.setFloat("sLight.quadratic", 0.032f);
        modelShader.setFloat("sLight.incutoff", glm::cos(glm::radians(12.5f)));
        modelShader.setFloat("sLight.outcutoff", glm::cos(glm::radians(17.0f)));
        
        //// diffuse map:
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, diffuseMap);
        //// specular map
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, specularMap);
        //
        //glBindVertexArray(VAO);
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    glm::mat4 model = glm::mat4(1.0f);
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    lightingShader.setMat4("model", model);
        //
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}
        //
        //constLightShader.use();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f));
        //constLightShader.setMat4("projection", projection);
        //constLightShader.setMat4("view", view);
        //constLightShader.setMat4("model", model);
        //constLightShader.setVec3("lightColor", dirLightAmbient);
        //
        //glBindVertexArray(lightVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", camera.GetViewMatrix());
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        modelShader.setMat4("model", model);
        
        backpack.Draw(modelShader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    camera.ProcessMouseMov(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {

    // Escape for quitting:
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

}
