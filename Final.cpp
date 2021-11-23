/*---------------------------------------------------------*/
/* --- Proyecto Final Computación Gráfica e Interacción Humano-Computadora---*/
/*-----------------  Semestre  2022-1   ---------------------------*/
/*------------- Alumno:   Chaveste Bermejo Carlos Alberto ---------------*/
/*------------- No. Cuenta    315311284     ---------------*/
/*------------- Alumno:   Ramirez Castañeda Jesus Eduardo ---------------*/
/*------------- No. Cuenta    315071669     ---------------*/
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

#include <irrklang/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

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

//Hamster
int estado = 0, estado1 = 0;
glm::vec3 hamstPosition(67.5f, 0.0f, 200.0f);
glm::vec3 hamstRot(0.0f, 0.0f, 0.0f);

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightLuzCMov(0.0f, 0.6f, 0.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -0.0f);
double myVariable = 0.0;
double LuzCMov = 0.0;



bool	animacion = false,
animacion1 = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;

//Variables para animacion muñeco de nieve
float	movY_bola3 = 0.5f,
	movY_bola2 = 0.5f,
	movY_bola1 = 0.0f,
	rot_cabeza3 = 0.0f;
int aux_muneco = 0;
bool animacion_muneco = false;

//Variables para movimiento del trineo
glm::vec3 posicionTrineo(500.0f, 150.0f, 0.0f);
float	Trineo_aument = 0.0f,
	Trineo_aument2 = 0.0f,
	rotpierna = 0.0f,
	rotpata = 0.0f;
bool animacion_trineo = false;
int aux_trineo = 0;

//variables para movimiento de puerta princ (JERC)

int estadoP = 0;
float rotPuertaAnima = 90.0f;
bool animacion_puerta = false;

//variables para movimiento de estrella (JERC)

int estadoE = 0;
float rotEstrella = 0.0f;
bool animacion_estrella = false;
float movY_estrella = 43.0f;


//Keyframes (Manipulación y dibujo)
//Cascanueces
float	brazoRot = 0.0f,
	piernasRot = 0.0f,
	cabezaPos = -4.0f;

float	brazoRotIncr = 0.0f,
	piernasRotIncr = 0.0f,
	cabezaPosIncr = 0.0f;

#define MAX_FRAMES 30
int i_max_steps = 10;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float	brazoRot = 0.0f,
			piernasRot = 0.0f,
			cabezaPos = 0.0f;
}FRAME;
FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 29;			//introducir datos
bool play = false;
int playIndex = 0;
void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	KeyFrame[FrameIndex].brazoRot = brazoRot;
	KeyFrame[FrameIndex].piernasRot = piernasRot;
	KeyFrame[FrameIndex].cabezaPos = cabezaPos;
	FrameIndex++;
}
void resetElements(void)
{
	brazoRot = KeyFrame[0].brazoRot;
	piernasRot = KeyFrame[0].piernasRot;
	cabezaPos = KeyFrame[0].cabezaPos;
	}
void interpolation(void)
{
	brazoRotIncr = (KeyFrame[playIndex + 1].brazoRot - KeyFrame[playIndex].brazoRot) / i_max_steps;
	piernasRotIncr = (KeyFrame[playIndex + 1].piernasRot - KeyFrame[playIndex].piernasRot) / i_max_steps;
	cabezaPosIncr = (KeyFrame[playIndex + 1].cabezaPos - KeyFrame[playIndex].cabezaPos) / i_max_steps;
}
void animate(void)
{
	lightPosition.x = 20.0 * cos(myVariable);
	lightPosition.y = 20.0 * sin(myVariable);
	myVariable += 0.1;

	lightLuzCMov.x = 80 + 25.0 * sin(LuzCMov);
	lightLuzCMov.z = 120.0 * cos(LuzCMov);

	LuzCMov += 0.1;
	
		//AnimacionPuertaPrinc (JERC)

	if (animacion_puerta) {
		if (estadoP == 0) {
			if (rotPuertaAnima >= 0.0f) {
				rotPuertaAnima -= 1.0f;
			}
			else {
				estadoP = 1;
			}
		}
		if (estadoP == 1) {
			if (rotPuertaAnima <= 90.0f) {
				rotPuertaAnima += 1.0f;
			}
			else {
				estadoP = 2;
				animacion_puerta = false;
			}
		}
		if (estadoP == 2) {
			estadoP = 0;
		}
	}
	
	//AnimacionHamster
	hamstPosition.x= 10.0 * cos(myVariable);
	hamstPosition.z = 90.0 * sin(myVariable);
	
	if (estado1 == 0) {
		hamstRot.y += 5;
		if (hamstRot.y > 40) {
			estado1 = 1;
		}
	}
	else if (estado1 == 1) {
		hamstRot.y -= 5;
		if (hamstRot.y < -40) {
			estado1 = 0;
		}
	}
	
	
	if (estado == 0) {
		hamstRot.x += 1;
		if (hamstRot.x > 35) {
			estado = 1;
		}
	}
	else if (estado == 1) {
		hamstRot.x -= 1;
		if (hamstRot.x < -35) {
			estado = 0;
		}
	}
	
	//Animacion muñeco de nieve
	if (animacion_muneco)
	{
		if (aux_muneco == 0)
		{
			if (movY_bola3 <= 25.0f)
			{
				movY_bola1 += 0.1;
				movY_bola2 += 0.2;
				movY_bola3 += 0.3;
				rot_cabeza3 += 2.204083;
			}
			else
				aux_muneco = 1;
		}
		if (aux_muneco == 1)
		{
			if (movY_bola3 >= -10.0f)
			{
				movY_bola1 -= 0.1;
				movY_bola2 -= 0.2;
				movY_bola3 -= 0.3;
				rot_cabeza3 += 1.4621;
			}
			else
				aux_muneco = 2;
		}
		if (aux_muneco == 2)
		{
			if (movY_bola3 < 0.5f)
			{
				movY_bola1 += 0.1;
				movY_bola2 += 0.2;
				movY_bola3 += 0.3;
				rot_cabeza3 += 0.2857;
			}
			else
			{
				aux_muneco = 3;
				animacion_muneco = false;
			}
		}
		if (aux_muneco == 3)
		{
			aux_muneco = 0;
		}
	}
	if (play){
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
					interpolation();
				}
			}
			else
			{
				//Draw animation
				cabezaPos += cabezaPosIncr;
				brazoRot += brazoRotIncr;
				piernasRot += piernasRotIncr;
				i_curr_steps++;
			}
	}
	
	//Animacion estrella arbol (JERC)
	if (animacion_estrella) {
		if (estadoE == 0)
		{
			if (movY_estrella <= 46.0f)
			{
				movY_estrella += 0.1;
				rotEstrella += 5.204083;
			}
			else
				estadoE = 1;
		}
		if (estadoE == 1)
		{
			if (movY_estrella >= 43.0f)
			{
				movY_estrella -= 0.1;
				rotEstrella += 5.4621;
			}
			else
				estadoE = 2;
		}
		if (estadoE == 2)
		{
			if (movY_estrella < 46.0f)
			{
				movY_estrella += 0.1;
				rotEstrella += 5.4857;
			}
			else
			{
				estadoE = 3;
				//animacion_estrella = false;
			}
		}
		if (estadoE == 3)
		{
			estadoE = 0;
		}
	}
	
	//Animacion trineo
	if (animacion_trineo)
	{
		posicionTrineo.x = 500.0 * cos(Trineo_aument);
		posicionTrineo.z = 500.0 * sin(Trineo_aument);
		Trineo_aument += 0.01;
		Trineo_aument2 += 0.57212;

		if (aux_trineo == 0)
		{
			if (rotpierna <= 5.0f)
			{
				rotpierna += 0.2;
				rotpata += 1.5; 
			}
			else
				aux_trineo = 1;
		}
		if (aux_trineo == 1)
		{
			if (rotpierna >= -5.0f)
			{
				rotpierna -= 0.2;
				rotpata -= 1.5;
			}
			else
				aux_trineo = 0;
		}
	}
}



void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	//Cancion
	irrklang::ISoundEngine* Sonido = createIrrKlangDevice();
	Sonido->play2D("audio/cancion.mp3", true);
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
	Model muneco1("resources/objects/muneco/muneco1.obj");
	Model muneco2("resources/objects/muneco/muneco2.obj");
	Model muneco3("resources/objects/muneco/muneco3.obj");
	Model hamster("resources/objects/Hamster/hamster.obj");
	Model pino("resources/objects/pino/pino.obj");
	Model puertaG("resources/objects/puertaPrinc/puertaG.obj");
	Model trineos_renos_santa("resources/objects/trineos_renos_santa/trineos_renos_santa.obj");
	Model pierna("resources/objects/trineos_renos_santa/pierna.obj");
	Model pata("resources/objects/trineos_renos_santa/pata.obj");
	Model calcetas("resources/objects/calceta/calceta.obj");
	Model campanas("resources/objects/campana/campana.obj");
	Model cSanta("resources/objects/chocoS/ChocoSantaClaus06.obj");
	Model pasto("resources/objects/pastop/pastop.obj");
	Model planta("resources/objects/planta/planta.obj");
	Model regalos("resources/objects/regalos/regalos.obj");
	Model mesa_comedor("resources/objects/mesa_comedor/mesa_comedor.obj");
	Model mesa_interior("resources/objects/mesa_interior/mesa_interior.obj");
	Model mesa_exterior("resources/objects/mesa_exterior/mesa_exterior.obj");
	Model cama_ninos("resources/objects/camas/cama_ninos.obj");
	Model casca("resources/objects/cascanueces/cuerpoDesp.obj");
	Model cascaCa("resources/objects/cascanueces/cabeza.obj");
	Model cascaBr("resources/objects/cascanueces/brazos.obj");
	Model cascaPrI("resources/objects/cascanueces/piernasI.obj");
	Model cascaPrD("resources/objects/cascanueces/piernasD.obj");
	Model puertaAnima("resources/objects/puertaPrinc/puertaAnima.obj");      
	Model Arbol_navida("resources/objects/Arbol_navida/Arbol_navida.obj");  
	Model Estrella_navida("resources/objects/Estrella_navida/Estrella_navida.obj"); 
	Model puerta_garage("resources/objects/garage/puerta_garage.obj");
	Model carro("resources/objects/garage/carro.obj");
	Model techoP("resources/objects/Techo/techoP.obj");
	Model techoG("resources/objects/Techo/techoG.obj");
	Model tele("resources/objects/tele/teleP.obj"); 
	Model estante("resources/objects/estante/estante.obj");
	Model couch("resources/objects/couch/couch.obj");
	Model pintura("resources/objects/pintura/pintura.obj");
	Model estante_cosas("resources/objects/estantes_varios/estante_cosas.obj");
	Model estante_cajones("resources/objects/estantes_varios/estante_cajones.obj");
	Model closet("resources/objects/estantes_varios/closet.obj");
	Model cama_padres("resources/objects/camas/cama_padres.obj");
	Model PC("resources/objects/PC/PC.obj");
	Model pilaRegalos("resources/objects/regalos/pilaRegalos.obj");
	Model esferasD("resources/objects/esferas/tira10.obj");
	Model corona("resources/objects/corona/corona.obj");
	Model carroM("resources/objects/carroMadera/carroMadera.obj");
	Model Sillon_cafe("resources/objects/Sillon_cafe/Sillon_cafe.obj"); 
	Model Sillon_flores("resources/objects/Sillon_flores/Sillon_flores.obj"); 
	Model Chimenea("resources/objects/Chimenea/Chimenea.obj"); 
	Model Estante_lounge("resources/objects/Estante_lounge/Estante_lounge.obj"); 
	Model Sillon_flores("resources/objects/Sillon_flores/Sillon_flores.obj");
	Model horno("resources/objects/hornoLavadora/horno.obj");
	Model lavadora("resources/objects/hornoLavadora/lavadora.obj");
	
	//Inicialización de KeyFrames
	
	KeyFrame[0].brazoRot = 0.0f;
		KeyFrame[0].cabezaPos = -4.0f;
		KeyFrame[0].piernasRot = 0;

		KeyFrame[1].brazoRot = 15.0f;
		KeyFrame[1].cabezaPos = -2.0f;
		KeyFrame[1].piernasRot = 0;

		KeyFrame[2].brazoRot = 30.0f;
		KeyFrame[2].cabezaPos = -4.0f;
		KeyFrame[2].piernasRot = 0;

		KeyFrame[3].brazoRot = 45.0f;
		KeyFrame[3].cabezaPos = -2.0f;
		KeyFrame[3].piernasRot = 0;

		KeyFrame[4].brazoRot = 60.0f;
		KeyFrame[4].cabezaPos = -4.0f;
		KeyFrame[4].piernasRot = 0.0f;

		KeyFrame[5].brazoRot = 45.0f;
		KeyFrame[5].cabezaPos = -4.0f;
		KeyFrame[5].piernasRot = 0.0f;

		KeyFrame[6].brazoRot = 30.0f;
		KeyFrame[6].cabezaPos = -2.0f;
		KeyFrame[6].piernasRot = 0.0f;

		KeyFrame[7].brazoRot = 15.0f;
		KeyFrame[7].cabezaPos = -4.0f;
		KeyFrame[7].piernasRot = 0.0f;

		KeyFrame[8].brazoRot = 0.0f;
		KeyFrame[8].cabezaPos = -2.0f;
		KeyFrame[8].piernasRot = 0.0f;

		KeyFrame[9].brazoRot = -15.0f;
		KeyFrame[9].cabezaPos = -4.0f;
		KeyFrame[9].piernasRot = 0.0f;

		KeyFrame[10].brazoRot = -30.0f;
		KeyFrame[10].cabezaPos = -2.0f;
		KeyFrame[10].piernasRot = 0.0f;

		KeyFrame[11].brazoRot = -45.0f;
		KeyFrame[11].cabezaPos = -4.0f;
		KeyFrame[11].piernasRot = 0.0f;

		KeyFrame[12].brazoRot = -60.0f;
		KeyFrame[12].cabezaPos = -2.0f;
		KeyFrame[12].piernasRot = 0.0f;

		KeyFrame[13].brazoRot = -45.0f;
		KeyFrame[13].cabezaPos = -4.0f;
		KeyFrame[13].piernasRot = 0.0f;

		KeyFrame[14].brazoRot = -30.0f;
		KeyFrame[14].cabezaPos = -2.0f;
		KeyFrame[14].piernasRot = 0.0f;

		KeyFrame[15].brazoRot = -15.0f;
		KeyFrame[15].cabezaPos = -4.0f;
		KeyFrame[15].piernasRot = 0.0f;

		KeyFrame[16].brazoRot = 0.0f;
		KeyFrame[16].cabezaPos = -2.0f;
		KeyFrame[16].piernasRot = 0.0f;

		KeyFrame[17].brazoRot = 15.0f;
		KeyFrame[17].cabezaPos = -4.0f;
		KeyFrame[17].piernasRot = 0.0f;

		KeyFrame[18].brazoRot = 30.0f;
		KeyFrame[18].cabezaPos = -2.0f;
		KeyFrame[18].piernasRot = 0.0f;

		KeyFrame[19].brazoRot = 45.0f;
		KeyFrame[19].cabezaPos = -4.0f;
		KeyFrame[19].piernasRot = 0.0f;

		KeyFrame[20].brazoRot = 60.0f;
		KeyFrame[20].cabezaPos = -2.0f;
		KeyFrame[20].piernasRot = 0.0f;

		KeyFrame[21].brazoRot = 45.0f;
		KeyFrame[21].cabezaPos = -4.0f;
		KeyFrame[21].piernasRot = 0.0f;

		KeyFrame[22].brazoRot = 30.0f;
		KeyFrame[22].cabezaPos = -2.0f;
		KeyFrame[22].piernasRot = 0.0f;

		KeyFrame[23].brazoRot = 15.0f;
		KeyFrame[23].cabezaPos = -4.0f;
		KeyFrame[23].piernasRot = 0.0f;

		KeyFrame[24].brazoRot = 0.0f;
		KeyFrame[24].cabezaPos = -2.0f;
		KeyFrame[24].piernasRot = 0.0f;

		KeyFrame[25].brazoRot = -15.0f;
		KeyFrame[25].cabezaPos = -4.0f;
		KeyFrame[25].piernasRot = 0.0f;

		KeyFrame[26].brazoRot = -30.0f;
		KeyFrame[26].cabezaPos = -2.0f;
		KeyFrame[26].piernasRot = 0.0f;

		KeyFrame[27].brazoRot = -45.0f;
		KeyFrame[27].cabezaPos = -4.0f;
		KeyFrame[27].piernasRot = 0.0f;

		KeyFrame[28].brazoRot = -60.0f;
		KeyFrame[28].cabezaPos = -2.0f;
		KeyFrame[28].piernasRot = 0.0f;

		KeyFrame[29].brazoRot = -45.0f;
		KeyFrame[29].cabezaPos = 0.0f;
		KeyFrame[29].piernasRot = 0.0f;
	

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
		glm::mat4 tmp_trineo = glm::mat4(1.0f);
		glm::mat4 tmp_trineo2 = glm::mat4(1.0f);
		glm::mat4 tmp_trineo3 = glm::mat4(1.0f);
		glm::mat4 tmp_trineo4 = glm::mat4(1.0f);
		
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
		
		//Tele
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(205.5f, 0.0f, 90.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.2f, 1.1f, 1.0f));
		staticShader.setMat4("model", model);
		tele.Draw(staticShader);

		//Sillon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(200.5f, 0.0f, 20.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f,0.8, 0.8f));
		staticShader.setMat4("model", model);
		couch.Draw(staticShader);

		//Estante
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, -3.0f, 90.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.67f, 0.5f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);

		//Pintura
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(162.5f, 10.0f, -150.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		staticShader.setMat4("model", model);
		pintura.Draw(staticShader);
		
		//CuerpoCasca
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, 250.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casca.Draw(staticShader);

		//CabezaCasca
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, cabezaPos, 250.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaCa.Draw(staticShader);

		//BrazosCasca
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, 26.0f, 250.0f));
		model = glm::rotate(model, glm::radians(brazoRot), glm::vec3(0.2f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaBr.Draw(staticShader);

		//PiernasCascaI
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, 7.0f, 250.0f));
		model = glm::rotate(model, glm::radians(brazoRot), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaPrI.Draw(staticShader);

		//PiernasCascaD
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, 7.0f, 250.0f));
		model = glm::rotate(model, glm::radians(brazoRot), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaPrD.Draw(staticShader);

		//CuerpoCasca2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, 250.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casca.Draw(staticShader);

		//CabezaCasca2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, cabezaPos, 250.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaCa.Draw(staticShader);

		//BrazosCasca2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 26.0f, 250.0f));
		model = glm::rotate(model, glm::radians(brazoRot), glm::vec3(0.2f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaBr.Draw(staticShader);

		//PiernasCascaI2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 7.0f, 250.0f));
		model = glm::rotate(model, glm::radians(brazoRot), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaPrI.Draw(staticShader);

		//PiernasCascaD2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 7.0f, 250.0f));
		model = glm::rotate(model, glm::radians(brazoRot), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		cascaPrD.Draw(staticShader);

		
		//Pino 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, 350.0f));
		model = glm::scale(model, glm::vec3(10.0f, 8.0f, 10.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//Pino 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, 345.0f));
		model = glm::scale(model, glm::vec3(10.0f, 8.0f, 10.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//Pino 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-295.0f, 0.0f, 345.0f));
		model = glm::scale(model, glm::vec3(10.0f, 8.0f, 10.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//Pino 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 305.0f));
		model = glm::scale(model, glm::vec3(9.5f, 9.0f, 10.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//Pino 5
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(250.0f, 0.0f, 305.0f));
		model = glm::scale(model, glm::vec3(9.5f, 9.0f, 10.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//Pino 6
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(295.0f, 0.0f, 345.0f));
		model = glm::scale(model, glm::vec3(10.0f, 8.0f, 10.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//Pasto 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f, 2.0f, 345.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);
		
		//arbol navidad //(JERC)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(220.0f, 0.0f, -27.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.15f, 1.5f));
		staticShader.setMat4("model", model);
		Arbol_navida.Draw(staticShader);

		//estrella navidad y= 43.0f //(JERC)

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(220.6f, movY_estrella, -25.2f));
		model = glm::rotate(model, glm::radians(rotEstrella), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.5f));
		staticShader.setMat4("model", model);
		Estrella_navida.Draw(staticShader);

		//Pasto 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 2.0f, 345.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		//Pasto 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-170.0f, 2.0f, 315.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		//Pasto 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-130.0f, 2.0f, 310.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		//Planta 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-160.0f, 2.0f, 345.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);
		
		//Hamster
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(hamstPosition.x, 0, 200+hamstPosition.z));
		model = glm::rotate(model, glm::radians(hamstRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(hamstRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f,0.1f));
		staticShader.setMat4("model", model);
		hamster.Draw(staticShader);
		
		//Calcetas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 30.0f, 7.5f));
		model = glm::scale(model, glm::vec3(0.5f,0.5f, 0.5f));
		staticShader.setMat4("model", model);
		calcetas.Draw(staticShader);

		//Calcetas 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-50.0f, 30.0f, 7.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		calcetas.Draw(staticShader);

		//SantaChocolatal
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-90.0f, 2.0f, 17.5f));
		model = glm::scale(model, glm::vec3(65.0f, 65.0f, 65.0f));
		staticShader.setMat4("model", model);
		cSanta.Draw(staticShader);
		
		//Campanas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-207.5f, 55.0f, 162.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		campanas.Draw(staticShader);

		//Campanas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-227.5f, 55.0f, 162.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		campanas.Draw(staticShader);

		//Campanas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-187.5f, 55.0f, 162.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		campanas.Draw(staticShader);

		//Campanas2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 37.0f, 7.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		campanas.Draw(staticShader);
		
		//Regalos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, 125.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		regalos.Draw(staticShader);

		//Pila Regalos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-90.0f, 0.0f, 150.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		pilaRegalos.Draw(staticShader);
		
		//Tira esferas 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-90.0f, 46.5f, 115.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		esferasD.Draw(staticShader);

		//Tira esferas 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, 46.5f, 115.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		esferasD.Draw(staticShader);

		//Tira esferas 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 46.5f, 115.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		esferasD.Draw(staticShader);

		//Tira esferas 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(60.0f, 46.5f, 115.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		esferasD.Draw(staticShader);

		//Tira esferas 5
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(110.0f, 46.5f, 115.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		esferasD.Draw(staticShader);

		//Tira esferas 6
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(160.0f, 46.5f, 115.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		esferasD.Draw(staticShader);

		//Tira esferas 7
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(210.0f, 46.5f, 115.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		esferasD.Draw(staticShader);
		

		//Corona
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(160.0f, 10.5f, 109.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f,0.5f));
		staticShader.setMat4("model", model);
		corona.Draw(staticShader);

		//Corona Izq
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 10.0f, 52.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		corona.Draw(staticShader);

		//Carro Madera
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(210.0f, 0.5f, -50.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setMat4("model", model);
		carroM.Draw(staticShader);

		//Pila Regalos adentro
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(200.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		staticShader.setMat4("model", model);
		pilaRegalos.Draw(staticShader);

				
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
		model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.5f));
		staticShader.setMat4("model", model);
		techo.Draw(staticShader);

		//Techo Izquierdo Lounge
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-213.5f, 10.5f, 150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.045f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.5f));
		staticShader.setMat4("model", model);
		techoI.Draw(staticShader);

		//TechoPrinc
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-27.50f, -103.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.49f, 0.7f, 0.6f));
		staticShader.setMat4("model", model);
		techoP.Draw(staticShader);

		//TechoGar
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(365.50f, -103.0f, -483.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.7f, 0.25f));
		staticShader.setMat4("model", model);
		techoG.Draw(staticShader);

		//TechoIzqAlPrinc
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-135.0f, -103.0f, -290.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.7f, 0.37f));
		staticShader.setMat4("model", model);
		techoP.Draw(staticShader);
		
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
		
		// pared derecha kids room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-118.60f, 24.18f, -117.08f));
		model = glm::scale(model, glm::vec3(0.1f, 0.20f, 0.30f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared superior kids room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-188.25f, 24.18f, -227.45f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared izquierda bathroom
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-251.25f, 24.18f, -267.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.15f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared superior bathroom
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-259.39f, 24.18f, -304.6f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.1f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared superior bathroom 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-177.39f, 24.18f, -304.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.2f, 0.1f)); 
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared derecha bathroom 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-162.1f, 24.18f, -297.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.2f, 0.1f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared derecha bathroom 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-139.7f, 24.18f, -290.2f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared derecha bathroom 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-112.1f, 24.18f, -279.7f)); 
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.2f, 0.1f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		// pared derecha bathroom 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-162.1f, 24.18f, -279.9f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.2f, 0.1f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader);

		//puerta del closet parents room
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-137.9f, 24.62f, -265.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.104f, 0.202f, 0.1f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);

		//Pared trasera
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(230.2f, 25.0f, -232.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.205f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared izquierda garage
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(257.2f, 25.0f, -379.92f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.205f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared superior garage
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(330.0f, 25.0f, -531.92f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.205f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared derecha garage
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(406.95f, 25.0f, -382.5f));  
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6068f, 0.205f, 0.15f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//Pared cocina trave
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(84.0f, 24.32f, -99.82f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.234f, 0.2f, 0.1f));
		staticShader.setMat4("model", model);
		Pared2_text.Draw(staticShader); 

		//Puerta Frontal Burnace
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-21.5f, 0.0f, -219.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6325f, 0.74f));
		staticShader.setMat4("model", model);
		puertaI.Draw(staticShader);

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
		
		//----------------------------------------------------------------------------------------------
		//Muñeco de nieve
		//----------------------------------------------------------------------------------------------
		//Parte inferior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-270.0f, movY_bola1, 180.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		muneco1.Draw(staticShader);

		//Parte intermedia
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-270.0f, movY_bola2, 180.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		muneco2.Draw(staticShader);

		//Parte superior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-270.0f, movY_bola3, 180.0f));
		model = glm::rotate(model, glm::radians(rot_cabeza3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		muneco3.Draw(staticShader); 
		
		//----------------------------------------------------------------------------------------------
		//Trineo, Santa y Renos con modelado jerarquico
		//----------------------------------------------------------------------------------------------
		//Trineo, santa y renos sin piernas
		model = glm::mat4(1.0f);
		model = glm::translate(model, posicionTrineo);
		model = glm::rotate(model, glm::radians(-Trineo_aument2), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp_trineo = tmp_trineo2 = tmp_trineo3 = tmp_trineo4 = model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		trineos_renos_santa.Draw(staticShader); 
		
		//Pierna reno 1 delantera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo, glm::vec3(-4.33f, 5.3f, 24.0f));
		tmp_trineo = model = glm::rotate(model, glm::radians(rotpierna), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pierna reno 1 delantera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo, glm::vec3(2.9f, 0.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pierna reno 2 delantera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo, glm::vec3(6.33f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pierna reno 2 delantera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo, glm::vec3(9.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pierna reno 1 trasera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo2, glm::vec3(-4.20f, 5.3f, 20.0f));
		tmp_trineo2 = model = glm::rotate(model, glm::radians(-rotpierna), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pierna reno 1 trasera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo2, glm::vec3(2.9f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pierna reno 2 trasera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo2, glm::vec3(6.33f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pierna reno 2 trasera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo2, glm::vec3(9.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		pierna.Draw(staticShader);

		//Pata reno 1 tarsera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo3, glm::vec3(-4.30f, 2.4f, 20.0f));
		tmp_trineo3 = model = glm::rotate(model, glm::radians(rotpata), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);

		//Pata reno 1 trasera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo3, glm::vec3(3.1f, 0.0f, -0.04f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);

		//Pata reno 2 trasera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo3, glm::vec3(6.35f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);

		//Pata reno 2 trasera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo3, glm::vec3(9.4f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);

		//Pata reno 1 delantera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo4, glm::vec3(-4.45f, 2.4f, 24.0f));
		tmp_trineo4 = model = glm::rotate(model, glm::radians(-rotpata), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);

		//Pata reno 1 delantera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo4, glm::vec3(3.1f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);

		//Pata reno 2 delantera 1
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo4, glm::vec3(6.35f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);

		//Pata reno 2 delantera 2
		model = glm::mat4(1.0f);
		model = glm::translate(tmp_trineo4, glm::vec3(9.4f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata.Draw(staticShader);
		
				//Comedor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.4f, 0.0f, -85.0f)); 
		model = glm::scale(model, glm::vec3(1.8f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa_comedor.Draw(staticShader);

		//mesa interior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(160.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa_interior.Draw(staticShader); 

		//mesa exterior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(69.0f, 0.0f, -196.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa_exterior.Draw(staticShader);
		
		//cama dui y malcolm 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-145.0f, 0.0f, -117.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama_ninos.Draw(staticShader);

		//cama reese
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-265.0f, 0.0f, -117.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama_ninos.Draw(staticShader);

		//puerta  garage
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(332.5f, 26.58f, -232.30f)); 
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.97f, 1.0f, 1.7f));
		staticShader.setMat4("model", model);
		puerta_garage.Draw(staticShader);

		//carro
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(335.5f, 0.0f, -380.30f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(40.0f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);
		
		//estante cuarto kids
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-210.0f, 0.0f, -58.0f)); 
		model = glm::scale(model, glm::vec3(0.9f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		estante_cosas.Draw(staticShader);

		//estante con cajones cuarto kids 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-145.0f, 0.0f, -165.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		estante_cajones.Draw(staticShader);

		//estante con cajones cuarto kids 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-265.0f, 0.0f, -165.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		estante_cajones.Draw(staticShader);

		//Closet 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f, 0.0f, -270.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		closet.Draw(staticShader);

		//Cama padres
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -444.0f)); 
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama_padres.Draw(staticShader);

		//Sillon padres
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-108.0f, 0.0f, -455.0f)); 
		model = glm::scale(model, glm::vec3(0.6f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Sillon_flores.Draw(staticShader);

		//Sillon padres 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-258.0f, 0.0f, -455.0f));
		model = glm::scale(model, glm::vec3(0.6f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Sillon_flores.Draw(staticShader);

		//Estante cajones padres
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-135.0f, 0.0f, -297.5f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		estante_cajones.Draw(staticShader);

		//Estante cajones padres 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-288.0f, 0.0f, -420.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		staticShader.setMat4("model", model);
		estante_cajones.Draw(staticShader);
		
		//PC
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-269.6f, 0.0f, -316.0f));
		model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		PC.Draw(staticShader);

		//Estante cocina
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-83.0f, -9.4f, -60.0f));
		model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader); 
		
		// -------------------------------------------------------------------------------------------------------------------------
		//  Transparente 
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// puerta principal casa //(JERC)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-19.0f, 25.0f, 5.0f));
		model = glm::rotate(model, glm::radians(rotPuertaAnima), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.579f, 0.93f));
		staticShader.setMat4("model", model);
		puertaAnima.Draw(staticShader);
		
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
		
		//PuertaG
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(242.5f, 0.0f, -44.5f));
		model = glm::scale(model, glm::vec3(0.58f, 0.58f, 0.87f));
		staticShader.setMat4("model", model);
		puertaG.Draw(staticShader);

		//PuertaG2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(242.5f, 0.0f, 65.0f));
		model = glm::scale(model, glm::vec3(0.58f, 0.58f, 0.91f));
		staticShader.setMat4("model", model);
		puertaG.Draw(staticShader);


		//PuertaG3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 0.0f, -198.0f));
		model = glm::scale(model, glm::vec3(0.58f, 0.58f, 0.75f));
		staticShader.setMat4("model", model);
		puertaG.Draw(staticShader);

		//PuertaG3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-296.5f, 0.0f, -98.0f));
		model = glm::scale(model, glm::vec3(0.58f, 0.58f, 0.75f));
		staticShader.setMat4("model", model);
		puertaG.Draw(staticShader);

		//PuertaG4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3 (-71.5f, 0.0f, -354.5f));
		model = glm::scale(model, glm::vec3(0.58f, 0.58f, 0.745f));
		staticShader.setMat4("model", model);
		puertaG.Draw(staticShader);

		//PuertaG5
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-71.5f, 0.0f, -305.50f));
		model = glm::scale(model, glm::vec3(0.58f, 0.58f, 0.745f));
		staticShader.setMat4("model", model);
		puertaG.Draw(staticShader);
		
		//lounge 
		//sillon cafe lounge    
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-208.0f, 0.0f, 130.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.5f, 2.0f));
		staticShader.setMat4("model", model);
		Sillon_cafe.Draw(staticShader);

		//chimenea  
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-285.0f, 0.0f, 50.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		staticShader.setMat4("model", model);
		Chimenea.Draw(staticShader);

		//sillon flores     
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-205.0f, 0.0f, 45.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.8f, 1.8f));
		staticShader.setMat4("model", model);
		Sillon_flores.Draw(staticShader);

		//estante     
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -43.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		Estante_lounge.Draw(staticShader);

		//horno
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-59.5f, 0.3f, -138.6f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		horno.Draw(staticShader);

		//lavadora
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-52.2f, 0.0f, -185.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		lavadora.Draw(staticShader);


		//lavadora 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-52.2f, 0.0f, -215.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		lavadora.Draw(staticShader); 
		
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
	
	//tecla muñeco de nieve
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		animacion_muneco ^= true;
	
	//tecla muñeco de trineo
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
		animacion_trineo ^= true;
	
	//tecla PuertaAnima //(JERC)
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		animacion_puerta ^= true;

	//tecla estrella animacion //(JERC)
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
		animacion_estrella ^= true;
	
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		cabezaPos=cabezaPos + 0.25f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		cabezaPos = cabezaPos - 0.25f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		brazoRot=brazoRot+3;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		brazoRot = brazoRot - 3;
	
	
	//To play KeyFrame animation
if (key == GLFW_KEY_K && action == GLFW_PRESS)
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
