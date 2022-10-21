//MudBall-mod https://github.com/dualword/mudball
#include <iostream>
#include "collisionobjectaffector.h"
//#include "irrbullet_compile_config.h"

ICollisionObjectAffector::ICollisionObjectAffector()
{
    #ifdef IRRBULLET_DEBUG_MODE
        printf("irrBullet: Creating object affector\n");
    #endif
}

ICollisionObjectAffector::~ICollisionObjectAffector()
{
    #ifdef IRRBULLET_DEBUG_MODE
        printf("irrBullet: Deleting object affector\n");
    #endif
}
