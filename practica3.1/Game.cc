#include <iostream>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sstream>

#include "Game.h"

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación class GameObject (serialización)
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void GameObject::to_bin()
{

	int32_t total = ( 5 * sizeof(int16_t));
	alloc_data(total);

	char* pointer = _data;

	memcpy(pointer,&type, sizeof(int16_t));
	pointer = _data + sizeof(int16_t);

	memcpy(pointer,&id, sizeof(int16_t));
	pointer = _data + sizeof(int16_t);

	memcpy(pointer,&x, sizeof(int16_t));
	pointer = _data + sizeof(int16_t);

	memcpy(pointer,&y, sizeof(int16_t));
	pointer = _data + sizeof(int16_t);

	memcpy(pointer,&a, sizeof(int16_t));
	pointer = _data + sizeof(int16_t);



}

int GameObject::from_bin(char * d)
{

	//De momento no escribimos la cabecera en el buffer,
	//asi que no saltamos la cabecera.

	char* pointer = d;

	memcpy(&type,pointer, sizeof(int16_t));
	pointer += sizeof(int16_t);


	memcpy(&id,pointer, sizeof(int16_t));
	pointer += sizeof(int16_t);


	memcpy(&x,pointer, sizeof(int16_t));
	pointer += sizeof(int16_t);


	memcpy(&y,pointer, sizeof(int16_t));
	pointer += sizeof(int16_t);


	memcpy(&a,pointer, sizeof(int16_t));
	pointer += sizeof(int16_t);

	return 0;




}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación class Player
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static sf::Texture * get_player_tank(int16_t id)
{
    static bool texture_loaded = false;

    static std::vector<sf::Texture *> tanks;

    if (!texture_loaded)
    {
        for (int i = 0; i < 4 ; i++)
        {
            std::ostringstream file;

            sf::Texture * t = new sf::Texture();
            file << "tank" << i << ".png";

            t->loadFromFile(file.str());

            tanks.push_back(t);
        }

        texture_loaded = true;
    }

    int16_t to = id % 4;

    return tanks[to];
}

void Player::render(sf::RenderWindow& window)
{
    sf::Sprite tank;

    tank.setTexture(*(get_player_tank(id)));
    tank.setColor(sf::Color(255,255,255,255));
    tank.setScale(0.25, 0.25);
    tank.setOrigin(48,103);
    tank.setPosition(x, y);
    tank.setRotation(a-90);

    window.draw(tank);
}

// ----------------------------------------------------------------------------
/**
 *  Esta función realiza el movimiento del jugador en cada paso de simulación
 *  debe actualizar la posición (x,y) según la entrada (Client-Proxy)
 *    x = x + Dxy cos(a * PI/180)
 *    y = y + Dxy sin(a * PI/180)
 *
 *  Esta función debe controlar los límites de la ventana.
 *
 *  Opciones movimiento:
 *    - Cada pulsación provoca un sólo avance (Dxy = 0) después de mover
 *    - Dejar Dxy (movimiento continuo) hasta que haya un giro o se llegue al
 *      borde
 */
void Player::move()
{




}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación class Shoot
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Shoot::render(sf::RenderWindow& window)
{
    sf::CircleShape circle(2);

    circle.setPosition(x, y);

    circle.setFillColor(sf::Color::Black);
    circle.setPosition(x, y);

    window.draw(circle);
}

/**
 *  Movimiento de la bala. Mismas ecuaciones de movimiento que player
 */
void Shoot::move()
{
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación class GameWorld
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void GameWorld::render(sf::RenderWindow& window)
{
    static sf::Texture t;
    static sf::Sprite bg;

    static bool initialized = false;

    if ( !initialized )
    {
        t.loadFromFile("sand.jpg");

        bg.setTexture(t);

        initialized = true;
    }

    pthread_mutex_lock(&mutex);

    window.clear(sf::Color::White);

    window.draw(bg);

    std::map<int16_t, GameObject *>::iterator it;

    for (it = gos.begin(); it != gos.end() ; ++it)
    {
        it->second->render(window);
    }

    window.display();

    pthread_mutex_unlock(&mutex);
}

// ----------------------------------------------------------------------------
/**
 *  Simulación del mundo. Recorre cada objeto para:
 *    1. realizar el movimiento en el paso (->move())
 *    2. Si es PLAYER y _shoot ==1.
 *      2.1. Crear Shoot en la posición del jugador
 *      2.2. Asignar un ID único a la bala
 *      2.3  Insertar objeto en el map del mundo.
 *    3. Si es SHOOT, comprobar si está fuera de limites para eliminarlo
 *
 *  NOTA: Revisar cómo eliminar elementos de un map en una iteración, para
 *  no invalidar el iterador.
 */
void GameWorld::simulate()
{

	//REVISAR DONDE PONER LOS MUTEX, AL PRINCIPIO DEL TODO?
	///SOLO LEEMOS, ASI QUE QUIZAS NO, SOLO AL MODIFICAR COSAS DEL GAMEWORLD
	//DEBEMOS PONER EL MUTEX

	std::map<int16_t, GameObject*>::iterator* it = gos.begin();
	std::map<int16_t, GameObject*>::iterator* itEnd = gos.end();

	while(it != itEnd){

		GameObject* gO = (*it)->second;
		gO->move();
		if(gO->type == PLAYER){
			Player* p = static_cast<Player*>(gO);
			if(p->_shoot){

				pthread_mutex_lock(&mutex);

				Shoot* s = new Shoot(iDtotal);
				s->x = p->x;
				s->y = p->y;

				gos.insert(std::pair<int16_t, GameObject*>(iDtotal,s));
				iDtotal++;

				pthread_mutex_unlock(&mutex);

			}
		}else{
			//if(gO->x >)



		}


		it++;
	}




}

// ----------------------------------------------------------------------------
/**
 *  Actualización del proxy de cliente, buscar al jugador por su ID:
 *    1. Si el jugador no está se asumirá mensaje login. Añadir al jugador
 *    al mapa, inicializar (x,y) aleatoriamente
 *    2. si el jugador está actualiar (_xy_d, _shoot, _a_d) con (x, y, a) de ply
 *
 */
void GameWorld::update_player_proxy(Player *ply)
{
}

// ----------------------------------------------------------------------------
/**
 *  Recorrer todos los objetos y serializarlos. Cada objeto serializarlo
 *  copiarlo en el buffer (_data)
 *
 *  NOTA: Todos los objetos se serializan con el mismo tamaño
 */
void GameWorld::to_bin()
{
}

// ----------------------------------------------------------------------------
/**
 *  1. Eliminar todos los objetos del map
 *  2. Calular el numero de objetos recibidos en el buffer, usando el
 *  tamaño de los datos (en la cabecera) y el tamaño de cada objeto
 *  3. Iterar para todos los objetos, avanzando el puntero en el buffer.
 *  Reconstruir un objeto genérico, y según el tipo crear el objeto adecuado,
 *  copiando los datos x,y,a.
 *  4. Insertar el objeto en el mapa
 */
int GameWorld::from_bin(char * d)
{
}


