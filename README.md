# SimpleECSGame

Implementation of a simple Entity-Component-System design, based on [this](https://austinmorlan.com/posts/entity_component_system/) blog article. Some changes were made to make the API for adding components to a system nicer.
The plan is to make a simple 2D game based on ECS test the capabilities of the design and learn about the ECS design/pattern.

At the moment, a 2D physics simulation with collisions between basic shapes is implemented.
## Dependencies

- *glm* is used for math
- *SFML* is used for making the rendering system

## Installation

the project can be installed using CMake. First create a build directory using `mkdir build` and switch into it with `cd build`. Then run cmake with `cmake ..` and compile with `make`:
```shell_session
mkdir build
cd build
cmake ..
make
```
