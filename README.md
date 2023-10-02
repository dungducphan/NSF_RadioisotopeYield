# NSF_IonShielding

Design analysis for ion shielding in heavy water fusion experiment.

To design the target and shielding for the laser-driven fusion experiment. In this experiment, D20 ice will be\
irradiated by a high-intensity laser to produce fusion reaction. The fusion reaction will produce high energy\
deuteron beam. The deuterons will undergo fusion reaction to produce neutrons. The neutrons will be used to study the\
rapid neutron capture reactions in a Rhodium target. The shielding is designed to protect the target from the\
irradiation of deuteron beam.

# Installation
## Dependencies
- Geant4
- ROOT
- CMake
- GCC
- Ninja: optional build system. You can use `make` instead.

## Build Instructions

```bash
cd /path/to/NSF_IonShielding
git clone git@github.com:dungducphan/NSF_IonShielding.git
mkdir build
cd build
cmake ../NSF_IonShielding -GNinja
```