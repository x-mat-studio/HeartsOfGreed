# HEARTS OF GREED

Hearts of Greed is a commando-like RTS game in which you'll have to resist the attack of a hostile alien force. Use your select few hero units, your bases, turrets and upgrades to stand your ground and conquer enemy territory. 
During the day, press on, be agressive and try to defeat the enemy. However, once night comes... go back to your base and defend from the incoming attacks. It's not your time to shine.

Good Luck!

Disclaimer: play using headphones.

This project is made as a 2nd year assigment in Bachelor’s Degree in Video Games by UPC at CITM, Terrassa, under the supervision of lecturer Marc Garrigó (www.linkedin.com/in/mgarrigo/).



	## Check our project!
Github:		https://github.com/x-mat-studio/HeartsOfGreed
Twitter:	https://twitter.com/ii_rts 


	## Game Keys:

### Control:

WASD:			      	Control camera
Cursor next to screen margin: 	Control camera
Click on minimap:		Teleport to that position
Mouse wheel:			Zoom in/out
Click & drag:			Select Unit group
Click on anything:		Select

### Debug Options (only in-game):

F1: Debug texture for Fog of war (only visible when on Fog of War mode).
F2: Reset fog of war(only visible when on Fog of War mode).
F3: Activate/Deactivate Fog of War mode.
F4: Full screen.
F5: Window mode.
F6: Kill all enemies.
F7: Activate/Deactivate collider and pathfinding vision.
F8: Deactivate night.
F9: Activate night.
F10: Activate heroes God mode.
F11: Deactivate heroes God mode.

0: Give resources to player.
4: Toggle debug build mode for player.
9: Activate/Deactivate camera movement by mouse.

KP 1: Spawn base.
KP 2: Spawn decorative building.
KP 3: Spawn enemy.
KP 4: Spawn gatherer hero (Suit Man).
KP 5: Spawn melee hero (Armored Man).
KP 7: Spawn turret.

N: Win if in-game.
M: Lose if in-game.


	## Team Members:

### Aaron Guerrero Cruz
* Lead
* [AaronGCProg](https://github.com/AaronGCProg)


### Jose Luís Redondo Tello
* Code
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

*Fade in/out between scenes 
*Multiple debug keys (listed above)
*InGame Support for windowed mode and fullscreen
*Automatic Continuous Integration and Deployment  
*Logo Scene, with custom art and disctintive audio
*Title Scene, with custom title spatial sound and options
*First implementation of a map showing our core game loop
*Single resource which is used for building turrets
*Two different hero units, each one with one disctintive skill:
	**Armored Man: A melee attack with an AoE effect
	**Suit Man: Throws a grenade in ranged which explodes on contact
*Group selection and movement for ally units
*Multiple buildings implemented:
	**Ally base: Center of operations, you can build and upgrade here
	**Enemy base: Your opponents stronghold, deplete it's health to make it your own
	**Turrets: Building which attacks entities of it's contrary alligment
	**Decorative Buildings: Flesh out our map and create chokepoints in the map
*Win condition: triggered by killing all enemies during night
*Win scene: custom made "you win message" and track  
*Lose condition: triggered by losing all units or losing all bases
*Lose scene: custom made "you lose message" and track  
*HUD implementation showing resources, heroes, cycle state...
*Real Time minimap implemented: 
	**Showing icons for heroes, enemies and important structures
 	**Showing rendered images of terrain and decorative buildings
	**Last seen image endures in past FoW
*Day / Night cycle. During night, enemies spawn and attack player bases
*All units and buildings display a Portrait in the bottom right corner showing their Healthbar and specific characteristics 
*Spatial audio: units on screen or out of it (but still close to the center) will make sounds of different volume and origin
*Zoom in/out of map (mouse wheel)


	## Credits

### All spritesheets and other images not 100% done by us are from Fallout 1-3 and can be found here:
https://www.spriters-resource.com/
https://itch.io/
https://opengameart.org/

### All sounds are from fallout 1-3 and can be found here:
https://www.sounds-resource.com/pc_computer/fallout3/
https://www.sounds-resource.com/pc_computer/fallout2/
https://www.sounds-resource.com/pc_computer/fallout/

### Thanks to Juan Hernández and David Carrasquet for dorming part of the team during the first sprint of the project and contributing in the wiki creation
	

	## License:
MIT License
Check the full license in our Github project
