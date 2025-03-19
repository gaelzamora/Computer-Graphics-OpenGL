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
uniform vec3 objectColor; // Uniform para el color

void main()
{
    FragColor = vec4(objectColor, 1.0); // Usar el color definido en el uniform
}
)";

float vertices[] = {
    // CUBO
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

    // PIRÁMIDE
     0.0f,  1.0f,  0.0f,  // 8 (Pico de la pirámide)

     // === PUERTA (Lado Frontal) ===
     -0.15f, -0.5f, 0.501f,  // 9  (Esquina inferior izquierda)
      0.15f, -0.5f, 0.501f,  // 10 (Esquina inferior derecha)
      0.15f, -0.2f, 0.501f,  // 11 (Esquina superior derecha)
     -0.15f, -0.2f, 0.501f,  // 12 (Esquina superior izquierda)

     // === VENTANA 1 (Arriba Izquierda) ===
     -0.4f,  0.1f, 0.501f,  // 13 (Esquina inferior izquierda)
     -0.1f,  0.1f, 0.501f,  // 14 (Esquina inferior derecha)
     -0.1f,  0.4f, 0.501f,  // 15 (Esquina superior derecha)
     -0.4f,  0.4f, 0.501f,  // 16 (Esquina superior izquierda)

     // === VENTANA 2 (Arriba Derecha) ===
      0.4f,  0.1f, 0.501f,  // 17 (Esquina inferior izquierda)
      0.1f,  0.1f, 0.501f,  // 18 (Esquina inferior derecha)
      0.1f,  0.4f, 0.501f,  // 19 (Esquina superior derecha)
      0.4f,  0.4f, 0.501f,  // 20 (Esquina superior izquierda)

      // === PINO 1 (IZQUIERDA) ===
      // Tronco (cubo pequeño)
      -1.2f, -0.5f, -0.2f,  // 21 Frontal inferior izquierda
      -0.9f, -0.5f, -0.2f,  // 22 Frontal inferior derecha
      -0.9f, -0.5f,  0.1f,  // 23 Trasera inferior derecha
      -1.2f, -0.5f,  0.1f,  // 24 Trasera inferior izquierda
      -1.2f, -0.1f, -0.2f,  // 25 Frontal superior izquierda
      -0.9f, -0.1f, -0.2f,  // 26 Frontal superior derecha
      -0.9f, -0.1f,  0.1f,  // 27 Trasera superior derecha
      -1.2f, -0.1f,  0.1f,  // 28 Trasera superior izquierda

      // Copa (pirámide)
      -1.05f, 0.5f, -0.05f,  // 29 Pico de la pirámide

      // === PINO 2 (DERECHA) ===
      // Tronco (cubo pequeño)
       1.2f, -0.5f, -0.2f,  // 30 Frontal inferior izquierda
       0.9f, -0.5f, -0.2f,  // 31 Frontal inferior derecha
       0.9f, -0.5f,  0.1f,  // 32 Trasera inferior derecha
       1.2f, -0.5f,  0.1f,  // 33 Trasera inferior izquierda
       1.2f, -0.1f, -0.2f,  // 34 Frontal superior izquierda
       0.9f, -0.1f, -0.2f,  // 35 Frontal superior derecha
       0.9f, -0.1f,  0.1f,  // 36 Trasera superior derecha
       1.2f, -0.1f,  0.1f,  // 37 Trasera superior izquierda

       // Copa (pirámide)
        1.05f, 0.5f, -0.05f,  // 38 Pico de la pirámide
};

unsigned int indices[] = {
    // Caras del cubo (CASA) (6 caras, 2 triángulos por cara)
    0, 1, 2,  2, 3, 0,  // Base inferior (0-5)
    4, 5, 6,  6, 7, 4,  // Base superior (6-11)
    0, 1, 5,  5, 4, 0,  // Lado frontal (12-17)
    1, 2, 6,  6, 5, 1,  // Lado derecho (18-23)
    2, 3, 7,  7, 6, 2,  // Lado trasero (24-29)
    3, 0, 4,  4, 7, 3,  // Lado izquierdo (30-35)

    // Caras de la pirámide (TECHO)
    4, 5, 8,  // Frente (36-38)
    5, 6, 8,  // Derecha (39-41)
    6, 7, 8,  // Atrás (42-44)
    7, 4, 8,  // Izquierda (45-47)

    // Puerta (lado frontal)
    9, 10, 11,  11, 12, 9,  // Rectángulo de la puerta (48-53)

    // Ventana 1 (Arriba Izquierda - lado frontal)
    13, 14, 15,  15, 16, 13,  // Rectángulo de la ventana 1 (54-59)

    // Ventana 2 (Arriba Derecha - lado frontal)
    17, 18, 19,  19, 20, 17,  // Rectángulo de la ventana 2 (60-65)

    // PINO 1 (IZQUIERDA)
    // Tronco (cubo pequeño)
    21, 22, 23,  23, 24, 21,  // Base inferior (66-71)
    25, 26, 27,  27, 28, 25,  // Base superior (72-77)
    21, 22, 26,  26, 25, 21,  // Lado frontal (78-83)
    22, 23, 27,  27, 26, 22,  // Lado derecho (84-89)
    23, 24, 28,  28, 27, 23,  // Lado trasero (90-95)
    24, 21, 25,  25, 28, 24,  // Lado izquierdo (96-101)

    // Copa del pino 1 (pirámide)
    25, 26, 29,  // Frente (102-104)
    26, 27, 29,  // Derecha (105-107)
    27, 28, 29,  // Atrás (108-110)
    28, 25, 29,  // Izquierda (111-113)

    // PINO 2 (DERECHA)
    // Tronco (cubo pequeño)
    30, 31, 32,  32, 33, 30,  // Base inferior (114-119)
    34, 35, 36,  36, 37, 34,  // Base superior (120-125)
    30, 31, 35,  35, 34, 30,  // Lado frontal (126-131)
    31, 32, 36,  36, 35, 31,  // Lado derecho (132-137)
    32, 33, 37,  37, 36, 32,  // Lado trasero (138-143)
    33, 30, 34,  34, 37, 33,  // Lado izquierdo (144-149)

    // Copa del pino 2 (pirámide)
    34, 35, 38,  // Frente (150-152)
    35, 36, 38,  // Derecha (153-155)
    36, 37, 38,  // Atrás (156-158)
    37, 34, 38,  // Izquierda (159-161)
};

// Procesamiento de entrada simplificado (solo tecla Escape)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Fallo al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar la ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "Casa 3D con Puerta, Ventanas y Pinos", NULL, NULL);
    if (!window) {
        std::cerr << "Fallo al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST); // Habilitar el buffer de profundidad
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Color de fondo

    // Crear shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Verificar compilación del shader de vértices
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Verificar compilación del shader de fragmentos
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verificar enlazado del programa
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

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

    // Matrices de Transformación - Vista estática
    glm::mat4 view = glm::mat4(1.0f);
    // Mover la cámara hacia atrás para ver la escena
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    // Matriz de proyección para la perspectiva 3D
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window)) {
        // Procesar entrada (solo Escape)
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Crear matriz de modelo sin rotación
        glm::mat4 model = glm::mat4(1.0f);
        // Ajustar la vista para ver mejor la casa
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        // Inclinar ligeramente para ver la estructura 3D
        model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Enviar matrices a los shaders
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");
        int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        // Dibujar el cubo (Casa) (Rojo)
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // Rojo
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36 índices del cubo

        // Dibujar la pirámide (Techo) (Azul)
        glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f); // Azul
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int))); // 12 índices de la pirámide

        // Dibujar la puerta (Verde)
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f); // Verde
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(48 * sizeof(unsigned int))); // 6 índices de la puerta

        // Dibujar la ventana 1 (Verde)
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f); // Verde
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(54 * sizeof(unsigned int))); // 6 índices de la ventana 1

        // Dibujar la ventana 2 (Verde)
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f); // Verde
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(60 * sizeof(unsigned int))); // 6 índices de la ventana 2

        // Dibujar el tronco del pino 1 (Marrón)
        glUniform3f(colorLoc, 0.55f, 0.27f, 0.07f); // Marrón
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(66 * sizeof(unsigned int))); // 36 índices del tronco del pino 1

        // Dibujar la copa del pino 1 (Verde oscuro)
        glUniform3f(colorLoc, 0.0f, 0.5f, 0.0f); // Verde oscuro
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(102 * sizeof(unsigned int))); // 12 índices de la copa del pino 1

        // Dibujar el tronco del pino 2 (Marrón)
        glUniform3f(colorLoc, 0.55f, 0.27f, 0.07f); // Marrón
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(114 * sizeof(unsigned int))); // 36 índices del tronco del pino 2

        // Dibujar la copa del pino 2 (Verde oscuro)
        glUniform3f(colorLoc, 0.0f, 0.5f, 0.0f); // Verde oscuro
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(150 * sizeof(unsigned int))); // 12 índices de la copa del pino 2

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpieza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}