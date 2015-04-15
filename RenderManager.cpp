
#include "GameManager.h"
#include "RenderManager.h"
#include "RenderListener.h"
#include "GUIManager.h"
#include "PhysicsManager.h"
#include "BulletConvexHullCreator.h"

using namespace Ogre;

//public methods start here ----------------------------------------------------------------------------------------------------------------------
RenderManager::RenderManager(GameManager *gman){

	assert(gman != NULL);

	gameManager = gman;
	gameManager->logInfo("Creating RenderManager...");

	//start creating ogre stuff
	root 		 = NULL;
	window 		 = NULL;
	sceneManager = NULL;

	try {

		root = OGRE_NEW Ogre::Root("","");
		root->loadPlugin("RenderSystem_GL");

		Ogre::RenderSystem *renderer = root->getRenderSystemByName("OpenGL Rendering Subsystem");

		if (!renderer){

			gameManager->logFatal("Error: OpenGL not avalible.", __LINE__, __FILE__);

		}

		root->setRenderSystem(renderer);
		
		//set some options and make the window
		renderer->setConfigOption("Full Screen", "No");
     	renderer->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
     	sceneManager = root->createSceneManager(Ogre::ST_GENERIC, "DefaultSceneManager");
     	window = root->initialise(true, "CSC 4903: Game Engine Programming");
    	window->getCustomAttribute("WINDOW", &windowHandle);

    	//set a render listener
     	renderListener = new RenderListener(this);
      	root->addFrameListener(renderListener);

      	//create the submanagers
      	guiManager 		= new GUIManager(this);
      	physicsManager 	= new PhysicsManager(this);

	} catch (Ogre::Exception &it){

		gameManager->logFatal(it.what(), __LINE__, __FILE__);

	} catch (...){

		gameManager->logFatal("Error while creating RenderManager", __LINE__, __FILE__);

	}

	gameManager->logInfo("RenderManager initialized successfully!");

}

//the destructor will stop rendering, unload any loaded resources, close the window.
RenderManager::~RenderManager(){

	//stop rendering things
	stopRendering();

	guiManager->unloadResourceGroup();
	delete guiManager;
	delete physicsManager;

	sceneManager->destroyAllCameras();
	sceneManager->clearScene();

	gameManager->unloadResources();
	
	window->removeAllViewports();
	window->destroy();

	//this also sometimes crashes the game on quit
	/*if (root){

		delete root;

	}*/

} 

//the getter methods are pretty self-explanitory
size_t RenderManager::getRenderWindowHandle(){

	return windowHandle;

}

size_t RenderManager::getWindowWidth(){

	return window->getWidth();

}

size_t RenderManager::getWindowHeight(){

	return window->getHeight();

}

Ogre::RenderWindow *RenderManager::getRenderWindow(){

	return window;

}

Ogre::SceneManager *RenderManager::getSceneManager(){

	return sceneManager;

}

//methods to alter the scene
void RenderManager::rotateNode(const string &nodeName, const float w, const float x, const float y, const float z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		node->rotate(Quaternion(Degree(w), Vector3(x, y, z)));

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());

	}

}

void RenderManager::translateNode(const string &nodeName, const float x, const float y, const float z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		node->translate(Vector3(x, y, z));

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());

	}

}

void RenderManager::scaleNode(const string &nodeName, const float x, const float y, const float z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		node->scale(Vector3(x, y, z));

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());

	}

}

void RenderManager::setRotation(const string &nodeName, const float w, const float x, const float y, const float z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		node->setOrientation(Quaternion(Degree(w), Vector3(x, y, z)));

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());

	}

}

void RenderManager::setTranslation(const string &nodeName, const float x, const float y, const float z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		node->setPosition(Vector3(x, y, z));

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());

	}

}

void RenderManager::setScale(const string &nodeName, const float x, const float y, const float z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		node->setScale(Vector3(x, y, z));

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());

	}

}

bool RenderManager::getRotation(const string &nodeName, float &w, float &x, float &y, float &z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		Quaternion q = node->getOrientation();

		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
		return true;

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());
		return false;

	}

}

bool RenderManager::getTranslation(const string &nodeName, float &x, float &y, float &z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		Vector3 p = node->getPosition();

		x = p.x;
		y = p.y;
		z = p.z;
		return true;

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());
		return false;

	}

}

bool RenderManager::getScale(const string &nodeName, float &x, float &y, float &z){

	try {

		SceneNode *node = sceneManager->getSceneNode(nodeName);
		Vector3 p = node->getScale();

		x = p.x;
		y = p.y;
		z = p.z;
		return true;

	} catch (Ogre::Exception &it){

		gameManager->logWarn(it.what());
		return false;

	}

}

void RenderManager::applyTorque(const string &name, const float x, const float y, const float z){

	physicsManager->applyTorque(name, x, y, z);

}

void RenderManager::applyForce(const string &name, const float x, const float y, const float z){

	physicsManager->applyForce(name, x, y, z);

}

void RenderManager::setLinearVelocity(const string &name, const float x, const float y, const float z){

	physicsManager->setLinearVelocity(name, x, y, z);

}

void RenderManager::setAngularVelocity(const string &name, const float x, const float y, const float z){

	physicsManager->setAngularVelocity(name, x, y, z);

}

//methods to make a manual object -----------------------------------------------------------------
void RenderManager::createManualObject(const string &name, const string &materialName){

	Ogre::ManualObject *manualObject = sceneManager->createManualObject(name);
	manualObject->setDynamic(true);

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getDefaultSettings()->clone(materialName);
	material->setReceiveShadows(false);
	material->getTechnique(0)->setLightingEnabled(false);

	SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(manualObject);

}

void RenderManager::clearManualObject(const string &name){

	SceneNode *objectNode = sceneManager->getSceneNode(name);
	ManualObject *object = static_cast<ManualObject*>(objectNode->getAttachedObject(name));

	if (object){

		object->clear();

	} else {

		gameManager->logWarn("No manual object named " + name);

	}

}

void RenderManager::drawLine(const string &name, const string &material, const float x, const float y, const float z, 
	const float dx, const float dy, const float dz, const float r, const float g, const float b){

	ColourValue color(r, g, b);
	Vector3 start(x, y, z);
	Vector3 end(dx, dy, dz);

	SceneNode *objectNode = sceneManager->getSceneNode(name);
	ManualObject *object = static_cast<ManualObject*>(objectNode->getAttachedObject(name));

	if (object){

		object->begin(material, Ogre::RenderOperation::OT_LINE_LIST);
		object->position(end);
		object->colour(color);
		object->position(start);
		object->colour(color);
		object->end();

	} else {

		gameManager->logWarn("No manual object named " + name);

	}

}

void RenderManager::addPathResource(const string &path, const string &pathType, const string &group){

	Ogre::ResourceGroupManager &rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.addResourceLocation(path, pathType, group);

}

void RenderManager::addMeshResource(const string &mesh, const string &type, const string &group){

	Ogre::ResourceGroupManager &rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.declareResource(mesh, type, group);

}

void RenderManager::initResourceGroup(const string &group){

	Ogre::ResourceGroupManager &rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.initialiseResourceGroup(group);

}

void RenderManager::loadResourceGroup(const string &group){

	Ogre::ResourceGroupManager &rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.loadResourceGroup(group, true, true);

}

void RenderManager::unloadResourceGroup(const string &group){

	Ogre::ResourceGroupManager &resourceManager = Ogre::ResourceGroupManager::getSingleton();
  
 	//for some reason, this crashes the game... I have no idea why
 	//it works without it, so it's commented out.
 	//resourceManager.destroyResourceGroup(group);

}

//this function updates every animation every frame
void RenderManager::processAnimations(const float timeStep){

   for (size_t i = 0; i < animationStates.size(); ++i){

      animationStates[i]->addTime(timeStep);

   }

}

//this method tells the game manager to check for input
void RenderManager::checkForInput(const float timeStep){

	gameManager->checkForInput(timeStep);

}

void RenderManager::updatePhysics(const float timeStep){

	physicsManager->updatePhysics(timeStep);

}

void RenderManager::updateAudio(const float timeStep){

	gameManager->updateAudio(timeStep);

}

void RenderManager::playAudioByID(const uint32_t id, const int numRepeats){

	gameManager->playAudioByID(id, numRepeats);

}

void RenderManager::logInfo(const string &message){

	gameManager->logInfo(message);

}
void RenderManager::logWarn(const string &message){

	gameManager->logWarn(message);

}
void RenderManager::logDebug(const string &message){

	gameManager->logDebug(message);

}

void RenderManager::logFatal(const string &message, const int line, const char *file){

	gameManager->logFatal(message, line, file);

}

//methods to to script stuff (also goes to GameMAnager)
void RenderManager::execute(const string &filename){

	gameManager->execute(filename);

}

void RenderManager::writeInt(const string &name, const int value){

	gameManager->writeInt(name, value);

}

void RenderManager::writeFloat(const string &name, const float value){

	gameManager->writeFloat(name, value);

}

void RenderManager::writeString(const string &name, const string &value){

	gameManager->writeString(name, value);

}

void RenderManager::startRendering(){

	gameManager->logInfo("Starting renderer...");
	root->startRendering();

}

void RenderManager::stopRendering(){

	if (renderListener && renderListener->getRenderStatus()){

   		renderListener->stopRendering();
   	}

}

//methods to alter the scene based on input -------------------------------------------------------
void RenderManager::mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy){

	guiManager->mouseMoved(x, y, dx, dy);

}

void RenderManager::mousePressed(const uint32_t x, const uint32_t y, const MouseButton button){

	guiManager->mousePressed(x, y, button);

}

void RenderManager::mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button){

	guiManager->mouseReleased(x, y, button);

}

//not used
void RenderManager::keyPressed(const KeyboardKey key){}
void RenderManager::keyReleased(const KeyboardKey key){}
void RenderManager::joystickAxisMoved(const JoystickAxis axis, const uint32_t amount) {}
void RenderManager::joystickButtonPressed(const JoystickButton button){}

//this massive function will build a scene from XML given its filename and a group
void RenderManager::buildSceneFromXML(const std::string &filename, const string &sceneName){

	gameManager->logInfo("Building scene...");

	//first, try to open the file
	TiXmlDocument file(filename.c_str());

	if (file.LoadFile()){

		//the first tag should be <scene>
		TiXmlNode *sceneTree = file.FirstChild("scenes");

		if (sceneTree){

			//loop through the scenes, looking for our scene
			for (TiXmlNode *scenes = sceneTree->FirstChild(); scenes; scenes = scenes->NextSibling()){

				TiXmlElement *sceneElement = static_cast<TiXmlElement*>(scenes->FirstChild("name"));

				if (sceneElement){

					std::string name = sceneElement->GetText();

					if (name == sceneName){

						//cameras
						TiXmlNode *cameraTree = scenes->FirstChild("cameras");

						if (cameraTree){

							//build every camera
							for (TiXmlNode *cameraItem = cameraTree->FirstChild("camera"); cameraItem; cameraItem = cameraItem->NextSibling()){

								TiXmlElement *cameraElement = static_cast<TiXmlElement*>(cameraItem->FirstChild("name"));
								string cameraName;

								if (cameraElement){

									cameraName = cameraElement->GetText();

								} else {

									gameManager->logFatal("ERROR: Cameras must have names!", __LINE__, __FILE__);

								}

								Camera *camera = sceneManager->createCamera(cameraName);
								float values[] = {0.0,0.0,0.0,0.0,0.0,0.0};

								cameraElement = static_cast<TiXmlElement*>(cameraItem->FirstChild("location"));

								if (cameraElement){

									string temp = cameraElement->GetText();
									parseFloats(temp, values);
									camera->setPosition(values[0], values[1], values[2]);

								} else {

									gameManager->logFatal("ERROR: Cameras must have a position!", __LINE__, __FILE__);

								}

								cameraElement = static_cast<TiXmlElement*>(cameraItem->FirstChild("lookAt"));

								if (cameraElement){

									string temp = cameraElement->GetText();
									parseFloats(temp, values);
									camera->lookAt(values[0], values[1], values[2]);

								} else {

									gameManager->logFatal("ERROR: Cameras must look somewhere!", __LINE__, __FILE__);

								}

								cameraElement = static_cast<TiXmlElement*>(cameraItem->FirstChild("clip"));

								if (cameraElement){

									int vals[] = {0,0};
									string temp = cameraElement->GetText();
									parseInts(temp, vals);

									camera->setNearClipDistance(vals[0]);
									camera->setFarClipDistance(vals[1]);

								} else {

									gameManager->logFatal("ERROR: Camseas must ahve a clipping distance!", __LINE__, __FILE__);

								}

								cameraElement = static_cast<TiXmlElement*>(cameraItem->FirstChild("viewport"));

								if (cameraElement){

									string temp = cameraElement->GetText();
									parseFloats(temp, values);

									Viewport *viewport = window->addViewport(camera, values[0], values[1], values[2], values[3], values[4]);
									viewport->setOverlaysEnabled(true);
									camera->setAspectRatio((float)(viewport->getActualWidth() / viewport->getActualHeight()));

								} else {

									gameManager->logWarn("WARNING: Camera has no viewport!");

								}

							}

						} else {

							gameManager->logWarn("WARNING: Did you mean not to include any cameras in this scene?");

						}

						//lights
						TiXmlNode *lightTree = scenes->FirstChild("lights");

						if (lightTree){

							for (TiXmlNode *lightNode = lightTree->FirstChild(); lightNode; lightNode = lightNode->NextSibling()){

								//there are several types; check for them
								TiXmlElement *item = static_cast<TiXmlElement*>(lightNode->FirstChild("type"));

								if (!item) {

									gameManager->logFatal("ERROR: Every light must have a type!", __LINE__, __FILE__);

								}

								string typeString = item->GetText();

								if (typeString == "ambient"){

									//this just has a color
									TiXmlElement *color = static_cast<TiXmlElement*>(lightNode->FirstChild("color"));

									if (!color){

										gameManager->logFatal("ERROR: Ambient light must have a color!", __LINE__, __FILE__);

									}

									float colors[3];
									parseFloats(string(color->GetText()), colors);
									sceneManager->setAmbientLight(Ogre::ColourValue(colors[0], colors[1], colors[2]));

								} else if (typeString == "directional"){

									TiXmlElement *name = static_cast<TiXmlElement*>(lightNode->FirstChild("name"));

									if (!name){

										gameManager->logFatal("ERROR: Light sources must have names!", __LINE__, __FILE__);

									}

									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_DIRECTIONAL);

									TiXmlElement *dColor = static_cast<TiXmlElement*>(lightNode->FirstChild("diffuse"));

									if (!dColor){

										gameManager->logFatal("ERROR: Lights need a diffuse color!", __LINE__, __FILE__);

									}

									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = static_cast<TiXmlElement*>(lightNode->FirstChild("specular"));

									if (!dColor){

										gameManager->logFatal("ERROR: Lights must have a specular color!", __LINE__, __FILE__);

									}

									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *direction = static_cast<TiXmlElement*>(lightNode->FirstChild("direction"));

									if (!direction){

										gameManager->logFatal("ERROR: Directional lights need a direction!", __LINE__, __FILE__);

									}

									parseFloats(string(direction->GetText()), colors);
									light->setDirection(Vector3(colors[0], colors[1], colors[2]));

								} else if (typeString == "point"){

									TiXmlElement *name = static_cast<TiXmlElement*>(lightNode->FirstChild("name"));

									if (!name){

										gameManager->logFatal("ERROR: Light sources must have names!", __LINE__, __FILE__);

									}

									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_POINT);

									TiXmlElement *dColor = static_cast<TiXmlElement*>(lightNode->FirstChild("diffuse"));

									if (!dColor){

										gameManager->logFatal("ERROR: Lights need a diffuse color!", __LINE__, __FILE__);

									}

									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = static_cast<TiXmlElement*>(lightNode->FirstChild("specular"));

									if (!dColor){

										gameManager->logFatal("ERROR: Lights must have a specular color!", __LINE__, __FILE__);

									}

									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *location = static_cast<TiXmlElement*>(lightNode->FirstChild("location"));

									if (!location){

										gameManager->logFatal("ERROR: Point lights need a location!", __LINE__, __FILE__);

									}

									parseFloats(string(location->GetText()), colors);
									light->setPosition(Vector3(colors[0], colors[1], colors[2]));

								} else if (typeString == "spot"){

									TiXmlElement *name = static_cast<TiXmlElement*>(lightNode->FirstChild("name"));

									if (!name){

										gameManager->logFatal("ERROR: Light sources must have names!", __LINE__, __FILE__);

									}

									Ogre::Light *light = sceneManager->createLight(name->GetText());
									light->setType(Ogre::Light::LT_SPOTLIGHT);

									TiXmlElement *dColor = static_cast<TiXmlElement*>(lightNode->FirstChild("diffuse"));

									if (!dColor){

										gameManager->logFatal("ERROR: Lights need a diffuse color!", __LINE__, __FILE__);

									}

									float colors[3];
									parseFloats(string(dColor->GetText()), colors);
									light->setDiffuseColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									dColor = static_cast<TiXmlElement*>(lightNode->FirstChild("specular"));

									if (!dColor){

										gameManager->logFatal("ERROR: Lights must have a specular color!", __LINE__, __FILE__);

									}

									parseFloats(string(dColor->GetText()), colors);
									light->setSpecularColour(Ogre::ColourValue(colors[0], colors[1], colors[2]));

									TiXmlElement *direction = static_cast<TiXmlElement*>(lightNode->FirstChild("direction"));

									if (!direction){

										gameManager->logFatal("ERROR: Spotlights need a direction!", __LINE__, __FILE__);

									}

									parseFloats(string(direction->GetText()), colors);
									light->setDirection(Vector3(colors[0], colors[1], colors[2]));

									TiXmlElement *location = static_cast<TiXmlElement*>(lightNode->FirstChild("location"));

									if (!location){

										gameManager->logFatal("ERROR: Spotlights need a location!", __LINE__, __FILE__);

									}

									parseFloats(string(location->GetText()), colors);
									light->setPosition(Vector3(colors[0], colors[1], colors[2]));

									TiXmlElement *range = static_cast<TiXmlElement*>(lightNode->FirstChild("range"));

									if (!range){

										gameManager->logFatal("ERROR: Spotslights need a range!", __LINE__, __FILE__);

									}

									parseFloats(string(range->GetText()), colors);
									light->setSpotlightRange(Ogre::Degree(colors[0]), Ogre::Degree(colors[1]));

								} else {

									gameManager->logFatal("ERROR: Invalid light type", __LINE__, __FILE__);

								}

							}

						} else {

							gameManager->logWarn("WARNING: It's very dark in here. Think you should turn on a light?");

						}

						TiXmlNode *gravityNode = scenes->FirstChild("gravity");

						if (gravityNode){

							TiXmlElement *gravityElement = static_cast<TiXmlElement*>(gravityNode);
							string gravityString = gravityElement->GetText();

							float values[] = {0.0,0.0,0.0};
							parseFloats(gravityString, values);
							physicsManager->setGravity(values[0], values[1], values[2]);

						} else {

							gameManager->logWarn("Should this scene have gravity?");

						}
                  
						//nodes in the tree
						TiXmlNode *nodeTree = scenes->FirstChild("nodes");

						if (nodeTree){ //only do this if there are actually any nodes here

							//process all the nodes by calling the recursive function
							createNodes(sceneManager->getRootSceneNode(), nodeTree);

						} else {

							gameManager->logWarn("WARNING: There aren't any nodes in this scene... This is going to be a boring video game...");

						}

						//gui
						TiXmlNode *guiNode = scenes->FirstChild("gui");
						TiXmlElement *guiElement = guiNode->ToElement();
						string guiFile = guiElement->GetText();

						guiManager->loadResourceGroup(guiFile, sceneName);

					}

				} else {

					gameManager->logFatal("Error parsing XML document.", __LINE__, __FILE__);

				}

			}

		} else {

			gameManager->logFatal("Error: Invalid configuration format.", __LINE__, __FILE__);

		}

	} else {

		gameManager->logFatal("Error: Invalid scene file.", __LINE__, __FILE__);

	}

	//guiManager->loadResourceGroup("./assets/xml/gui.xml", sceneName);

	gameManager->logInfo("Scene built successfully!");

}

void RenderManager::buildSceneManually(){} //nothing to see here

//private methods below here ----------------------------------------------------------------------

//counts and gets the verticies for a mesh
//stolen from: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
void RenderManager::getMeshInformation(const Ogre::MeshPtr& mesh,
                        size_t &vertex_count,
                        Ogre::Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
 
    vertex_count = index_count = 0;
 
    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
 
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                                          static_cast<unsigned long>(offset);
            }
        }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}

//recursive function for making all the scene nodes
void RenderManager::createNodes(Ogre::SceneNode *parent, TiXmlNode *nodeTree){

	//loop through every node in the tree, set its stuff, create its children, and attatch it to its parent
	for (TiXmlNode *nodeNode = nodeTree->FirstChild(); nodeNode; nodeNode = nodeNode->NextSibling()){

		TiXmlElement *nodeElement = static_cast<TiXmlElement*>(nodeNode->FirstChild("name"));

		if (!nodeElement){

			gameManager->logFatal("ERROR: Nodes must have a name!", __LINE__, __FILE__);

		}

		string nodeName = nodeElement->GetText();

		nodeElement = static_cast<TiXmlElement*>(nodeNode->FirstChild("type"));

		if (!nodeElement) {

			gameManager->logFatal("ERROR: Nodes must have a type!", __LINE__, __FILE__);

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
				nodeElement = static_cast<TiXmlElement*>(entityTree->FirstChild("name"));

				if (nodeElement){

					entityName = nodeElement->GetText();

				} else {

					gameManager->logFatal("ERROR: Entities must have names!", __LINE__, __FILE__);
					exit(EXIT_FAILURE);

				}

				nodeElement = static_cast<TiXmlElement*>(entityTree->FirstChild("mesh"));

				if (entityTree){

					entityMesh = nodeElement->GetText();

				} else {

					gameManager->logFatal("ERROR: Entities must have a mesh!", __LINE__, __FILE__);
					exit(EXIT_FAILURE);

				}

				nodeElement = static_cast<TiXmlElement*>(entityTree->FirstChild("material"));

				if (nodeElement){

					entityMaterial = nodeElement->GetText();

				} else {

					gameManager->logFatal("ERROR: Entities must have materials!", __LINE__, __FILE__);
					exit(EXIT_FAILURE);

				}

				Ogre::Entity *entity = sceneManager->createEntity(entityName, entityMesh);
				entity->setMaterialName(entityMaterial);
				sceneNode->attachObject(entity);

			}

			nodeElement = static_cast<TiXmlElement*>(nodeNode->FirstChild("camera"));

			if (nodeElement){

				//cameras have a name to reference by
				string cameraName = nodeElement->GetText();
				Ogre::Camera *camera = sceneManager->getCamera(cameraName);
				sceneNode->attachObject(camera);

			}

			nodeElement = static_cast<TiXmlElement*>(nodeNode->FirstChild("scale"));

			if (nodeElement){

				string scaleString = nodeElement->GetText();
				float values[3];
				parseFloats(scaleString, values);
				sceneNode->scale(Vector3(values[0], values[1], values[2]));

			}

			nodeElement = static_cast<TiXmlElement*>(nodeNode->FirstChild("translate"));

			if (nodeElement){

				string transString = nodeElement->GetText();
				float values[3];
				parseFloats(transString, values);
				sceneNode->translate(Vector3(values[0], values[1], values[2]));

			}

			nodeElement = static_cast<TiXmlElement*>(nodeNode->FirstChild("rotate"));

			if (nodeElement){

				string rotateString = nodeElement->GetText();
				float values[4];
				parseFloats(rotateString, values);
				sceneNode->rotate(Quaternion(Degree(values[3]), Vector3(values[0], values[1], values[2])));

			}

			TiXmlNode *physicsTree = nodeNode->FirstChild("physics");

			if (physicsTree){

				float values[] = {0.0, 0.0, 0.0, 0.0, 0.0};

				nodeElement = static_cast<TiXmlElement*>(physicsTree->FirstChild("mass"));
				string massString = nodeElement->GetText();
				values[0] = atof(massString.c_str()); //parseFloats(massString, values);

				nodeElement = static_cast<TiXmlElement*>(physicsTree->FirstChild("name"));
				string physicsName = nodeElement->GetText();

				nodeElement = static_cast<TiXmlElement*>(physicsTree->FirstChild("shape"));
				string shape = nodeElement->GetText();

				nodeElement = static_cast<TiXmlElement*>(physicsTree->FirstChild("parameters"));
				string parameterString = nodeElement->GetText();

				if (shape == "box"){

					parseFloats(parameterString, values + 1);
					physicsManager->createRigidBox(physicsName, values[0], values[1], values[2], values[3]);

				} else if (shape == "sphere"){

					parseFloats(parameterString, values + 1);
					physicsManager->createRigidSphere(physicsName, values[0], values[1]);

				} else if (shape == "cylinderx"){

					parseFloats(parameterString, values + 1);
					physicsManager->createRigidCylinderX(physicsName, values[0], values[1], values[2], values[3]);

				} else if (shape == "hull"){

					Entity *entity = sceneManager->getEntity(parameterString);
					size_t numVerticies = 0;
					size_t numIndicies = 0;
					Vector3 *verticies = NULL;
					long unsigned *indicies = NULL;

					getMeshInformation(entity->getMesh(), numVerticies, verticies, numIndicies, indicies, 
						sceneNode->getPosition() - Vector3(0,2,0), Quaternion(Degree(180), Vector3(1,0,0)) * sceneNode->getOrientation(), sceneNode->getScale());

					BulletConvexHullCreator hull(verticies, numVerticies);
					physicsManager->createRigidHull(physicsName, values[0], &hull);

					delete [] verticies;
					delete [] indicies;


				} else if (shape == "plane"){

					parseFloats(parameterString, values + 1);
					physicsManager->createRigidPlane(physicsName, values[0], values[1], values[2], values[3], values[4]);

				} else {

					gameManager->logWarn("Invalid physics shape: " + shape);

				}

			}

		} else {

			gameManager->logFatal("ERROR: Invalid node type for node.", __LINE__, __FILE__);
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

   TiXmlElement *nodeElement = static_cast<TiXmlElement*>(nodeTree->FirstChild("length"));
   int length = 0;

   if (nodeElement){

      length = atoi(nodeElement->GetText());

   } else {

      gameManager->logFatal("ERROR: No length specified in animation!", __LINE__, __FILE__);

   }

   nodeElement = static_cast<TiXmlElement*>(nodeTree->FirstChild("animationName"));
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

         nodeElement = static_cast<TiXmlElement*>(keyFrameNode->FirstChild("time"));

         if (!nodeElement){

            gameManager->logFatal("ERROR: Key frame has no time set!", __LINE__, __FILE__);

         }

         float time = atof(nodeElement->GetText());

         Ogre::TransformKeyFrame *keyFrame = animationTrack->createNodeKeyFrame(time);
         float values[4] = {0,0,0,0};

         nodeElement = static_cast<TiXmlElement*>(keyFrameNode->FirstChild("translate"));

         if (nodeElement){

            string transString = nodeElement->GetText();
            parseFloats(transString, values);
            keyFrame->setTranslate(Vector3(values[0], values[1], values[2]));

         }

         nodeElement = static_cast<TiXmlElement*>(keyFrameNode->FirstChild("rotate"));

         if (nodeElement){

            string transString = nodeElement->GetText();
            parseFloats(transString, values);
            keyFrame->setRotation(Quaternion(Degree(values[3]), Vector3(values[0], values[1], values[2])));

         }

         nodeElement = static_cast<TiXmlElement*>(keyFrameNode->FirstChild("scale"));

         if (nodeElement){

            string transString = nodeElement->GetText();
            parseFloats(transString, values);
            keyFrame->setScale(Vector3(values[0], values[1], values[2]));

         }

      }

      Ogre::AnimationState *animationState = sceneManager->createAnimationState(animationName);

      nodeElement = static_cast<TiXmlElement*>(nodeTree->FirstChild("enabled"));

      if (nodeElement){

         animationState->setEnabled((strcmp(nodeElement->GetText(), "true") == 0));

      } else {

         animationState->setEnabled(false);

      }

      nodeElement = static_cast<TiXmlElement*>(nodeTree->FirstChild("loop"));

      if (nodeElement){

         animationState->setLoop((strcmp(nodeElement->GetText(), "true") == 0));

      } else {

         animationState->setLoop(false);

      }

      animationStates.push_back(animationState);

   } else {

      gameManager->logFatal("ERROR: No key frames specified for this animation node!", __LINE__, __FILE__);

   }

}
