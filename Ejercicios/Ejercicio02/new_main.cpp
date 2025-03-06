#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 objectColor; // Nuevo uniform para el color

void main()
{
    FragColor = vec4(objectColor, 1.0); // Usar el color definido en el uniform
}
)";

// Vértices del cubo y la pirámide
float vertices[] = {
    // === CUBO ===

    // Base inferior
    -0.5f, -0.5f, -0.5f,  // 0
     0.5f, -0.5f, -0.5f,  // 1
     0.5f, -0.5f,  0.5f,  // 2
    -0.5f, -0.5f,  0.5f,  // 3

    // Base superior
    -0.5f,  0.5f, -0.5f,  // 4
     0.5f,  0.5f, -0.5f,  // 5
     0.5f,  0.5f,  0.5f,  // 6
    -0.5f,  0.5f,  0.5f,  // 7

    // === PIRÁMIDE ===
     0.0f,  1.0f,  0.0f,  // 8 (Pico de la pirámide)

     // === PUERTA ===
    -0.15f, -0.5f, -0.51f,  // 9  (Esquina inferior izquierda)
     0.15f, -0.5f, -0.51f,  // 10 (Esquina inferior derecha)
     0.15f,  0.0f, -0.51f,  // 11 (Esquina superior derecha)
    -0.15f,  0.0f, -0.51f,  // 12 (Esquina superior izquierda)
};

unsigned int indices[] = {
    // Caras del cubo (6 caras, 2 triángulos por cara)
    0, 1, 2,  2, 3, 0,  // Base inferior
    4, 5, 6,  6, 7, 4,  // Base superior
    0, 1, 5,  5, 4, 0,  // Lado frontal
    1, 2, 6,  6, 5, 1,  // Lado derecho
    2, 3, 7,  7, 6, 2,  // Lado trasero
    3, 0, 4,  4, 7, 3,  // Lado izquierdo

    // Caras de la pirámide
    4, 5, 8,  // Frente
    5, 6, 8,  // Derecha
    6, 7, 8,  // Atrás
    7, 4, 8,   // Izquierda

    // Caras de la puerta
    // Caras de la puerta
    9, 10, 11,  11, 12, 9,  // Rectángulo de la puerta
};

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Fallo al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar la ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "Casa 3D", NULL, NULL);
    if (!window) {
        std::cerr << "Fallo al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST); // Habilitar el buffer de profundidad

    // Crear shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Configurar VAO, VBO y EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Matrices de Transformación
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(2.0f, 2.0f, 2.0f), // Posición de la cámara
        glm::vec3(0.0f, 0.5f, 0.0f), // Punto donde mira
        glm::vec3(0.0f, 1.0f, 0.0f)  // Vector "arriba" (up)
    );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Enviar matrices a los shaders
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");
        int colorLoc = glGetUniformLocation(shaderProgram, "objectColor"); // Obtener ubicación del color

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        // Dibujar el cubo (Rojo)
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // Rojo
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36 índices del cubo

        // Dibujar la pirámide (Azul)
        glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f); // Azul
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int))); // 12 índices de la pirámide

        // Dibujar la puerta (Verde)
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f); // Verde
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(48 * sizeof(unsigned int))); // 6 índices de la puerta

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
