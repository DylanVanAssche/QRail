# QRail

QRail is a Qt library to access linkedconnections.org data resources in an easy way.

## Features

- Connection Scan Algorithm based route planner
- Liveboard, get all the departing or arriving trains in a station
- Station database with search functionality (by ID, location, ...)
- Modular approach
- CI builds and tests

## Build status

### Develop branch (bleeding edge)

![develop](https://travis-ci.com/DylanVanAssche/QRail.svg?branch=develop)

All the latest features are available from the `develop` branch.
This branch might be unstable, never use it in production!

### Master branch (stable)

![master](https://travis-ci.com/DylanVanAssche/QRail.svg?branch=master)

Stable features are released from the `develop` to the `master` branch.
This branch aims to be stable at all times. If you want to use QRail in production, please checkout the release tags!

## Build instructions

In order to run QRail you need to have a Sailfish OS device or use the Sailfish Emulator from the Sailfish IDE.
You can find the Sailfish IDE on [https://sailfishos.org/wiki/Application_Development](https://sailfishos.org/wiki/Application_Development)
You must have set up a device (with developer mode enabled) if you want to deploy it to a physical Sailfish OS device.

1. Open `qrail.pro` in the Sailfish IDE
2. Check if the kit (`SailfishOS-3.0.2.8-armv7hl` for the Sony Xperia X), build mode (`debug` is fine) and that the deployment device (`Xperia X (ARM)` for the Sony Xperia X) are correct.
3. Click on the hammer button to build QRail.
4. Open `tests/qrail-tests.pro` in the Sailfish IDE
5. Click on the run (green arrow) button to run the tests.

These instructions can also be executed from the command line:

For the QRail library:

1. `qmake CONFIG+=debug`
2. `make -j4` 

For the tests:

1. `cd tests`
2. `qmake CONFIG+=debug`
3. `make -j4`

The tests are depending on the [https://lc.dylanvanassche.be](https://lc.dylanvanassche.be/) as Linked Connections Server.
They are automatically executed on the Travis CI build infrastructure with every Git commit.
