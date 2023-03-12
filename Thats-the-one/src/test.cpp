#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/Buffers/VertexBufferLayout.h"
#include "Rendering/Buffers/VertexBuffer.h"
#include "Rendering/Buffers/IndexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Texture.h"

#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

int main(void)
{
    int width = 640, height = 480;
    Application app;
    app.Run();

    /*GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    float deltaTime = 0.0f;
    float currentFrame = 0.0f, lastFrame = 0.0f;

    if (glewInit() != GLEW_OK)
        printf("Error");

    float points[] = {
          0.0f,   0.0f, 0.0f, 0.0f, // Bottom left
        100.0f,   0.0f, 1.0f, 0.0f, // Bottom right
          0.0f, 100.0f, 0.0f, 1.0f, // Top left
        100.0f, 100.0f, 1.0f, 1.0f  // Top right
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };    

    Texture texture("res/Textures/grass.png");
    texture.Bind();

    VertexBuffer vb(points, 4 * 4 * sizeof(float));
    vb.Bind();

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    VertexArray va;
    va.AddBuffer(vb, layout);
    va.Bind();    

    IndexBuffer ib(indices, 6);
    ib.Bind();

    Shader shader("res/Shader/Texture.shader");
    shader.Bind();

    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 trans(1.0f);
    float theta = 1.0f;
    
    glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f);
    shader.SetUniformMat4fvLocation("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    shader.SetUniformMat4fvLocation("model", model);
    
    glm::mat4 view = glm::mat4(1.0f);
    shader.SetUniformMat4fvLocation("view", view);


    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        const float cameraSpeed = 100.0f * deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            view = glm::translate(view, cameraPos - cameraSpeed * up);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            view = glm::translate(view, cameraPos + cameraSpeed * up);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            view = glm::translate(view, cameraPos + cameraSpeed * right);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            view = glm::translate(view, cameraPos - cameraSpeed * right);

        shader.SetUniformMat4fvLocation("view", view);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();*/
    return 0;
}