<p align="left"> 
	<img src="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/blob/master/docs/bs.png?raw=true">
</p>

# What is it?

Bouncy Slime is a 2D platformer game developed by 2 students from CITM pursuing a degree in Videogames Design and Development.
You'll have to prove yourself parkouring through the world and get your reward treasure. Being a slime provides you with this awesome advantage: WALL JUMPING!

# Who we are?

<table>  
  <tr>
    <td> <a href="https://github.com/davidcarrasquet">David Carrasquet</a></td>
    <td> <em>Main design and code</em></a></td>    
    <td> <img src="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/blob/master/docs/david.png?raw=true"> </td>    
  </tr>
  <tr>
    <td><a href="https://github.com/AlexMelenchon">Àlex Melenchón</a></td>
    <em>Main code and design</em></a></td>     
    <td> <img src="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/blob/master/docs/alex.png?raw=true"> </td>
  </tr>
</table>

 
# What are you waiting for to play the game?



<p align="center"> 
 	<video src="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/blob/master/docs/trailer.mp4?raw=true" width="640" height="360" controls preload></video>	
</p>

<p align="center">
<img src="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/blob/master/docs/mushroom.gif?raw=true"> <a href="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/releases/download/2.0/BouncySlime.zip"><strong>DOWNLOAD</strong></a> <img src="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/blob/master/docs/mushroom.gif?raw=true"> 
</p>

# Instructions

<a href="https://alexmelenchon.github.io/BouncySlime-XMatSigma#what-are-you-waiting-for-to-play-the-game">Download</a> the latest release and open the .exe in the game folder.

## Game Controls 

+ <strong>A:</strong>  		 Move left
+ <strong>S:</strong>		 Move right
+ <strong>D:</strong>		 Move down
+ <strong>Space:</strong>	 Jump, and wall jumping +WASD
+ <strong>L:</strong>		 Throw shuriken + WASD
+ <strong>NumPad +:</strong>     Volume up
+ <strong>NumPad -:</strong>	 Volume Down

## Debug Keys

+ <strong>F1:</strong>  	 Load the tutorial map		
+ <strong>F2:</strong>		 Load the first map
+ <strong>F3:</strong>		 Load the second map
+ <strong>F4:</strong>	 	 Load the third map
+ <strong>F5:</strong>		 Save game
+ <strong>F6:</strong>           Load game
+ <strong>F7:</strong>           
+ <strong>F8:</strong>	         
+ <strong>F9:</strong>		 Draw colliders in screen, and enemy paths
+ <strong>F10:</strong>          God mode
+ <strong>F11:</strong>	     	 Enable/disable framecap
+ <strong>F12:</strong> 	 Change title info display (Frame or Map Data)

## Console comands 

+ <strong>º/ª:</strong> Open the In-Game Console (only while in the mid of a level) 
+ <strong>Left Arrow:</strong> Positions cursor one letter to the left
+ <strong>Right Arrow:</strong> Positions cursor one letter to the right
+ <strong>Backspace:</strong> Deletes one letter in the left of the cursor position
+ <strong>Supr:</strong> Deletes one letter in the right of the cursor position
+ <strong>Enter:</strong> Enters a command into the console
_Enter the command 'list' in the console to see a list of all avaliable commands_


# Quick description of the system

This 2D platformer is a game made with SDL with the integration of pugui::xml for loading/saving. Entities and UI work with the structure of an entity system.

# Innovations

_innovations are features that weren't needed as a requirement for our assigments_

- Volume control in-game
- Forward-propagation collision system, colliders are loaded from an object layer of the tmx
- Camera culling with parallax
- Animations load from XML document
- Music loads from tmx
- Enemies have a default patrol movement, if they are not chasing the player, they do a patrol
- Enemies make pathfinding to their patrol when they stop chasing the player
- Player has a cooldown ability, a shuriken throw. When thrown, tha player can't throw another one until it comes back to him with pathfinding. 
- Land enemies have "intelligent" jump method, they will chase the player and jump whe they are able to make a succesful jump.
- Game can be paused with "p".
- Enemies and their patrol load from tmx.

# Credits

## Art

 Level tileset by: <a href="https://anokolisa.itch.io/">Anokolisa</a>
 
 Player and UI by: <a href="https://bayat.itch.io/">Bayat Games</a> 
 
 Enemies by: <a href="https://pixel-frog.itch.io/">Pixel Frog</a> 
 
 Shuriken by: <a href="https://sorceressgamelab.itch.io/">SorceressGameLab</a> 

## Music

 Loop musics by: <a href="https://soundcloud.com/pascalbelisle">Pascal Belisle</a> 

 Kahoot music by: a href="https://kahoot.com/library/kahoot-sheet-music/">Alf Inge Wang</a>  
 
 Sound effects by: Nintendo, ripped by <a href="https://www.sounds-resource.com/wii/ssbb/">Sound-Resource</a>  
 
 Enemy death fx by: <a href="https://freesound.org/people/thecheeseman/sounds/44430/">thecheeseman</a>

 Shuriken fx by: <a href="https://freesound.org/people/marchon11/sounds/493224/">marchon11</a> and <a href="https://freesound.org/people/DWOBoyle/sounds/144266/">DWOBoyle</a> 

 Coin fx: <a href="https://freesound.org/people/ProjectsU012/sounds/341695/">ProjectsU012</a>

 Click fx: <a href="https://freesound.org/people/kwahmah_02/sounds/256116/">kwahmah_02</a>

 Win fx: <a href="https://freesound.org/people/LittleRobotSoundFactory/">LittleRobotSoundFactory</a>
 


# License
<a href="https://github.com/AlexMelenchon/BouncySlime-XMatSigma/blob/master/LICENSE"><strong>GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007</strong></a>
