
#include "RenderManager.h"
#include "GameManager.h"

#include <iostream>

using namespace Ogre;

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

void RenderManager::loadResourcesFromXML(const std::string &filename){}
void RenderManager::buildSceneFromXML(const std::string &filename){}

//private methods here-----------------------------------------------------------------------------

void RenderManager::loadResourcesManually(){

	std::string file_name = "./xml/resources.xml";
	std::string group_name = "0";

	  //use tiny xml to parse an xml file with the ogre paths in it
   TiXmlDocument doc(file_name.c_str());
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
      //log the fact that the resource metadata file was not found
   }

}


void RenderManager::buildSceneManually(){

	camera = sceneManager->createCamera("Camera");

    //z-order (for possible overlapping), left, top, width, height
    viewport = window->addViewport(camera, 0, 0, 0, 1.0, 1.0);  //assign a camera to a viewport (can have many cameras and viewports in a single window)
    viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

    float actual_width = Ogre::Real(viewport->getActualWidth());
    float actual_height = Ogre::Real(viewport->getActualHeight());
    float aspect_ratio = actual_width/actual_height;
    camera->setAspectRatio(aspect_ratio);

    camera->setPosition(Ogre::Vector3(0, 0, 10));
    camera->lookAt(Ogre::Vector3(0, 0, 0));
    camera->setNearClipDistance(2);
    camera->setFarClipDistance(50);
    
    sceneManager->setAmbientLight(Ogre::ColourValue(.05,.05,.05));
    Ogre::Light* light = sceneManager->createLight("Light");
    light->setType(Ogre::Light::LT_DIRECTIONAL);

    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setDirection(Ogre::Vector3(0.0,0.0,-1.0));

    Ogre::SceneNode* rsn = sceneManager->getRootSceneNode();

    //Propeller
    //scene nodes can perform transformations and contain entities
    Ogre::SceneNode* propeller_node = sceneManager->createSceneNode("PropellerNode");
    
    //entities are directly associated with meshes and materials
    Ogre::Entity* propeller_entity = sceneManager->createEntity("Propeller", "Propeller.mesh");
    propeller_entity->setMaterialName("Propeller");
    
    //link the entity to the scene node
    propeller_node->attachObject(propeller_entity);
    //link the scene node to the root node
    rsn->addChild(propeller_node);

    Vector3 propeller_translation(0,0,0);
    propeller_node->translate(propeller_translation);

    //assign transformations to the scene node
    Vector3 propeller_axis(.577, .577, -.577);
    Quaternion propeller_quat(Degree(120), propeller_axis);
    propeller_node->rotate(propeller_quat);

    Vector3 propeller_scale(.18,.18,.18);
    propeller_node->scale(propeller_scale);

    //Rudder
    Ogre::SceneNode* rudder_node = sceneManager->createSceneNode("RudderNode");
    
    Ogre::Entity* rudder_entity = sceneManager->createEntity("Rudder", "Rudder.mesh");
    rudder_entity->setMaterialName("Rudder");
    
    //link the entity to the scene node
    rudder_node->attachObject(rudder_entity);
    //link the scene node to the root node
    rsn->addChild(rudder_node);

    Vector3 rudder_translation(0,0,0);
    rudder_node->translate(rudder_translation);

    //assign transformations to the scene node
    Vector3 rudder_axis(.577, .577, -.577);
    Quaternion rudder_quat(Degree(120), rudder_axis);
    rudder_node->rotate(rudder_quat);

    Vector3 rudder_scale(.18,.18,.18);
    rudder_node->scale(rudder_scale);

    Ogre::SceneNode* sub_transform_node = sceneManager->createSceneNode("SubTransformNode");
    rsn->addChild(sub_transform_node);

    Vector3 entire_sub_translation(2,0,0);
    sub_transform_node->translate(entire_sub_translation);

    //quaternion version
    Vector3 entire_submarine_axis(.7,.7,0);
    Quaternion entire_submarine_quat(Degree(27.5), entire_submarine_axis);
    sub_transform_node->rotate(entire_submarine_quat);

    //Periscope

    Ogre::SceneNode* periscope_transform_node = sceneManager->createSceneNode("PeriscopeTransformNode");
    Vector3 periscope_translation2(0,.7,0);
    periscope_transform_node->translate(periscope_translation2);
    sub_transform_node->addChild(periscope_transform_node);
    
    Ogre::SceneNode* periscope_node = sceneManager->createSceneNode("PeriscopeNode");
    
    Ogre::Entity* periscope_entity = sceneManager->createEntity("Periscope", "Periscope.mesh");
    periscope_entity->setMaterialName("Periscope");
    
    //link the entity to the scene node
    periscope_node->attachObject(periscope_entity);
    //link the scene node to the root node
    periscope_transform_node->addChild(periscope_node);

    Vector3 periscope_translation(0,0,0);
    periscope_node->translate(periscope_translation);

    //assign transformations to the scene node
    Vector3 periscope_axis(0,1,0);
    Quaternion periscope_quat(Degree(90), periscope_axis);
    periscope_node->rotate(periscope_quat);

    Vector3 periscope_scale(.1,.1,.1);
    periscope_node->scale(periscope_scale);

    //Submarine
    Ogre::SceneNode* submarine_node = sceneManager->createSceneNode("SubmarineNode");
    sub_transform_node->addChild(submarine_node);
    
    Ogre::Entity* submarine_entity = sceneManager->createEntity("Submarine", "Submarine.mesh");
    submarine_entity->setMaterialName("Submarine");
    
    //link the entity to the scene node
    submarine_node->attachObject(submarine_entity);
    //link the scene node to the root node
    //rsn->addChild(submarine_node);

    Vector3 submarine_scale(1,1,1);
    submarine_node->scale(submarine_scale);

    Vector3 submarine_translation(0,0,0);
    submarine_node->translate(submarine_translation);

    //quaternion version
    Vector3 submarine_axis2(0,1,0);
    Quaternion submarine_quat2(Degree(90), submarine_axis2);
    submarine_node->rotate(submarine_quat2);

    //rotate to adjust to Ogre coordinates (from Blender coordinates)
    //this must be applied first, so it is the last rotation added
    Vector3 submarine_axis(1,0,0);
    Quaternion submarine_quat(Degree(90), submarine_axis);
    submarine_node->rotate(submarine_quat);

}

