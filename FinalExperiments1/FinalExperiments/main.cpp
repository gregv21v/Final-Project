//------------------------------------------------------------------------------------
// Experimentation
// 11/24/14
// Description: Blocks
//------------------------------------------------------------------------------------


#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>

#include "LoadShaders.h"
#include "World.h"

// this is a useful compiler directive for mapping a value in a specified range to another range
// i.e. if value = 300, inLow = 0, inHigh = 300, outLow = -1.0, outHight = 1.0, would be the value 0.0
#define map(value,inLow,inHigh,outLow,outHigh) ((value - inLow) * (outHigh - outLow) / (inHigh - inLow) + outLow)

// window width and height
#define WIN_WIDTH 900
#define WIN_HEIGHT 900

using std::cerr;
using std::endl;
using std::cout;

World world;

void display()
{
	world.display();
}

void keyPress(unsigned char key, int x, int y)
{
	world.keyPress(key, x, y);
}

void specialFunc(int key, int x, int y)
{
	world.arrowInput(key, x, y);
}

void idleFunc()
{
	world.idleFunc();
}

void mouseFunc(int button,int state,int x, int y)
{
	world.mouseFunc(button, state, x, y);
}

void motionFunc(int x, int y)
{
	world.motionFunc(x, y);
}

void passiveMotionFunc(int x, int y)
{
	world.passiveMotionFunc(x, y);
}

void reshapeFunc(int w, int h)
{
	world.reshapeFunc(w, h);
}

int main(int argc, char* argv[])
{
	

	// setup display/version information
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Experiment");
	
	glewExperimental = GL_TRUE;

	//error check
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	
	// initialize the world
	world.init(WIN_WIDTH,WIN_HEIGHT);

	// set up the display and keyboard functions
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutSpecialFunc(specialFunc);
	glutIdleFunc(idleFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutReshapeFunc(reshapeFunc);

	//glut main loop
	glutMainLoop();

	return 0;
}

