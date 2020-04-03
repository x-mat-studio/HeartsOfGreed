#pragma once

#ifndef __SPAWNER_H__
#define __SPAWNER_H_


#include "Entity.h"
#include "Enemy.h"


class Spawner : public Entity
{
public:

    Spawner(Enemy* prototype);
        
    Enemy* spawnEnemy();
    Enemy* spawnEnemy(fMPoint positionToBe);
    Enemy* spawnEnemy(int x, int y);  

private:

    Enemy* prototype; 
 
    /*

    We need a reference. This enemy needs to be somewhere "hidden" ingame
    Enemy* meleePrototype = new Enemy(7000, 7000);
    Spawner* 1stSpawner = new Spawner(meleePrototype);

    */

};




#endif //__SPAWNER_H_
