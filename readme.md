# Pico W MQTT

Raspberry Pi Pico W executable for MQTT communication. Is based on the [LwIP](https://savannah.nongnu.org/projects/lwip/) library
implementation that comes bundled with [pico_sdk](https://github.com/raspberrypi/pico-sdk).

The purpose is to have a basic implementation of the API to build upon.

## Usage

The pico_sdk is included as a submodule and needs to be pulled along with this repo.

```
  git pull https://github.com/AndFroSwe/pico_w_mqtt.git --recurse-submodules
```

Configure and build the repo using `cmake` . The project is setup to use the folder __build__ for artefacts. Remember to add the `PICO_BOARD` definition on configuring.

```
  cmake . -B build -DPICO_BOARD=pico_w
  cmake --build build --config=Release
```

## Documentation

Documentation is build using [Doxygen](https://www.doxygen.nl/index.html). Install using apt if on Ubuntu, download from homepage if on Windows.
Build documentation for the project from the command line using

```
  doxygen
```
The documentation is build into the _docs_ folder. Open _docs/html/index.html_ for documentation.
