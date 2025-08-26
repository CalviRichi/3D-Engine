#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> // C with vectors (and classes)
#include <chrono>
#include <cmath>
#include "glad.h"       // graphics api
#include <GLFW/glfw3.h> // graphics api
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#define MOVE_SPEED 5
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

#define COORDINATE 3
#define TEXTURE 2

/*
Right now I just want to be able to show one block on the screen and move the camera around it

Weak perspective projection

each vertex will be put into the projection matrix
vec3(x,y,z) X projMat = projected coordinates

I could be making my functions static, but nothing is 
using main right now, so I don't see a point


Create a camera class
*/

using namespace std; // get rid of this at some point, 
// I just like saying cout and string

enum Keys {
    ESCAPE = 1 << 0,
    W_DOWN = 1 << 1,
    A_DOWN = 1 << 2,
    S_DOWN = 1 << 3,
    D_DOWN = 1 << 4,
    UP_DOWN = 1 << 5,
    LEFT_DOWN = 1 << 6,
    RIGHT_DOWN = 1 << 7,
    DOWN_DOWN = 1 << 8,
    FORWARD_DOWN = 1 << 9,
    BACKWARD_DOWN = 1 << 10
};
// glGetError(int errorCode); can be used for older openGL versions
// gldebugmessagecallback is a new, better error function

static bool firstMouse = true;
static double mousePos[2] = {0.0,0.0};
static double yaw = -89.99;
static double pitch = 0.0;
static glm::vec3 direction(0.0,0.0,-1.0);

static const glm::vec3 up(0.0,1.0,0.0);
static const glm::vec3 speed(2.0);
static glm::vec3 position(0.0,0.0,0.0);

static float angleX = glm::radians(0.0f);
static float angleY = glm::radians(0.0f);

glm::vec3 x_axis(0.0f,1.0,0.0);
glm::vec3 y_axis(1.0f, 0.0f, 0.0f);

void keyboard(GLFWwindow * window) {

    double x, y;
    
    glfwGetCursorPos(window, &x, &y);

    if (firstMouse) {
        mousePos[0] = x;
        mousePos[1] = y;
        firstMouse = false;
    }

    float xoff = x - mousePos[0];
    float yoff = mousePos[1] - y;

    mousePos[0] = x;
    mousePos[1] = y;

    xoff *= 0.1; // sensitivity
    yoff *= 0.1; 

    yaw += xoff;
    pitch += yoff;

    if (pitch > 89.99) {
        pitch = 89.99;
    }
    if (pitch < -89.99) {
        pitch = -89.99;
    }

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction); 
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position.x += direction.x * speed.x;
        position.z += direction.z * speed.z;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= glm::normalize(glm::cross(direction, up)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position.x -= direction.x * speed.x;
        position.z -= direction.z * speed.z;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += glm::normalize(glm::cross(direction, up)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position.y += speed.y;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position.y -= speed.y;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        angleY -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        angleY += 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT)== GLFW_PRESS) {
        angleX -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        angleX += 0.05;
    }

}

void frameBufferSize(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void fruityRotate() {

}

// void handleMouseInput();

int main(int, char **)
{
    //cout << "Hello World" << endl;
    
    GLFWwindow *window;
    if (!glfwInit())
    {
        cout << "GLFW init failed" << endl;
        glfwTerminate();
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // equivalent to saying 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "Fruity Engine", NULL, NULL);
    if (!window)
    {
        cout << "window init failed" << endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Could not load window" << endl;
        glfwTerminate();
    }
    
    //glfwSetFramebufferSizeCallback(window, frameBufferSize);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    cout << "Version: " << glGetString(GL_VERSION) << endl;
    cout << "GLFW: "<< glfwGetVersionString() << endl;
    {
    float positions[] = {
        // 3D            Textures
        -5,-5, -5,    0.0f, 0.0f, // 0 Back
         5, -5, -5,    1.0f, 0.0f, // 1
         5,  5,-5,    1.0f, 1.0f, // 2
        -5, 5,-5,    0.0f, 1.0f, // 3
        -5,-5, 5,    0.0f, 0.0f, // 4 Front
         5, -5, 5,    1.0f, 0.0f, // 5
         5,  5,  5,    1.0f, 1.0f, // 6
        -5, 5,  5,    0.0f, 1.0f  // 7
    };
    
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // back face
        4, 5, 6, 6, 7, 4, // front face
        0, 1, 5, 5, 4, 0, // bottom face
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1
    };
    
    // BLENDING SETTINGS
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCALL(glBlendEquation(GL_FUNC_ADD));

    int col = COORDINATE + TEXTURE; // 5
    int row = (sizeof(positions) / sizeof(float)) / col; // 8

    VertexArray va;
    VertexBuffer vb(positions, row * col * sizeof(float));
    
    VertexBufferLayout layout;
    layout.Push<float>(3); // 3D
    layout.Push<float>(2); // Texture
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 36); 

    /*
    Both of these buffers are working together, you are not
    overwriting the buffer when you are specifying a different kind
    of buffer
    */
    // maybe place res inside of source

    //cout << "VERTEX: " << source.VertexSource << endl; // shader debug lines
    //cout << "FRAGMENT: " << source.FragmentSource << endl;
    Shader shader("../res/shaders/basic.glsl");
    shader.Bind();
    //shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    Texture texture("../res/textures/charli.jpeg");
    texture.Bind(); // implicitly using slot 0
    shader.setUniform1i("u_Texture", 0); // binding texture to slot 0

    // unbinding
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    // end unbinding

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init((char*)glGetString(330));
    
    //bool show_demo_window = true;
	//bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float aspect = 960.0 / 540.0;
    glm::mat4 p = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
    glm::vec3 t(0.0f,0.0f,-100.0f);

    while(!glfwWindowShouldClose(window)) {

        // ** UPDATE STAGE **
        // *****************************************
        glfwPollEvents();
        static float f = 0.0f;
        static int counter = 0;
        
        // camera updates
        keyboard(window); // process input
    
        glm::mat4 v = glm::lookAt(
        position, // where you are
        position + direction, // where you're looking
        up
        );

        //updateLocations(&t, &angleX, &angleY);
        glm::mat4 m = glm::translate(glm::mat4(1.0f), t); // model translation
        m = glm::rotate(m, angleX, x_axis); // model rotate on x 
        m = glm::rotate(m, angleY, y_axis); // model rotate on y

        //m = glm::translate(m, t);
        // add a scaling to keep the image smaller
        glm::mat4 mvp = p * v * m;
        // *****************************************
        // ** END UPDATE STAGE **


        // ** RENDER STAGE **
        // ***********************************************
        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        shader.Bind();
        //shader.setUniform4f("u_Color", r, 0.2f, 0.8f, 1.0f);
        shader.setUniformMat4f("u_MVP", mvp);
        renderer.draw(va, ib, shader);

        /*
        m = glm::translate(glm::mat4(1.0), t);
        mvp = p * v * m;
        shader.Bind();
        shader.setUniformMat4f("u_MVP", mvp);
        renderer.draw(va, ib, shader);
        
        
        
        for (int i = 1; i < 10; ++i) {
            for (int j = 1; j < 10; ++j) {
                t.x += 10;
                m = glm::translate(glm::mat4(1.0), t);
                mvp = p * v * m;
                shader.Bind();
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }
            t.x = -10;
            t.z += 10;
        }
        */
        //t.x = 0;
        //t.z = -100;
/*
        {
        ImGui::SliderFloat("translation x", &t.x, -126.0f, 126.0f);
        ImGui::SliderFloat("translation y", &t.y, -60.0f, 60.0f);
        ImGui::Text("Mouse X position: %.3f, Mouse Y position: %.3f", direction.x, direction.y);

        ImGui::Text("Camera X pos: %.3f, Camera Y pos: %.3f", position.x, position.y);
        ImGui::Text("Camera Z depth: %.3f", position.z);
        //ImGui::SliderFloat("angle", &angle, 0.0f, 2*M_PI);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
*/
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        
        // *********************************************
        // ** END RENDER STAGE ** 
    
    }
    }
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    // glfw terminate will get mad at you
    // delete ib and vb 
    glfwTerminate();

    return 0;
}
