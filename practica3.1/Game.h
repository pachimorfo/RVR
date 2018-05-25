#include <pthread.h>
#include <map>

#include <SFML/Graphics.hpp>

#include "UDPServer.h"
#include "Serializable.h"

//------------------------------------------------------------------------------
// Definición de Constantes
//   Tipo de Objeto: PLAYER, SHOOT (debería definirse como enum)
//   WINDOW_SIZE: Tamaño de ventana
//------------------------------------------------------------------------------
#define PLAYER 0
#define SHOOT  1

#define WINDOW_SIZE 400
#define PI 3.14159265
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 *  GameObject representa un objeto genérico del juego.
 *    - type: Clase del Objecto del juego: PLAYER, SHOOT
 *    - id: Identificador único del objeto
 *
 *    - x,y: Posición del objeto
 *    - a: Ángulo orientación
 *
 *  Este objeto también lo utilizaremos para representar la entrada que envía
 *  cliente en este caso:
 *    - x: FORWARD. Desplazamiento en el eje de la dirección.
 *    - y: SHOOT. 1 = disparo y 0 = no disparo
 *    - a: ROTATE. Ángulo de giro +/- 5 grad.
 */
class GameObject : public Serializable
{
public:
    GameObject(int16_t t, int16_t o):type(t), id(o), x(0), y(0), a(0){};

    GameObject():type(0), id(0), x(0), y(0), a(0){};

    virtual ~GameObject(){};

    //--------------------------------------------------------------------------
    // Atributos para la Replicación
    //--------------------------------------------------------------------------
    int16_t type;
    int16_t id;

    //--------------------------------------------------------------------------
    // Atributos del Objeto. Posición
    //--------------------------------------------------------------------------
    int16_t x;
    int16_t y;

    int16_t a;

    //--------------------------------------------------------------------------
    // Interfaz serializable
    //--------------------------------------------------------------------------
    void to_bin();

    int from_bin(char * data);

    //--------------------------------------------------------------------------
    // Interfaz GameObject
    //--------------------------------------------------------------------------
    virtual void render(sf::RenderWindow& window){};

    virtual void move(){};
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/**
 *  Esta clase implementa el paradigma Client-Proxy. En este caso la clase sólo
 *  guarda 1 mensaje de entrada en general incluiría un secuencia de entradas
 *  ordenadas.
 *
 *  NOTA: Este objeto sólo se usa en el servidor, no se envía por la red así
 *  que no implementa el interfaz Serializable.
 *  - _xy_d: entrada FORWARD, representa el desplazamiento en la dirección a
 *  - _a_d: entrada ROTATE,  es la variación (delta) en el ángulo de orientación
 *  - _shoot: entrada SHOOT, si el jugador ha disparado o no
 *
 */
class Player : public GameObject
{
public:
    Player(int16_t o):GameObject(PLAYER, o), _xy_d(0), _a_d(0), _shoot(0){};

    virtual ~Player(){};

    //--------------------------------------------------------------------------
    // Proxy Client
    //--------------------------------------------------------------------------
    int16_t _xy_d;

    int16_t _a_d;

    int16_t _shoot;

    //--------------------------------------------------------------------------
    // Interfaz del objeto para el juego
    //--------------------------------------------------------------------------
    void render(sf::RenderWindow& window);

    void move();
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/**
 *  Esta clase representa una bala. Simplemente expone el interfaz del juego
 *  para el objeto.
 */
class Shoot : public GameObject
{
public:
    Shoot(int16_t o):GameObject(SHOOT, o){};

    virtual ~Shoot(){};

    //--------------------------------------------------------------------------
    // Interfaz del objeto para el juego
    //--------------------------------------------------------------------------
    void render(sf::RenderWindow& window);

    void move();
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/**
 * Esta clase representa el mundo del juego será un array de GameObjects
 *
 *   +-------------+-------------+-------------+---
 *   | PLAYER ID:1 | SHOOT ID:23 | PLAYER ID:2 |...
 *   +-------------+-------------+-------------+--
 *
 * Está implementado con un std::map indexado por el ID del objeto.
 *
 * El objeto GameWorld se actualiza en cada paso de simulación. Además se
 * serializa y se envía a los clientes para que lo presenten a los jugadores.
 *
 * NOTA: Este objeto puede usarse por varios threads, cada modificación de
 * hacerse bloqueando el mutex asociado
 */
class GameWorld : public Serializable
{
public:
    GameWorld()
    {
        pthread_mutex_init(&mutex, 0);
        iDtotal = 0;
    }

    ~GameWorld()
    {
        pthread_mutex_destroy(&mutex);
    }

    //--------------------------------------------------------------------------

    void update_player_proxy(Player *ply);

    //--------------------------------------------------------------------------
    // Interfaz del objeto para el juego
    //--------------------------------------------------------------------------
    void render(sf::RenderWindow& window);

    void simulate();

    //--------------------------------------------------------------------------
    // Interfaz serializable
    //--------------------------------------------------------------------------
    void to_bin();

    int from_bin(char * data);

private:

    std::map<int16_t, GameObject *> gos;

    pthread_mutex_t mutex;

    int iDtotal;
};

