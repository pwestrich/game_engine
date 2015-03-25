
#include <iostream>

#include "ResourceManager.h"
#include "GameManager.h"

#include "PathResource.h"
#include "MeshResource.h"
#include "AudioResource.h"

using namespace std;

ResourceManager::ResourceManager(GameManager *gm){

	gameManager = gm;
	groupLoaded = ""; //nothing loaded

}

ResourceManager::~ResourceManager(){

	//unload the resources if they aren't already
	unloadResources();

}

void ResourceManager::loadResourcesFromXML(const string &filename){

    TiXmlDocument document(filename.c_str());

    if (document.LoadFile()){

        TiXmlNode *groupsTree = document.FirstChild("groups");

        if (groupsTree){

            for (TiXmlNode *group = groupsTree->FirstChild(); group; group = group->NextSibling()){

                TiXmlElement *nameElement = static_cast<TiXmlElement*>(group->FirstChild("name"));
                string groupName = nameElement->GetText();

                //create this resource group
                vector<GameResource*> &thisGroup = resources[groupName];

                  TiXmlNode *pathTree = group->FirstChild("paths");

                  if (pathTree){

                        for (TiXmlNode *path = pathTree->FirstChild(); path; path = path->NextSibling()){

                            TiXmlElement *pathElement = static_cast<TiXmlElement*>(path->FirstChild("name"));
                            string pathText = pathElement->GetText();

                            pathElement = static_cast<TiXmlElement*>(path->FirstChild("id"));
                            uint32_t id = strtoul(pathElement->GetText(), NULL, 0);
                            
                            PathResource *item = new PathResource(id, groupName, pathText, gameManager);
                            thisGroup.push_back(item);

                        }

                  } else {

                        gameManager->logWarn("You have no paths set... Are you sure about that?");

                  }

                  TiXmlNode *meshTree = group->FirstChild("meshes");

                  if (meshTree){

                        for (TiXmlNode *mesh = meshTree->FirstChild(); mesh; mesh = mesh->NextSibling()){

                            TiXmlElement *meshElement = static_cast<TiXmlElement*>(mesh->FirstChild("name"));
                            string meshString = meshElement->GetText();

                            meshElement = static_cast<TiXmlElement*>(mesh->FirstChild("id"));
                            uint32_t id = strtoul(meshElement->GetText(), NULL, 0);

                            MeshResource *item = new MeshResource(id, groupName, meshString, gameManager);
                            thisGroup.push_back(item);

                        }

                  } else {

                        gameManager->logWarn("You don't have any meshes... Are you sure about that?");

                  }

                   TiXmlNode *audioTree = group->FirstChild("audios");

                  if (audioTree){

                        for (TiXmlNode *audio = audioTree->FirstChild(); audio; audio = audio->NextSibling()){

                            TiXmlElement *audioElement = static_cast<TiXmlElement*>(audio->FirstChild("file"));
                            string audioFile = audioElement->GetText();

                            audioElement = static_cast<TiXmlElement*>(audio->FirstChild("type"));
                            string audioType = audioElement->GetText();

                            audioElement = static_cast<TiXmlElement*>(audio->FirstChild("id"));
                            uint32_t id = strtoul(audioElement->GetText(), NULL, 0);

                            AudioResource *item = new AudioResource(id, groupName, audioFile, gameManager, gameManager->createAudioInfo(), audioType);
                            thisGroup.push_back(item);

                        }

                  } else {

                      gameManager->logInfo("No audio in this group.");

                  }


            	}

        } else {

            gameManager->logFatal("Error: Invalid resource file format: missing <groups>", __LINE__, __FILE__);

        }

    } else {

        gameManager->logFatal("Error: Resource file not found.", __LINE__, __FILE__);

    }

}

void ResourceManager::loadGroup(const string &groupName){

	if (groupLoaded == groupName) return;

	gameManager->logInfo("Loading resources...");

	vector<GameResource*> group = resources[groupName];

	cerr << group.size() << endl;

	for (size_t i = 0; i < group.size(); ++i){

		group[i]->load();

	}

	gameManager->logInfo("Initalizing Ogre resource group...");
	gameManager->initResourceGroup(groupName);

	gameManager->logInfo("Loading Ogre resources...");
	gameManager->loadResourceGroup(groupName);

	gameManager->logInfo("Resources loaded!");

	groupLoaded = groupName;

}

void ResourceManager::unloadResources(){

  if (groupLoaded == "") return;

  vector<GameResource*> &group = resources[groupLoaded];

  for (size_t i = 0; i < group.size(); ++i){

  		group[i]->unload();

  }

  gameManager->unloadResourceGroup(groupLoaded);

  groupLoaded = "";

}

GameResource *ResourceManager::getResourceByID(uint32_t id){

	vector<GameResource*> &group = resources[groupLoaded];

	for (size_t i = 0; i < group.size(); ++i){

		if (group[i]->getResourceID() == id){

			return group[i];

		}

	}

	return NULL;

}
