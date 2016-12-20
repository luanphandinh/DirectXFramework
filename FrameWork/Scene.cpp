#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

Viewport* Scene::getViewport()
{
	return _viewport;
}

Director* Scene::getDirector()
{
	return _director;
}

eID Scene::getIdLevel() {
	return _idLevel;
}
