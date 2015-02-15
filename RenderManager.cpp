
#include "RenderManager.h"
#include "GameManager.h"

#include <iostream>

using namespace Ogre;

//public methods start here ----------------------------------------------------------------------------------------------------------------------

RenderManager::RenderManager(GameManager *gman){

	if (gman == NULL){

		exit(1);

	}

	gameManager = gman;
	groupLoaded = "";

	//start creating ogre stuff
	root = NULL;
	window = NULL;
	sceneManager = NULL;
	camera = NULL;
	viewport = NULL;

	try {

		root = OGRE_NEW Ogre::Root("","");
		root->loadPlugin("RenderSystem_GL");

		Ogre::RenderSystem *renderer = root->getRenderSystemByName("OpenGL Rendering Subsystem");

		if (!renderer){

			cerr << "Error: OpenGL not avalible." << endl;
			exit(EXIT_FAILURE);

		}

		root->setRenderSystem(renderer);
		
		//set some options and make the window
		renderer->setConfigOption("Full Screen", "No");
        renderer->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
        sceneManager = root->createSceneManager(Ogre::ST_GENERIC, "DefaultSceneManager");
        window = root->initialise(true, "CSC 4903: Game Engine Programming");
        window->getCustomAttribute("WINDOW", &windowHandle);

	} catch (Ogre::Exception &it){

		cerr << "Exception while creating RenderManager: " << it.what() << endl;
		exit(EXIT_FAILURE);

	}

}

RenderManager::~RenderManager(){

	//stop rendering things
	stopRendering();

  unloadResources();

	//clear the scene
	sceneManager->destroyAllCameras();
	sceneManager->clearScene();

	window->removeAllViewports();
	window->destroy();

	delete root;

} 

//a ton of getter methods
size_t RenderManager::getRenderWindowHandle(){

	return windowHandle;

}

int RenderManager::getRenderWindowWidth(){

	return viewport->getActualWidth();

}

int RenderManager::getRenderWindowHeight(){

	return viewport->getActualHeight();

}

Ogre::RenderWindow *RenderManager::getRenderWindow(){

	return window;

}

Ogre::SceneManager *RenderManager::getSceneManager(){

	return sceneManager;

}

//the setter methods
void RenderManager::setTimeSinceLastFrame(Ogre::Real timeElapsed){

	frameTimeElapsed = timeElapsed;

}

//and actions
void RenderManager::startRendering(){

	root->startRendering();

}

void RenderManager::stopRendering(){



}

void RenderManager::loadResourcesFromXML(const std::string &filename, const std::string &group_name){

    //use tiny xml to parse an xml file with the ogre paths in it
   TiXmlDocument doc(filename.c_str());
   if (doc.LoadFile())
   {
      Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
      TiXmlNode* ogre_groups_tree = doc.FirstChild("ogre_groups");
      if (ogre_groups_tree)
      {
         //Enumerate group objects (eventually, child will be false and loop will terminate)
         for(TiXmlNode* child = ogre_groups_tree->FirstChild(); child; child = child->NextSibling())
         {
            TiXmlElement* group_element = child->ToElement();
            if(group_element)
            {
               TiXmlElement* name_element = (TiXmlElement*) group_element->FirstChild("name");
               std::string name_text = name_element->GetText();

               //continue with this section if it matches the requested section
               if (name_text == group_name)
               {
                  TiXmlNode* paths_tree = group_element->FirstChild("paths");
                  if (paths_tree)
                  {
                     //Enumerate path objects
                     for(TiXmlNode* child = paths_tree->FirstChild(); child; child = child->NextSibling())
                     {
                        TiXmlElement* path_element = (TiXmlElement*) child->ToElement();
                        std::string path_text = path_element->GetText();

                        //FileSystem or Zip
                        //Ogre will look for scripts in these directories
                        rgm.addResourceLocation(path_text, "FileSystem", group_name);
                     }
                  }

                  TiXmlNode* meshes_tree = group_element->FirstChild("meshes");
                  if (meshes_tree)
                  {
                     //Enumerate path objects
                     for(TiXmlNode* child = meshes_tree->FirstChild(); child; child = child->NextSibling())
                     {
                        TiXmlElement* mesh_element = (TiXmlElement*) child->ToElement();
                        std::string mesh_text = mesh_element->GetText();

                        //Ogre will look for meshes in the paths defined above
                        rgm.declareResource(mesh_text, "Mesh", group_name);  //so that the mesh is loaded when its resource group is loaded
                     }
                  }

                  //scripts loaded and resources are created, but not loaded
                  //use load/unload resource group to manage resource memory footprint
                  rgm.initialiseResourceGroup(group_name);  //pre-load the resources located in the specific paths (parse scripts)
                  rgm.loadResourceGroup(group_name, true, true);  //load the resources in the specific paths
 
                  groupLoaded = group_name;

               }  //end if
            }
         }
      }
   }
   else 
   {
      cerr << "ERROR: Resource file not found: " << filename << endl;
   }

}

void RenderManager::buildSceneFromXML(const std::string &filename, const string &sceneName){

	//first, try to open the file
	TiXmlDocument file(filename.c_str());

	if (file.LoadFile()){

		//the first tag should be <scene>
		TiXmlNode *sceneTree = file.FirstChild("scenes");

		if (sceneTree){

			//loop through the scenes, looking for our scene
			for (TiXmlNode *scenes = sceneTree->FirstChild(); scenes; scenes = scenes->NextSibling()){

				TiXmlElement *sceneElement = (TiXmlElement*) scenes->FirstChild("name");

				if (sceneElement){

					const char *charName = sceneElement->GetText();

					std::string name = sceneElement->GetText();

					if (name == sceneName){

						//this is our scene, start building it
						//first are cameras
						clog << "Building scene: " << sceneName << endl;
						TiXmlNode *cameraTree = scenes->FirstChild("cameras");

						if (cameraTree){

							for (TiXmlNode *cameraNode = cameraTree->FirstChild(); cameraNode; cameraNode = cameraNode->NextSibling()){

								//get the camera's name
								TiXmlElement *item = (TiXmlElement*) cameraNode->FirstChild("name");
								camera = sceneManager->createCamera(item->GetText());

								clog << "Creating camera: " << item->GetText() << endl;

								//then its location
								item = (TiXmlElement*) cameraNode->FirstChild("location");
								float location[3];
								parseFloats(string(item->GetText()), location);
								camera->setPosition(Vector3(location[0], location[1], location[2]));

								//and where it looks at
								item = (TiXmlElement*) cameraNode->FirstChild("lookAt");
								parseFloats(string(item->GetText()), location);
								camera->setPosition(Vector3(location[0], location[1], location[2]));

								//and near and far clip
								item = (TiXmlElement*) cameraNode->FirstChild("nearClip");
								camera->setNearClipDistance(atof(item->GetText()));

								item = (TiXmlElement*) cameraNode->FirstChild("farClip");
								camera->setFarClipDistance(atof(item->GetText()));

							}

						} else {

							cerr << "WARNING: Did you mean to not include any cameras in this scene?" << endl;

						}

						//next are lights
						TiXmlNode *lightTree = scenes->FirstChild("lights");

						if (lightTree){

							for (TiXmlNode *lightNode = lightTree->FirstChild(); lightNode; lightNode = lightNode->NextSibling()){

								//there are several types; check for them
								TiXmlElement *item = (TiXmlElement*) lightNode->FirstChild("type");
								string typeString = item->GetText();

								clog << "Creating light: ";

								if (typeString == "ambient"){

									clog << "ambient" << endl;

									//this just ahs a color
									TiXmlElement *color = (TiXmlElement*) lightNode->FirstChild("color");
									float colors[3];
									parseFloats(string(color->GetText()), colors);
									sceneManager->setAmbientLight(Ogre::ColourValue(colors[0], colors[1], colors[2]));

								} else if (typeString == "directional"){

									TiXmlElement *name = (TiXmlElement*) lightNode->FirstChild("name");
									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_DIRECTIONAL);

									clog << name->GetText() << endl;

									TiXmlElement *dColor = (TiXmlElement*) lightNode->FirstChild("diffuse");
									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = (TiXmlElement*) lightNode->FirstChild("specular");
									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *direction = (TiXmlElement*) lightNode->FirstChild("direction");
									parseFloats(string(direction->GetText()), colors);
									light->setDirection(Vector3(colors[0], colors[1], colors[2]));

								} else if (typeString == "point"){

									TiXmlElement *name = (TiXmlElement*) lightNode->FirstChild("name");
									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_POINT);

									clog << name->GetText() << endl;

									TiXmlElement *dColor = (TiXmlElement*) lightNode->FirstChild("diffuse");
									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = (TiXmlElement*) lightNode->FirstChild("specular");
									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *location = (TiXmlElement*) lightNode->FirstChild("location");
									parseFloats(string(location->GetText()), colors);
									light->setPosition(Vector3(colors[0], colors[1], colors[2]));

								} else if (typeString == "spot"){

									TiXmlElement *name = (TiXmlElement*) lightNode->FirstChild("name");
									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_SPOTLIGHT);

									clog << name->GetText() << endl;

									TiXmlElement *dColor = (TiXmlElement*) lightNode->FirstChild("diffuse");
									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = (TiXmlElement*) lightNode->FirstChild("specular");
									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *direction = (TiXmlElement*) lightNode->FirstChild("direction");
									parseFloats(string(direction->GetText()), colors);
									light->setDirection(Vector3(colors[0], colors[1], colors[2]));

									TiXmlElement *location = (TiXmlElement*) lightNode->FirstChild("location");
									parseFloats(string(location->GetText()), colors);
									light->setPosition(Vector3(colors[0], colors[1], colors[2]));

									TiXmlElement *range = (TiXmlElement*) lightNode->FirstChild("range");
									parseFloats(string(range->GetText()), colors);
									light->setSpotlightRange(Ogre::Degree(colors[0]), Ogre::Degree(colors[1]));

								} else {

									cerr << "ERROR: Invalid light type: " << item->GetText() << endl;
									continue;

								}

							}

						} else {

							cerr << "WARNING: It's very dark in here. Think you should turn on a light?" << endl;

						}

						//then viewports
						TiXmlNode *viewTree = scenes->FirstChild("viewports");

						for (TiXmlNode *viewportNode = viewTree->FirstChild(); viewportNode; viewportNode = viewportNode->NextSibling()){

							clog << "Creating viewport..." << endl;

							TiXmlElement *cameraNameItem = (TiXmlElement*) viewportNode->FirstChild("camera");
							string cameraName = cameraNameItem->GetText();

							Ogre::Camera *camera = sceneManager->getCamera(cameraName);
							Ogre::Viewport *viewport = window->addViewport(camera, 0, 0, 0, 1.0, 1.0);

							camera->setAspectRatio(viewport->getActualWidth() / viewport->getActualHeight());

							TiXmlElement *color = (TiXmlElement*) viewportNode->FirstChild("color");
							float colors[3];
							parseFloats(string(color->GetText()), colors);
							viewport->setBackgroundColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

						}

						//entities
						TiXmlNode *entityTree = scenes->FirstChild("entities");

						if (entityTree){

							//loop through every entity and create them
							for (TiXmlNode *entityNode = entityTree->FirstChild(); entityNode; entityNode = entityNode->NextSibling()){

								TiXmlElement *entityElement = (TiXmlElement*) entityNode->FirstChild("name");
								string entityName = entityElement->GetText();

								entityElement = (TiXmlElement*) entityNode->FirstChild("mesh");
								string entityMesh = entityElement->GetText();

								entityElement = (TiXmlElement*) entityNode->FirstChild("material");
								string entityMaterial = entityElement->GetText();

								Ogre::Entity *entity = sceneManager->createEntity(entityName, entityMesh);
								entity->setMaterialName(entityMaterial);

							}

						} else {

							cerr << "WARNING: There aren't any entites in the scene... This is going to be a pretty boring video game." << endl;

						}

						//nodes in the tree
						TiXmlNode *nodeTree = scenes->FirstChild("nodes");

						if (nodeTree){

							//process all the nodes by calling the recursive function
							createNodes(sceneManager->getRootSceneNode(), nodeTree);

						} else {

							cerr << "WARNING: There aren't any nodes in this scene... This is going to be a boring video game..." << endl;

						}

					}

				} else {

					cerr << "Error parsing XML document: " << filename << endl;
					exit(EXIT_FAILURE);

				}

			}

		} else {

			cerr << "Error: Invalid configuration format." << endl;
			exit(EXIT_FAILURE);

		}

	} else {

		cerr << "Error: Invalid scene file: " << filename << endl;
		exit(EXIT_FAILURE);

	}

}

void RenderManager::unloadResources(){

  if (groupLoaded == ""){

    return;

  }

  Ogre::ResourceGroupManager &resourceManager = Ogre::ResourceGroupManager::getSingleton();
  resourceManager.destroyResourceGroup(groupLoaded);
  groupLoaded = "";

}

void RenderManager::buildSceneManually(){}

//private methods below here ------------------------------------------------------------------------------------------------------

//recursive function for making all the scene nodes
void RenderManager::createNodes(Ogre::SceneNode *parent, TiXmlNode *nodeTree){

	//loop through every node in the tree, set its stuff, create its children, and attatch it to its parent
	for (TiXmlNode *nodeNode = nodeTree->FirstChild(); nodeNode; nodeNode = nodeNode->NextSibling()){

		TiXmlElement *nodeElement = (TiXmlElement*) nodeNode->FirstChild("name");
		string nodeName = nodeElement->GetText();

		nodeElement = (TiXmlElement*) nodeNode->FirstChild("type");
		string nodeType = nodeElement->GetText();

		Ogre::SceneNode *sceneNode = sceneManager->createSceneNode(nodeName);

		if (nodeType == "animation"){

			//do nothing for now

		} else if (nodeType == "scene"){

			//nodes can have a scale, translate, rotation, and/or entity
			nodeElement = (TiXmlElement*) nodeNode->FirstChild("entity");

			if (nodeElement){

				string entityName = nodeElement->GetText();
				Ogre::Entity *entity = sceneManager->getEntity(entityName);
				sceneNode->attachObject(entity);

			}

			nodeElement = (TiXmlElement*) nodeNode->FirstChild("scale");

			if (nodeElement){

				string scaleString = nodeElement->GetText();
				float values[3];
				parseFloats(scaleString, values);
				sceneNode->scale(Vector3(values[0], values[1], values[2]));

			}

			nodeElement = (TiXmlElement*) nodeNode->FirstChild("translate");

			if (nodeElement){

				string transString = nodeElement->GetText();
				float values[3];
				parseFloats(transString, values);
				sceneNode->translate(Vector3(values[0], values[1], values[2]));

			}

			nodeElement = (TiXmlElement*) nodeNode->FirstChild("rotate");

			if (nodeElement){

				string rotateString = nodeElement->GetText();
				float values[4];
				parseFloats(rotateString, values);
				sceneNode->rotate(Quaternion(Degree(values[3]), Vector3(values[0], values[1], values[2])));

			}

		} else {

			cerr << "ERROR: Invalid node type " << nodeType << " for node " << nodeName << "." << endl;
			continue;
		}

		TiXmlNode *childTree = nodeTree->FirstChild("children");

		if (childTree){

			for (TiXmlNode *child = childTree->FirstChild(); child; child = child->NextSibling()){

				createNodes(sceneNode, child);

			}

		}

		parent->addChild(sceneNode);

	}

}
