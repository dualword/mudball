//MudBall-mod https://github.com/dualword/mudball
#ifndef APP_H
#define APP_H

#include <irrlicht.h>
#include <irrbullet.h>
#include <vector>
//#include "tinyxml.h"
#include <dirent.h>
#include "XEffects/XEffects.h"
#include <SFML/Audio.hpp>

#include "Element.h"
#include "EElevator.h"
#include "EElevator2X.h"
#include "EPlatform.h"
#include "EMud.h"
#include "EGoal.h"
#include "ETubeBig.h"
#include "ETubeBigCurve.h"
#include "ETubeH.h"
#include "ETube2.h"
#include "ETube2C.h"
#include "ETube2H.h"
#include "EWind.h"
#include "ERampFlat.h"
#include "ERamp.h"
#include "ERampStart.h"
#include "ERampEnd.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

struct vMode{
    int w;
    int h;
};

class App
{
    public:
        static App* getInstance();

        vector3df getBallPosition();
        void applyForceToBall(vector3df force);

        void setupWiimote();
        void cleanWiimote();

        void eventKeyPressed(int key);
        void eventKeyReleased(int key);

        void loadLevel(stringc levelname);

        void resetGame();
        void setWin();

        void runGame();

        void showHighScores();

        void showInGameMenu();

        void setupResolution();

        virtual ~App();
    protected:
    private:
        App();

        IrrlichtDevice* device;
        ISceneManager* smgr;
        IVideoDriver* driver;
        IGUIEnvironment* guienv;

        int screenWidth;
        int screenHeight;
        bool useEffects;
        bool fullScreen;

//        wiimote** wiimotes;
//        wiimote* wm;
        bool usingWiiMote;

        float sensKeyboard;

        irrBulletWorld* world;

        void checkWiimoteEvent();

        float pitch;
        float roll;

        float camZoom;

        bool run;

        ISceneNode* levelRoot;
        vector<Element*> level;

        ISceneNode* ball;
        ISphereShape* ballShape;
        IRigidBody* ballBody;

        vector3df ballStart;

        int currentElement;

        IGUIStaticText* gameTime, *fps;

        static const int jumpForce = 250;

        bool resetGameVar;

        stringc currentLevelName;
        int currentLevelTime;

        IGUIListBox* levelsBox;

        void removeElementAt(vector3df pos);
        Element* addElementAt(vector3df pos);

        void ballJump(int force = 250);

        void show321GO();

        //this function is used to create a list of files in a folder
        int getdir(stringc dir, vector<stringc> &files);

        void setupBulletPhysics();
        void clearBulletPhysics();

        void setupBall();
        void removeBall();

        EffectHandler* effect;

        void setupEffects();
        void clearEffects();

        bool win;

        sf::SoundBuffer bufL, bufJ;
        sf::Sound sndL, sndJ;
};

#endif // APP_H
