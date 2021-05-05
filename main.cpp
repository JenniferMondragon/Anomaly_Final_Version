//
//  main.cpp
//  


#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <glew.h>


// GLFW
#include <glfw3.h>


// GLM Mathematics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//react physics
#include <reactphysics3d/reactphysics3d.h>

// Other includes
#include "Shader.h"
#include "Camera.h"

//react physics namespace
using namespace reactphysics3d;


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void drawStuff();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(-2.0f, 2.0f, 3.0f);

// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

//**************** P H Y S I C S   S T U F F********************//
//PhysicsCommon physicsCommon;
// Create a physics world 
//PhysicsWorld* world = physicsCommon.createPhysicsWorld();

//***********end***** P H Y S I C S   S T U F F********************//


// The MAIN function, from here we start the application and run the game loop
int main()
{
    /******** I N I T I A L I Z E  ***********/

 // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Anomaly", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    /***end***** I N I T I A L I Z E ***********/





    /********* G A M E   L O O O O O O O O P ******/

    while (!glfwWindowShouldClose(window))
    {

        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /******* P L A Y E R  I N P U T ********/
        glfwPollEvents();
        DoMovement();
        

        // Clear the colorbuffer
        glClearColor(0.8f, 0.4f, 0.6f, 0.2f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


       //******** R E N D E R   S C E N E *******/
        drawStuff();


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (keys[GLFW_KEY_C])
    {
        Shader lightingShader("lightingShader.vert", "lightingShader.frag");
        lightingShader.Use();
        GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 0.0f);

    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


void drawStuff()
{
    // Load Shader programs here!!!!
    Shader lightingShader("lightingShader.vert", "lightingShader.frag");
    Shader lampShader("lampShader.vert", "lampShader.frag");
    Shader shader("core.vert", "core.frag");




    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,       //back
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,       //front
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,       //left
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,        //right
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,       //bottom
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,       //top
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };


    GLfloat octahedron[] =
    {
        //top front
        -1.5f, 0.5f, 0.5f,  0.0f, 0.5f, 0.5f,
        -2.5f, 0.5f, 0.5f,  0.0f, 0.5f, 0.5f,
        -2.0f, 1.0f, 0.0f,  0.0f, 0.5f, 0.5f,

        //top back
        -2.0f, 1.0f, 0.0f,  -0.5f, 0.5f, -0.5f,
        -2.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
        -1.5f, 0.5f, -0.5f,  -0.5f, 0.5f, -0.5f,

        //top left
        -2.0f, 1.0f, 0.0f,  -0.5f, -0.5f, 0.0f,
        -2.5f, 0.5f, 0.5f,  -0.5f, -0.5f, 0.0f,
        -2.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.0f,

        //top right
        -2.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f,
        -1.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.0f,
        -1.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.0f,

        //bottom front
        -1.5f, 0.5f, 0.5f,  0.0f, 0.5f, 1.0f,
        -2.5f, 0.5f, 0.5f,  0.0f, 0.5f, 1.0f,
        -2.0f, -0.5f, 0.0f, 0.0f, 0.5f, 1.0f,

        //bottom back
        -2.0f, -0.5f, 0.0f,  0.0f, 0.5f, -1.0f,
        -2.5f, 0.5f, -0.5f, 0.0f, 0.5f, -1.0f,
        -1.5f, 0.5f, -0.5f,  0.0f, 0.5f, -1.0f,

        //bottom left
        -2.0f, -0.5f, 0.0f,  -1.0f, 0.5f, 0.0f,
        -2.5f, 0.5f, 0.5f,  -1.0f, 0.5f, 0.0f,
        -2.5f, 0.5f, -0.5f, -1.0f, 0.5f, 0.0f,

        //bottom right
        -2.0f, -0.5f, 0.0f, 1.0f, -0.5f, 0.0f,
        -1.5f, 0.5f, 0.5f,  1.0f, -0.5f, 0.0f,
        -1.5f, 0.5f, -0.5f, 1.0f, -0.5f, 0.0f,


    };

    GLfloat tetras[] =
    {
        // base
        -3.5f, 0.0f, 0.5f,  0.0f, 0.0f, 0.0f,
        -4.0f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
        -4.5f, 0.0f, 0.5f,  0.0f, 0.0f, 0.0f,

        //front face
        -3.5f, 0.0f, 0.5f,  0.0f, -0.5f, 1.0f,
         -4.0f, 1.0f, 0.0f, 0.0f, -0.5f, 1.0f,
        -4.5f, 0.0f, 0.5f,  0.0f, -0.5f, 1.0f,

        //right face
        -3.5f, 0.0f, 0.5f,   0.25f, 0.25f, 0.0f,
        -4.0f, 0.0f, -0.5f,  0.25f, 0.25f, 0.0f,
        -4.0f, 1.0f, 0.0f,   0.25f, 0.25f, 0.0f,

        //left face
        -4.5f, 0.0f, 0.5f,  -1.0f, 0.25f, -0.5f,
        -4.0f, 0.0f, -0.5f, -1.0f, 0.25f, -0.5f,
        -4.0f, 1.0f, 0.0f,  -1.0f, 0.25f, -0.5f,

        // 2nd tetrahedron

        //base
        -4.0f, 0.5f, 0.75f,   0.0f, 1.0f, 0.0f,
        -3.5f, 0.5f, -0.25f,  0.0f, 1.0f, 0.0f,
        -4.5f, 0.5f, -0.25f,  0.0f, 1.0f, 0.0f,

        //right
        -4.0f, 0.5f, 0.75f,     1.0f, -0.25f, 0.5f,
        -3.5f, 0.5f, -0.25f,    1.0f, -0.25f, 0.5f,
         -4.0f, -0.5f, 0.25f,    1.0f, -0.25f, 0.5f,

         //left
         -4.5f, 0.5f, -0.25f,  -1.0f, 0.0f, 0.0f,
         -4.0f, 0.5f, 0.75f,   -1.0f, 0.0f, 0.0f,
         -4.0f, -0.5f, 0.25f,  -1.0f, 0.0f, 0.0f,

         //back
         -4.0f, -0.5f, 0.25f, 0.0f, -0.5f, -1.0f,
         -3.5f, 0.5f, -0.25f,  0.0f, -0.5f, -1.0f,
         -4.5f, 0.5f, -0.25f, 0.0f, -0.5f, -1.0f,

    };

    GLfloat floor[]
    {
        // position             //normals
        -10.0f, -2.0f, -5.0f,   0.0f, 1.0f, 0.0f,
        -10.0f, -2.0f, 5.0f,     0.0f, 1.0f, 0.0f,
        5.0f, -2.0f, 5.0f,      0.0f, 1.0f, 0.0f,

        5.0f, -2.0f, -5.0f,     0.0f, 1.0f, 0.0f,
        5.0f, -2.0f, 5.0f,      0.0f, 1.0f, 0.0f,
        -10.0f, -2.0f, -5.0f,    0.0f, 1.0f, 0.0f


    };



    // First, set the container's VAO (and VBO)
    GLuint VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(containerVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    /**************** octahedron ********************/
    // First, set the container's VAO (and VBO)
    GLuint VBO1, octaVAO;
    glGenVertexArrays(1, &octaVAO);
    glGenBuffers(1, &VBO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(octahedron), octahedron, GL_STATIC_DRAW);

    glBindVertexArray(octaVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    /**************** double tetrahedron ********************/
    // First, set the container's VAO (and VBO)
    GLuint VBO2, tetraVAO;
    glGenVertexArrays(1, &tetraVAO);
    glGenBuffers(1, &VBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetras), tetras, GL_STATIC_DRAW);

    glBindVertexArray(tetraVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    GLuint VBO3, VAOfloor;
    glGenVertexArrays(1, &VAOfloor);
    glGenBuffers(1, &VBO3);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAOfloor);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO








    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);


   


    // Use cooresponding shader when setting uniforms/drawing objects
    lightingShader.Use();
    GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
    GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

    //glUniform3f(lightPosLoc, (lightPos.x* sin(glfwGetTime())), lightPos.y, (lightPos.z* glfwGetTime()));
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


   
    // Create camera transformations
    glm::mat4 view;
    view = camera.GetViewMatrix();

    // Get the uniform locations
    GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
    GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



    // Draw the container (using container's vertex attributes)
    glBindVertexArray(containerVAO);
    glm::mat4 model;
    glUniform3f(objectColorLoc, 0.8f, 0.91f, 0.11f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);



    /************ octahedron **********/
    // Draw the container (using container's vertex attributes)
    glBindVertexArray(octaVAO);
    lightingShader.Use();
    glUniform3f(objectColorLoc, 0.8f, 0.1f, 0.31f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 24);
    glBindVertexArray(0);

    /************ double tetrahedrons **********/
    // Draw the container (using container's vertex attributes)
    glBindVertexArray(tetraVAO);
    lightingShader.Use();
    glUniform3f(objectColorLoc, 0.3f, 0.8f, 0.7f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 24);
    glBindVertexArray(0);

    /************ floor plane **********/
   // Draw the container (using container's vertex attributes)
    glBindVertexArray(VAOfloor);
    lightingShader.Use();
    glUniform3f(objectColorLoc, 0.2f, 0.8f, 0.4f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


    /************ M A I N  L A M P ************************/

    // Also draw the lamp object, again binding the appropriate shader
    lampShader.Use();
    // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
    modelLoc = glGetUniformLocation(lampShader.Program, "model");
    viewLoc = glGetUniformLocation(lampShader.Program, "view");
    projLoc = glGetUniformLocation(lampShader.Program, "projection");

    // Set matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    model = glm::mat4(0.5f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // Draw the light object (using light's vertex attributes)
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    /************ M A I N  L A M P ************************/

   


      

  

}

