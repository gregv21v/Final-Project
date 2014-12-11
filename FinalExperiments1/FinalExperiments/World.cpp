#include "World.h"

World::World() : moundY(1), activeTool(NONE)
{
	srand(time(NULL));

	globalProperties.lighting_on = true;
	globalProperties.shadow_maps_on = false;

	mouse_prev_x = 0;
	mouse_prev_y = 0;

	pan_camera = false;

	current_camera = 0;
}

World::~World()
{
	
}

void World::init(int in_width,int in_height)
{
	window_width = in_width;
	window_height = in_height;

	initValues();

	initLights();

	// initialize cameras
	initCameras();

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

	setupTerrain();

	sky.init("Models/sky.obj");
	sky.scale(500);
	sky.setTexture(_texManager.get("Sky"));
}

void World::initValues()
{
	cube.init("Models/mineCraftCube.obj");
	cube.translate(0, 1, 0);

	ground.init("Models/mineCraftGround.obj");
	ground.scale(200);

	water.init("Models/water.obj");
	//water.translate(50, -5, 50);
	//water.scale(200);
}

void World::initLights()
{
	Color lightColor = { 1, 1, 1, 1 };
	Color ambientColor = { .0, .0, .0, 1 };

	Light* directionalLight1 = new Light();
	lights.push_back(directionalLight1);

	// init light values
	lights.at(DIRECTIONAL_1)->setIsEnabled(true);
	lights.at(DIRECTIONAL_1)->setIsLocal(false);
	lights.at(DIRECTIONAL_1)->setIsSpot(false);
	lights.at(DIRECTIONAL_1)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(DIRECTIONAL_1)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(DIRECTIONAL_1)->setPosition(vec3(1000, 1000, -1000));
	lights.at(DIRECTIONAL_1)->setHalfVector(vec3(0, 0, 0));
	lights.at(DIRECTIONAL_1)->setConeDirection(vec3(0, 0, -1));
	lights.at(DIRECTIONAL_1)->setSpotCosCutoff(.9);
	lights.at(DIRECTIONAL_1)->setSpotExponent(100);
	lights.at(DIRECTIONAL_1)->setConstantAttenuation(.05);
	lights.at(DIRECTIONAL_1)->setLinearAttenuation(.005);
	lights.at(DIRECTIONAL_1)->setQuadraticAttenuation(.005);
	lights.at(DIRECTIONAL_1)->setIsShadowMapEnabled(false);

	
	lightColor.red = 1;
	lightColor.green = 1;
	lightColor.blue = 1;
	lightColor.alpha = 1;

	ambientColor.red = 0;
	ambientColor.green = 0;
	ambientColor.blue = 0;
	ambientColor.alpha = 1;

	Light* directionalLight2 = new Light();
	lights.push_back(directionalLight2);

	// init light values
	lights.at(DIRECTIONAL_2)->setIsEnabled(true);
	lights.at(DIRECTIONAL_2)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(DIRECTIONAL_2)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(DIRECTIONAL_2)->setPosition(vec3(-1000, 1000, 1000));
	lights.at(DIRECTIONAL_2)->setIsShadowMapEnabled(false);

	lightColor.red = 1;
	lightColor.green = 1;
	lightColor.blue = 1;
	lightColor.alpha = 1;

	ambientColor.red = 0;
	ambientColor.green = 0;
	ambientColor.blue = 0;
	ambientColor.alpha = 1;

	Light* directionalLight3 = new Light();
	lights.push_back(directionalLight3);

	// init light values
	lights.at(DIRECTIONAL_3)->setIsEnabled(true);
	lights.at(DIRECTIONAL_3)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(DIRECTIONAL_3)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(DIRECTIONAL_3)->setPosition(vec3(1000, 1000, 1000));
	lights.at(DIRECTIONAL_3)->setIsShadowMapEnabled(false);

	ambientLight = vec3(.5, .5, .5);
}

void World::initCameras()
{
	glGenFramebuffers(NUM_FBS, FBs);
	glGenRenderbuffers(NUM_RBS, RBs);

	updateRenderBufferSize();

	initMainCam();
	initOverheadCam();
}

void World::initMainCam()
{
	Camera* temp1 = new Camera();
	cams.push_back(temp1);

	cams.at(MAIN_CAM)->init(terrain.getWidth() / 2, 20, terrain.getHeight() / 2);	
}

void World::initOverheadCam()
{
	Camera* tempOverhead = new Camera();
	cams.push_back(tempOverhead);

	cams.at(OVERHEAD)->init(0,0,0);
	cams.at(OVERHEAD)->setIsOrtho(true);
	cams.at(OVERHEAD)->setOrtho(-.5 * terrain.getWidth(), .5 * terrain.getWidth() - 1, -.5 * terrain.getHeight(), .5 * terrain.getHeight() - 1, 0, 10000);
	cams.at(OVERHEAD)->setView(
		
		glm::rotate(glm::mat4(), 90.0f, vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(), 180.0f, vec3(0, 1, 0)) *
		glm::translate(glm::mat4(), vec3(-.5 * terrain.getWidth(), -10, -.5 * terrain.getHeight())));
}

void World::updateRenderBufferSize()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBs[MAIN_CAM_FB]);

	glBindRenderbuffer(GL_RENDERBUFFER, RBs[MAIN_CAM_COLOR_RB]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, window_width, window_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_RENDERBUFFER, RBs[MAIN_CAM_COLOR_RB]);

	glBindRenderbuffer(GL_RENDERBUFFER, RBs[MAIN_CAM_DEPTH_RB]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, window_width, window_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, RBs[MAIN_CAM_DEPTH_RB]);



	glBindFramebuffer(GL_FRAMEBUFFER, FBs[OH_CAM_FB]);

	glBindRenderbuffer(GL_RENDERBUFFER, RBs[OH_CAM_COLOR_RB]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, window_width, window_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_RENDERBUFFER, RBs[OH_CAM_COLOR_RB]);

	glBindRenderbuffer(GL_RENDERBUFFER, RBs[OH_CAM_DEPTH_RB]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, window_width, window_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, RBs[OH_CAM_DEPTH_RB]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void World::setupTextures()
{
	// TODO: These should be moved to a text files
	_texManager.add("Textures/Grass.png", "Grass");
	_texManager.add("Textures/Grass.png", "Rock");
	_texManager.add("Textures/Snow.png", "Snow");
	_texManager.add("Textures/Sky.png", "Sky");

	_texManager.loadAll();
}

void World::display()
{
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// if shadow maps enabled, render shadow maps
	if (globalProperties.shadow_maps_on)
		renderShadowMaps();

	// use main shader and render cameras
	shader.use();
	renderOverheadCamera();
	renderMainCamera();
	shader.unuse();

	// combine the framebuffers
	assembleFramebuffers();

	// swap the buffers at the end of the display sequence
	glutSwapBuffers();

}

void World::assembleFramebuffers()
{
	int minimum_value = window_width / 40;

	if (window_height / 40 < minimum_value)
	{
		minimum_value = window_height / 40;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBs[MAIN_CAM_FB]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
		0,
		0,
		window_width,
		window_height,
		0,
		0,
		window_width,
		window_height,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBs[OH_CAM_FB]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
		0,
		0,
		window_width,
		window_height,
		minimum_value * 9,
		window_height - minimum_value,
		minimum_value,
		window_height - minimum_value * 9,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);
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
	glUniform1f(shader.getUniformLocation("Strength"), .1);
	glUniform1f(shader.getUniformLocation("Shininess"), 100);
	glUniform1i(shader.getUniformLocation("LightingOn"), globalProperties.lighting_on);
	glUniform1i(shader.getUniformLocation("ShadowsOn"), globalProperties.shadow_maps_on);
	
	_texManager.get("Grass")->activate(shader.getUniformLocation("tex[0]"), 0);
	_texManager.get("Rock")->activate(shader.getUniformLocation("tex[1]"), 1);
	_texManager.get("Snow")->activate(shader.getUniformLocation("tex[2]"), 2);
	terrain.setupUniforms(shader);

	// setup lighting uniforms
	for (int i = 0; i < lights.size(); i++)
	{
		lights.at(i)->setUniforms(shader, i);
	}

	// setup camera uniforms
	cams.at(current_camera)->setUniforms(shader);
}

void World::renderMainCamera()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBs[MAIN_CAM_FB]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	current_camera = MAIN_CAM;

	setUniforms();
	draw(shader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void World::renderOverheadCamera()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBs[OH_CAM_FB]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	current_camera = OVERHEAD;

	setUniforms();
	draw(shader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void World::draw(Shader in_shader)
{
	//water.draw(in_shader);
	
	glUniform1i(in_shader.getUniformLocation("IsTerrain"), 1);
	terrain.draw(in_shader);
	//glUniform1i(in_shader.getUniformLocation("IsTerrain"), 0);
	//sky.draw(in_shader);

	water.draw(in_shader);

}

void World::keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	//--------------------------------------------------------------------------------------
	// Terrain Manipulation
	//--------------------------------------------------------------------------------------
	case 'z':
		//terrain.mound(target[0], target[1], 1);
		moundY = 1;
		break;
	case 'x':
		//terrain.mound(target[0], target[1], -1);
		moundY = -1;
		break;
	case 'u':
		target[1] ++;
		break;
	case 'j':
		target[1] --;
		break;
	case 'h':
		target[0] --;
		break;
	case 'k':
		target[0] ++;
		break;
	case 'b':
		terrain.increaseSize();
		break;
	case 'n':
		terrain.decreaseSize();
		break;
	case 'i':
		terrain.increaseRoundness();
		break;
	case 'o':
		terrain.decreaseRoundness();
		break;
	//--------------------------------------------------------------------------------------
	case 'c':
		//current_camera = (current_camera + 1) % cams.size();
		break;
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
		cams.at(current_camera)->setEyeMove(.5);
		cams.at(current_camera)->camUp();
		break;
	case 'a':
		cams.at(current_camera)->setEyeMove(.5);
		cams.at(current_camera)->camLeft();
		break;
	case 's':
		cams.at(current_camera)->setEyeMove(.25);
		cams.at(current_camera)->camDown();
		break;
	case 'd':
		cams.at(current_camera)->setEyeMove(.5);
		cams.at(current_camera)->camRight();
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
	// Save
	//--------------------------------------------------------------------------------------
	case 'p':
		readPixelColor(x,y);
		break;
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
		cams.at(current_camera)->camLookUp();
		break;
	case GLUT_KEY_DOWN:
		cams.at(current_camera)->camLookDown();
		break;
	case GLUT_KEY_LEFT:
		cams.at(current_camera)->camLookLeft();
		break;
	case GLUT_KEY_RIGHT:
		cams.at(current_camera)->camLookRight();
		break;
	}

	glutPostRedisplay();
}

void World::idleFunc()
{

}

void World::mouseFunc(int button, int state, float x, float y)
{
	Ray ray;
	vec3 intersection;

	switch (button)
	{
		//-------------------------------------------------------
		// Left Button ( Mod Terrain )
		//-------------------------------------------------------
	case GLUT_LEFT_BUTTON:

		// Mouse Picking
		if (state == GLUT_DOWN)
		{
			ray.fromMouse(x, y, cams[current_camera]);
			if (ray.isCollidingWithPlane(0, 300, 0, 300))
			{
				intersection = ray.getIntersection();
				terrain.mound(intersection.x, intersection.z, moundY);
			}
			activeTool = MOUND;
		}
		else 
		{
			activeTool = NONE;
		}
			
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
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			move_camera = true;				
		else
			move_camera = false;		
		break;
		//-------------------------------------------------------
		// Wheel Forward ( Zoom In )
		//-------------------------------------------------------
	case 3:
		cams.at(current_camera)->setEyeMove(1);
		if (state == GLUT_UP) return;
		cams.at(current_camera)->zoomIn();
		//cout << "( " << cam.getEyePosition().x << " , " << cam.getEyePosition().y << " , " << cam.getEyePosition().z << " )\n";
		break;
		//-------------------------------------------------------
		// Wheel Back ( Zoom Out )
		//-------------------------------------------------------
	case 4:
		cams.at(current_camera)->setEyeMove(1);
		if (state == GLUT_UP) return;
		cams.at(current_camera)->zoomOut();
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

		cams.at(current_camera)->setLookAtMove(abs(move_direction.x) * 75);

		if (move_direction.x > 0)
			cams.at(current_camera)->camLookRight();
		if (move_direction.x < 0)
			cams.at(current_camera)->camLookLeft();

		cams.at(current_camera)->setLookAtMove(abs(move_direction.y) * 75);

		if (move_direction.y > 0)
			cams.at(current_camera)->camLookUp();
		if (move_direction.y < 0)
			cams.at(current_camera)->camLookDown();

		mouse_prev_x = x;
		mouse_prev_y = y;
	}

	if (move_camera)
	{
		vec2 move_direction = vec2(x - mouse_prev_x, y - mouse_prev_y);

		cams.at(current_camera)->setEyeMove(abs(move_direction.x) * 50);

		if (move_direction.x > 0)
			cams.at(current_camera)->camRight();
		if (move_direction.x < 0)
			cams.at(current_camera)->camLeft();

		cams.at(current_camera)->setEyeMove(abs(move_direction.y) * 50);

		if (move_direction.y > 0)
			cams.at(current_camera)->camIn();
		if (move_direction.y < 0)
			cams.at(current_camera)->camOut();

		mouse_prev_x = x;
		mouse_prev_y = y;
	}
	// Mouse Picking
	if (activeTool == MOUND)
	{
		Ray ray;
		vec3 intersection;
		ray.fromMouse(x, y, cams[current_camera]);
		if (ray.isCollidingWithPlane(0, 300, 0, 300))
		{
			intersection = ray.getIntersection();
			terrain.mound(intersection.x, intersection.z, moundY/2.0f);
		}
		activeTool = MOUND;
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

	cams.at(MAIN_CAM)->setFrustum(-1.0, 1.0,
		map(0, 0, w, -1.0, 1.0),
		map(h, 0, w, -1.0, 1.0),
		1.0, 10000);

	updateRenderBufferSize();

	glutPostRedisplay();
}

void World::setupTerrain()
{
	terrain.init();

	target[0] = terrain.getWidth()/2;
	target[1] = terrain.getHeight()/2;

	terrain.setTileFactor(5);
	terrain.setSize(15);

	terrain.setIsTextured(true);

	//water.init(terrain.getWidth() / 2, terrain.getHeight() / 2);
}

void World::saveImage()
{
	/*
	GLubyte image[MAX_IMAGE_WIDTH][MAX_IMAGE_HEIGHT];

	glReadPixels(0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, &image);

	vector<unsigned char> png;

	for (int i = 0; i < MAX_IMAGE_WIDTH; i++)
	{
		for (int j = 0; j < MAX_IMAGE_HEIGHT; j++)
		{
			png.push_back(image[i][j]);
			cout << "transfer\n";
		}
	}

	cout << "Encode\n";
	lodepng::encode("testOut.png", png, window_width, window_height);

	*/

}

void World::readPixelColor(int x, int y)
{
	GLubyte colors[3];

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glReadPixels(x, window_height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, colors);

	int temp[3];

	temp[0] = colors[0];
	temp[1] = colors[1];
	temp[2] = colors[2];

	cout << "R: " << temp[0] << endl;
	cout << "G: " << temp[1] << endl;
	cout << "B: " << temp[2] << endl;

	vec4 position = vec4(
		map(x, 0, window_width, -1.0, 1.0),
		map(y, 0, window_width, -1.0, 1.0), 
		0, 1.0) * glm::inverse(cams.at(MAIN_CAM)->getFrustum());

	cout << "( " << position.x << " , " << position.y << " , " << position.z << " )\n";
}