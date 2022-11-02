//MudBall-mod https://github.com/dualword/mudball
#include "App.h"
#include "tinyXML/tinyxml.h"

#include "EventReceiver.h"

#include "XEffects/XEffects.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

App::App()
{
    this->setupResolution();

    run = true;

    device = createDevice( video::EDT_OPENGL, dimension2d<u32>(screenWidth, screenHeight), 32,fullScreen, true, true, 0);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    EventReceiver* recv = new EventReceiver(device);

    device->setEventReceiver(recv);

    smgr->addCameraSceneNode(0, vector3df(0,5,-5), vector3df(0,0,0));

    ILightSceneNode* light = smgr->addLightSceneNode(0);
    light->setRotation(vector3df(90,0,0));
    smgr->setAmbientLight(SColorf(0.5,0.5,0.5,1));

    SLight & l = light->getLightData();
    l.Type = ELT_DIRECTIONAL;

    currentElement = EELEVATOR;

    levelRoot = smgr->addEmptySceneNode();

    this->loadLevel("level.xml");
    resetGameVar = true;

    setupWiimote();

    bufL.loadFromFile("media/loser.ogg");
	sndL.setBuffer(bufL);
    bufJ.loadFromFile("media/ball_jump.ogg");
    sndJ.setBuffer(bufJ);

    //world->setDebugMode(EPDM_DrawAabb | EPDM_DrawContactPoints);

    usingWiiMote = false;

    sensKeyboard = 3;

    camZoom = 5;

    guienv->getSkin()->setFont(guienv->getFont("media/dialog.xml"));
    fps = guienv->addStaticText(L"FPS:0",rect<s32>(10,10,300,50));
    gameTime = guienv->addStaticText(L"Time:0",rect<s32>(10,60,300,100));

    ISceneNode* skybox=smgr->addSkyBoxSceneNode(
                driver->getTexture("media/irrlicht2_up.jpg"),
                driver->getTexture("media/irrlicht2_dn.jpg"),
                driver->getTexture("media/irrlicht2_lf.jpg"),
                driver->getTexture("media/irrlicht2_rt.jpg"),
                driver->getTexture("media/irrlicht2_ft.jpg"),
                driver->getTexture("media/irrlicht2_bk.jpg"));

    //create the levels list
    levelsBox = guienv->addListBox(rect<s32>(screenWidth/2 - 200, screenHeight/2 - 150, screenWidth/2 + 200, screenHeight/2 + 150),0,-1,true);
    levelsBox->setVisible(false);

    vector<stringc> files;
    getdir("levels/",files);

    for (unsigned int i = 0;i < files.size();i++)
    {
        stringc lfile = files[i];
        if( lfile.subString(lfile.size() - 11, 11) != "_scores.xml" && lfile.subString(lfile.size() - 4, 4) == ".xml")
            levelsBox->addItem(stringw(files[i]).c_str());
    }
}

App::~App()
{
    //dtor
}

App* App::getInstance()
{
    static App* instance = 0;
    if(!instance) instance = new App();
    return instance;
}

vector3df App::getBallPosition()
{
    return ball->getPosition();
}

void App::applyForceToBall(vector3df force)
{

    ballBody->applyCentralForce(force);
}

void App::resetGame()
{
    resetGameVar = true;
    win = false;
}

void App::runGame()
{
    device->setWindowCaption(L"MudBall 1.0 Beta - mudball.sourceforge.net");

    EventReceiver* recv = (EventReceiver*)device->getEventReceiver();

    currentLevelTime=device->getTimer()->getTime();

    u32 TimeStamp = device->getTimer()->getTime();
    u32 DeltaTime = 0;
    while(device->run() && run)
    {
        if(useEffects)
        {
            effect->getShadowLight(0).setPosition(ball->getPosition() + vector3df(0,20,0));
            effect->getShadowLight(0).setTarget(ball->getPosition());
        }

        driver->beginScene(true, true, SColor(255,100,101,140));

        if(useEffects)
        {
            effect->update();
        }
        else
            smgr->drawAll();

        guienv->drawAll();

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

        world->stepSimulation(DeltaTime*0.001f, 120);

        driver->endScene();

        //check wiimote events
        checkWiimoteEvent();

        if(ball->getPosition().Y < -10)
        {
        	sndL.play();
            resetGameVar = true;
        }

        if(win)
        {
            showHighScores();
            resetGame();
        }

        if (resetGameVar)
        {
            world->removeCollisionObject(ballBody,false);
            ball->setPosition(ballStart);
            ballBody->clearForces();
            ballBody->setLinearVelocity(vector3df(0,0,0));
            ballBody->setAngularVelocity(vector3df(0,0,0));
            ballBody->applyCentralForce(vector3df(0,-1,0));
            ballBody->setWorldTransform(ball->getAbsoluteTransformation());

            device->getSceneManager()->getActiveCamera()->setPosition(ball->getPosition() + vector3df(0,camZoom,-camZoom));
            device->getSceneManager()->getActiveCamera()->setTarget(ball->getPosition());
            this->show321GO();

            ballBody = world->addRigidBody(ballShape);
            ballBody->forceActivationState(EAS_DISABLE_DEACTIVATION);
            resetGameVar = false;
            currentLevelTime = device->getTimer()->getTime();

            for(int i=0;i<level.size();i++)
            {
                Element* e = level[i];
                e->resetElement();
            }

            continue;
        }

        //also check the keyboard inputs
        if(recv->isKeyPressed(KEY_UP))
            ballBody->applyCentralForce(vector3df(0,0,sensKeyboard));
        else if(recv->isKeyPressed(KEY_DOWN))
            ballBody->applyCentralForce(vector3df(0,0,-sensKeyboard));
        else if(recv->isKeyPressed(KEY_LEFT))
            ballBody->applyCentralForce(vector3df(-sensKeyboard,0,0));
        else if(recv->isKeyPressed(KEY_RIGHT))
            ballBody->applyCentralForce(vector3df(sensKeyboard,0,0));


        ballBody->applyCentralForce(vector3df(-roll,0,pitch));

        for(int i=0;i<level.size();i++)
        {
            Element* el = level[i];
            if(el)
            {
                el->updateBulletPhysics();
                el->animate();
            }
        }

        device->getSceneManager()->getActiveCamera()->setPosition(ball->getPosition() + vector3df(0,camZoom,-camZoom));
        device->getSceneManager()->getActiveCamera()->setTarget(ball->getPosition());

        //update level timer
        stringw timeStr = stringw("TIME: ");
        timeStr.append( stringw( (device->getTimer()->getTime() - currentLevelTime)*0.001) );
        gameTime->setText(timeStr.subString(0,timeStr.findFirst('.') + 2).c_str());
        stringw f = L"FPS: ";
        f.append( stringw( driver->getFPS()) );
        fps->setText(f.c_str());

        if( ((EventReceiver*)device->getEventReceiver())->isKeyPressed(KEY_ESCAPE) )
        {
            this->showInGameMenu();
            ((EventReceiver*)device->getEventReceiver())->flushKeys();
        }
    }

    delete world;
    if(usingWiiMote) cleanWiimote();
}

void App::eventKeyPressed(int key)
{
}

void App::eventKeyReleased(int key)
{
    switch(key)
    {
        case KEY_SPACE:
            this->ballJump();
            break;
        case KEY_KEY_A:
            if(ball->getPosition().Y < -20) resetGameVar = true;
            break;
    }
}

void App::setupWiimote()
{
//    wiimotes =  wiiuse_init(1);//inicializar 1 controle
//    wm = wiimotes[0];
//
//    if (!wiiuse_find(wiimotes, 1, 5))
//	{
//		printf ("No wiimotes found.");
//		return;
//	}

	usingWiiMote = true;

//	if (wiiuse_connect(wiimotes, 1))
//		printf("Connected to wiimote\n");
//	else
//	{
//		printf("Failed to connect to wiimote.\n");
//		exit(0);
//	}
//
//	wiiuse_set_leds(wm, WIIMOTE_LED_1);

    //ativar sensor de movimentos
	//wiiuse_motion_sensing(wiimotes[0], 1);
}

void App::cleanWiimote()
{
    //wiiuse_cleanup(wiimotes, 1);
}

void App::checkWiimoteEvent()
{
//    if(wiiuse_poll(wiimotes, 1))
//    {
//        switch (wm->event)
//        {
//            case WIIUSE_EVENT:
//
//                if (WIIUSE_USING_ACC(wm))
//                {
//                    //printf("wiimote roll  = %f [%f]\n", wm->orient.roll,wm->orient.a_roll);
//                    //printf("wiimote pitch = %f [%f]\n",wm->orient.pitch, wm->orient.a_pitch);
//                    //printf("wiimote yaw   = %f\n",wm->orient.yaw);
//
//                    roll = wm->orient.pitch;
//                    pitch = wm->orient.roll;
//
//                    float s = 30;
//
//                    pitch/=s;
//                    roll/=s;
//                }
//
//
//                if(wm->accel.z > 170)
//                {
//                    this->ballJump(50);
//                }
//
//                //printf("%d\n",ballBody->get());
//
//                if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))
//                {
//                    run = false;
//                }
//
//                /*
//                //uncomment this if you want to reset the game with the wiimote A button
//                if (IS_PRESSED(wm, WIIMOTE_BUTTON_A) && ball->getPosition().Y < -20 )
//                {
//                    ball->setPosition(ballStart);
//                    ballBody->clearForces();
//                    ballBody->setLinearVelocity(vector3df(0,0,0));
//                    ballBody->setAngularVelocity(vector3df(0,0,0));
//                    ballBody->applyCentralForce(vector3df(0,-1,0));
//                    ballBody->setWorldTransform(ball->getAbsoluteTransformation());
//                }
//                */
//
//                /*
//                //move the ball with wiimote arrows
//                int force = 5;
//
//                if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP))
//                    ballBody->applyCentralForce(vector3df(-force,0,0));
//                else if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))
//                    ballBody->applyCentralForce(vector3df(force,0,0));
//
//                if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))
//                    ballBody->applyCentralForce(vector3df(0,0,-force));
//                else if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))
//                    ballBody->applyCentralForce(vector3df(0,0,force));
//
//                if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE))
//                    ballBody->applyCentralForce(vector3df(0,jumpForce,0));
//                */
//
////                if(IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) camZoom += 1;
////                if(IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) camZoom -= 1;
//
//                if(camZoom < 5) camZoom = 5;
//                if(camZoom > 25) camZoom = 25;
//
//                break;
//        }
//    }
}

void App::loadLevel(stringc levelname)
{
    this->clearEffects();
    this->removeBall();
    this->clearBulletPhysics();
    this->setupBulletPhysics();
    this->setupBall();


    currentLevelName = levelname.subString(0,levelname.size() - 4);
    currentLevelName.make_upper();

    stringc levelFolder = "levels/";
    levelFolder.append(levelname);
    levelname = levelFolder;

    TiXmlDocument doc(levelname.c_str());
	if (!doc.LoadFile())
	{
	    cout << "Error loading file : " << levelname.c_str() << endl;
	    return;
	}

	//clear current level
    for(int i=0;i<level.size();i++)
    {
        //removeElementAt(level[i]->getPosition());
        delete level[i];
    }

    level.clear();

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
            newEl->setParent(levelRoot);

            if(type == 17)
            {
                ball->setPosition(vector3df(posX,posY+1,posZ));
                ballStart = vector3df(posX,posY+1,posZ);
                printf("BALL POS %f,%f,%f\n",posX,posY+1,posZ);
            }

            level.push_back(newEl);

            xelement = root->IterateChildren( "element", xelement );
        }
    }

    //setup physics world
    for(int i=0;i<level.size();i++)
    {
        Element* el = level[i];
        if(el) el->setBulletPhysics(world);
    }

    this->setupEffects();

    cout << "Loaded file: " << levelname.c_str() << endl;
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
        case 17:
            newEl = new EPlatform(smgr);
            break;
    }

    newEl->setPosition(pos);

    return newEl;
}

void App::ballJump(int force)
{
    bool collided = false;

    for(int i=0;i<level.size();i++)
    {
        ISceneNode* elementNode = level[i]->getSceneNode();
        if( ball->getTransformedBoundingBox().intersectsWithBox(elementNode->getTransformedBoundingBox()) ) collided = true;
    }

    if(collided)
    {
        ballBody->applyCentralForce(vector3df(0,force,0));
    	sndJ.play();
    }
}

void App::showHighScores()
{
    bool highScoresScreen = true;

    float angle = 0;

    vector<stringc> highScoresNames;
    vector<float> highScoresTimes;

    int attempts = 0;

    //high scores shadow rect
    rect<s32> boxRect = rect<s32>(driver->getScreenSize().Width/2 - 300,
                                  driver->getScreenSize().Height/2 - 250,
                                  driver->getScreenSize().Width/2 + 300,
                                  driver->getScreenSize().Height/2 + 200);

    float myTime = (device->getTimer()->getTime() - currentLevelTime)*0.001;

    //declare an XML document
    stringc scoresFilename = "levels/";
    scoresFilename.append(stringc(currentLevelName));
    scoresFilename.append("_scores.xml");
    scoresFilename.make_lower();

    TiXmlDocument doc(scoresFilename.c_str());

    //the root node (all scores will be childs of this node)
    TiXmlElement* xmlRoot;

    //if the current level already have scores load current high scores from XML
	if (doc.LoadFile())
	{
        xmlRoot = doc.FirstChildElement( "level_high_scores" );
        if ( xmlRoot )
        {
            attempts = atoi(xmlRoot->ToElement()->Attribute("attempts"));

            //iterate scores loading them
            TiXmlNode* xelement = xmlRoot->FirstChild( "score" );
            while( xelement != NULL )
            {
                highScoresNames.push_back(stringc(xelement->ToElement()->Attribute("name")));
                highScoresTimes.push_back(atof(xelement->ToElement()->Attribute("time")));

                xelement = xmlRoot->IterateChildren( "score", xelement );
            }
        }
    }
    else//else create the scores
	{
        //setup the root
        xmlRoot = new TiXmlElement( "level_high_scores" );
        doc.LinkEndChild(xmlRoot);
    }

    stringc playerName = "";

    //check if player score is in top ten
    bool topTen = false;
    for(int i=0;i<highScoresTimes.size();i++)
    {
        if(myTime < highScoresTimes[i] || highScoresTimes.size() < 10)
        {
            topTen = true;
            break;
        }
    }

    //read player name
    if(topTen || highScoresTimes.size() == 0)
    {
        bool exitWrite = false;
        while(!exitWrite)
        {
            device->run();

            driver->beginScene(true, true, SColor(255,100,101,140));

            smgr->drawAll();

            guienv->drawAll();

            //draw box shadow
            driver->draw2DRectangle(SColor(100,0,0,0),boxRect);

            guienv->getSkin()->getFont()->draw(L"INPUT YOUR NAME", rect<s32>(driver->getScreenSize().Width/2 - 260,
                                                                             driver->getScreenSize().Height/2 - 75,
                                                                             driver->getScreenSize().Width/2 + 260,
                                                                             driver->getScreenSize().Height/2 - 25),SColor(255,255,255,255),true);

            guienv->getSkin()->getFont()->draw(stringw(playerName).c_str(), rect<s32>(driver->getScreenSize().Width/2 - 260,
                                                                             driver->getScreenSize().Height/2,
                                                                             driver->getScreenSize().Width/2 + 260,
                                                                             driver->getScreenSize().Height/2 + 50),SColor(255,255,255,255),true);
            driver->endScene();

            //append chars when keyboard is pressed
            for(int i=0;i<KEY_KEY_CODES_COUNT;i++)
            {
                if(((EventReceiver*)device->getEventReceiver())->isKeyPressed(i) && i!=KEY_RETURN)
                {
                    if(i == KEY_BACK)
                    {
                        playerName = playerName.subString(0,playerName.size()-1);
                    }
                    else
                    {
                        playerName.append(i);
                    }
                    device->sleep(150);
                }
            }


            if( ((EventReceiver*)device->getEventReceiver())->isKeyPressed(KEY_RETURN) ) exitWrite = true;
        }

        ((EventReceiver*)device->getEventReceiver())->flushKeys();
    }

    //remove white spaces - some idiots write spaces before the name and this fucks the scores alignment
    playerName.trim();

    //add player score to the vectors
    highScoresTimes.push_back(myTime);
    highScoresNames.push_back(playerName);

    //Bubble Sort...
    float tmp;
    stringc tmpName;
    for(long i=0;i<highScoresTimes.size();i++)
    {
        for(long x=0; x<highScoresTimes.size()-1-i; x++)
        {
            if(highScoresTimes[x] > highScoresTimes[x+1])
            {
                tmp = highScoresTimes[x];
                tmpName = highScoresNames[x];

                highScoresTimes[x] = highScoresTimes[x+1];
                highScoresNames[x] = highScoresNames[x+1];

                highScoresTimes[x+1] = tmp;
                highScoresNames[x+1] = tmpName;
            }
        }
    }


    //clear root and record the top ten
    xmlRoot->Clear();
    attempts++;
    xmlRoot->SetAttribute("attempts",attempts);

    for(int i=0;i<  (highScoresNames.size() <= 10 ? highScoresNames.size() : 10) ;i++)
    {
        TiXmlElement* xelement = new TiXmlElement( "score" );
	    xelement->SetAttribute("name",highScoresNames[i].c_str());
	    xelement->SetAttribute("time",stringc(highScoresTimes[i]).c_str());

	    xmlRoot->LinkEndChild(xelement);
    }

    //finally save the new XML file
    doc.SaveFile();

    cout << "Scores Saved!" << endl;

    gameTime->setVisible(false);

    while(highScoresScreen && device->run())
    {
        dimension2di scoresPos = dimension2di(driver->getScreenSize().Width/2 - 300 + 20,
                                              driver->getScreenSize().Height/2 - 200 + 20);

        driver->beginScene(true, true, SColor(255,100,101,140));

        smgr->drawAll();

        guienv->drawAll();

        //draw box shadow
        driver->draw2DRectangle(SColor(100,0,0,0),boxRect);

        //draw level name
        stringw levelName = stringw(L"LEVEL: ");
        levelName.append(currentLevelName);
        levelName.append(L" - ATTEMPTS:");
        levelName.append(stringw(attempts));
        guienv->getSkin()->getFont()->draw(levelName, rect<s32>(scoresPos.Width,scoresPos.Height-50,driver->getScreenSize().Width/2 + 260,50),SColor(255,255,255,255),true);

        //draw high scores
        for(int i=0;i<(highScoresNames.size() <= 10 ? highScoresNames.size() : 10);i++)
        {
            if(i>10) break;//just the first 10 will be drawed

            rect<s32> textRect = rect<s32>(scoresPos.Width,
                                          scoresPos.Height,
                                          driver->getScreenSize().Width/2 + 260,
                                          50);

            stringw timeStr = stringw( highScoresTimes[i] );
            timeStr = timeStr.subString(0,timeStr.findFirst('.') + 2);

            stringw playerNameAndScore = stringw(highScoresNames[i]);
            playerNameAndScore.append(" : ");
            playerNameAndScore.append( timeStr );

            guienv->getSkin()->getFont()->draw(playerNameAndScore, textRect,SColor(255,255,255,255));

            scoresPos.Height += 38;
        }

        driver->endScene();

        if( ((EventReceiver*)device->getEventReceiver())->isKeyPressed(KEY_RETURN) ) highScoresScreen = false;

        smgr->getActiveCamera()->setPosition(vector3df(-cos(angle)*3,1,sin(angle)*3) + ball->getPosition() );

        angle += 0.01;
        if(angle > 360) angle = 360;
    }

    gameTime->setVisible(true);
}

void App::setWin()
{
    win = true;
}

void App::show321GO()
{
    int count = 3;

    int frames = 30;

    for(int i=0;i<frames;i++)
    {
        device->run();
        driver->beginScene(true, true, SColor(255,100,101,140));


        smgr->drawAll();

        guienv->drawAll();

        position2di pos = position2di(driver->getScreenSize().Width/2-128,driver->getScreenSize().Height/2-128);

        switch(count)
        {
            case 3:
                driver->draw2DImage(driver->getTexture("media/3.png"),pos,rect<s32>(0,0,256,256),0,SColor(255,255,255,255),true);
                break;
            case 2:
                driver->draw2DImage(driver->getTexture("media/2.png"),pos,rect<s32>(0,0,256,256),0,SColor(255,255,255,255),true);
                break;
            case 1:
                driver->draw2DImage(driver->getTexture("media/1.png"),pos,rect<s32>(0,0,256,256),0,SColor(255,255,255,255),true);
                break;
        }


        driver->endScene();

        if(count > 1 && i == frames-1)
        {
            i = 0;
            count--;
        }
    }
}

void App::showInGameMenu()
{
    bool levelSelected = false;

    int menuItem = 0;

    enum MENU_OPTION { INGAME_MENU = 0, SELECT_LEVEL = 1, ABOUT = 2};
    enum MENU_ITEM { I_CHANGE_LEVEL = 0, I_ABOUT = 1, I_EXIT = 2};

    int option = INGAME_MENU;

    while(!levelSelected)
    {
        device->run();
        driver->beginScene(true, true, SColor(255,100,101,140));

        smgr->drawAll();
        guienv->drawAll();

        if(option == INGAME_MENU)
        {
            for(int i=0;i<3;i++)
            {
                switch(i)
                {
                    case I_CHANGE_LEVEL:
                        driver->draw2DImage(driver->getTexture("media/change_off.png"),position2di(driver->getScreenSize().Width/2 - 161,driver->getScreenSize().Height/2 - 150),rect<s32>(0,0,326,52),0,SColor(255,255,255,255),true);
                        if(menuItem==0) driver->draw2DImage(driver->getTexture("media/change_on.png"),position2di(driver->getScreenSize().Width/2 - 161,driver->getScreenSize().Height/2 - 150),rect<s32>(0,0,326,52),0,SColor(255,255,255,255),true);
                        break;
                    case I_ABOUT:
                        driver->draw2DImage(driver->getTexture("media/about_off.png"),position2di(driver->getScreenSize().Width/2 - 79,driver->getScreenSize().Height/2),rect<s32>(0,0,158,51),0,SColor(255,255,255,255),true);
                        if(menuItem==1) driver->draw2DImage(driver->getTexture("media/about_on.png"),position2di(driver->getScreenSize().Width/2 - 79,driver->getScreenSize().Height/2),rect<s32>(0,0,158,51),0,SColor(255,255,255,255),true);
                        break;
                    case I_EXIT:
                        driver->draw2DImage(driver->getTexture("media/exit_off.png"),position2di(driver->getScreenSize().Width/2 - 117,driver->getScreenSize().Height/2 + 150),rect<s32>(0,0,234,51),0,SColor(255,255,255,255),true);
                        if(menuItem==2)driver->draw2DImage(driver->getTexture("media/exit_on.png"),position2di(driver->getScreenSize().Width/2 - 117,driver->getScreenSize().Height/2 + 150),rect<s32>(0,0,234,51),0,SColor(255,255,255,255),true);
                        break;
                }
            }

            if( ((EventReceiver*)device->getEventReceiver())->isKeyPressed(KEY_UP) )
            {
                menuItem--;
                device->sleep(150);
            }

            if( ((EventReceiver*)device->getEventReceiver())->isKeyPressed(KEY_DOWN) )
            {
                menuItem++;
                device->sleep(150);
            }

            if(menuItem < 0) menuItem = 2;
            if(menuItem > 2) menuItem = 0;
        }
        else if(option == ABOUT)
        {
            driver->draw2DImage(driver->getTexture("media/about.jpg"),position2di(driver->getScreenSize().Width/2 - 320,driver->getScreenSize().Height/2 - 240));
        }


        if( ((EventReceiver*)device->getEventReceiver())->isKeyPressed(KEY_RETURN) )
        {
            if(option == ABOUT)
            {
                option = I_CHANGE_LEVEL;
            }
            else if (option == SELECT_LEVEL)
            {
                levelsBox->setVisible(false);
                option = I_CHANGE_LEVEL;

                loadLevel( stringc( levelsBox->getListItem(levelsBox->getSelected()) ) );
                resetGame();
                levelSelected = true;
            }
            else if(menuItem == I_ABOUT)
            {
                option = ABOUT;
            }
            else if(menuItem == I_EXIT)//EXIT
            {
                run = false;
                levelSelected = true;
            }
            else if(menuItem == I_CHANGE_LEVEL)
            {
                option = SELECT_LEVEL;
                levelsBox->setSelected(0);
                levelsBox->setVisible(true);
                guienv->setFocus(levelsBox);
            }

            device->sleep(150);
        }

        ((EventReceiver*)device->getEventReceiver())->flushKeys();

        driver->endScene();
    }
}

void App::setupResolution()
{
    screenWidth = 800;
    screenHeight = 600;
    fullScreen = false;

    //create a simple software based device
    IrrlichtDevice* cfgDevice = createDevice(EDT_SOFTWARE,dimension2du(250,240),16,false);
    //EventReceiver* cfgRecv = new EventReceiver(cfgDevice);
    //cfgDevice->setEventReceiver(cfgRecv);
    cfgDevice->setWindowCaption(L"MudBall - Seetings");

    //remove the fuckin gui alpha
    for (s32 i=0; i<gui::EGDC_COUNT ; ++i)
    {
        video::SColor col = cfgDevice->getGUIEnvironment()->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
        col.setAlpha(200);
        cfgDevice->getGUIEnvironment()->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
    }


    IGUIComboBox* cb = cfgDevice->getGUIEnvironment()->addComboBox(rect<s32>(10,10,240,50));

    IVideoModeList *modeList = cfgDevice->getVideoModeList();

    //we will store the video modes here
    vector<vMode> vModes;

    //scan available video modes and populate the comboBox
    for ( int i = 0; i != modeList->getVideoModeCount (); ++i )
    {
        u16 d = modeList->getVideoModeDepth ( i );
        if ( d < 16 ) continue;

        u16 w = modeList->getVideoModeResolution ( i ).Width;
        u16 h = modeList->getVideoModeResolution ( i ).Height;

        bool vModeAlreadyExist = false;

        //check if the video mode is unique (16 or 32 bits and refresh rate does not matters)
        for(int j=0;j<vModes.size();j++)
        {
            if( vModes[j].w == w && vModes[j].h == h ) vModeAlreadyExist = true;
        }

        if(!vModeAlreadyExist)
        {
            stringw modeName = stringw(w);
            modeName.append(" x ");
            modeName.append( stringw(h) );

            cb->addItem(modeName.c_str());

            vMode v;
            v.w = w;
            v.h = h;

            vModes.push_back(v);
        }
    }

    IGUIComboBox* cbUseEffects = cfgDevice->getGUIEnvironment()->addComboBox(rect<s32>(10,60,240,100));
    cbUseEffects->addItem(L"DISABLE SPECIAL EFFECTS");
    cbUseEffects->addItem(L"ENABLE SPECIAL EFFECTS");


    IGUIComboBox* cbFullscreen = cfgDevice->getGUIEnvironment()->addComboBox(rect<s32>(10,110,240,160));
    cbFullscreen->addItem(L"WINDOWED");
    cbFullscreen->addItem(L"FULLSCREEN");

    IGUIButton* btOK = cfgDevice->getGUIEnvironment()->addButton(rect<s32>(140,170,240,220),0,233,L"OK",L"RUN GAME WITH CURRENT SEETINGS");
    IGUIButton* btExit = cfgDevice->getGUIEnvironment()->addButton(rect<s32>(10,170,100,220),0,233,L"EXIT",L"EXIT TO SYSTEM");

    bool resolutionOk = false;

    while(!resolutionOk && cfgDevice->run())
    {
        cfgDevice->getVideoDriver()->beginScene(true,true,SColor(100,100,100,100));
        cfgDevice->getGUIEnvironment()->drawAll();
        cfgDevice->getVideoDriver()->endScene();

        if(btOK->isPressed())
        {
            resolutionOk = true;
            screenWidth = vModes[cb->getSelected()].w;
            screenHeight = vModes[cb->getSelected()].h;
            useEffects = cbUseEffects->getSelected() == 0 ? false : true;
            fullScreen = cbFullscreen->getSelected() == 0 ? false : true;
        }

        if(btExit->isPressed())
        {
            break;
        }
    }

    cfgDevice->drop();

    if(!resolutionOk) exit(0);
}

int App::getdir(stringc dir, vector<stringc> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error opening levels directory" << endl;
        exit(0);
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(stringc(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void App::setupBulletPhysics()
{
    world = new irrBulletWorld(device,true,true);
}

void App::clearBulletPhysics()
{
    if(world)
    {
        ///TODO: Fix me: crash when delete world here
        //delete world;
        world = NULL;
    }
}

void App::setupBall()
{
    if(!world)
    {
        cout << "SETUP BULLET PHYSICS FIRST!" << endl;
        exit(0);
    }

    ball = smgr->addSphereSceneNode(0.5);
    ball->setPosition(vector3df(0,-500,0));
    ball->setMaterialTexture(0,driver->getTexture("media/ball.jpg"));

    ballShape = new ISphereShape(ball,1);
    ballBody = world->addRigidBody(ballShape);
    ballBody->forceActivationState(EAS_DISABLE_DEACTIVATION);
}

void App::removeBall()
{
    if(ball)
    {
        ball->remove();
        ball = NULL;
    }
}

void App::setupEffects()
{
    if(useEffects)
    {
        effect = new EffectHandler(device, driver->getScreenSize(), false, true);
        effect->setAmbientColor(SColor(255, 32, 32, 32));

        for(int i=0;i<level.size();i++)
        {
            ISceneNode* n = level[i]->getSceneNode();

            if(level[i]->getType() == EELEVATOR) ((EElevator*)level[i])->addShadow(effect);
            if(level[i]->getType() == EELEVATOR2X) ((EElevator2X*)level[i])->addShadow(effect);
            if(level[i]->getType() == EWIND) ((EWind*)level[i])->addShadow(effect);

            effect->addShadowToNode(n, EFT_4PCF,ESM_BOTH);
        }

        effect->setClearColour(SColor(255, 250, 100, 0));

        effect->addShadowLight(SShadowLight(1024, ball->getPosition() + vector3df(0,5,0), ball->getPosition(),
		SColor(0, 255, 255, 255), 5.0f, 100.0f, 60.0f * DEGTORAD));

		effect->addShadowToNode(ball, EFT_4PCF,ESM_BOTH);
    }
}

void App::clearEffects()
{
    if(useEffects)
    {
        delete effect;
    }
}
