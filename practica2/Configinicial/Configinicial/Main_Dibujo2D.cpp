#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D Guillermo Hernández", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		-0.88f, 0.51f,  0.0f,    0.0f, 0.0f, 1.0f, // C
		-0.18f, -0.07f, 0.0f,    0.0f, 1.0f, 0.0f, // D
		0.15f,  0.32f,  0.0f,    0.0f, 1.0f, 1.0f, // E
		0.05f,  -0.5f,  0.0f,    1.0f, 0.0f, 0.0f, // F
		-0.1f,  -0.6f,  0.0f,    1.0f, 0.0f, 1.0f, // G
		0.3f,   -0.73f, 0.0f,    1.0f, 1.0f, 0.0f, // H
		0.26f,  -0.36f, 0.0f,    1.0f, 1.0f, 1.0f, // I
		0.26f,  0.31f,  0.0f,    0.0f, 0.0f, 1.0f, // J
		0.27f,  0.41f,  0.0f,    0.0f, 1.0f, 0.0f, // K
		0.6f,   0.42f,  0.0f,    0.0f, 1.0f, 1.0f, // L
		0.66f,  0.23f,  0.0f,    1.0f, 0.0f, 0.0f, // M
		0.7f,   0.06f,  0.0f,    1.0f, 0.0f, 1.0f, // N
		0.62f,  0.18f,  0.0f,    1.0f, 1.0f, 0.0f, // O
		0.6f,   0.15f,  0.0f,    1.0f, 1.0f, 1.0f, // P
		0.73f,  0.09f,  0.0f,    0.0f, 0.0f, 1.0f, // Q
		0.69f,  -0.09f, 0.0f,    0.0f, 1.0f, 0.0f, // R
		0.81f,  0.43f,  0.0f,    0.0f, 1.0f, 1.0f, // S
		0.84f,  0.58f,  0.0f,    1.0f, 0.0f, 0.0f, // T
		0.86f,  0.47f,  0.0f,    1.0f, 0.0f, 1.0f, // U
		0.51f,  0.71f,  0.0f,    1.0f, 1.0f, 0.0f, // V
		0.08f,  -0.26f, 0.0f,	 1.0f, 1.0f, 1.0f, // W
		0.63f,  0.0f,   0.0f,	 0.0f, 0.0f, 1.0f, // Z
		0.82f,  0.47f,  0.0f,    0.0f, 1.0f, 0.0f, // A1
		0.36f,  -0.27f, 0.0f,	 1.0f, 0.0f, 0.0f, // B1
		0.4f,   -0.23f, 0.0f,	 1.0f, 0.0f, 1.0f, // C1
		0.67f,  -0.38f, 0.0f,	 1.0f, 1.0f, 0.0f, // D1
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2, // cola
		2, 1, 4, 3, // pierna
		3, 4, 5, // pie adelante
		2, 20, 6, 7, // espalda
		8, 6, 10, 9, // pecho
		6, 11, 13, // brazo adelante
		11, 21, 15, // mano adelante
		12, 13, 11, 15, 14, // brazo/mano atras
		24, 23, 25, // pie atras
		8, 16, 17, 19, // cabeza
		22, 18, 17, // boca
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int))); // cola
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int))); // pierna
		glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, (void*)(7 * sizeof(unsigned int))); // pie adelante
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void*)(10 * sizeof(unsigned int))); // espalda
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void*)(14 * sizeof(unsigned int))); // pecho
		glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int))); // brazo adelante
		glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, (void*)(21 * sizeof(unsigned int))); // mano adelante
		glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_INT, (void*)(24 * sizeof(unsigned int))); // brao/mano atras
		glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, (void*)(29 * sizeof(unsigned int))); // pie atras
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void*)(32 * sizeof(unsigned int))); // cabeza
		glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int))); // boca

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}
