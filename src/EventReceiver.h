#ifndef RECV
#define RECV

#include <iostream>
#include <irrlicht.h>
#include "App.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EventReceiver : public IEventReceiver
{
public:
    EventReceiver(IrrlichtDevice* device)
    {
        this->device = device;

        for(int i=0;i<irr::KEY_KEY_CODES_COUNT;i++) keys[i]=0;
    }

    bool isKeyPressed(int key)
    {
        return keys[key];
    }

    void flushKeys()
    {
        for(int i=0;i<irr::KEY_KEY_CODES_COUNT;i++) keys[i]=0;
    }

	virtual bool OnEvent(const SEvent& event)
	{
		if(event.EventType == EET_KEY_INPUT_EVENT)
		{
		    if(event.KeyInput.PressedDown == true)
            {
                keys[event.KeyInput.Key] = 1;
                App::getInstance()->eventKeyPressed(event.KeyInput.Key);
            }
            else
            {
                keys[event.KeyInput.Key] = 0;
                App::getInstance()->eventKeyReleased(event.KeyInput.Key);
            }
		}

		if(event.EventType == EET_GUI_EVENT)
		{
			switch(event.GUIEvent.EventType)
			{
			}
		}
		return false;
	}
private:
    IrrlichtDevice* device;

    bool keys[KEY_KEY_CODES_COUNT];
};

#endif
