# Project brief
Once the Galaxian exercise is completed to a satisfactory level, we move on to your second
exercise: to produce a version of the classic multiplayer game Gauntlet, [(youtube link)](https://www.youtube.com/watch?v=HDTsTvxcuTE) [(playable
link)](https://codeincomplete.com/games/gauntlet/) from Atari 1985. Again this is a 2D game, but employs a more advanced display system
where you must display a scrolling map area, which is variable in size.

All your game logic must be map based, and rendering should be separated from the main
game play logic. Take note of the generation of characters, their movement, animation, and
attack patterns. The logic of different characters is actually quite simplistic, but you must
maintain a means to avoid overlapping of motion and decipher the simple logic of the
characters. The original game allowed for multiple players, however we do not require you to do
that, but a choice of controlled player with the different attributes they have is desired.

Students should take note of the difficulties of spawning new characters and removing them
from the game when killed, this presents a challenge in memory management. Additionally
students should also take note of the increase in collision tests required and provide a means to
minimize this with a suitable broad phase system.

# Table of contents
- [Gameplay observation](#gameplay-observation)
- [Programming problems](#programming-problems)
    - [Unsure problems](#unsure-problems)
    - [Implementations](#implementations)
- [Archived notes](#archived-notes)

# Gameplay observation
The game is a top down view dungeon crawler. The levels are predetermined. There are four characters:
* Warrior
* Valkyrie
* Wizard
* Elf

I'm not yet sure about the differences between these characters, but they appear to differ in these subjects:
* Projectile damage
* Projectile move speed
* Projectile fire rate
* Entity move speed
* Entity defense/resistance
* Magic points (not sure what this affects, maybe potion radius)
  
The objective is to get as far as possible in the level to reach the end and get the highest highscore. You die if you have 0 HP, a value that decreases by time and damage.

## Things the game does

### Player related

* Things only the player can
    * The player can pick things up.
    * The player can open doors.
    * The player can finish the level.
    * The player can heal hp.
    * The player's health degenerates overtime.
    * The player can use potions.
    * The player can increase its stats.
    * The player has magic power. 
    * The player can become invincible.
    * The player can use teleporters.
* Inventory
    * The player can hold up to 12 items. 
    * The inventory can hold keys and potions.
    * The inventory is kept between levels.

### Enemy related

* Movement `DOES NOT APPLY TO THE LOBBER`
    * All enemies move towards the player, they search for the shortest distance along the 8 directions and move in that direction to you.
    * Some enemies have weapons and some don't. If the enemy has a weapon, they will continue moving until they have sight of you.
    * Monsters can shoot eachother.
    * Monsters don't shoot when directly facing a wall. `They probably check their surrounding tiles and if it's uncollideable and the player is in the direction they shoot.` They also won't move in a better position to shoot the player, just the shortest distance.

### Unique enemy properties

* Lobber ~~ohgod~~
    * The lobber keeps a wall between the player. If there isn't a wall inbetween. He will run away. 
    * The lobber never stands next to a wall corner because the player could shoot him if he did.
    * The lobbers projectile always arrives at the place where the player stands when the player would continue moving, the distance doesn't matter.
    * Lobber projectiles can go over walls, but only deal damage when they land. This means that they can also fly over players.
* Darkness
    * Darkness can only be killed by magic.

### Entity related

* Behaviour
    * There are different breeds. Each with their own unique base stats. These stats affect:
        * movement speed
        * melee damage
        * health
        * defense
        * attack speed (this could be weapon related)
    * All entities (but the player, he has additional behaviour) share the same behaviour. They only differ in excecutuion. (Lobber or darkness for example).
* Classes
    * Ghosts    10 points
    * Demons    ?? points
    * Wizards   ?? points
    * Lobbar    ?? points
    * Darkness  ?? points
    * Grunts    ?? points

### Object related

* Potions
    * Potions can be shot by an entity, the effect is weaker though.
    * There is only one for each, the permenantly upgrade your stats, unless they are stolen by a thief. Then they also turn into a regular blue potion. 
        * Movement speed potion
        * Magic power potion
        * Defense
        * Shot speed
        * Damage
        * Melee power
        * ...Maybe some more
* Keys
    * You can hold 9 keys at a time.
    * Enemies can't walk over them (or any object) but a player can shoot over keys.
    * Keys are used to open gates.
    * Picking one up increases score by 100.
* Doors
    * Upon colliding with a door and having a key in the inventory. The door will dissapear and all the adjacent doors will also. But sometimes adjecent doors won't dissapear.
* Food
    * Breakable food
        * Vase shaped, gives 100 hp
    * Unbreakable food
        * Meat, gives 100 hp
        * Milk and chicken, 100 hp
* Treasure
    * Treasures increase score by 200 points
    * Enemies can't walk over them
* Teleporters
    * Teleporters can only be used by the player.
    * The move the player to the nearest teleporter.
    * The player can only spawn on cells surrounding the teleporter.
* Exits
    * There are 3 types:
        * Exit to 1.
        * Exit to 4.
        * Exit to 8.
* Spawners
    * Spawners exist for every monster type.
    * They have a spawn rate of 1-3 seconds.
    * Spawners only spawn onto empty cells around them.
    * Spawners only spawn when displayed on screen.
* Traps
    * They make walls dissapear.
    * They are visable in game by pulsating a bit [21:12](https://www.youtube.com/watch?v=YqVvvARVq3w).

### Collision related

* Collisions between entities or projectiles results into damage.
* Everything is solid (unpassable) for monsters. For players nothing but monsters and walls are solid.
* If the player collides with an item, he will pick it up and put it in his inventory.
* If the player collides with a door, he will open the door if he has a key.
* If a projectile collides with a breakable wall, the wall will be destroyed.

### Gameplay related

* All levels are always the same. They are indexed.
* The player loses health over time. 
* In some levels the walls can disappear.
* Every time something new will happen, like an item being picked up for the first time, or a type of monset deading damage for the first time. The game will temporarily pause and display a message and tell the player what to do or don't.
* When transitioning from level, A hint is given or some text is written. These are random.
* If there is an upgrade potion in the level, the game will announce it with yellow text.
* The camera has a width of 15 blocks.
* There is some kind of score modifier.

# Programming problems

## Unsure problems

### Collision
#### IS1
We need a way to implement collision. But I'm not to sure how we are going to implement this. Will I make an object that is responsible for this? I could also just call it in the update function. I'd normally use the SAT method but there are a lot of entities in this game. I'll be looking into the map collision video from Brian for reference.  
[Solution](#SL1)

### Map scope
#### IS2
I'm not sure what kind of access is required for the map. Should I just expose the array? Or make functions to call only surrounding cells? There is no reason why an object should know more than what's around him and the players position, but maybe the projectiles will.  
[Solution](#SL2)

### Entity implementation
#### IS3
All entities share the same behaviour, they only differ a bit in execution. Entities do not add new functions that other classes have to access. I can probably get away with only using one update function because all objects have two states. Walk and attack. This will not be to complicated to implement, but requires planning nonetheless. I am not sure how objects should interact with eachother. Do they even need to interact with eachother?  
[Solution](#SL3)

## Implementations

#### SL1
According to the video of Brian on map collision, you only need to check the cell where the entity is at on the given moment. Initially I thought that I should check all the cells around the object and make a descision based on that, but the approach of Brian saves me about 7 checks. That really adds up. Because pathfinding is so simple in this game, there is no reason to not implement it that way.  
We won't even be needing a collision manager of some sorts for this game. We will have a map class. The map class has a function in which you can pass a position. The  function will return the object type currently in that position.  
This simple function is all we need for the object to do it's thing. But if it's so simple, why not use it for everything? As always in programming, there is a drawback. By making the object responsible for collisions, we can't do that much with collisions. Our system is quite flexible. When a collision happens, the object will update according to the object is on at that moment. But the object that it is colliding with won't be updated. This could be a huge deal. If we had to implement collision response where objects are moved when they are touched, this system would be a pain to work with. But we do not have such response in this game.  
`The map will probably have to save the object class insted of it's type though.` This is because when we touch keys, we need to make them dissapear. If we touch doors, we need to make them dissapear, if we touch an exit we need to go to the next level. Are we gonna manage all this in the player class?  
The OOP anwser would be no of course. An player is not responsible for loading the next level, the exit is. A player is not responsbile for destroying doors, the doors are. We need to change things up.

We have fixed our collision, but not our response. How are we gonna make the object communicate when they collide. Let´s see, we have a few odd types of collision. If we collide with the exit tile, we need to load the next level. `The exit tile has to communicate with the gameplay manager. The door has to communicate with the player's inventory. Entities have to comunicate with other entities, to deal damage. But the score system also has to communicate with the monsters.`  
Call me crazy but I don't want a reference to a score system in my monster code. I actually **DO** think that a collision manager is needed for this.  
Why? well, it serves as an abstraction between the entities. This makes sure the objects aren't coupled to eachother which is nice for code reuse.  
It also takes the collision detection out of the object class. While the logic is simple, this should help code redundancy a bit because all objects have the same collision detection (by checking the tile they're on).

But still, there is something I'm uncertain about. When will the collision be resolved? Will it happen after the objects moved? Or during their move? The first option is faster but speed is not a real concern with this game right now. I actually think we should resolve collisions inbetween those state. `Instead of a collision manager, we have a physics engine.` Object request movement in the physics engine. And it goes a bit like this

```
// In physic engine update
...

Save current position to oldX, oldY
Save new position to newX, newY
If entity at cell newX newY != 0
    Handle collision
else 
    Move entity to newX newY

```
There still is a problem. It has to do with our collision detection. Our collision detection assumes that every object sits in its cell. Thing is, entities don't. They move and intersect with multiple tiles at a time. So how do we check that?

I have just found out a great way to do it. I almost went with a SAT approach again and giving every object its own box. Instead, we know that every object is of the same size. 16 by 16. We can use this to our advantage. We can get both positions and subtract them from eachother. If the distance is smaller than half the width, they collide. This will give our entities rounded coordinates.

Let's take things back a bit. I have been resting for a while.  
Let's start with collision detection. I will be using the observer pattern for this. This will make it easier to implement the pop up system later on. We will use circle hitboxes for the collision, because that's simple to implement and not too expensive, we know that every entity has the same size so that spares us calculations. We will only check collision with the potential surrounding 4 tiles. For this the map is needed, it will supply such function to the map.  
All movement is done via the physics engine. The reason why I call it this is because it is also responsible for the movement. Every movement will be forwarded to the physic engine. The physics engine then checks if the movement of the entity will collide with something. If it does, the physic engine will calulate a position that sits against the entity. This is the position the entity will be moved towards. 

For collision response, the two objects that have collided will have a virtual function called onCollision. This function is called for both objects. The only problem is that when a monster moves toward the player. They will collide twice in the same frame, thus the function will be called twice with the same objects. We have to fix this.
To do this. We safe structures of movement and id. When the object is moved. We will calculate it's position. If it has a collision, we will update both move vector accordingly. This will result into only one collision. It would look a bit like this
```
requestX, requestY
surroundTiles = getSurroundingTiles(requestX,requestY)
for tile in surroundTiles
    if tile collides with requestX,requestY
        resolve collision and update both position
```

#### SL2
The map is used by many things. It is used to load the level because it contains the location of all the objects. Everything that happens in the game, is reflected in the map. You could call the map the game actually. Because of this, we can work with two coordinate systems. The map based system and the coordinate based one. The difference is that the map based system contains all the object positions from a relative unit and the coordinate based one contains all the object positions from a pixel based unit. Working with two different coordinate systems is quite a pain which is why I'd rather do everything on the map based system but this comes with its costs.  
First of all you lose precision. Pixels are more precise then cell units or whatever that is. Second of all you lose freedom. Positions are locked to multiples of cell unit. We can't have inbetween.

But let's just throw that aside. This is to talk about the map scope. How much may an object access from it. Let's first write down what kind of things we need from the map. We need the map for:
* Collision
* Updating
* Drawing

Especially objects need access to this map. And I think only objects do. The condition to go to the next level is also based on the map. When the player reaches the exit, the next level will be loaded. But that is something that is collision dependent.

The map won't be responsible for the lifetime of the objects, only for the postions. Because of this, when an object is destroyed, the map needs to know because our collision system depends on it.

To simplify drawing and updating, every object needs to be represented on the map, except for projectiles. Most object's won't interact with the map. But spawners will. As do doors when they need to find their adjecent ones. 

So we know that every object will have access to the map and the game instance. But how much can they access from the map? For collisions, we only need the 4 potential tiles the object is standing on. The game instance needs access to the whole map though because the game instance decides what will be drawn and what not.

So for access we will have two get type functions. One takes an index or position to the map and return the potential 4 tiles the object sits ontop of, the other function return the 8 surrounding tiles and the last function returns the whole map. 

The map will also have funcions to remove a certain id and add an id to the array with a x and y offset. 

#### SL3

# Archived notes

## Object interactions and response
All players and entities have the same **moveset**. They can move in 8 directions. `Movespeed values aren't normalized. This means that the movespeed diagonal is faster than the movespeed in an axis direction.` 

All entities have the same **attack set**. They all throw projectiles or not in 8 directions. The projectile is determined by the stats of the entity. `Some projectiles can shoot through wall corners.`

All entities **damage** each other. Enemies can kill enemies. I'm not sure about players damaging players.

The **pathfinding** is very simple. It only moves in a straight line. This line can be horizontal,vertical or diagonal. Observations have shown that enemies tend to take the shortest path. They do not take walls or any other collideable object into consideration and will just walk against them. When the player dies, all enemies will walk away from him. `But the gobbler moves away from you because he can throw projectiles over walls.`

From a players perspective, **collision** with enemies or projectiles results into damage. Collision with treasury results into a higher score. Collision with vases will increase the HP `by 100 but not sure`. Collision with keys or potions will give you the item. Collision with doors will open them **IF** you have a key and collision with exit will finish the level. 

### Sightings

`You have upgrade potions.`  
We need some way to update player stats.

`Entities can't overlap.`  
We need some way to decide where they can claim a spot.

`You can shoot over keys, but enemies can't pass through them.`  
Some items only collide when certain conditions are met.

`Enemies can shoot eachother and deal damage to eachother. But some enemies can't shoot, only use melee.`  
We need to implement some sort of weapon for each enemy.

`All enemies walk towards the player, except for the Gobbler.`  
We need different behaviour for how enemies walk.

`The player can only hold up to 12 items at a time.`  
We need to manage the players inventory and be able to interact with it.

`The player can open doors if he has a key. He does so by touching it.`  
We need some way to make the player be able to interact with the map.

`Some walls can be broken. They have different textures.`  
The map must be able to adapt to surounding events.

`There are different kind of exits, three to be exact.`  
We need to support different ways to finish the level.