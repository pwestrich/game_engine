
#include "GameManager.h"
#include "RenderManager.h"
#include "AnimationRenderListener.h"

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

     	renderListener = new AnimationRenderListener(this);
      	root->addFrameListener(renderListener);

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

	//this also sometimes crashes the game on quit
	/*if (root){

		delete root;
		root = NULL;

	}*/

} 

//a ton of getter methods
size_t RenderManager::getRenderWindowHandle(){

	return windowHandle;

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

void RenderManager::processAnimations(const float timeStep){

   for (size_t i = 0; i < animationStates.size(); ++i){

      animationStates[i]->addTime(timeStep);

   }

}

void RenderManager::startRendering(){

	root->startRendering();

}

void RenderManager::stopRendering(){

   renderListener->stopRendering();

}

//stolen from Dr. Boshart
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

					std::string name = sceneElement->GetText();

					if (name == sceneName){

						clog << "Building scene: " << sceneName << endl;

						//cameras
						TiXmlNode *cameraTree = scenes->FirstChild("cameras");

						if (cameraTree){

							//build every camera
							for (TiXmlNode *cameraItem = cameraTree->FirstChild("camera"); cameraItem; cameraItem = cameraItem->NextSibling()){

								//name, lookAt, location, viewport
								TiXmlElement *cameraElement = (TiXmlElement*) cameraItem->FirstChild("name");
								string cameraName;

								if (cameraElement){

									cameraName = cameraElement->GetText();

								} else {

									cerr << "ERROR: Cameras must ahve names!" << endl;
									exit(EXIT_FAILURE);

								}

								Ogre::Camera *camera = sceneManager->createCamera(cameraName);
								float values[] = {0.0,0.0,0.0,0.0,0.0,0.0};

								cameraElement = (TiXmlElement*) cameraItem->FirstChild("location");

								if (cameraElement){

									string temp = cameraElement->GetText();
									parseFloats(temp, values);
									camera->setPosition(values[0], values[1], values[2]);

								} else {

									cerr << "ERROR: Cameras must have a position!" << endl;
									exit(EXIT_FAILURE);

								}

								cameraElement = (TiXmlElement*) cameraItem->FirstChild("lookAt");

								if (cameraElement){

									string temp = cameraElement->GetText();
									parseFloats(temp, values);
									camera->lookAt(values[0], values[1], values[2]);

								} else {

									cerr << "ERROR: Cameras must look somewhere!" << endl;
									exit(EXIT_FAILURE);

								}

								cameraElement = (TiXmlElement*) cameraItem->FirstChild("clip");

								if (cameraElement){

									int vals[] = {0,0};
									string temp = cameraElement->GetText();
									parseInts(temp, vals);

									camera->setNearClipDistance(vals[0]);
									camera->setFarClipDistance(vals[1]);

								} else {

									cerr << "ERROR: Camseas must ahve a clipping distance!" << endl;
									exit(EXIT_FAILURE);

								}

								cameraElement = (TiXmlElement*) cameraItem->FirstChild("viewport");

								if (cameraElement){

									string temp = cameraElement->GetText();
									parseFloats(temp, values);

									Ogre::Viewport *viewport = window->addViewport(camera, values[0], values[1], values[2], values[3], values[4]);

								} else {

									cerr << "WARNING: Camera " << cameraName << " has no viewport!" << endl;

								}

							}

						} else {

							cerr << "WARNING: Did you mean not to include any cameras in this scene?" << endl;

						}

						//lights
						TiXmlNode *lightTree = scenes->FirstChild("lights");

						if (lightTree){

							for (TiXmlNode *lightNode = lightTree->FirstChild(); lightNode; lightNode = lightNode->NextSibling()){

								//there are several types; check for them
								TiXmlElement *item = (TiXmlElement*) lightNode->FirstChild("type");

								if (!item) {

									cerr <<"ERROR: Every light must have a type!" << endl;
									continue;

								}

								string typeString = item->GetText();

								if (typeString == "ambient"){

									//this just has a color
									TiXmlElement *color = (TiXmlElement*) lightNode->FirstChild("color");

									if (!color){

										cerr << "ERROR: Ambient light must have a color!" << endl;
										continue;

									}

									float colors[3];
									parseFloats(string(color->GetText()), colors);
									sceneManager->setAmbientLight(Ogre::ColourValue(colors[0], colors[1], colors[2]));

								} else if (typeString == "directional"){

									TiXmlElement *name = (TiXmlElement*) lightNode->FirstChild("name");

									if (!name){

										cerr << "ERROR: Light sources must have names!" << endl;
										continue;

									}

									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_DIRECTIONAL);

									TiXmlElement *dColor = (TiXmlElement*) lightNode->FirstChild("diffuse");

									if (!dColor){

										cerr << "ERROR: Lights need a diffuse color!" << endl;
										continue;

									}

									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = (TiXmlElement*) lightNode->FirstChild("specular");

									if (!dColor){

										cerr << "ERROR: Lights must have a specular color!" << endl;
										continue;

									}

									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *direction = (TiXmlElement*) lightNode->FirstChild("direction");

									if (!direction){

										cerr << "ERROR: Directional lights need a direction!" << endl;
										continue;

									}

									parseFloats(string(direction->GetText()), colors);
									light->setDirection(Vector3(colors[0], colors[1], colors[2]));

								} else if (typeString == "point"){

									TiXmlElement *name = (TiXmlElement*) lightNode->FirstChild("name");

									if (!name){

										cerr << "ERROR: Light sources must have names!" << endl;
										continue;

									}

									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_POINT);

									TiXmlElement *dColor = (TiXmlElement*) lightNode->FirstChild("diffuse");

									if (!dColor){

										cerr << "ERROR: Lights need a diffuse color!" << endl;
										continue;

									}

									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = (TiXmlElement*) lightNode->FirstChild("specular");

									if (!dColor){

										cerr << "ERROR: Lights must have a specular color!" << endl;
										continue;

									}

									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *location = (TiXmlElement*) lightNode->FirstChild("location");

									if (!location){

										cerr << "ERROR: Point lights need a location!" << endl;
										continue;

									}

									parseFloats(string(location->GetText()), colors);
									light->setPosition(Vector3(colors[0], colors[1], colors[2]));

								} else if (typeString == "spot"){

									TiXmlElement *name = (TiXmlElement*) lightNode->FirstChild("name");

									if (!name){

										cerr << "ERROR: Light sources must have names!" << endl;
										continue;

									}

									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_SPOTLIGHT);

									TiXmlElement *dColor = (TiXmlElement*) lightNode->FirstChild("diffuse");

									if (!dColor){

										cerr << "ERROR: Lights need a diffuse color!" << endl;
										continue;

									}

									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = (TiXmlElement*) lightNode->FirstChild("specular");

									if (!dColor){

										cerr << "ERROR: Lights must have a specular color!" << endl;
										continue;

									}

									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *direction = (TiXmlElement*) lightNode->FirstChild("direction");

									if (!direction){

										cerr << "ERROR: Spotlights need a direction!" << endl;
										continue;

									}

									parseFloats(string(direction->GetText()), colors);
									light->setDirection(Vector3(colors[0], colors[1], colors[2]));

									TiXmlElement *location = (TiXmlElement*) lightNode->FirstChild("location");

									if (!location){

										cerr << "ERROR: Spotlights need a location!" << endl;
										continue;

									}

									parseFloats(string(location->GetText()), colors);
									light->setPosition(Vector3(colors[0], colors[1], colors[2]));

									TiXmlElement *range = (TiXmlElement*) lightNode->FirstChild("range");

									if (!range){

										cerr << "ERROR: Spotslights need a range!" << endl;
										continue;

									}

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
                  
						//nodes in the tree
						TiXmlNode *nodeTree = scenes->FirstChild("nodes");

						if (nodeTree){ //only do this if there are actually any nodes here

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

	cout << "Finished building scene." << endl;

}

void RenderManager::unloadResources(){

  if (groupLoaded == ""){

    return;

  }

  Ogre::ResourceGroupManager &resourceManager = Ogre::ResourceGroupManager::getSingleton();
  
  //for some reason, this crashes the game... I ahve no idea why
  //it works without it, so it's commented out.
  //resourceManager.destroyResourceGroup(groupLoaded);
  groupLoaded = "";

}

void RenderManager::buildSceneManually(){



}

//private methods below here ------------------------------------------------------------------------------------------------------

//recursive function for making all the scene nodes
void RenderManager::createNodes(Ogre::SceneNode *parent, TiXmlNode *nodeTree){

	//loop through every node in the tree, set its stuff, create its children, and attatch it to its parent
	for (TiXmlNode *nodeNode = nodeTree->FirstChild(); nodeNode; nodeNode = nodeNode->NextSibling()){

		TiXmlElement *nodeElement = (TiXmlElement*) nodeNode->FirstChild("name");

		if (!nodeElement){

			cerr << "ERROR: Nodes must have a name!" << endl;
			continue;

		}

		string nodeName = nodeElement->GetText();

		nodeElement = (TiXmlElement*) nodeNode->FirstChild("type");

		if (!nodeElement) {

			cerr << "ERROR: Nodes must have a type!" << endl;
			continue;

		}

		string nodeType = nodeElement->GetText();
		Ogre::SceneNode *sceneNode = sceneManager->createSceneNode(nodeName);

		if (nodeType == "animation"){

			createAnimation(sceneNode, nodeNode);

		} else if (nodeType == "scene"){

			//nodes can have a scale, translate, rotation, and/or entity
			TiXmlNode *entityTree = nodeNode->FirstChild("entity");

			if (entityTree){

				//entites have a name, mesh, and material
				string entityName, entityMesh, entityMaterial;
				nodeElement = (TiXmlElement*) entityTree->FirstChild("name");

				if (nodeElement){

					entityName = nodeElement->GetText();

				} else {

					cerr << "ERROR: Entities must have names!" << endl;
					exit(EXIT_FAILURE);

				}

				nodeElement = (TiXmlElement*) entityTree->FirstChild("mesh");

				if (entityTree){

					entityMesh = nodeElement->GetText();

				} else {

					cerr << "ERROR: Entities must have a mesh" << endl;
					exit(EXIT_FAILURE);

				}

				nodeElement = (TiXmlElement*) entityTree->FirstChild("material");

				if (nodeElement){

					entityMaterial = nodeElement->GetText();

				} else {

					cerr << "ERROR: Entities must have materials!" << endl;
					exit(EXIT_FAILURE);

				}

				Ogre::Entity *entity = sceneManager->createEntity(entityName, entityMesh);
				entity->setMaterialName(entityMaterial);

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

		TiXmlNode *childTree = nodeNode->FirstChild("children");

		if (childTree){

			createNodes(sceneNode, childTree);

		}

		parent->addChild(sceneNode);

	}

}

void RenderManager::createAnimation(Ogre::SceneNode *node, TiXmlNode *nodeTree){

   static int animationNumber = 0;

   TiXmlElement *nodeElement = (TiXmlElement*) nodeTree->FirstChild("length");
   int length = 0;

   if (nodeElement){

      length = atoi(nodeElement->GetText());

   } else {

      cerr << "ERROR: No length specified in animation!" << endl;
      exit(EXIT_FAILURE);

   }

   nodeElement = (TiXmlElement*) nodeTree->FirstChild("animationName");
   string animationName;

   if (nodeElement){

      animationName = nodeElement->GetText();

   } else {

      char temp[32];
      sprintf(temp, "%d", length);
      animationName = temp;

   }

   Ogre::Animation *animation = sceneManager->createAnimation(animationName, length);

   TiXmlNode *frameTree = nodeTree->FirstChild("keyFrames");

   if (frameTree){

      Ogre::NodeAnimationTrack *animationTrack = animation->createNodeTrack(1, node);

      for (TiXmlNode *keyFrameNode = frameTree->FirstChild(); keyFrameNode; keyFrameNode = keyFrameNode->NextSibling()){

         nodeElement = (TiXmlElement*) keyFrameNode->FirstChild("time");

         if (!nodeElement){

            cerr << "ERROR: Key frame has no time set!" << endl;
            exit(EXIT_FAILURE);

         }

         float time = atof(nodeElement->GetText());

         Ogre::TransformKeyFrame *keyFrame = animationTrack->createNodeKeyFrame(time);
         float values[4] = {0,0,0,0};

         nodeElement = (TiXmlElement*) keyFrameNode->FirstChild("translate");

         if (nodeElement){

            string transString = nodeElement->GetText();
            parseFloats(transString, values);
            keyFrame->setTranslate(Vector3(values[0], values[1], values[2]));

         }

         nodeElement = (TiXmlElement*) keyFrameNode->FirstChild("rotate");

         if (nodeElement){

            string transString = nodeElement->GetText();
            parseFloats(transString, values);
            keyFrame->setRotation(Quaternion(Degree(values[3]), Vector3(values[0], values[1], values[2])));

         }

         nodeElement = (TiXmlElement*) keyFrameNode->FirstChild("scale");

         if (nodeElement){

            string transString = nodeElement->GetText();
            parseFloats(transString, values);
            keyFrame->setScale(Vector3(values[0], values[1], values[2]));

         }

      }

      Ogre::AnimationState *animationState = sceneManager->createAnimationState(animationName);

      nodeElement = (TiXmlElement*) nodeTree->FirstChild("enabled");

      if (nodeElement){

         animationState->setEnabled((strcmp(nodeElement->GetText(), "true") == 0));

      } else {

         animationState->setEnabled(false);

      }

      nodeElement = (TiXmlElement*) nodeTree->FirstChild("loop");

      if (nodeElement){

         animationState->setLoop((strcmp(nodeElement->GetText(), "true") == 0));

      } else {

         animationState->setLoop(false);

      }

      animationStates.push_back(animationState);

   } else {

      cerr << "ERROR: No key frames specified for this animation node!" << endl;
      return;

   }

}
