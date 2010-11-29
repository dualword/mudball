#include "App.h"

#include "EventReceiver.h"
#include "../tinyXML/tinyxml.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

App::App()
{
    device = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 32,false, false, true, 0);

    recv = new EventReceiver(device);
    device->setEventReceiver(recv);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"ESC- exit; Arrows - move; A - add;  D - remove; C,X - change; R - rotate; RShift - UP; RCtrl - DOWN; F2 - save; F3 - load",rect<s32>(10,10,500,40),false,true);

    ILightSceneNode* light = smgr->addLightSceneNode(0);
    light->setRotation(vector3df(90,0,0));
    smgr->setAmbientLight(SColorf(0.5,0.5,0.5,1));

    SLight & l = light->getLightData();
    l.Type = ELT_DIRECTIONAL;

    wcube = smgr->addCubeSceneNode(2);
    wcube->setMaterialFlag(EMF_LIGHTING,false);
    wcube->setMaterialFlag(EMF_WIREFRAME,true);

    camera = smgr->addCameraSceneNode(0,vector3df(0,10,-5));
    camera->setTarget(wcube->getPosition());

    currentElement = EELEVATOR;
}

App::~App()
{
    device->drop();
}

App* App::getInstance()
{
    static App* instance = 0;
    if(!instance) instance = new App();
    return instance;
}



void App::eventKeyPressed(int key)
{
    switch (key)
    {
        case KEY_ESCAPE:
            device->drop();
            exit(0);
            break;
    }
}

void App::eventKeyReleased(int key)
{
    switch (key)
    {
        case KEY_LEFT:
            wcube->setPosition(wcube->getPosition() + vector3df(-2,0,0) );
            break;
        case KEY_RIGHT:
            wcube->setPosition(wcube->getPosition() + vector3df(2,0,0) );
            break;
        case KEY_UP:
            wcube->setPosition(wcube->getPosition() + vector3df(0,0,2) );
            break;
        case KEY_DOWN:
            wcube->setPosition(wcube->getPosition() + vector3df(0,0,-2) );
            break;
        case KEY_RSHIFT:
            wcube->setPosition(wcube->getPosition() + vector3df(0,1,0) );
            break;
        case KEY_RCONTROL:
            wcube->setPosition(wcube->getPosition() + vector3df(0,-1,0) );
            break;

        case KEY_KEY_A:

            if(!existElementAt(wcube->getPosition()))
            {
                Element* el = this->addElementAt(wcube->getPosition());
                level.push_back(el);
            }

            break;

        case KEY_KEY_C:
            {
                removeElementAt(wcube->getPosition());

                currentElement = (currentElement)%17 + 1;

                //printf("Current Element: %d\n",currentElement);

                Element* el = this->addElementAt(wcube->getPosition());
                level.push_back(el);
            }
            break;

        case KEY_KEY_X:
            {
                removeElementAt(wcube->getPosition());

                currentElement--;
                if(currentElement < 1) currentElement = 17;

                Element* el = this->addElementAt(wcube->getPosition());
                level.push_back(el);
            }
            break;

        case KEY_KEY_D:
            removeElementAt(wcube->getPosition());
            break;

        case KEY_KEY_R:
            {
                Element* el = existElementAt(wcube->getPosition());
                if(el)
                {
                    el->setRotation( el->getRotation() + vector3df(0,90,0));
                }
            }
            break;

        case KEY_F2:
            saveToXML();
            break;
        case KEY_F3:
            loadFromXML();
            break;
    }
}

void App::run()
{
    while(device->run())
    {
        camera->setPosition(wcube->getPosition() + vector3df(0,4,-2.5));
        camera->setTarget(wcube->getPosition());

        for(int i=0;i<level.size();i++) level[i]->animate();

        driver->beginScene(true, true, SColor(255,100,100,100));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }
}

Element* App::existElementAt(vector3df pos)
{
    for(int i=0;i<level.size();i++)
    {
        if(  pos.getDistanceFrom(((Element*)level[i])->getPosition()) < 1 )
        {
            return level[i];
        }
    }

    return NULL;
}

Element* App::addElementAt(vector3df pos)
{
    Element* newEl = NULL;

    switch(currentElement)
    {
        case EELEVATOR:
            newEl = new EElevator(smgr);
            break;
        case EELEVATOR2X:
            newEl = new EElevator2X(smgr);
            break;
        case EGOAL:
            newEl = new EGoal(smgr);
            break;
        case EMUD:
            newEl = new EMud(smgr);
            break;
        case ERAMP:
            newEl = new ERamp(smgr);
            break;
        case ERAMPEND:
            newEl = new ERampEnd(smgr);
            break;
        case ERAMPFLAT:
            newEl = new ERampFlat(smgr);
            break;
        case ERAMPSTART:
            newEl = new ERampStart(smgr);
            break;
        case ETUBE2:
            newEl = new ETube2(smgr);
            break;
        case ETUBE2C:
            newEl = new ETube2C(smgr);
            break;
        case ETUBE2H:
            newEl = new ETube2H(smgr);
            break;
        case ETUBEBIG:
            newEl = new ETubeBig(smgr);
            break;
        case ETUBEBIGCURVE:
            newEl = new ETubeBigCurve(smgr);
            break;
        case ETUBEH:
            newEl = new ETubeH(smgr);
            break;
        case EWIND:
            newEl = new EWind(smgr);
            break;
        case EPLATFORM:
            newEl = new EPlatform(smgr);
            break;
        case ESTART:
            newEl = new EStart(smgr);
            break;
    }

    newEl->setPosition(pos);

    return newEl;
}

void App::removeElementAt(vector3df pos)
{
    for(int i=0;i<level.size();i++)
    {
        if(  pos.getDistanceFrom(((Element*)level[i])->getPosition()) < 1 )
        {
            delete level[i];
            level.erase(level.begin() + i);
        }
    }
}

void App::saveToXML()
{
    char filename[256];

    cout << "__________________________________________________" << endl;
    cout << "Insert the level name without extension, example: levelOne" << endl;
    cout << "Save filename: ";
    cin.getline (filename,256);

    stringc rfilename = "levels\\";
    rfilename.append(filename);
    rfilename.append(".xml");

    TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );

	TiXmlElement* b_project = new TiXmlElement( "b_level" );
	b_project->SetAttribute("version","1.0");

	for(int i=0;i<level.size();i++)
	{
	    TiXmlElement* xelement = new TiXmlElement( "element" );
	    xelement->SetAttribute("type",level[i]->getType());

	    xelement->SetAttribute("x",level[i]->getPosition().X);
	    xelement->SetAttribute("y",level[i]->getPosition().Y);
	    xelement->SetAttribute("z",level[i]->getPosition().Z);

	    xelement->SetAttribute("rot",level[i]->getRotation().Y);

	    b_project->LinkEndChild(xelement);
	}

	doc.LinkEndChild( decl );
	doc.LinkEndChild( b_project );
	doc.SaveFile( rfilename.c_str() );

    cout << "Saved file: " << rfilename.c_str() << endl;
}

void App::loadFromXML()
{
    char filename[256];

    cout << "__________________________________________________" << endl;
    cout << "Insert the level name without extension, example: levelOne" << endl;
    cout << "Load filename: ";
    cin.getline (filename,256);

    stringc rfilename = "levels\\";
    rfilename.append(filename);
    rfilename.append(".xml");


    TiXmlDocument doc(rfilename.c_str());
	if (!doc.LoadFile())
	{
	    cout << "Error loading file : " << rfilename.c_str() << endl;
	    return;
	}

	//clear current level
    for(int i=0;i<level.size();i++)
    {
        removeElementAt(level[i]->getPosition());
    }

    TiXmlElement* root = doc.FirstChildElement( "b_level" );

    if ( root )
    {
        TiXmlNode* xelement = root->FirstChild( "element" );

        while( xelement != NULL )
        {
            int type = atoi(xelement->ToElement()->Attribute("type"));

            f32 posX = atof(xelement->ToElement()->Attribute("x"));
            f32 posY = atof(xelement->ToElement()->Attribute("y"));
            f32 posZ = atof(xelement->ToElement()->Attribute("z"));

            f32 rot = atof(xelement->ToElement()->Attribute("rot"));

            currentElement = type;

            Element* newEl = addElementAt(vector3df(posX,posY,posZ));
            newEl->setRotation(vector3df(0,rot,0));

            level.push_back(newEl);

            xelement = root->IterateChildren( "element", xelement );
        }
    }

    cout << "Loaded file: " << rfilename.c_str() << endl;
}
