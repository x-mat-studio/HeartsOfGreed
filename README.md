﻿# HEARTS OF GREED

Hearts of Greed is a commando-like RTS game in which you'll have to resist the attack of a hostile alien force. Use your select few hero units, your bases, turrets and upgrades to stand your ground and conquer enemy territory. 
During the day, press on, be agressive and try to defeat the enemy. However, once night comes... go back to your base and defend from the incoming attacks. It's not your time to shine.

Good Luck!

Disclaimer: play using headphones.

This project is made as a 2nd year assigment in Bachelor’s Degree in Video Games by UPC at CITM, Terrassa, under the supervision of lecturer Marc Garrigó (www.linkedin.com/in/mgarrigo/).


## What to do
To play this vertical slice, you need to micromanage your two hero units to conquer the enemy base. Once that happens, go back to your first base and defend it (maybe build a turret to help you). Once all enemies are dead, you've won.
If all your heroes die, you will lose the game.


## Check our project!
Github:		https://github.com/x-mat-studio/HeartsOfGreed
Twitter:	https://twitter.com/ii_rts 


## Game Keys:

### Control:

* WASD:			      	
	- Control camera
* Cursor next to screen margin: 
	- Control camera
* Click on minimap:		
	- Teleport to that position
* Mouse wheel:			
	- Zoom in/out
* Click & drag:			
	- Select Unit group
* Click on anything:		
	- Select

* E: 				
	- Prepare focused hero skill.
*(when preparing skill)
	- Left click : 	Use skill.
	- Right click : Cancel skill.

* Tab: Change focus between the selected heroes.

### Debug Options (only in-game):

* F1: Debug texture for Fog of war (only visible when on Fog of War mode).
* F2: Reset fog of war(only visible when on Fog of War mode).
* F3: Activate/Deactivate Fog of War mode.
* F4: Full screen on / off
* F6: Kill all enemies.
* F7: Activate/Deactivate collider and pathfinding vision.
* F8: Deactivate night.
* F9: Activate night.
* F10: Activate heroes God mode.
* F11: Deactivate heroes God mode.
* 0: Give resources to player.
* 4: Toggle debug build mode for player.
* 9: Activate/Deactivate camera movement by mouse.
* KP 1: Spawn base.
* KP 2: Spawn decorative building.
* KP 3: Spawn enemy.
* KP 4: Spawn gatherer hero (Suit Man).
* KP 5: Spawn melee hero (Armored Man).
* KP 7: Spawn turret.
* L: Level up all heroes by 1.
* N: Win if in-game.
* M: Lose if in-game.


## Team Members:

### Aaron Guerrero Cruz
* Lead
* [AaronGCProg](https://github.com/AaronGCProg)


### Jose Luís Redondo Tello
* Code + UI
* [jose-tello](https://github.com/jose-tello)


### Ferran-Roger Basart i Bosch
* Management + UI
* [ferba93](https://github.com/ferba93)


### Àlex Melenchón Maza
* Design
* [AlexMelenchon](https://github.com/AlexMelenchon)


### Adrià Serrano López
* Audio + Art
* [adriaserrano97](https://github.com/adriaserrano97)


### Oscar Pérez Martín
* QA
* [oscarpm5](https://github.com/oscarpm5)

 
## Features

* Fade in/out between scenes.
* Multiple debug keys (listed above).
* InGame Support for windowed mode and fullscreen.
* Automatic Continuous Integration and Deployment .
* Logo Scene, with custom art and disctintive audio.
* Title Scene, with custom title spatial sound and options.
* First implementation of a map showing our core game loop.
* **Single resource** which is used for building turrets.
* Two different hero units, each one with one disctintive skill:
	- **Armored Man**: A melee attack with an AoE effect.
	- **Suit Man**: Throws a grenade in ranged which explodes on contact. This hero gets resources on killing enemies.
* **Group selection and movement** for ally units.
* Multiple buildings implemented:
	- **Ally base**: Center of operations, you can build and upgrade here.
	- **Enemy base**: Your opponents stronghold wich they will defend if your heroes get near, deplete it's health to make it your own.
	- **Turrets**: Building which attacks entities of it's contrary alligment.
	- **Decorative Buildings**: Flesh out our map and create chokepoints in the map.
* Win condition: triggered by killing all enemies during night.
* Win scene: custom made "you win message" and track  .
* Lose condition: triggered by losing all units or losing all bases.
* Lose scene: custom made "you lose message" and track  .
* HUD implementation showing resources, heroes, cycle state...
* **Real Time minimap** implemented: 
	- Showing icons for heroes, enemies and important structures.
 	- Showing rendered images of terrain and decorative buildings.
	- Last seen image endures in past FoW.
* **Day / Night cycle**. During night, enemies spawn and attack player bases.
* All units and buildings display a Portrait in the bottom right corner showing their Healthbar and specific characteristics.
* **Spatial audio**: units on screen or out of it (but still close to the center) will make sounds of different volume and origin.
* Zoom in/out of map (mouse wheel).

_________________________________________________________________________________________________________________________________

## Changelog

### Release 0.0.3

#### General Updates
- Skeleton Code Implemented.
- All the variables, functions, enums and classes in the project are now renamed using the coding nomenclature criteria.


#### Bug Fixes
- Solved a bug that made the game crash when trying to close it with the cross button
- Adjusted the spacing between functions to be consistent with the coding nomenclature criteria.
- Modified some constructors to be consistent with the coding criteria.
. . .

### Release 0.3.19

#### General Updates
- Entities & Entity Manager implemented.
- Basic AI for enemies (follow objective & attack it)
- Pathfinding added to units (HPA* 1 layer)
- Animations for all entities
- UI skeleton implemented (Manager & Base classes)
- Re-sizeable screen added (fullscreen)
- Group Movement & Event manager added


#### Bug Fixes
- A lot of entities bugfixes
- Adjusted UI values & Healthbar crash fixed
- Corrected some animations
. . .

### Release 0.3.20

#### General Updates
- Upgrades folder structure
- Deleted some useless files

#### Bug Fixes
. . .

### Release 0.3.23

#### General Updates
- Added Minimap
- UI menus & Specific elements (with custom functionallity)
- Day/Night cycle added
- AI for the night implemented
- Skills & Areas added
- Added Turrets, Bases & Buildings as working entities 
- Debug & God Mode added
- Finished the scenes (intro, main menu, gameplay & win/lose)
- Level Up system
- AI for heroes

#### Bug Fixes
- Fixed a bunch of audio effects
- Adjusted UI values & Healthbar crash fixed
- Player select logic upgraded
- Fixed a lot of general code bugs & memory leaks
- Adjusted pathfinding values

. . .

### Release 0.3.24

#### General Updates
- Animations now sync with attacks
- Skills pollish
- Healthbar now updates
- Added portraits for turrets 

#### Bug Fixes
- Adjusted animations to match player's feedback
- Adjusted UI values & Healthbar crash fixed
- Player select logic bug fix
- Fixed & adjusted values in the scenes outside gameplay
- All entities' logic & state machine upgraded
- Fixed some std::Vector miss use
- Fixed some animations
- Now buildings are beign printed in the minimap

. . .


### Release 0.3.27

#### General Updates
- UI & logic for the shop
- You can buy & place turrets in an area!
- Added fully functional Spawners
- Healthbar & UI portraits update in real time
- Menus can now we open & closed w/ ESC
- Map re-do
- LevelUp bonifications added

#### Bug Fixes
- Now audio channels are correctly used
- Fixed some memory leaks & release crashes
- Fog of War correctly works with entities
- UI fixes & improvements
- Adjusted UI values & Healthbar crash fixed
- CleanUp() of some modules are now correctly executed
- Adjusted pathfinding values
- Fixed Heroes' state machine
- Events are now being unregistered correctly.
- Fixed some turret display bugs
- Upgraded construction area mode
- Skill now consume energy
- HPA* now deletes nodes the right way
- Polishes pathfinding algorithms
- Polished armored hero skill
- Fixed animations

. . .


## Credits

### All spritesheets and other images not 100% done by us are from Fallout 1-3 and can be found here:
https://www.spriters-resource.com/
https://itch.io/
https://opengameart.org/

### All sounds are from fallout 1-3 and can be found here:
https://www.sounds-resource.com/pc_computer/fallout3/
https://www.sounds-resource.com/pc_computer/fallout2/
https://www.sounds-resource.com/pc_computer/fallout/

Thanks to Juan Hernández and David Carrasquet for dorming part of the team during the first sprint of the project and contributing in the wiki creation.
	

## License:
MIT License
Check the full license in our Github project
