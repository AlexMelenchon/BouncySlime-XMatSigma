﻿# BouncySlime-XMatSigma
Bouncy-Slime is a 2D platformer game in which you'll have to prove yourself parkouring through the world and get your reward treasure.
Being a slime provides you with this awesome advantage: WALL JUMPING! What are you waiting for to play the game?
This project is made by sophomore students from CITM.


## Link to our Github Project:
https://github.com/AlexMelenchon/BouncySlime-XMatSigma

## Link to our Website:
https://alexmelenchon.github.io/BouncySlime-XMatSigma/


## Game Keys:

### Keyboard control:

- A: Move left
- D: Move right
- S: Move down
- Space: Jump (When wall jumping, jump first, then direct the jump)
- L: Throw shuriken, WASD to direct it (it includes diagonals)
          
- NumPad +: Volume up
- NumPad -: Volume down

- ESC:  Open the In-Game Pause Menu

### Debug Options (only in-game):


- F1:  Load the tutorial map
- F2:  Load the first map
- F3:  Load the second map
- F4:  Reset the current map
- F5:  Save game
- F6:  Load game
- F7: User Interface Debug Mode (this is the only command that can be activated in the Main Menu)
- F8: Returns to the main menu
- F9:  Draw colliders in screen, and enemy paths
- F10: God mode
- F11: Enable/disable framecap
- F12: Change title info display (Frame or Map Data)
- TAB: Iteratus Focus through all enabled elements on screen
<br></br>

- KeyPad 1: Adds a Life
- KeyPad 2: Kills the Player
- KeyPad 4: Adds 1000 score
- KeyPad 5: Lowers the score by 1000
- KeyPad 7: Adds a Coin
- KeyPad 8: Downs 1 Coin


### Console Debug Options (only in-game):


- º/ª: Open the In-Game Console (only while in the mid of a level) 
- Left Arrow: Positions cursor one letter to the left
- Right Arrow: Positions cursor one letter to the right
- Backspace: Deletes one letter in the left of the cursor position
- Supr: Deletes one letter in the right of the cursor position
- Enter: Enters a command into the console
- Enter the command 'list' in the console to see a list of all avaliable commands



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

 Enemies by: Pixel Frog
 https://pixel-frog.itch.io/

 Shuriken by: SorceressGameLab
 https://sorceressgamelab.itch.io/

### Music:
 Loop musics by: Pascal Belisle
 https://soundcloud.com/pascalbelisle
 
 Main Menu Loop by: Kahoot
 https://kahoot.com/library/kahoot-sheet-music/
 
 Sound effects by: Nintendo, ripped by Sound-Resource
 https://www.sounds-resource.com/wii/ssbb/
 
 Enemy death fx:
 https://freesound.org/people/thecheeseman/sounds/44430/
 
 Shuriken fx:
 https://freesound.org/people/marchon11/sounds/493224/
 https://freesound.org/people/DWOBoyle/sounds/144266/

 Coin fx: 
 https://freesound.org/people/ProjectsU012/sounds/341695/

 Click fx: 
 https://freesound.org/people/kwahmah_02/sounds/256116/

 Win fx: 
 https://freesound.org/people/LittleRobotSoundFactory/

## License:
GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
You can check the full license in our project


