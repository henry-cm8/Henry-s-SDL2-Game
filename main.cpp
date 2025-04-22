#include <iostream>
#include "Engine.h"

int main(int argc, char* argv[])
{
    Engine Game;
    if(!Game.Init())
    {
        std::cerr << "Failed to initialize game engine.\n";
        return -1;
    }

    while (Game.IsRunning())
    {
        Game.HandleEvents();
        Game.Update();
        Game.Render();
    }
    Game.Clean();

    return 0;
}


