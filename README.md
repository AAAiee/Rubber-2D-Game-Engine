# Rubber Game Engine
## an early-stage 2D game engine

Rubber Engine is an early-stage interactive application and rendering engine for Windows 64-bit. 

## Player
#### Basic Control
Use W, A, D, and Left Shift to control the player  
- W: Jump  
- A: Run left  
- D: Run right  
- L-Shift: Roll  

![PlayerControl](https://github.com/AAAiee/Rubber-2D-Game-Engine/blob/ECS_ENTT_IMPL_GAME_EXAMPLE/Demo%20GIfs/PlayerControl.gif)

#### Player Attack
Use the left mouse button to attack. The player can attack in 4 directions based on the mouse click position.  
![PlayerAttack](https://github.com/AAAiee/Rubber-2D-Game-Engine/blob/ECS_ENTT_IMPL_GAME_EXAMPLE/Demo%20GIfs/AttackOnGround.gif)

#### Player Attack in the Air
You can also attack while the player is in the air.  
![PlayerAttackInTheAir](https://github.com/AAAiee/Rubber-2D-Game-Engine/blob/ECS_ENTT_IMPL_GAME_EXAMPLE/Demo%20GIfs/AttackInAir.gif)

#### Player Hit 
The player will blink and become invincible for 1 second after being hit.  
![PlayerHit](https://github.com/AAAiee/Rubber-2D-Game-Engine/blob/ECS_ENTT_IMPL_GAME_EXAMPLE/Demo%20GIfs/PlayerHit.gif)

#### Invulnerable during Roll 
The player is invincible while rolling.  
![PlayerRollInVulnerable](https://github.com/AAAiee/Rubber-2D-Game-Engine/blob/ECS_ENTT_IMPL_GAME_EXAMPLE/Demo%20GIfs/InvincipleDuringRoll.gif)

## Enemy
The enemy will randomly choose to attack either from the air or on the ground. The interval between attacks shortens as the enemy’s health decreases.  
![EnemyAttack](https://github.com/AAAiee/Rubber-2D-Game-Engine/blob/ECS_ENTT_IMPL_GAME_EXAMPLE/Demo%20GIfs/EnemyAttack.gif)

## Future Plan
- Currently, the engine does not support saving/loading from config files (such as JSON/YAML), so the game cannot be paused or ended. This feature will be added once the engine supports it.  
- Add more attack types for enemies.  
- Add audio once the engine includes its own audio system.  
- Refactor current code to better align with ECS design principles.

Reference
- Assets are from game Katana Zero and Hollow Knight 
