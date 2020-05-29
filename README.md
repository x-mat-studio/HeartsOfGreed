# HEARTS OF GREED

Hearts of Greed is a commando-like RTS game in which you'll have to resist the attack of a hostile alien force. Use your select few hero units, your bases, turrets and upgrades to stand your ground and conquer enemy territory. 
During the day, press on, be agressive and try to defeat the enemy. However, once night comes... go back to your base and defend from the incoming attacks. It's not your time to shine.

Good Luck!

Disclaimer: play using headphones.

This project is made as a 2nd year assigment in Bachelor’s Degree in Video Games by UPC at CITM, Terrassa, under the supervision of lecturer Marc Garrigó (www.linkedin.com/in/mgarrigo/).


## What to do
Start the game by playing the tutorial. Next, use your heroes skillfully to conquer all enemy bases and win the game!
You can unlock extra heroes by completing their rescue mission (keep an eye on the minimap!)

If all your heroes die, you will lose the game.


## Check our project!
Github:		https://github.com/x-mat-studio/HeartsOfGreed
Twitter:	https://twitter.com/ii_rts 


## Game Keys:

### Control:

* WASD
	- Control camera

* Cursor next to screen margin: 
	- Control camera

* Click on minimap:		
	- Teleport camera to that position

* Mouse wheel:			
	- Zoom in/out

* Click & drag:			
	- Select Unit group

* Click on anything:		
	- Select

* E: 				
	- Prepare focused hero skill.

* (when preparing skill)
	- Left click : 	Use skill.
	- Right click : Cancel skill.

* Tab: Change focus between the selected heroes.

* R: When building barricades, rotate them.

* U: Focus Gatherer Hero

* I: Focus Ranged Hero

* O: Focus Melee Hero

* P: Focus Robot Hero

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
* 1: Spawn gigamingo.
* 2: Spawn fast wannamingo.
* 3: Spawn ranged wannamingo.
* 4: Toggle debug build mode for player.
* 9: Activate/Deactivate camera movement by mouse.
* KP 1: Spawn base.
* KP 2: Spawn decorative building.
* KP 3: Spawn enemy.
* KP 4: Spawn gatherer hero (Suit Man).
* KP 5: Spawn melee hero (Armored Man).
* KP 7: Spawn turret.
* L: Level up all heroes.
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
* Save / Load functionallity.
* InGame Support for windowed mode and fullscreen.
* Automatic Continuous Integration and Deployment .
* Logo Scene video, with custom art and disctintive audio.
* Title Scene, with custom title spatial sound and options and easy in.
* Implementation of the final map.
* Progression system for hero units: lvl, stat boosters, skill upgrades...
* Quest system implemented to unlock different heroes.
* Paused game on ingame menu .
* Particle system used in lvl up scenario.
* Three resources implemented:
	- **Blue resource**: Used for building turrets, barricades, upgrade center, and revive dead heroes.
	- **Red resource**: Used to upgrade hero related stats.
	- **Purple resource**: Used to level up hero skills (active and passive, from 1 to 4).
* Four different hero units, each one with two disctintive skills:
	- **Armored Man**: A melee attack with an AoE effect, hight health regeneration and receioves less damage the more enemies surround him.
	- **Suit Man**: Throws a ranged grenade which explodes on contact. This hero gets extra resources for killing enemies and overcharges near robotic allies.
	- **Ranged**: Hit and run specialist, makes enemies bleed after hitting them, and is equipped with a grenade that heavily slows enemies.
	- **Robot**: Can´t regenerate health, however, gains speed and attack when killing enemies in succession, his skill is self destruction (massive AoE).
* Group selection and movement for ally units, group movement for enemy units.
* Multiple buildings implemented:
	- **Ally base**: Center of operations, you can build and revive heroes here.
	- **Enemy base**: Your opponents stronghold wich they will defend if your heroes get near, deplete it's health to make it your own.
	- **Turrets**: Building which attacks entities of it's contrary alligment.
	- **Decorative Buildings**: Flesh out our map and create chokepoints in the map.
	- **Barricades**: Defensive structures with high hp.
	- **Upgrade center**: Lets you buy and level up turrets and barricades. 
* Win condition: Conquer all bases.
* Win scene: custom made "you win message" and track  .
* Lose condition: triggered by losing all units or losing all bases.
* Lose scene: custom made "you lose message" and track  .
* HUD implementation showing resources, heroes, cycle state...
* Full fade-in / fade-out audio
* **Real Time minimap** implemented: 
	- Showing icons for heroes, enemies and important structures.
 	- Showing rendered images of terrain and decorative buildings.
	- Last seen image endures in past FoW.
* **Day / Night cycle**. During night, enemies spawn and attack player bases.
* All units and buildings display a Portrait in the bottom right corner showing their Healthbar and specific characteristics.
* **Spatial audio**: units on screen or out of it (but still close to the center) will make sounds of different volume and origin.
* Zoom in/out of map (mouse wheel).
* IA driven enemies, with priority settings and objectives.
* Spawner system which generates increasingly more dangerous enemies.
* Dialogue system, with crafted portraits and UI; all tied up ingame via quests.
* Different easings implemented in numerous points: audio, night cycle, win/lose animations...

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
