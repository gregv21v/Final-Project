/*
	Press 1,2,3:	Toggle Individual Lights
	Press 'l':		Toggle Lighting
	Press 'm':		Toggle Shadow Mapping
	Press 'q':		Fullscreen
	Press esc:		Exit Fullscreen

	Mouse:
		Left Button:		none
		Wheel Button:		Press and hold and drag to rotate view
		Right Button:		Press and hold and drag to move eye location
		Wheel Forward:		zoom in on axis of view
		Wheel Backwards:	zoom out on axis of view
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>

#include "LoadShaders.h"
#include "vgl.h"
#include "vmath.h"
#include "Object.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "ShadowMap.h"
#include "Terrain.h"

#define map(value,inLow,inHigh,outLow,outHigh) ((value - inLow) * (outHigh - outLow) / (inHigh - inLow) + outLow)

#define NUM_TEXTURES 2 


using glm::mat4;

static class World
{

public:

	World();
	~World();
	void init(int,int);							// initializes World
	void display();							// display function

	//--------------------------------------------
	// GLUT Functions
	//--------------------------------------------
	void keyPress(unsigned char, int, int);
	void arrowInput(int, int, int);
	void mouseFunc(int,int,float, float);
	void motionFunc(float, float);
	void passiveMotionFunc(float, float);
	void reshapeFunc(int, int);
	//--------------------------------------------

	void draw(Shader);
	void initValues();				// initializes values
	void initLights();				// initializes lights ( edit values in this function )
	void setupTextures();
	void renderShadowMaps();
	void setUniforms();

	void setupTerrain(int, int);

	void idleFunc();

private:

	//---------------------------------------
	// Shaders
	//---------------------------------------
	Shader shader;
	Shader shadowMapShader;
	//---------------------------------------

	//---------------------------------------
	// Lighting
	//---------------------------------------
	vec3 ambientLight;
	vector<Light*> lights;
	
	enum{ DIRECTIONAL_1, DIRECTIONAL_2, DIRECTIONAL_3, NUM_LIGHTS };
	//---------------------------------------

	//---------------------------------------
	// Textures
	//---------------------------------------
	std::string textureFilenames[NUM_TEXTURES];
	Texture* textures[NUM_TEXTURES];
	//---------------------------------------

	//---------------------------------------
	// Camera
	//---------------------------------------
	vector<Camera*> cams;
	bool pan_camera;
	bool move_camera;
	int current_camera;
	//---------------------------------------

	//---------------------------------------
	// Models
	//---------------------------------------
	Model cube;
	Model ground;
	//---------------------------------------

	//---------------------------------------
	// Global Properties
	//---------------------------------------
	struct GlobalProperties {
		bool lighting_on;
		bool shadow_maps_on;
	} globalProperties;
	//---------------------------------------

	//---------------------------------------
	// Mouse Movement
	//---------------------------------------
	float mouse_prev_x;
	float mouse_prev_y;
	//---------------------------------------

	//---------------------------------------
	// Window size
	//---------------------------------------
	int window_width;
	int window_height;
	int win_full_prev_width;
	int win_full_prev_height;
	//---------------------------------------

	//---------------------------------------
	// Terrain
	//---------------------------------------
	Terrain terrain;
	//---------------------------------------
};

