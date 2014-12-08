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
	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
	}
	for (int i = 0; i < _blocks.size(); i++)
	{
		delete _blocks[i];
	}
}

void World::init(Window * window)
{
	_window = window;
	
	globalProperties.shadow_maps_on = false;

	initValues();

	initLights();

	// initialize camera
	cam.init(0, 10, 20);

	// set camera movement characteristics
	cam.setEyeMove(1);
	cam.setLookAtMove(.7);

	// initialize shaders
	shader.init("Shaders/vertices.vert", "Shaders/fragments.frag");
	simpleShader.init("Shaders/simplified.vert", "Shaders/simplified.frag");
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

	ray.init();
}

void World::initLights()
{
	Color lightColor;
	lightColor.setColor(.5, .5, 1, 1);
	Color ambientColor;
	ambientColor.setColor(.0, .0, .0, 1);

	Light* directionalLight1 = new Light();
	lights.push_back(directionalLight1);

	// init light values
	lights.at(DIRECTIONAL_1)->setIsEnabled(true);
	lights.at(DIRECTIONAL_1)->setIsLocal(false);
	lights.at(DIRECTIONAL_1)->setIsSpot(false);
	lights.at(DIRECTIONAL_1)->setAmbient(vec3(ambientColor._red, ambientColor._green, ambientColor._blue));
	lights.at(DIRECTIONAL_1)->setColor(vec3(lightColor._red, lightColor._green, lightColor._blue));
	lights.at(DIRECTIONAL_1)->setPosition(vec3(-10, 10, 10));
	lights.at(DIRECTIONAL_1)->setHalfVector(vec3(0, 0, 0));
	lights.at(DIRECTIONAL_1)->setConeDirection(vec3(0, 0, -1));
	lights.at(DIRECTIONAL_1)->setSpotCosCutoff(.9);
	lights.at(DIRECTIONAL_1)->setSpotExponent(100);
	lights.at(DIRECTIONAL_1)->setConstantAttenuation(.05);
	lights.at(DIRECTIONAL_1)->setLinearAttenuation(.005);
	lights.at(DIRECTIONAL_1)->setQuadraticAttenuation(.005);
	lights.at(DIRECTIONAL_1)->setIsShadowMapEnabled(true);

	lightColor.setColor(1, .5, .5, 1);
	ambientColor.setColor(.0, .0, .0, 1);

	Light* directionalLight2 = new Light();
	lights.push_back(directionalLight2);

	// init light values
	lights.at(DIRECTIONAL_2)->setIsEnabled(true);
	lights.at(DIRECTIONAL_2)->setAmbient(vec3(ambientColor._red, ambientColor._green, ambientColor._blue));
	lights.at(DIRECTIONAL_2)->setColor(vec3(lightColor._red, lightColor._green, lightColor._blue));
	lights.at(DIRECTIONAL_2)->setPosition(vec3(10, 10, 10));
	lights.at(DIRECTIONAL_2)->setIsShadowMapEnabled(true);

	lightColor = { .5, 1, .5, 1 };
	ambientColor = { .0, .0, .0, 1 };

	Light* directionalLight3 = new Light();
	lights.push_back(directionalLight3);

	// init light values
	lights.at(DIRECTIONAL_3)->setIsEnabled(true);
	lights.at(DIRECTIONAL_3)->setAmbient(vec3(ambientColor._red, ambientColor._green, ambientColor._blue));
	lights.at(DIRECTIONAL_3)->setColor(vec3(lightColor._red, lightColor._green, lightColor._blue));
	lights.at(DIRECTIONAL_3)->setPosition(vec3(0, 10, -10));
	lights.at(DIRECTIONAL_3)->setIsShadowMapEnabled(true);

	ambientLight = vec3(.1, .1, .1);
}

void World::setupTextures()
{

	// Texture Files
	textureFilenames.push_back("Textures/MineCraft.png");
	textureFilenames.push_back("Textures/MineCraftGround.png");

	for (int i = 0; i < textureFilenames.size(); i++)
	{
		textures.push_back(new Texture());
		textures.back()->loadFromFile(textureFilenames[i]);
		textures.back()->load();
	}

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

	simpleDraw();

	
	
	
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
	glViewport(0, 0, _window->getWidth(), _window->getHeight());
	
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
	//cube.draw(in_shader);
	ground.draw(in_shader);
	
	for (int i = 0; i < _blocks.size(); i++)
	{
		_blocks[i]->draw(shader);
	}
}

void World::simpleDraw()
{
	simpleShader.use();
	cam.setUniforms(simpleShader);
	ray.draw();
	simpleShader.unuse();
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
		win_full_prev_width = _window->getWidth();
		win_full_prev_height = _window->getHeight();
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
	vec3 farPoint, nearPoint, origin, direction, intersection, rayWorld, N;

	vec4 rayClip, rayEye;
	float t;
	Block * block;

	switch (button)
	{
		//-------------------------------------------------------
		// Left Button ( Mod Terrain )
		//-------------------------------------------------------
	case GLUT_LEFT_BUTTON:
	
		rayClip = vec4(x, y, 1.0f, 1.0f);
		rayEye = cam.convertToEyeSpace(rayClip);
		rayEye = vec4(glm::vec2(rayEye), -1.0f, 0.0f);
		rayWorld = vec3(cam.convertToWorldSpace(rayEye));
		rayWorld = glm::normalize(rayWorld);
		origin = cam.getEyePosition();
		direction = rayWorld;


	

		ray.setStart(nearPoint);
		t = -origin.y / direction.y;

		cout << "t: " << t << endl;
		if (t != 0)
		{
			cout << "Ray intersected with plane." << endl;
			
			intersection = origin + t * direction;
			
			ray.setEnd(intersection);
			if (intersection.x < 200 && intersection.x > -200 && intersection.z < 200 && intersection.z > -200)
			{
				cout << "Block added" << endl;
				block = new Block();
				block->init("Models/mineCraftCube.obj");
				block->translate(intersection.x, intersection.y + 1.0f, intersection.z);
				block->setTexture(textures[0]);
				_blocks.push_back(block);
				
			}
			
		}

		cout << "Intersection: ( " << intersection.x << " , " << intersection.y << " , " << intersection.z << " )" << endl;
		cout << "Origin: ( " << origin.x << " , " << origin.y << " , " << origin.z << " )" << endl;
		cout << "Direction: ( " << direction.x << " , " << direction.y << " , " << direction.z << " )" << endl;



		

		break;

		//-------------------------------------------------------
		// Wheel Button ( Camera Panning )
		//-------------------------------------------------------
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
			pan_camera = true;			
		else			pan_camera = false;		
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
	_window->setWidth(w);
	_window->setHeight(h);

	cam.setFrustum(-.30, .30,
		map(0, 0, w, -.30, .30),
		map(h, 0, w, -.30, .30),
		.3, 10000);
}

void World::setupTerrain(int width, int height)
{

}