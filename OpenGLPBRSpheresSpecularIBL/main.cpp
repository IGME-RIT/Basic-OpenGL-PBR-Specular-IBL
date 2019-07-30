
#include <iostream>
#include <vector>

// We are using the glew32s.lib
// Thus we have a define statement
// If we do not want to use the static library, we can include glew32.lib in the project properties
// If we do use the non static glew32.lib, we need to include the glew32.dll in the solution folder
// The glew32.dll can be found here $(SolutionDir)\..\External Libraries\GLEW\bin\Release\Win32
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "Entity.h"
#include "FirstPersonDebug.h"


// Variables for the Height and width of the window
const GLint WIDTH = 800, HEIGHT = 600;

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

glm::vec2 mousePosition = glm::vec2();

std::vector<Entity*> gameEntities;
void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
	mousePosition = glm::vec2(mouseX, mouseY);
}

//Helper method for uploading a vector3 to the shader
void UpdateVec3(char * name, glm::vec3 data, ShaderProgram* program)
{
	GLint uniform = glGetUniformLocation(program->GetGLShaderProgram(), name);
	if (uniform == -1)
	{
		std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
		return;
	}


	glUniform3fv(uniform, 1, &data[0]);
}

//Helper method for uploading a float to the shader
void UpdateFloat(char * name, GLfloat data, ShaderProgram* program)
{
	GLint uniform = glGetUniformLocation(program->GetGLShaderProgram(), name);
	if (uniform == -1)
	{
		std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
		return;
	}


	glUniform1f(uniform, data);
}


//Source:https://github.com/JoeyDeVries/LearnOpenGL
//Helper method for rendering a cube
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

//Source:https://github.com/JoeyDeVries/LearnOpenGL
//Helper method for rendering a quad
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {

			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


int main()
{
#pragma region GL setup
	//Initializes the glfw
	glfwInit();

	// Setting the required options for GLFW

	// Setting the OpenGL version, in this case 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 99);
	// Setting the Profile for the OpenGL.

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// Setting the forward compatibility of the application to true
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// We don't want the window to resize as of now.
	// Therefore we will set the resizeable window hint to false.
	// To make is resizeable change the value to GL_TRUE.
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create the window object
	// The first and second parameters passed in are WIDTH and HEIGHT of the window we want to create
	// The third parameter is the title of the window we want to create
	// NOTE: Fourth paramter is called monitor of type GLFWmonitor, used for the fullscreen mode.
	//		 Fifth paramter is called share of type GLFWwindow, here we can use the context of another window to create this window
	// Since we won't be using any of these two features for the current tutorial we will pass nullptr in those fields
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shaders Tutorial", nullptr, nullptr);

	// We call the function glfwGetFramebufferSize to query the actual size of the window and store it in the variables.
	// This is useful for the high density screens and getting the window size when the window has resized.
	// Therefore we will be using these variables when creating the viewport for the window
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// Check if the window creation was successful by checking if the window object is a null pointer or not
	if (window == nullptr)
	{
		// If the window returns a null pointer, meaning the window creation was not successful
		// we print out the messsage and terminate the glfw using glfwTerminate()
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		// Since the application was not able to create a window we exit the program returning EXIT_FAILURE
		return EXIT_FAILURE;
	}

	// Creating a window does not make it the current context in the windows.
	// As a results if the window is not made the current context we wouldn't be able the perform the operations we want on it
	// So we make the created window to current context using the function glfwMakeContextCurrent() and passing in the Window object
	glfwMakeContextCurrent(window);

	// Enable GLEW, setting glewExperimental to true.
	// This allows GLEW take the modern approach to retrive function pointers and extensions
	glewExperimental = GL_TRUE;

	glfwSetCursorPosCallback(window, mouseMoveCallback);

	// Initialize GLEW to setup OpenGL function pointers
	if (GLEW_OK != glewInit())
	{
		// If the initalization is not successful, print out the message and exit the program with return value EXIT_FAILURE
		std::cout << "Failed to initialize GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	// Setting up the viewport
	// First the parameters are used to set the top left coordinates
	// The next two parameters specify the height and the width of the viewport.
	// We use the variables screenWidth and screenHeight, in which we stored the value of width and height of the window,
	glViewport(0, 0, screenWidth, screenHeight);
	#pragma endregion

#pragma region Asset Loading
	Mesh* sphereMesh = new Mesh("Assets/sphere.obj", true);

	FirstPersonDebug* fpsCam = new FirstPersonDebug(90, WIDTH, HEIGHT, 0.1f, 100.f);

	//Core PBR Shader
	Shader* defaultvShader = new Shader("shaders/defaultvShader.glsl", GL_VERTEX_SHADER);
	Shader* pbrfShader = new Shader("shaders/pbrfShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram* pbrprogram = new ShaderProgram();
	pbrprogram->AttachShader(defaultvShader);
	pbrprogram->AttachShader(pbrfShader);
	Material* pbrMat = new Material(pbrprogram);

	Shader* cubemapvShader = new Shader("shaders/cubemapvShader.glsl", GL_VERTEX_SHADER);
	
	//Converting equirectangular hdr files to cubemaps shader
	Shader* hdrconvertfShader = new Shader("shaders/conversionfShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram* hdrconvertprogram = new ShaderProgram();
	hdrconvertprogram->AttachShader(cubemapvShader);
	hdrconvertprogram->AttachShader(hdrconvertfShader);
	Material* hdrconvertMat = new Material(hdrconvertprogram);

	//Simple skybox shader
	Shader* skyboxvShader = new Shader("shaders/skyboxvShader.glsl", GL_VERTEX_SHADER);
	Shader* skyboxfShader = new Shader("shaders/skyboxfShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram* skyboxprogram = new ShaderProgram();
	skyboxprogram->AttachShader(skyboxvShader);
	skyboxprogram->AttachShader(skyboxfShader);
	Material* skyboxMat = new Material(skyboxprogram);

	//Prefilter shader
	Shader* prefilterfShader = new Shader("shaders/prefilterfShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram* prefilterprogram = new ShaderProgram();
	prefilterprogram->AttachShader(cubemapvShader);
	prefilterprogram->AttachShader(prefilterfShader);
	Material* prefilterMat = new Material(prefilterprogram);

	//BRDF Reflection shader
	Shader* brdfvShader = new Shader("shaders/brdfvShader.glsl", GL_VERTEX_SHADER);
	Shader* brdffShader = new Shader("shaders/brdffShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram* brdfprogram = new ShaderProgram();
	brdfprogram->AttachShader(brdfvShader);
	brdfprogram->AttachShader(brdffShader);
	Material* brdfMat = new Material(brdfprogram);
#pragma endregion

#pragma region Image_Loading
	//Loading our HDR environment map
	GLuint HDRimage;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType("Assets/Newport_Loft_Ref.hdr");
	FIBITMAP* bitmap32 = FreeImage_Load(fif, "Assets/Newport_Loft_Ref.hdr");

	glGenTextures(1, &HDRimage);
	glBindTexture(GL_TEXTURE_2D, HDRimage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, FreeImage_GetWidth(bitmap32), FreeImage_GetHeight(bitmap32),
		0, GL_RGB, GL_FLOAT, static_cast<void*>(FreeImage_GetBits(bitmap32)));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FreeImage_Unload(bitmap32);

	//Loading irradiance map
	//If you want to create your own based on a HDR texture follow this link:
	//https://learnopengl.com/PBR/IBL/Diffuse-irradiance - Cubemap Convolution section
	GLuint irradianceMap;
	fif = FreeImage_GetFileType("Assets/Newport_Loft_Env.hdr");
	bitmap32 = FreeImage_Load(fif, "Assets/Newport_Loft_Env.hdr");

	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_2D, irradianceMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, FreeImage_GetWidth(bitmap32), FreeImage_GetHeight(bitmap32),
		0, GL_RGB, GL_FLOAT, static_cast<void*>(FreeImage_GetBits(bitmap32)));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	FreeImage_Unload(bitmap32);

	//Preallocating memory for our BRDF look up texture (LUT)
	GLuint brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#pragma endregion

#pragma region 

	//Initializing skybox cubemap
	GLuint skyboxMap;
	glGenTextures(1, &skyboxMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxMap);
	for (unsigned int i = 0; i < 6; ++i)
	{

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//initalizing irradiance cubemap
	GLuint irradianceCubeMap;
	glGenTextures(1, &irradianceCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubeMap);
	for (unsigned int i = 0; i < 6; ++i)
	{

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//Creating pre-filter cubemap
	GLuint prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
			128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	//Trilinear filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
#pragma endregion

#pragma region Equirectangular_To_CubeMap

	glm::mat4 captureProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	//View matrix for looking at each side of the cube individually
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//Render each of the sides of the equirectangular map onto the cube, then store rendered data into a cubemap texture
	//Requires rendering onto a seperate frame buffer for each side of the cube (6 times)
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	hdrconvertMat->SetMatrix((char*)"proj", captureProj);
	glUniform1i(glGetUniformLocation(hdrconvertprogram->GetGLShaderProgram(), (char*)"equirectangularMap"), 0);
	glActiveTexture(GL_TEXTURE0);

	//Environment map (skybox) conversion
	glBindTexture(GL_TEXTURE_2D, HDRimage);
	for (unsigned int i = 0; i < 6; ++i)
	{
		
		hdrconvertMat->SetMatrix((char*)"view", captureViews[i]);	
		hdrconvertMat->Bind();


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skyboxMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube(); 

	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxMap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//Irradiance map conversion
	glBindTexture(GL_TEXTURE_2D, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		
		hdrconvertMat->SetMatrix((char*)"view", captureViews[i]);	
		hdrconvertMat->Bind();
		

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceCubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube(); 

	}
	hdrconvertMat->Unbind();

#pragma endregion

#pragma region Specular IBL Pre-rendering
	//Prefiltering our environment map (skybox) for varied roughness reflections
	prefilterMat->SetMatrix((char*)"proj", captureProj);
	glUniform1i(glGetUniformLocation(prefilterprogram->GetGLShaderProgram(), (char*)"environmentMap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxMap);
	GLuint maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		GLuint mipWidth = 128 * std::pow(0.5, mip);
		GLuint  mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		UpdateFloat((char*)"roughness", roughness, prefilterprogram);

		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterMat->SetMatrix((char*)"view", captureViews[i]);
			prefilterMat->Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	prefilterMat->Unbind();

	//BRDF LUT creation
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfMat->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Render a quad that the texture is drawn over
	renderQuad();
	brdfMat->Unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion
	//Creating our sphere grid
	int side = 10;
	int index = 0;
	for (float y = 1; y > -2; y -= 3.f / side)
	{
		for (float x = -1; x < 2; x += 3.f / side)
		{
			gameEntities.push_back(new Entity("sphere" + std::to_string(index)));

			gameEntities[index]->transform->position = glm::vec3(x*side / 2.5f, y*side / 2.5f, -1.0f);
			gameEntities[index]->transform->recalculateWMatrix = true;
			gameEntities[index]->SetModelMesh(sphereMesh);
			gameEntities[index]->SetModelMaterial(pbrMat);
			index++;
		}
	}
	glm::vec4 lightPositions[4] = {
	   glm::vec4(-10.0f,  10.0f, 10.0f, 1.0),
	   glm::vec4(10.0f,  10.0f, 10.0f, 1.0),
	   glm::vec4(-10.0f, -10.0f, 10.0f, 1.0),
	   glm::vec4(10.0f, -10.0f, 10.0f, 1.0)
	};

	//MSA
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LEQUAL);

	pbrprogram->Bind();
	//Upload light data
	glUniform4fv(glGetUniformLocation(pbrprogram->GetGLShaderProgram(), "lightPositions"), 4, glm::value_ptr(lightPositions[0]));
	//Set texture addresses
	glUniform1i(glGetUniformLocation(pbrprogram->GetGLShaderProgram(), (char*)"irradianceMap"), 0);
	glUniform1i(glGetUniformLocation(pbrprogram->GetGLShaderProgram(), (char*)"prefilterMap"), 1);
	glUniform1i(glGetUniformLocation(pbrprogram->GetGLShaderProgram(), (char*)"brdfLUT"), 2);
	// This is the game loop, the game logic and render part goes in here.
	// It checks if the created window is still open, and keeps performing the specified operations until the window is closed
	while (!glfwWindowShouldClose(window))
	{
	
		// Calculate delta time.
		float dt = glfwGetTime();
		// Reset the timer.
		glfwSetTime(0);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, WIDTH, HEIGHT);
	#pragma region Updates	
		//Update camera and camera matrix
		fpsCam->Update(window, WIDTH, HEIGHT, mousePosition, dt);		
		pbrMat->SetMatrix((char*)"cameraMatrix", fpsCam->camera->GetCameraMatrix());
		
		//Activate and bind our various textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubeMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

		//Update PBR shader variables
		UpdateFloat((char*)"ao", 1.0f, pbrprogram);
		UpdateVec3((char*)"camPos", fpsCam->transform->position, pbrprogram);
		UpdateVec3((char*)"albedo", glm::vec3(0.5f, 0.0f, 0.0f), pbrprogram);
		index = 0;
		for (int y = 0; y < side; y++)
		{
			UpdateFloat((char*)"metallic", (float)(side - y) / side, pbrprogram);
			for (int x = 0; x < side; x++)
			{
				//Update model matrix
				gameEntities[index]->Update();
				UpdateFloat((char*)"roughness", glm::clamp((float)x / side, 0.025f, 1.0f), pbrprogram);
				
				//Bind material and render
				pbrMat->Bind();
				gameEntities[index]->DrawModel();
				index++;
			}
		}
	#pragma endregion

	
	#pragma region Skybox_Rendering

		//Update skybox matrix values
		skyboxMat->SetMatrix((char*)"view", fpsCam->camera->GetView());
		skyboxMat->SetMatrix((char*)"proj", fpsCam->camera->GetProj());
		skyboxMat->Bind();
		
		//Update skybox texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxMap);
		renderCube();
		skyboxMat->Unbind();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	#pragma endregion
		
		// Swaps the front and back buffers of the specified window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	pbrMat->Unbind();

	delete fpsCam;
	delete sphereMesh;
	delete pbrMat;
	delete hdrconvertMat;
	delete skyboxMat;
	delete prefilterMat;
	delete brdfMat;

	for (int i = 0; i < gameEntities.size(); i++)
		delete gameEntities[i];

	// Terminate all the stuff related to GLFW and exit the program using the return value EXIT_SUCCESS
	glfwTerminate();

	return EXIT_SUCCESS;
}


