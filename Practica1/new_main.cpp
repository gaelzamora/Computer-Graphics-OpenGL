#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h>
#include <time.h>

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO_G, VBO_G, VAO_J, VBO_J, VAO_H, VBO_H, shader;

// Vertex Shader
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); 	\n\
}";

// Fragment Shader
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
uniform vec4 ourColor;								\n\
void main()											\n\
{													\n\
    color = vec4(1.0f,0.0f,0.0f,1.0f);	 							\n\
}";

void CrearJ()
{
    GLfloat vertices[] = {
        // Parte vertical de la J
        -0.6f,  -0.2f,  0.0f,  // Arriba izquierda
        -0.6f,  0.5f,  0.0f,  // Arriba derecha
        -0.7f, 0.5f,  0.0f,  // Abajo derecha
        
        -0.7f,  -0.2f,  0.0f,  // Arriba izquierda
        -0.7f,  0.5f,  0.0f,  // Arriba derecha
        -0.6f, -0.2f,  0.0f,  // Abajo derecha

        -0.8f,  -0.2f,  0.0f,  // Arriba izquierda
        -0.6f,  -0.2f,  0.0f,  // Arriba derecha
        -0.6f, 0.0f,  0.0f,  // Abajo derecha

        -0.8f,  -0.2f,  0.0f,  // Arriba izquierda
        -0.8f,  0.0f,  0.0f,  // Arriba derecha
        -0.6f, 0.0f,  0.0f,  // Abajo derecha

        -0.8f,  0.05f,  0.0f,  // Arriba izquierda
        -0.8f,  0.0f,  0.0f,  // Arriba derecha
        -0.75f, 0.05f,  0.0f,  // Abajo derecha
        
        -0.75f,  0.05f,  0.0f,  // Arriba izquierda
        -0.75f,  0.0f,  0.0f,  // Arriba derecha
        -0.8f, 0.0f,  0.0f,  // Abajo derecha

        -0.8f,  0.5f,  0.0f,  // Arriba izquierda
        -0.7f,  0.4f,  0.0f,  // Arriba derecha
        -0.7f, 0.5f,  0.0f,  // Abajo derecha
    
        -0.5f,  0.5f,  0.0f,  // Arriba izquierda
        -0.6f,  0.5f,  0.0f,  // Arriba derecha
        -0.6f, 0.4f,  0.0f,  // Abajo derecha
    };

    glGenVertexArrays(1, &VAO_J);
    glBindVertexArray(VAO_J);

    glGenBuffers(1, &VBO_J);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_J);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void CrearG()
{
    GLfloat vertices[] = {
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

    glGenVertexArrays(1, &VAO_G);
    glBindVertexArray(VAO_G);

    glGenBuffers(1, &VBO_G);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_G);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CrearH()
{
    GLfloat vertices[] = {
         0.5f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
         0.55f, 0.5f, 0.0f,  // Vértice 2 (derecha abajo)
         0.55f, -0.2f,  0.0f, // Vértice 3 (arriba centro)

         0.5f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
         0.55f, 0.5f, 0.0f,  // Vértice 2 (derecha abajo)
         0.5f, 0.5f,  0.0f, // Vértice 3 (arriba centro)

         0.55f, 0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
         0.55f, 0.15f, 0.0f,  // Vértice 2 (derecha abajo)
         0.7f, 0.2f,  0.0f, // Vértice 3 (arriba centro)

         0.7f, 0.15f, 0.0f,  // Vértice 1 (izquierda abajo)
         0.55f, 0.15f, 0.0f,  // Vértice 2 (derecha abajo)
         0.7f, 0.2f,  0.0f, // Vértice 3 (arriba centro)

         0.7f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
         0.75f, 0.5f, 0.0f,  // Vértice 2 (derecha abajo)
         0.75f, -0.2f,  0.0f, // Vértice 3 (arriba centro)

         0.7f, -0.2f, 0.0f,  // Vértice 1 (izquierda abajo)
         0.7f, 0.5f, 0.0f,  // Vértice 2 (derecha abajo)
         0.75f, 0.5f,  0.0f, // Vértice 3 (arriba centro)

    };

    glGenVertexArrays(1, &VAO_H);
    glBindVertexArray(VAO_H);

    glGenBuffers(1, &VBO_H);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_H);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("El error al compilar el shader %d es: %s \n", shaderType, eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    if (!shader)
    {
        printf("Error creando el shader");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("El error al linkear es: %s \n", eLog);
        return;
    }
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("El error al validar es: %s \n", eLog);
        return;
    }
}

void generarColorAleatorio(float* r, float* g, float* b) {
    *r = (float)rand() / (float)RAND_MAX;
    *g = (float)rand() / (float)RAND_MAX;
    *b = (float)rand() / (float)RAND_MAX;
}

int main()
{
    if (!glfwInit())
    {
        printf("Falló inicializar GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1", NULL, NULL);

    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }

    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("Falló inicialización de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, BufferWidth, BufferHeight);

    CrearJ();
    CrearG();
    CrearH();
    CompileShaders();

    double tiempoUltimoCambio = 0.0;
    float r = 0.0f, g = 0.0f, b = 0.0f;

    srand((unsigned int)time(NULL));

    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        double tiempoActual = glfwGetTime();

        if (tiempoActual - tiempoUltimoCambio >= 2.0) {
            generarColorAleatorio(&r, &g, &b);
            tiempoUltimoCambio = tiempoActual;
        }

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO_G);
        glDrawArrays(GL_TRIANGLES, 0, 30);

        glBindVertexArray(VAO_J);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        glBindVertexArray(VAO_H);
        glDrawArrays(GL_TRIANGLES, 0, 18);
            
        glfwSwapBuffers(mainWindow);
    }

    // Limpieza de recursos
    glDeleteVertexArrays(1, &VAO_G);
    glDeleteBuffers(1, &VBO_G);
    glDeleteVertexArrays(1, &VAO_J);
    glDeleteBuffers(1, &VBO_J);
    glDeleteProgram(shader);

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}