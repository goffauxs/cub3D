# cub3D
42 project cub3D (ray caster)

## About
### Map files (.cub)

Cub3D parses files comprised of the following details:
- `NO`, `SO`, `EA`, `WE` followed by a path to a texture file (in the [xpm](https://en.wikipedia.org/wiki/X_PixMap) format), representing the textures to be displayed on the walls of the corresponding cardinal direction.
- `F`, `C` followed by `R, G, B` representing the colors of the floor and ceiling of the map.

This is then followed by a top down reprensentation of the map using the following characters for each tile:

- `1`: Wall
- `0`: Empty space (floor)
- `N`/`S`/`E`/`W`: Starting position of the player with the letter representing the cardinal direction they are facing when the program starts.

A flood fill algorithm is used to ensure that the map is enclosed by walls.

### Game
The game uses [raycasting](https://en.wikipedia.org/wiki/Ray_casting#Ray_casting_in_early_computer_games) to draw all the walls within the player's field of view, maps the corresponding textures on them and fills in the colors for the floor and ceiling given in the .cub file.
A frame is only drawn when the player's position or angle changes.

The player is moved using `WASD` and the view is rotated using `left/right arrow keys` or by `left clicking and dragging`.

Collision between the player and the walls is calculated to prevent the player from phasing through walls.

A minimap is displayed in the top left corner showing the player's position and angle.

## Installation
The program is compiled using `make all`

## Example usage
The program can be run by passing it a .cub file as parameter (examples in `test_maps`).

Here is a short demonstration of the program running.

![cub3d.gif](https://github.com/goffauxs/cub3D/blob/main/cub3d.gif)

Map used for this example:
```
NO textures/greystone.xpm
SO textures/colorstone.xpm
WE textures/greystone.xpm
EA textures/colorstone.xpm

C 135,245,251
F 117,142,79

1111111111     111
1000000001     101
1111000001     111
1000000001
100N001001
1010000011
100000000111111111
111111100000000001
      100000111001
      100000001001
      100011111001
      100001100001
      100001000001
      111111111111
```
