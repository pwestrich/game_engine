
#include "ResourceManager.h"
#include "GameManager.h"

#include <iostream>

ResourceManager::ResourceManager(GameManager *gm){

	gameManager = gm;
	groupLoaded = ""; //nothing loaded

}

ResourceManager::~ResourceManager(){

	//unload the resources if they aren't already
	unloadResources();

}

//loads resources from the given XML files
//stolen from Dr. Boshart (still)
/*void ResourceManager::loadResourcesFromXML(const std::string &filename, const std::string &group_name){

  //use tiny xml to parse an xml file with the ogre paths in it
  TiXmlDocument doc(filename.c_str());
  if (doc.LoadFile()){

    TiXmlNode* ogre_groups_tree = doc.FirstChild("ogre_groups");
    if (ogre_groups_tree){

      //Enumerate group objects (eventually, child will be false and loop will terminate)
      for(TiXmlNode* child = ogre_groups_tree->FirstChild(); child; child = child->NextSibling()){

        TiXmlElement* group_element = child->ToElement();
        if(group_element){

          TiXmlElement* name_element = (TiXmlElement*) group_element->FirstChild("name");
          std::string name_text = name_element->GetText();

          //continue with this section if it matches the requested section
          if (name_text == group_name){

            TiXmlNode* paths_tree = group_element->FirstChild("paths");
            if (paths_tree){

              //Enumerate path objects
              for(TiXmlNode* child = paths_tree->FirstChild(); child; child = child->NextSibling()){

                TiXmlElement* path_element = (TiXmlElement*) child->ToElement();
                std::string path_text = path_element->GetText();

                gameManager->addPathResource(path_text, "FileSystem", group_name);
              }

            }

            TiXmlNode* meshes_tree = group_element->FirstChild("meshes");
            if (meshes_tree){

              //Enumerate path objects
              for(TiXmlNode* child = meshes_tree->FirstChild(); child; child = child->NextSibling()){

                TiXmlElement* mesh_element = (TiXmlElement*) child->ToElement();
                std::string mesh_text = mesh_element->GetText();

                gameManager->addMeshResource(mesh_text, "Mesh", group_name);
              }

            }

            //scripts loaded and resources are created, but not loaded
            //use load/unload resource group to manage resource memory footprint
            gameManager->initResourceGroup(group_name);
            gameManager->loadResourceGroup(group_name);  //load the resources in the specific paths
 
            groupLoaded = group_name;

          }

        }

      }

    }

  } else {

    cerr << "ERROR: Resource file not found: " << filename << endl;

  }

}*/

void ResourceManager::loadResourcesFromXML(const string &filename, const string &group_name){

    TiXmlDocument document(filename.c_str());

    if (document.LoadFile()){

        TiXmlNode *groupsTree = document.FirstChild("groups");

        if (groupsTree){

            for (TiXmlNode *group = groupsTree->FirstChild(); group; group = group->NextSibling()){

                TiXmlElement *nameElement = static_cast<TiXmlElement*>(group->FirstChild("name"));

                if (group_name == nameElement->GetText()){

                    TiXmlNode *pathTree = group->FirstChild("paths");

                    if (pathTree){

                        for (TiXmlNode *path = pathTree->FirstChild(); path; path = path->NextSibling()){

                            TiXmlElement *pathElement = static_cast<TiXmlElement*>(path->ToElement());

                            string pathText = pathElement->GetText();
                            gameManager->addPathResource(pathText, "FileSystem", group_name);

                        }

                    } else {

                        gameManager->logWarn("You have no paths set... Are you sure about that?");

                    }

                    TiXmlNode *meshTree = group->FirstChild("meshes");

                    if (meshTree){

                        for (TiXmlNode *mesh = meshTree->FirstChild(); mesh; mesh = mesh->NextSibling()){

                            TiXmlElement *meshElement = static_cast<TiXmlElement*>(mesh->ToElement());

                            string meshString = meshElement->GetText();
                            gameManager->addMeshResource(meshString, "Mesh", group_name);

                        }

                    } else {

                        gameManager->logWarn("You don't have any meshes... Are you sure about that?");

                    }

                    TiXmlNode *audioTree = group->FirstChild("audio");

                    if (audioTree){

                        for (TiXmlNode *audio = audioTree->FirstChild(); audio; audio = audio->NextSibling()){

                            TiXmlElement *audioElement = static_cast<TiXmlElement*>(audio->FirstChild("file"));
                            string audioFile = audioElement->GetText();

                            audioElement = static_cast<TiXmlElement*>(audio->FirstChild("type"));
                            string audioType = audioElement->GetText();

                            sounds.push_back(gameManager->createAudioInfo());

                            if (audioType == "stream"){

                                gameManager->loadAudioStream(audioFile, sounds[sounds.size() - 1]);

                            } else if (audioType == "sample"){

                                gameManager->loadAudioSample(audioFile, sounds[sounds.size() - 1]);

                            } else {

                                gameManager->logFatal("Error: Invalid audio type.", __LINE__, __FILE__);

                            }

                        }

                    } else {

                        gameManager->logInfo("No audio in this group.");

                    }

                }

            }

        } else {

            gameManager->logFatal("Error: Invalid resource file format: missing <groups>", __LINE__, __FILE__);

        }

    } else {

        gameManager->logFatal("Error: Resource file not found.", __LINE__, __FILE__);

    }

    gameManager->initResourceGroup(group_name);
    gameManager->loadResourceGroup(group_name);  //load the resources in the specific paths
    groupLoaded = group_name;

}

void ResourceManager::unloadResources(){

  if (groupLoaded == ""){

    return;

  }

  gameManager->unloadResourceGroup(groupLoaded);

  groupLoaded = "";

}
