//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/random.hpp>
//clases para dar orden y limpieza al código
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";

//------------------------------------------------------------------------------
vector<Mesh*> CrearTetraedrosPorCara(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C,
    const glm::vec3& O, float gapFactor, float depth) {
    vector<Mesh*> tetraedros;
    int n = 3; // Dividir cada lado en 3 segmentos
    auto P = [&](int i, int j) -> glm::vec3 {
        float s = float(i) / float(n);
        float t = float(j) / float(n);
        return A * (1.0f - s - t) + B * s + C * t;
        };

    // La siguiente doble iteración crea dos tetraedros en algunas celdas, lo que produce 9 tetraedros por cara.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i; j++) {
            // Primer tetraedro basado en el triángulo (p0, p1, p2)
            glm::vec3 p0 = P(i, j);
            glm::vec3 p1 = P(i + 1, j);
            glm::vec3 p2 = P(i, j + 1);
            glm::vec3 centro = (p0 + p1 + p2) / 3.0f;
            p0 = centro + gapFactor * (p0 - centro);
            p1 = centro + gapFactor * (p1 - centro);
            p2 = centro + gapFactor * (p2 - centro);
            glm::vec3 dir = glm::normalize(O - centro);
            glm::vec3 apex = centro + depth * dir;

            vector<GLfloat> verts = {
                p0.x, p0.y, p0.z,
                p1.x, p1.y, p1.z,
                p2.x, p2.y, p2.z,
                apex.x, apex.y, apex.z
            };
            vector<unsigned int> inds = {
                0, 1, 2,   // base
                0, 1, 3,   // lado 1
                1, 2, 3,   // lado 2
                2, 0, 3    // lado 3
            };
            Mesh* m = new Mesh();
            m->CreateMeshGeometry(verts, inds, verts.size(), inds.size());
            tetraedros.push_back(m);

            // Segundo tetraedro en la celda (para completar la subdivisión y obtener 9 piezas por cara)
            if (i + j < n - 1) {
                glm::vec3 q0 = P(i + 1, j);
                glm::vec3 q1 = P(i + 1, j + 1);
                glm::vec3 q2 = P(i, j + 1);
                glm::vec3 centro2 = (q0 + q1 + q2) / 3.0f;
                q0 = centro2 + gapFactor * (q0 - centro2);
                q1 = centro2 + gapFactor * (q1 - centro2);
                q2 = centro2 + gapFactor * (q2 - centro2);
                glm::vec3 dir2 = glm::normalize(O - centro2);
                glm::vec3 apex2 = centro2 + depth * dir2;

                vector<GLfloat> verts2 = {
                    q0.x, q0.y, q0.z,
                    q1.x, q1.y, q1.z,
                    q2.x, q2.y, q2.z,
                    apex2.x, apex2.y, apex2.z
                };
                vector<unsigned int> inds2 = {
                    0, 1, 2,
                    0, 1, 3,
                    1, 2, 3,
                    2, 0, 3
                };
                Mesh* m2 = new Mesh();
                m2->CreateMeshGeometry(verts2, inds2, verts2.size(), inds2.size());
                tetraedros.push_back(m2);
            }
        }
    }
    return tetraedros;
}

//------------------------------------------------------------------------------
// Función que crea la pirámide subdividida (Rubik) en piezas tetraédricas.
// Cada una de las 4 caras se subdivide en 9 piezas, dando un total de 36 tetraedros.
void CrearPiramideRubik9(float gapFactor, float depth) {
    glm::vec3 A(-0.7f, -0.7f, 0.0f);
    glm::vec3 B(0.7f, -0.7f, 0.0f);
    glm::vec3 C(0.0f, -0.7f, sqrt(1.47f));
    glm::vec3 D(0.0f, sqrt(1.47f) - 0.7f, sqrt(1.47f) / 3.0f);

    vector<Mesh*> cara1 = CrearTetraedrosPorCara(A, B, C, D, gapFactor, depth); // Cara ABC, O = D
    vector<Mesh*> cara2 = CrearTetraedrosPorCara(A, B, D, C, gapFactor, depth); // Cara ABD, O = C
    vector<Mesh*> cara3 = CrearTetraedrosPorCara(B, C, D, A, gapFactor, depth); // Cara BCD, O = A
    vector<Mesh*> cara4 = CrearTetraedrosPorCara(C, A, D, B, gapFactor, depth); // Cara CAD, O = B

    for (Mesh* m : cara1) meshList.push_back(m);
    for (Mesh* m : cara2) meshList.push_back(m);
    for (Mesh* m : cara3) meshList.push_back(m);
    for (Mesh* m : cara4) meshList.push_back(m);
}

// Nota: Se ha eliminado la función CrearPiramide() y toda referencia a la malla “de borde”,
// de forma que solo se generan las subdivisiones de las caras de la pirámide.

void CreateShaders() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    Shader* shader2 = new Shader();
    shader2->CreateFromFiles(vShaderColor, fShader);
    shaderList.push_back(*shader2);
}

int main() {
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    // Sólo se crean las piezas subdivididas (36 tetraedros en total)
    CrearPiramideRubik9(0.99f, 0.2f);
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -60.0f, 0.0f, 0.3f, 0.3f);

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
        mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f, 100.0f);

    while (!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;
        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        // Renderizamos las 36 piezas (9 tetraedros por cada una de las 4 caras)
        for (unsigned int i = 0; i < meshList.size(); i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
            model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

            // Primer pase: renderizado de la pieza en modo relleno
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 faceColor;
            switch ((i / 9) % 4) {
            case 0: faceColor = glm::vec3(1.0f, 0.0f, 0.0f); break; // rojo
            case 1: faceColor = glm::vec3(0.0f, 1.0f, 0.0f); break; // verde
            case 2: faceColor = glm::vec3(0.0f, 0.0f, 1.0f); break; // azul
            default: faceColor = glm::vec3(1.0f, 1.0f, 0.0f); break; // amarillo
            }
            glUniform3fv(uniformColor, 1, glm::value_ptr(faceColor));
            meshList[i]->RenderMeshGeometry();

            // Segundo pase: renderizado de la misma malla en modo línea para el borde
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(2.0f); // Puedes ajustar el grosor del borde
            glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
            meshList[i]->RenderMeshGeometry();

            // Restaurar el modo de polígonos a relleno
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
}
