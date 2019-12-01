# BouncySlime-XMatSigma
Bouncy-Slime is a 2D platformer game in which you'll have to prove yourself parkouring through the world and get your reward treasure.
Being a slime provides you with this awesome advantage: WALL JUMPING! What are you waiting for to play the game?
This project is made by sophomore students from CITM.


## Link to our Github Project:
https://github.com/AlexMelenchon/BouncySlime-XMatSigma


## Game Keys:

### Keyboard control:

- A: Move left
- D: Move right
- S: Move down
- Space: Jump (When wall jumping, jump first, then direct the jump)
- L: Throw shuriken, WASD to direct it (it includes diagonals)
          
- NumPad +: Volume up
- NumPad -: Volume down

- Esc: Exit game

### Debug Options:

- F1:  Load the tutorial map
- F2:  Load the first map
- F3:  Load the second map
- F4:  Reset the current map
- F5:  Save game
- F6:  Load game
- F9:  Draw colliders in screen, and enemy paths
- F10: God mode
- F11: Enable/disable framecap
- F12: Change title info display (Frame or Map Data)
- P:   Pause game

## Innovations:

### First assigment:

- Volume control in-game
- Forward-propagation collision system, colliders are loaded from an object layer of the tmx
- Camera culling with parallax
- Animations load from XML document
- Music loads from tmx

### Second assigment:

- Enemies have a default patrol movement, if they are not chasing the player, they do a patrol
- Enemies make pathfinding to their patrol when they stop chasing the player
- Player has a cooldown ability, a shuriken throw. When thrown, tha player can't throw another one until it comes back to him with pathfinding. 
- Land enemies have "intelligent" jump method, they will chase the player and jump whe they are able to make a succesful jump.
- Game can be paused with "p".
- Enemies and their patrol load from tmx.


## Team Members:

Alex Melenchón Maza:
 -  Main Programmer
 - https://github.com/AlexMelenchon

David Carrasquet Iniesta: 
 - Main designer
 - https://github.com/davidcarrasquet
 

## Credits

### Art:

 Level tileset by: Anokolisa
 https://anokolisa.itch.io/

 Player by: Bayat Games
 https://bayat.itch.io/

### Music:
 Loop musics by: Pascal Belisle
 https://soundcloud.com/pascalbelisle
 
 Sound effects by: Nintendo, ripped by Sound-Resource
 https://www.sounds-resource.com/wii/ssbb/

## License:
GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
You can check the full license in our project


