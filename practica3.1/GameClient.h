#include <pthread.h>
#include <map>

#include <SFML/Graphics.hpp>

#include "Socket.h"
#include "Game.h"

class GameClient
{
private:
    /**
     *  Identificador único del cliente en el juego
     */
    int16_t id;

    /**
     *  Copia del mundo del juego. Enviada en cada actualización por el servidor
     */
    GameWorld gw;

    /**
     *  Clase Socket que representa la conexión con el servidor del juego
     */
    Socket cs;

    /**
     *  Ventana para pintar el mundo del juego
     */
    sf::RenderWindow window;

public:
    GameClient(const char * s, const char * p, int16_t i):id(i), cs(s, p),
        window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "RVR - Tanks")
    {
        window.setActive(false);
    };

    /**
     *  Esta función recibe las actualizaciones del servidor y las renderiza.
     *  Asume un actualización constante a 30Hz.
     *
     *  NOTA: En situaciones de red reales NUNCA se acopla el render con la
     *  recepción de los mensajes.
     *     1. Recivir Mensaje
     *     2. Deserializar
     *     3. Pintar
     *
     */
    void network_render_thread()
    {

    }

    /**
     *  Esta función simula un inicio de sesión en el servidor del juego:
     *    1. Crear un objeto Player con x=y=a=0 y el id del jugador.
     *    2. Enviar este mensaje al servidor del juego para registart el cliente
     */
    void login()
    {
        Player ply(id);

        ply.x = 0;
        ply.y = 0;
        ply.a = 0;

        ply.to_bin();

        cs.send(&ply, &cs);
    }

    /**
     * Lee el teclado y manda actualizaciones a 30Hz como máximo
     */
    int input();
};

