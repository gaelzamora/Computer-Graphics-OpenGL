#include <glew.h>
#include <glfw3.h>
#include <iostream>

// Vértices de los dos triángulos en coordenadas normalizadas (-1 a 1)
GLfloat vertices[] = {
    // Primer triángulo
    -0.2f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
     0.1f, -0.2f, 0.0f,  // Vértice 2 (derecha abajo)
     0.1f,  0.0f,  0.0f, // Vértice 3 (arriba centro)

     // Segundo triángulo (arreglado)
     -0.2f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
     -0.2f, 0.0f, 0.0f,  // Vértice 2 (derecha abajo)
     0.1f,  0.0f,  0.0f,  // Vértice 3 (arriba centro)

     -0.2f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
     -0.1f, 0.5f, 0.0f,  // Vértice 2 (derecha abajo)
     -0.2f,  0.5f,  0.0f,  // Vértice 3 (arriba centro)

     -0.2f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
     -0.1f, 0.5f, 0.0f,  // Vértice 2 (derecha abajo)
     -0.1f,  -0.2f,  0.0f,  // Vértice 3 (arriba centro)

     -0.1f, 0.5f, 0.0f,  // Vértice 1 (izquierda abajo)
     0.1f, 0.5f, 0.0f,  // Vértice 2 (derecha abajo)
     0.1f,  0.4f,  0.0f,  // Vértice 3 (arriba centro)

     -0.1f, 0.5f, 0.0f,  // Vértice 1 (izquierda abajo)
     -0.1f, 0.4f, 0.0f,  // Vértice 2 (derecha abajo)
     0.1f,  0.4f,  0.0f,  // Vértice 3 (arriba centro)

     0.1f, 0.0f, 0.0f,  // Vértice 1 (izquierda abajo)
     0.1f, 0.1f, 0.0f,  // Vértice 2 (derecha abajo)
     0.0f,  0.0f,  0.0f,  // Vértice 3 (arriba centro)

     0.0f, 0.1f, 0.0f,  // Vértice 1 (izquierda abajo)
     0.1f, 0.1f, 0.0f,  // Vértice 2 (derecha abajo)
     0.0f,  0.0f,  0.0f,  // Vértice 3 (arriba centro)

     0.0f, 0.05f, 0.0f,  // Vértice 1 (izquierda abajo)
     0.0f, 0.1f, 0.0f,  // Vértice 2 (derecha abajo)
     -0.05f,  0.05f,  0.0f,  // Vértice 3 (arriba centro)

     -0.05f, 0.05f, 0.0f,  // Vértice 1 (izquierda abajo)
     -0.05f, 0.1f, 0.0f,  // Vértice 2 (derecha abajo)
     0.0f,  0.1f,  0.0f,  // Vértice 3 (arriba centro)

};

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

int main() {
    // Inicialización de GLFW
    if (!glfwInit()) return -1;

    // Crear una ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triángulo OpenGL", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    // Crear VAO y VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Definir atributos del vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Compilar shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 30);  // Dibujar 2 triángulos (6 vértices)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
