
#include "RenderManager.h"
#include "GameManager.h"

RenderManager::RenderManager(GameManager *gman){

	if (gman == NULL){

		exit(1);

	}

	gameManager = gman;

}

RenderManager::~RenderManager(){} //do nothing for now
