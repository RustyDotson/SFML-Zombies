
# SFML Asteroids
### Asteroids build in SFML Graphics and C++

![asteroids_demo](https://github.com/user-attachments/assets/b83a136f-665d-4873-8352-0f746fb606b2)

### What is this?
This Asteroids project was built utilizing a custom engine with an [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system), a popular architectural pattern for large-scale video games. Unity now uses an ECS in their own game engine, allowing thousands of entities to exist at once without causing a lot of lag. Building a personal project through a custom ECS is a great way to understand how a lot of modern game engines work behind the scenes.

While Asteroids isn't necessarily a large scale game, it is possible to create a type of Asteroids where thousands of collidable entities do all appear at once, which is what this version has. Each round, the number of asteroids increases by n*2+1 (subject to change), quickly increasing the number of asteroids that spawn. 4 mini asteroids come from 1 large asteroid, so you could imagine the number of collidable entities that would be present within 10 rounds. However, it is nearly impossible to get to that high of a round until I implement player power ups.

### Setup
In order to play the game, build by using cmake in the root directory of the project. If you don't
have cmake, please install it at https://cmake.org/download/ OR if you use Visual Studio Code, it would be easier to just install the cmake package located on the extensions page. installing cmake via the downloads on the website may require some configuration beforehand.

After you have cmake setup, use the command `cmake --build build` in the root directory. The output program (depending on the OS) should be located somewhere like: build>bin>main.

You shouldn't need to download any other dependencies, as cmake should install everything for SFML on its own.

All sounds were designed in [LMMS](https://github.com/LMMS/lmms), and all sprites were designed in [Aseprite](https://github.com/aseprite/aseprite)

### *NOTICE* 
On my macbook, the game is able to start very quickly, but my Windows machine requires around 10 seconds to load. This could just be an old Windows computer being an old Windows computer, or a problem somewhere in the code when initializing for windows systems. I'm currently looking into the issue. 

### Future Updates (notes for myself)
As I continue to build this project, I would like to add the following:
- add health to asteroids, and asteroid health increases each round.
- increase asteroid speed each round.
- explosions.
- points system where player receives points for destroying asteroids, maybe add bonus points for every time a player shoots without missing a target.
- create weapon entities, so that each weapon that a player has contains different traits.
- Player starts with a really crappy weapon, able to purchase stronger weapons that have more range, bullet speed, bullet damage, and fire rate.
- asteroids drop abilities/temporary power ups
- places within the game where the player can buy character or weapon upgrades using the points obtained.
- z axis to add prioritization to sprite rendering order.
- add barrier to the outside of the view so the player cannot leave the screen.
