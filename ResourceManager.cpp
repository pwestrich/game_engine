
#include "ResourceManager.h"
#include "GameManager.h"


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
void ResourceManager::loadResourcesFromXML(const std::string &filename, const std::string &group_name){

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

void ResourceManager::unloadResources(){

  if (groupLoaded == ""){

    return;

  }

  Ogre::ResourceGroupManager &resourceManager = Ogre::ResourceGroupManager::getSingleton();
  
  //for some reason, this crashes the game... I ahve no idea why
  //it works without it, so it's commented out.
  //resourceManager.destroyResourceGroup(groupLoaded);
  groupLoaded = "";

}
