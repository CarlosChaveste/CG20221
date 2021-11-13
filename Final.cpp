/*---------------------------------------------------------*/
/* --- Proyecto Final Computación Gráfica e Interacción Humano-Computadora---*/
/*-----------------  Semestre  2022-1   ---------------------------*/
/*------------- Alumno:   Chaveste Bermejo Carlos Alberto ---------------*/
/*------------- No. Cuenta    315311284     ---------------*/
/*------------- Alumno:   Ramirez Castañeda Jesus Eduardo ---------------*/
/*------------- No. Cuenta    3153     ---------------*/
/*------------- Alumno:   Torres Rojas Eduardo ---------------*/
/*------------- No. Cuenta    315072123   ---------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 25.0f, 525.0f));
float MovementSpeed = 20.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightLuzCMov(0.0f, 0.6f, 0.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -0.0f);
double myVariable = 0.0;
double LuzCMov = 0.0;

// posiciones
//float x = 0.0f;
//float y = 0.0f;
//float	//movAuto_x = 0.0f,
//movAuto_y = 0.0f,
//movAuto_z = 0.0f,
//orienta = 0.0f,
//giroLlanta = 0.0f;

//int		estado = 0;

bool	animacion = false,
animacion1 = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;


//Keyframes (Manipulación y dibujo)
/*float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
//rotRodIzq = 0.0f,
//giroMonito = 0.0f,
//movBrazoIzq = 0.0f,
movBrazoDer = 0.0f,
movRodDer = 0.0f,
movCabeza = 0.0f;


float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
giroMonitoInc = 0.0f,
movBrazoIzqInc = 0.0f,
movBrazoDerInc = 0.0f,
movRodDerInc = 0.0f,
movCabezaInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazoIzq;
	float movBrazoDer;
	float MovRodDer;
	float movCabeza;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;


void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzq;
	KeyFrame[FrameIndex].MovRodDer = movRodDer;
	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer;
	KeyFrame[FrameIndex].movCabeza = movCabeza;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	movBrazoIzq = KeyFrame[0].movBrazoIzq;
	movRodDer = KeyFrame[0].MovRodDer;
	movBrazoDer = KeyFrame[0].movBrazoDer;
	movCabeza = KeyFrame[0].movCabeza;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	movBrazoIzqInc = (KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps;
	movRodDerInc = (KeyFrame[playIndex + 1].MovRodDer - KeyFrame[playIndex].MovRodDer) / i_max_steps;
	movBrazoDerInc = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;
	movCabezaInc = (KeyFrame[playIndex + 1].movCabeza - KeyFrame[playIndex].movCabeza) / i_max_steps;
}
*/
void animate(void)
{
	lightPosition.x = 20.0 * cos(myVariable);
	lightPosition.y = 20.0 * sin(myVariable);
	myVariable += 0.1;

	lightLuzCMov.x = 80 + 25.0 * sin(LuzCMov);
	lightLuzCMov.z = 120.0 * cos(LuzCMov);

	LuzCMov += 0.1;
	/*
	movBrazoIzq += movBrazoIzqInc;
	movRodDer += movRodDerInc;
	movBrazoDer += movBrazoDerInc;
	movCabeza += movCabezaInc;
	
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				//interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;


			movBrazoIzq += movBrazoIzqInc;
			movRodDer += movRodDerInc;
			movBrazoDer += movBrazoDerInc;
			movCabeza += movCabezaInc;

			i_curr_steps++;
		}*/
	}



void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model pared("resources/objects/Pared/Pared.obj");
	Model puerta("resources/objects/Puerta/Puerta.obj");
	Model puertaI("resources/objects/PuertaI/PuertaI.obj");
	Model paredV("resources/objects/ParedV/ParedV.obj");
	Model puertaPrinc("resources/objects/puertaPrinc/puertaPrinc.obj");
	Model techo("resources/objects/Techo/techo.obj");
	Model techoI("resources/objects/Techo/techoI.obj");
	Model Kitchen("resources/objects/Kitchen/Kitchen.obj");
	Model Kitchen2("resources/objects/Kitchen2/Kitchen2.obj");
	Model Pared2_text("resources/objects/Pared2_text/Pared2_text.obj");
	Model refrigerador("resources/objects/refrigerador/refrigerador.obj");
	/*Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model casaVieja("resources/objects/casa/OldHouse.obj");
	//Model cubo("resources/objects/cubo/cube02.obj");
	Model casaDoll("resources/objects/casa/DollHouse.obj");
	*///Model casaBrujas("resources/objects/CasaBrujas/casaBrujas.obj");
	//Model pasto("resources/objects/pasto/pasto.obj");

	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);
	*/
	//Inicialización de KeyFrames
	
	/**for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
		KeyFrame[i].movBrazoIzq = 0;
		KeyFrame[i].MovRodDer = 0;
		KeyFrame[i].movBrazoDer = 0;
		KeyFrame[i].movCabeza = 0;
	}
	*/
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.8f, 0.8f, 0.8f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.0009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.0032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		/*animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		/*animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		ninja.Draw(animShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casaDoll.Draw(staticShader);
		*/
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		
		
		//Pared Superior Parents Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(67.5f, 25.0f, -232.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Puerta 1 Derecha Parents Room 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.5f, 0.0f, -437.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.6325f, 1.0f));
		staticShader.setMat4("model", model);
		puertaI.Draw(staticShader);


		//Pared 2 Derecha Parents Room 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.5f, 25.0f, -461.765f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0375f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared 2 Derecha Parents Room 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.5f, 25.0f, -400.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.085f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared 3 Derecha Parents Room 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.5f, 25.0f, -255.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		//Pared Superior Parents Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-182.5f, 25.0f, -473.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Puerta 1 Izquierda Parents Room 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 0.0f, -380.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.6325f, 1.0f));
		staticShader.setMat4("model", model);
		puertaI.Draw(staticShader);

		//Pared 1 Izquierda Parents Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 25.0f, -435.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.16f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared 2 Izquierda Parents Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 25.0f, -315.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Lounge-KidsRoom
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-207.5f, 25.0f, -48.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared 1 Izquierda Kids Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 25.0f, -248.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		//Pared 2 Izquierda Kids Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 25.0f, -148.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared 3 Izquierda Kids Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 25.0f, -48.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		//Pared Izquierda Lounge
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 25.0f, 64.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
			
		
		//Pared Inferior Lounge
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-207.5f, 25.0f, 150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f,0.2f,0.15f));
		staticShader.setMat4("model", model);
		paredV.Draw(staticShader);
		
		//Techo Derecho Lounge
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-205.5f, 11.0f, 150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(-0.04f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.125f));
		staticShader.setMat4("model", model);
		techo.Draw(staticShader);
		
		//Techo Izquierdo Lounge
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-213.5f, 10.5f, 150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.045f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.125f));
		staticShader.setMat4("model", model);
		techoI.Draw(staticShader);
		
		//Pared Derecha Lounge 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-124.0f, 25.0f, 113.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Derecha Lounge 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-124.0f, 25.0f, 16.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Izquierda Entrada
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.0f, 25.0f, 5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Derecha Entrada
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 25.0f, 5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Izquierda Living Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(75.0f, 25.0f, 52.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Inferior Izquierda Living Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(103.9f, 25.0f, 104.50f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.125f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Inferior Derecha Living Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(213.25f, 25.0f, 104.50f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.125f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Izquierda Burnace
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.5f, 24.15f, -207.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.5f, 24.15f, -157.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		//Pared Frontal Burnace
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-21.5f, 24.15f, -182.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		//Pared Kitchen y Living Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(101.5f, 24.15f, -157.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.528f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		//Pared Derecha Living Room 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(242.5f, 25.0f, -116.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Derecha Living Room 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(242.5f, 25.0f, 9.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared Derecha Living Room 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(242.5f, 25.0f, 100.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.2f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Cocina parte1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-13.0f, 8.0f, -133.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.4f));
		staticShader.setMat4("model", model);
		Kitchen.Draw(staticShader); 

		//Cocina parte2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(74.0f, 8.0f, -61.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		Kitchen2.Draw(staticShader);

		//Refrigerador
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(61.0f, 0.0f, -135.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.11f));
		staticShader.setMat4("model", model);
		refrigerador.Draw(staticShader);
		
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		casaVieja.Draw(staticShader);
		*/
		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, 0.0f, -70.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		casaBrujas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 0.0f, -70.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		/*model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f + movAuto_y, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-movRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(1.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabeza.Draw(staticShader);
		*/
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 30.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.579f, 0.93f));
		staticShader.setMat4("model", model);
		puertaPrinc.Draw(staticShader);
				//puerta Principal Living Room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(158.50f, 0.0f, 104.50f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.05f, 0.579f, 0.93f));
		staticShader.setMat4("model", model);
		puertaPrinc.Draw(staticShader);
		//puerta Izquierda Entrada
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-123.5f, 0.0f, 52.0f));
		model = glm::scale(model, glm::vec3(1.055f, 0.579f, 0.93f));
		staticShader.setMat4("model", model);
		puertaPrinc.Draw(staticShader);
		
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	/*if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		lightDirection.x += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		lightDirection.x -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		movBrazoIzq += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		movBrazoIzq -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		movBrazoDer += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		movBrazoDer -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		movRodDer += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		movRodDer -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		movCabeza += 3.03f;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		movCabeza -= 3.03f;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		posY += 3.03f;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		posY -= 3.03f;

*/
	/*



	//Car animation/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;



	if (key == GLFW_KEY_L && action == GLFW_PRESS && movAuto_z <= 205.0f) {
		movAuto_z = 0.0f;
	}



	

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
	*/
	}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
