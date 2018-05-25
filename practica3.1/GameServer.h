#include <pthread.h>
#include <map>

#include "UDPServer.h"
#include "Game.h"

/**
 *  La clase GameServer representa el servidor dedicado del juego. La
 *  funcionalidad de red se implmenta mediante el interfaz de UDPServer (Prác-
 *  tica 2.3).
 *
 *  Esta clase se encarga de realizar además la simulación del juego.
 *
 */
class GameServer : public UDPServer
{
private:
    /**
     *  Estado del juego. Se actualiza en cada tick y se envía a los clientes
     *  de forma completa.
     */
    GameWorld gw;

public:
    /**
     *  Constructor:
     *    @param s, nombre del servidor (e.g. "localhost" o "192.168.0.1")
     *    @param p, puerto en el que escucha (e.g. "8080")
     */
    GameServer(const char * s, const char * p):UDPServer(s, p){};

    virtual ~GameServer(){};

    /**
     *  Tratamiento del mensaje. Los clientes envían las ordénes del tanque
     *  (girar, disparar, avanzar):
     *    1. Crear un clase Player (Client Proxy)
     *    2. Deserializar el mensaje en la clase
     *    3. Actualiza el proxy de cliente para el jugador
     */
    void do_message(char * buffer)
    {
    }

    /**
     *  Función envoltorio para ejecutar un paso de simulación
     */
    void simulate()
    {
        gw.simulate();
    }

    /**
     *  Esta función recorre la lista de conexiones (gestionada por la clase
     *  UDPServer) y envía el mundo del juego serializado después de cada paso
     *  de simulación
     *      1. Serializar el mundo
     *      2. Para cada conexión, enviar mundo actualizado. En caso de error
     *      eliminar conexión de la lista
     */
    void broadcast();
};

