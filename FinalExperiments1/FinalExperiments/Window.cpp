#include "Window.h"


Window::Window()
{
}


Window::~Window()
{
}


glm::vec2 Window::normalizeTo(glm::vec2 vect)
{
	return glm::vec2(2 * vect.x / _width - 1.0f, 1.0f - 2 * vect.y / _height);
}


void Window::init(int width, int height)
{
	_width = width;
	_height = height;

	glutInitWindowSize(_width, _height);
	glutInitWindowPosition(0, 0);
}

int Window::getWidth()
{
	return _width;
}

int Window::getHeight()
{
	return _height;
}

void Window::setWidth(int width)
{
	_width = width;
}

void Window::setHeight(int height)
{
	_height = height;
}
