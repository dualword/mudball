#include <stdio.h>
#include <stdlib.h>

#include "src/App.h"
#include "src/wiiuse.h"

int main()
{
    App::getInstance()->runGame();
    return 0;
}
