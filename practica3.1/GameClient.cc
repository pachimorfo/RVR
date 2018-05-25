#include "GameClient.h"
#include <unistd.h>

extern "C" void * _net_thread(void *arg)
{
    GameClient * gc = static_cast<GameClient *>(arg);

    while ( true )
    {
        gc->network_render_thread();
    }

    return 0;
}

int GameClient::input()
{
    sf::Event event;

    int16_t a = 0;
    int16_t x = 0;
    int16_t y = 0;

    if (window.waitEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                return -1;

            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                    case sf::Keyboard::Right:
                        a = 5;
                    break;
                    case sf::Keyboard::Left:
                        a = -5;
                    break;
                    case sf::Keyboard::Up:
                        x = 3;
                    break;
                    case sf::Keyboard::Down:
                        y = 1;
                    break;
                }
                break;

            default:
                break;
        }
    }

    if ( a != 0 || x != 0 || y != 0 )
    {
        Player ply(id);

        ply.x = x;
        ply.y = y;
        ply.a = a;

        ply.to_bin();

        cs.send(&ply, &cs);
    }

    usleep(33000);

    return 0;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    GameClient gc(argv[1], argv[2], atoi(argv[3]));

    gc.login();

    pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&id, &attr, _net_thread, static_cast<void *>(&gc));

    while (gc.input() == 0){};
};
