# libmoor

This repository contains code for dealing with a [Moorhuhn][mh] installation.

[mh]: https://en.wikipedia.org/wiki/Moorhuhn#Original_game
[dl]: https://archive.org/details/Moorhuhnjagd_1020

It includes:

- libmoor, a C99 library for dealing with game files
- command-line tools that provide a front-end to libmoor

The game itself is available [here][dl].

## tools

### moorextract

Extract assets from `MOORHUHN.DAT`.

### moorconvert (WIP)

Convert raw graphics to Windows bitmap files.

### moortest

Just a test binary to check whether the build succeeds.

## copyright

(C) 2021 Martin Frederic

Licensed under the terms of the MIT license.