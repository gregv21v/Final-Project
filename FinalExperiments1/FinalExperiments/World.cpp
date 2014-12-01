#include "World.h"

World::World()
{
	srand(time(NULL));

	globalProperties.lighting_on = true;
	globalProperties.shadow_maps_on = true;

	mouse_prev_x = 0;
	mouse_prev_y = 0;

	pan_camera = false;
}

World::~World()
{
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		delete textures[i];
	}
}

void World::init(int in_width,int in_height)
{
	window_width = in_width;
	window_height = in_height;

	initValues();

	initLights();

	// initialize camera
	cam.init(0, 10, 20);

	// set camera movement characteristics
	cam.setEyeMove(1);
	cam.setLookAtMove(.7);

	// initialize shaders
	shader.init("Shaders/vertices.vert", "Shaders/fragments.frag");
	shadowMapShader.init("Shaders/shadowMap.vert", "Shaders/shadowMap.frag");

	// Antialiasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	// line width
	glLineWidth(3);

	// set for 3d culling
	glEnable(GL_DEPTH_TEST);

	// setup textures
	setupTextures();

	glClearColor(0, 0, 0, 0);
}

void World::initValues()
{
	cube.init("Models/mineCraftCube.obj");
	cube.translate(0, 1, 0);

	ground.init("Models/mineCraftGround.obj");
	ground.scale(200);
}

void World::initLights()
{
	Color lightColor = { .5, .5, 1, 1 };
	Color ambientColor = { .0, .0, .0, 1 };

	Light* directionalLight1 = new Light();
	lights.push_back(directionalLight1);

	// init light values
	lights.at(DIRECTIONAL_1)->setIsEnabled(true);
	lights.at(DIRECTIONAL_1)->setIsLocal(false);
	lights.at(DIRECTIONAL_1)->setIsSpot(false);
	lights.at(DIRECTIONAL_1)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(DIRECTIONAL_1)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(DIRECTIONAL_1)->setPosition(vec3(-10, 10, 10));
	lights.at(DIRECTIONAL_1)->setHalfVector(vec3(0, 0, 0));
	lights.at(DIRECTIONAL_1)->setConeDirection(vec3(0, 0, -1));
	lights.at(DIRECTIONAL_1)->setSpotCosCutoff(.9);
	lights.at(DIRECTIONAL_1)->setSpotExponent(100);
	lights.at(DIRECTIONAL_1)->setConstantAttenuation(.05);
	lights.at(DIRECTIONAL_1)->setLinearAttenuation(.005);
	lights.at(DIRECTIONAL_1)->setQuadraticAttenuation(.005);
	lights.at(DIRECTIONAL_1)->setIsShadowMapEnabled(true);

	lightColor = { 1, .5, .5, 1 };
	ambientColor = { .0, .0, .0, 1 };

	Light* directionalLight2 = new Light();
	lights.push_back(directionalLight2);

	// init light values
	lights.at(DIRECTIONAL_2)->setIsEnabled(true);
	lights.at(DIRECTIONAL_2)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(DIRECTIONAL_2)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(DIRECTIONAL_2)->setPosition(vec3(10, 10, 10));
	lights.at(DIRECTIONAL_2)->setIsShadowMapEnabled(true);

	lightColor = { .5, 1, .5, 1 };
	ambientColor = { .0, .0, .0, 1 };

	Light* directionalLight3 = new Light();
	lights.push_back(directionalLight3);

	// init light values
	lights.at(DIRECTIONAL_3)->setIsEnabled(true);
	lights.at(DIRECTIONAL_3)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(DIRECTIONAL_3)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(DIRECTIONAL_3)->setPosition(vec3(0, 10, -10));
	lights.at(DIRECTIONAL_3)->setIsShadowMapEnabled(true);

	ambientLight = vec3(.1, .1, .1);
}

void World::setupTextures()
{

	// Texture Files
	textureFilenames[0] = "Textures/MineCraft.png";
	textureFilenames[1] = "Textures/MineCraftGround.png";

	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		textures[i] = new Texture();
		textures[i]->loadFromFile(textureFilenames[i]);
	}
	textures[0]->load();
	textures[1]->load();
	cube.setTexture(textures[0]);
	ground.setTexture(textures[1]);
}

void World::display()
{
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	renderShadowMaps();

	// use main shader
	shader.use();

	setUniforms();
	draw(shader);

	shader.unuse();
	
	// swap the buffers at the end of the display sequence
	glutSwapBuffers();
}

void World::renderShadowMaps()
{
	shadowMapShader.use();
	for (int i = 0; i < lights.size(); i++)
	{
		lights.at(i)->mapShadows(shadowMapShader);
		draw(shadowMapShader);
		lights.at(i)->endMapShadows();
	}
	shadowMapShader.unuse();
}

void World::setUniforms()
{
	glViewport(0, 0, window_width, window_height);
	
	// setup global uniforms
	glUniform3f(shader.getUniformLocation("EyeDirection"), 0, 0, 0);
	glUniform3f(shader.getUniformLocation("Ambient"), ambientLight.x, ambientLight.y, ambientLight.z);
	glUniform1f(shader.getUniformLocation("Strength"), .5);
	glUniform1f(shader.getUniformLocation("Shininess"), 10);
	glUniform1i(shader.getUniformLocation("LightingOn"), globalProperties.lighting_on);
	glUniform1i(shader.getUniformLocation("ShadowsOn"), globalProperties.shadow_maps_on);

	// setup lighting uniforms
	for (int i = 0; i < lights.size(); i++)
	{
		lights.at(i)->setUniforms(shader, i);
	}

	// setup camera uniforms
	cam.setUniforms(shader);
}

void World::draw(Shader in_shader)
{
	cube.draw(in_shader);
	ground.draw(in_shader);
}

void World::keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	//--------------------------------------------------------------------------------------
	// Individual Light Toggle
	//--------------------------------------------------------------------------------------
	case '1':
		lights.at(DIRECTIONAL_1)->setIsEnabled(!lights.at(DIRECTIONAL_1)->getIsEnabled());
		break;
	case '2':
		lights.at(DIRECTIONAL_2)->setIsEnabled(!lights.at(DIRECTIONAL_2)->getIsEnabled());
		break;
	case '3':
		lights.at(DIRECTIONAL_3)->setIsEnabled(!lights.at(DIRECTIONAL_3)->getIsEnabled());
		break;
	//--------------------------------------------------------------------------------------
	// Lighting and Shadow Maps Toggle
	//--------------------------------------------------------------------------------------
	case 'l':	
		globalProperties.lighting_on = !globalProperties.lighting_on;
		break;
	case 'm':	
		globalProperties.shadow_maps_on = !globalProperties.shadow_maps_on;
		break;
	//--------------------------------------------------------------------------------------
	// Movement Keyboard
	//--------------------------------------------------------------------------------------
	case 'w':
		cam.setEyeMove(.5);
		cam.camUp();
		break;
	case 'a':
		cam.setEyeMove(.5);
		cam.camLeft();
		break;
	case 's':
		cam.setEyeMove(.25);
		cam.camDown();
		break;
	case 'd':
		cam.setEyeMove(.5);
		cam.camRight();
		break;
	//--------------------------------------------------------------------------------------
	// Screen Shaping
	//--------------------------------------------------------------------------------------
	case 27:
		glutReshapeWindow(win_full_prev_width, win_full_prev_height);
		break;
	case 'q':
		win_full_prev_width = window_width;
		win_full_prev_height = window_height;
		glutFullScreen();
	//--------------------------------------------------------------------------------------
	default:
		break;
	}

	glutPostRedisplay();
}

void World::arrowInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		cam.camLookUp();
		break;
	case GLUT_KEY_DOWN:
		cam.camLookDown();
		break;
	case GLUT_KEY_LEFT:
		cam.camLookLeft();
		break;
	case GLUT_KEY_RIGHT:
		cam.camLookRight();
		break;
	}

	glutPostRedisplay();
}

void World::idleFunc()
{

}

void World::mouseFunc(int button, int state, float x, float y)
{
	switch (button)
	{
		//-------------------------------------------------------
		// Left Button ( Mod Terrain )
		//-------------------------------------------------------
	case 0:
		break;
		//-------------------------------------------------------
		// Wheel Button ( Camera Panning )
		//-------------------------------------------------------
	case 1:
		if (state == GLUT_DOWN)
			pan_camera = true;			
		else
			pan_camera = false;		
		break;
		//-------------------------------------------------------
		// Right Button ( Camera Movement xz-plane )
		//-------------------------------------------------------
	case 2:
		if (state == GLUT_DOWN)
			move_camera = true;				
		else
			move_camera = false;		
		break;
		//-------------------------------------------------------
		// Wheel Forward ( Zoom In )
		//-------------------------------------------------------
	case 3:
		cam.setEyeMove(1);
		if (state == GLUT_UP) return;
		cam.zoomIn();
		//cout << "( " << cam.getEyePosition().x << " , " << cam.getEyePosition().y << " , " << cam.getEyePosition().z << " )\n";
		break;
		//-------------------------------------------------------
		// Wheel Back ( Zoom Out )
		//-------------------------------------------------------
	case 4:
		cam.setEyeMove(1);
		if (state == GLUT_UP) return;
		cam.zoomOut();
		//cout << "( " << cam.getEyePosition().x << " , " << cam.getEyePosition().y << " , " << cam.getEyePosition().z << " )\n";
		break;
	default:
		break;
	}

   glutPostRedisplay();
}

void World::motionFunc(float x, float y)
{
	if (pan_camera)
	{
		vec2 move_direction = vec2(x - mouse_prev_x, y - mouse_prev_y);

		cam.setLookAtMove(abs(move_direction.x) * 75);

		if (move_direction.x > 0)
			cam.camLookRight();
		if (move_direction.x < 0)
			cam.camLookLeft();

		cam.setLookAtMove(abs(move_direction.y) * 75);

		if (move_direction.y > 0)
			cam.camLookUp();
		if (move_direction.y < 0)
			cam.camLookDown();

		mouse_prev_x = x;
		mouse_prev_y = y;
	}

	if (move_camera)
	{
		vec2 move_direction = vec2(x - mouse_prev_x, y - mouse_prev_y);

		cam.setEyeMove(abs(move_direction.x) * 50);

		if (move_direction.x > 0)
			cam.camRight();
		if (move_direction.x < 0)
			cam.camLeft();

		cam.setEyeMove(abs(move_direction.y) * 50);

		if (move_direction.y > 0)
			cam.camIn();
		if (move_direction.y < 0)
			cam.camOut();

		mouse_prev_x = x;
		mouse_prev_y = y;
	}

	//cout << "( " << cam.getEyePosition().x << " , " << cam.getEyePosition().y << " , " << cam.getEyePosition().z << " )\n";

	glutPostRedisplay();
}

void World::passiveMotionFunc(float x, float y)
{
	mouse_prev_x = x;
	mouse_prev_y = y;
}

void World::reshapeFunc(int w, int h)
{
	window_width = w;
	window_height = h;

	cam.setFrustum(-.30, .30,
		map(0, 0, w, -.30, .30),
		map(h, 0, w, -.30, .30),
		.3, 10000);
}

void World::setupTerrain(int width, int height)
{

}