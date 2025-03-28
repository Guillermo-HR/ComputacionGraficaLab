// Guillermo Hernández Ruiz de Esparza
// 319220553
// Practica 8
// 16/3/25


// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 35.0f);
float moveLightPosY, moveLightPosZ = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;
bool dia = true;


int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Carga de modelos y luz Guillermo Hernandez", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    
    // Load models
    Model dog((char*)"Models/perro/RedDog.obj");
	Model escudo((char*)"Models/escudo/escudo.obj");
    Model casa1((char*)"Models/casa1/casa1.obj");
    Model casa2((char*)"Models/casa2/casa2.obj");
    Model camino((char*)"Models/camino/camino.obj");
    Model estatua((char*)"Models/estatua/estatua.obj");
	Model sol((char*)"Models/sol/sol.obj");
	Model luna((char*)"Models/luna/luna.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
  
    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        if (dia) {
            glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        }
		else {
            glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		}
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Shader lighting
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y + moveLightPosY, lightPos.z + moveLightPosZ);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Dibujar modelo
		glm::mat4 model1(1);
        model1 = glm::translate(model1, glm::vec3(lightPos.x, lightPos.y + moveLightPosY, lightPos.z + moveLightPosZ));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));

        // Set lights properties
        if (dia) {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.4f, 0.3f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 2.0f, 1.8f, 1.6f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.8f, 1.8f, 1.6f);
			model1 = glm::scale(model1, glm::vec3(7.0f, 7.0f, 7.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));
			sol.Draw(shader);
        }
        else {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.15f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.8f, 1.4f, 2.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.6f, 1.2f, 1.8f);
            model1 = glm::scale(model1, glm::vec3(7.0f, 7.0f, 7.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));
            luna.Draw(shader);
        }

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.6f, 0.5f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.7f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.7f, 0.7f, 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
        
        // dibujar caja de luz
        
        //model1 = glm::scale(model1, glm::vec3(1.0f, 1.0f, 1.0f));
        //glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));
        //glBindVertexArray(VAO);

        //glDrawArrays(GL_TRIANGLES, 0, 36);


        //glBindVertexArray(0);
        

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        
        // perro
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

		// escudo
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::translate(model, glm::vec3(0.4f, -0.4f, 0.6f));
        model = glm::rotate(model, glm::degrees(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        escudo.Draw(shader);

        // casa1
        model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
        model = glm::translate(model, glm::vec3(-0.4f, 0.38f, -0.25f));
        model = glm::rotate(model, glm::degrees(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa1.Draw(shader);

        // casa2
        model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa2.Draw(shader);

        // camino
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
        model = glm::translate(model, glm::vec3(-0.3f,-0.4f, -0.35f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        camino.Draw(shader);

        // camino complemento
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        camino.Draw(shader);

		// estatua
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.3f));
		model = glm::translate(model, glm::vec3(-1.6f, 0.47f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estatua.Draw(shader);


        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O])
    {
		dia = true;
		if (moveLightPosZ > -70.0f)
		{
			moveLightPosZ -= 0.1f;
			moveLightPosY = 20 * sin((-3.1416/70)*moveLightPosZ);
		}
    }

    if (keys[GLFW_KEY_L])
    {
		dia = false;
		if (moveLightPosZ < 0.0f)
		{
			moveLightPosZ += 0.1f;
			moveLightPosY = 20 * sin((-3.1416 / 70) * moveLightPosZ);
		}
    }

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

