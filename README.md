# HEARTS OF GREED

Hearts of Greed is a commando-like RTS game in which you'll have to resist the attack of a hostile alien force. Use your select few hero units, your bases, turrets and upgrades to stand your ground and conquer enemy territory. 
During the day, press on, be agressive and try to defeat the enemy. However, once night comes... go back to your base and defend from the incoming attacks. It's not your time to shine.

Good Luck!

Disclaimer: play using headphones.

This project is made as a 2nd year assigment in Bachelor’s Degree in Video Games by UPC at CITM, Terrassa, under the supervision of lecturer [Marc Garrigó](www.linkedin.com/in/mgarrigo/).


## What to do
Start the game by playing the tutorial. Next, use your heroes skillfully to conquer all enemy bases and win the game!
You can unlock extra heroes by completing their rescue mission (keep an eye on the minimap!)

If all your heroes die, you will lose the game.


## Check our project!
Web Page:       https://x-mat-studio.github.io/HeartsOfGreed/

Github:		https://github.com/x-mat-studio/HeartsOfGreed

Twitter:	https://twitter.com/ii_rts 


## Game Keys:

### Control:

* WASD
	- Control camera

* Shift
	- Accelerate camera when pressing WASD

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

* 1: Focus Gatherer Hero

* 2: Focus Ranged Hero

* 3: Focus Melee Hero

* 4: Focus Robot Hero

* Esc: open or close the pause menu (in-game) / close the game (on main menu)

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
* U: Spawn gigamingo.
* I: Spawn fast wannamingo.
* O: Spawn ranged wannamingo.
* P: Toggle debug build mode for player.
* 9: Activate/Deactivate camera movement by mouse.
* KP 1: Spawn base.
* KP 2: Spawn decorative building.
* KP 3: Spawn enemy.
* KP 4: Spawn gatherer hero (Suit Man).
* KP 5: Spawn melee hero (Armored Man).
* KP 6: Spawn ranged hero (Female Sniper).
* KP 7: Spawn robotic hero (Mecha).
* KP 8: Spawn turret.
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
* Automatic Continuous Integration and Deployment.
* Logo Scene video, with custom art and disctintive audio.
* Title Scene, with custom title spatial sound and options and easy in.
* Implementation of the final map.
* Progression system for hero units: lvl, stat boosters, skill upgrades...
* Quest system implemented to unlock different heroes.
* Paused game on in-game menu.
* Various particles added on different relevant actions of the gameplay.
* Three resources implemented:
	- **Blue resource**: Used for building turrets, barricades, upgrade center, and revive dead heroes.
	- **Red resource**: Used to upgrade hero related stats.
	- **Purple resource**: Used to level up hero skills (active and passive, from 1 to 5).
* Four different hero units, each one with two disctintive skills:
	- **Armored Man**: A melee attack with an AoE effect, hight health regeneration and receioves less damage the more enemies surround him.
	- **Suit Man**: Throws a ranged grenade which explodes on contact. This hero gets extra resources for killing enemies and overcharges near robotic allies.
	- **Ranged**: Hit and run specialist, makes enemies bleed after hitting them, and is equipped with a grenade that heavily slows enemies.
	- **Robot**: Can´t regenerate health, however, gains speed and attack when killing enemies in succession, his skill is self destruction (massive AoE).
* Group selection and movement for ally units, group movement for enemy units.
* Multiple buildings implemented:
	- **Ally base**: Center of operations, you can build the upgrade center and revive heroes here.
	- **Enemy base**: Your opponents stronghold wich they will defend if your heroes get near; deplete it's health to make it your own.
	- **Turrets**: Building which attacks entities of it's contrary alligment.
	- **Barricades**: Defensive structures with high hp.
	- **Upgrade center**: Lets you buy and level up turrets and barricades. 
	- **Decorative Buildings**: Flesh out our map and create chokepoints in the map.
* Win condition: Conquer all bases.
* Win scene: custom made "you win message" and track.
* Lose condition: triggered by losing all units or losing all bases.
* Lose scene: custom made "you lose message" and track.
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
* Tutorial and pop-ups to guide the player through the various mechanics of the game.
* Visual and auditive feedback for high to mid relevant action and interactions.
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

### Release 0.3.32

#### General Updates
- Balanced Stats for all Entities
- Enemies now spawn in the map
- Introduced a turorial Level
- Gatherer now gains resources when killing enemies (half of the XP gained)

#### Bug Fixes
- Fixed UI fullscreen button not changing text
- Fixed some sound effects volume & channels
- Fixed mem leaks in Skill areas
- Fixed some animations
- Camera now resets with the game loop
- Pathfinding now finds a way to get out of a walkable tile
. . .

### Release 0.5.0.- Vertical Slice

#### General Updates
- App Veyor is now more accurate at generating automatic builds
- Vertical Slice status
- Core game experience implemented

#### Bug Fixes
- Solved a memory segmentation crash (Fonts format related)
- UI remake 100%

. . .

### Release 0.5.53.

#### General Updates
- Pathfinding is improved
- New types of enemies implemented (Giga / speed / ranged)
- New 4th herot: Robo Hero.
- Heroes now have passive abilities
- Final map implemented
- Quest system added

#### Bug Fixes
- Minimap did show icons out of FoW
- Multiple entities related bugs
- Multiple animation bugs
. . .

### Release 0.6.78.

#### General Updates
- Night AI completed
- Melee hero added
- Skills added
- Options menu added
- Credit menu added
- Added an icon on selected heroes
- Heroes speak while moving
- Hierarchical pathfinding added
- Hero behavior improved
- Building mode added
- Energy and health recovery added
- Buildings drawn under the fog of war
- Various sounds added
- Enemy spawners added
- Fullscreen through a button added

#### Bug Fixes
- Various selection bug
- Minimap bugs
- Armored animation fixes
- Memory leaks solved
. . .

### Release 0.7.56.

#### General Updates
- UI code reworked and greatly improved
- Pathfinding adjustments
- Ranged hero implemented

#### Bug Fixes
. . .

### Release 0.7.82.

#### General Updates
- Robot hero implemented
- Minimap fog of war implemented
- Player feedback expanded
- Group movement expanded
- Load and save logic implemented

#### Bug Fixes
. . .

### Release 0.8.0. - Alpha

#### General Updates
- Hover text implemented
- UI visual rework
- Quests added
- Miniupgrades added
- Skill buttons added
- All different wanamingo types added
- Particle systems added
- Easings added
- Dialog system added
- Tutorial added
- 2nd and 3rd resource added
- Hero reviving added
- Barricades added
- All skills implemented
- Day / night cycle improved
- Pathfinding upgraded
- Balancing

#### Bug Fixes
- Final Alpha refinements
. . .

### Release 0.8.9

#### General Updates
- Created Trailer & WebPage
- Added Asset Management with PhysFS.
- Added Camera Shake for Heroes' Skills
- Improved all Animations Timing
- Improved Animations Pivots
- Added more sounds to improve the Game's feedback & feel.
- Suitman skill now summons a Turret instead of throwing a Graneade. 
- Added a Button to skip dialogues & now they only show the last said dialogue.
- Various gameplay improvements based on Alpha feedback.


#### Bug Fixes
- Robot's Active skill does not longer kill itself 2 times.
- Balanced all Heroes' stats & skills.
- Various UI bug fixes
- Various Gameplay bug fixes

. . .

### Release 1.0.0 - Gold

#### General Updates
- Added a new cursor & new animations for it.
- Improved Animations based on users' feedback.
- Multiple sound Added to improve gameplay feedback.
- Now all entities have some kind of particle feedback.
- Added Easings to UI, camera movement & some entitties' interactions.
- Added animated texture & sounds for the loading screens.
- Changes to Armored & Ranged Skills:
	- Armored now deals less damage, hurts itself ignoring armor & slightly stuns enemies.
	- Ranged now only applies the Blood Loss effect if the target has an another debuff.
- Added Pop-Ups to better guide the player through the experience & improvements to the tutorial.
- UI visual and functional upgrades
- Final iteration to the map: changed walkability in some areas & re-distributed enemies.
- Added an icon to the application.
- Entities stats balancing.

#### Bug Fixes
- Fixed some gameplay related bugs.
- Improved entities' interaction: attack, search & vision.
- Adjusted Day & Night time cycle.
- Balanced Wanamingos for the overall map & the night spawners.
- Fixed memory leaks & crashes.
- Attack range & objective detection changes.
- Improved Pathfinding handling.
-  Folder Naming Improvements.

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

Thanks to Juan Hernández and David Carrasquet for forming part of the team during the first sprint of the project and contributing in the wiki creation.
	

## License:
MIT License
Check the full license in our Github project

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/dab77e736d8a4dcea73e56d0cc04baa5)](https://app.codacy.com/gh/x-mat-studio/HeartsOfGreed?utm_source=github.com&utm_medium=referral&utm_content=x-mat-studio/HeartsOfGreed&utm_campaign=Badge_Grade_Dashboard)
