
===================


# Please Note that AimTux-Hitlerhook Requires Additional Packages. See Below!
[![Join the chat at https://gitter.im/Aimtux-Hitlerhook/Lobby]](https://gitter.im/Hitlerhook/Lobby#)

Want to chat or play with other Hitlerhook users? Join us on Gitter!

## What is Hitlerhook?

A fully featured internal hack for *CounterStrike : Global Offensive* written in C++.


## Compiling

**Note:** _Hitlerhook Requires an additional package for Lua._

**Note:** _Do NOT download or compile as the root user._

#### Download the dependencies required to build Hitlerhook:


If you're having problems compiling make sure you've got the latest version of `g++`.

[How to update g++](https://github.com/AimTuxOfficial/AimTux/wiki/Updating-your-compiler)

==================

#### Attention! You need lua and xdo to compile Hitlerhook. Please don't skip this!

__Ubuntu-Based / Debian:__
```bash
sudo apt-get install cmake g++ gdb git libsdl2-dev zlib1g-dev liblua5.3 libxdo-dev patchelf
```
__Arch:__
```bash
sudo pacman -S base-devel cmake gdb git sdl2 lua xdotool patchelf
```
__Fedora:__
```bash
sudo dnf install cmake gcc-c++ gdb git libstdc++-static mesa-libGL-devel SDL2-devel zlib-devel lua-devel libX11-devel libxdo-devel patchelf
```

__Gentoo:__
```bash
sudo emerge cmake dev-vcs/git gdb libsdl2 mesa lua xdotool patchelf
```
===================

#### Download Hitlerhook:

```bash
git clone --recursive https://github.com/raspbian/Hitlerhook
```

```bash
cd Hitlerhook
```

===================


#### Compile with build script

You can build easily with the included build script.
```bash
./build
```
You can later update with 
```bash
./update
```

## Injecting

First of all, make sure CSGO is open, it doesn't matter whether you're in game or not. However, it is not recommended to inject while CSGO is loading into a map. 

Navigate to the directory where Hitlerhook was built if you haven't ready.

```bash
cd Hitlerhook
```

Now, you can inject the hack with the `load` script

```bash
./load
```

You might be prompted to enter in your password, this is because the injection script requires root access.

The text printed out during injection is not important. 

If the injection was successful you'll see a message at the bottom saying `Successfully injected!`, however, if the message says `Injection failed`, then you've most likely done something wrong.


## Using the hack

Now that Hitlerhook has been injected into the game, press <kbd>Insert</kbd> on your keyboard to open the hack menu (<kbd>ALT</kbd>+<kbd>I</kbd> if you're using a laptop).

If you want to change skins, create and load configs or open the player list, you can find those buttons at the top of the screen.

## Unloading the hack

If you wish to unload the hack from the game, you can do so by entering the command:

```bash
./uload
```

## Configs

Configs are stored in a hidden directory in your home folder. Specifically 

```
~/.config/Hitlerhook
```

Each `config.json` is stored in a seperately named folder (The name you see in-game, in the config window). 

To add a config, create a folder inside of the `~/.config/Hitlerhook` folder with a name of your choice, and paste the `config.json` inside of that folder.

To see hidden folders inside your home folder, press <kbd>CTRL</kbd>+<kbd>H</kbd> when using a file manager.

On your Command line, You can also add the -a flag on ls     `ex: ls -la ~/` 

## Grenade Configs

```
~/.config/HitlerhookGH
```

Each `config.json` is stored in the folder named after them map name.

To add a config, copy the folder containing it to `~/.config/HitlerhookGH`


## Screenshots

## Credits
Special thanks to the Original AimTux project: https://github.com/AimTuxOfficial/AimTux

Special thanks to [@aixxe](http://www.github.com/aixxe/) ([aixxe.net](http://www.aixxe.net)) for the skin changer and with the initial project, as well as helping this project with source code (Available on [@aixxe's](http://www.github.com/aixxe/) github page.)

This project was also originally based upon Atex's [Linux Basehook](http://unknowncheats.me/forum/counterstrike-global-offensive/181878-linux-basehook.html).
# Hitlerhook
# Hitlerhook
# Hitlerhook
# Hitlerhook
# Hitlerhook
