# NSF_IonShielding

This repo is a clone of `NSF_IonShielding` (https://github.com/dungducphan/NSF_IonShielding). 

From the `NSF_IonShielding` simulation, we know that 4mm of Pb shielding together with 1mm of residual heavy water layer is sufficient to shield all deuterons upto 30 MeV. However, the deuteron beam will still interact with the shielding materials and produce tertiaries such as neutrons and gammas. The spectra and yields of tertiaries are summarized in the Overleaf document (https://www.overleaf.com/project/6532e3b5746498e4e24f7690).

The focus of this repo is the Geant4 simulation of Radioisotope Yield due to the interactions of tertiary particles such as neutron and gammas with the nuclear waiting materials (Rh and Au, in this study).

# Installation
## Dependencies
- Geant4
- ROOT
- CMake
- GCC
- Ninja: optional build system. You can use `make` instead.

## Build Instructions

```bash
cd /path/to/NSF_RadioisotopeYield
git clone git@github.com:dungducphan/NSF_RadioisotopeYield.git
mkdir build
cd build
cmake ../NSF_RadioisotopeYield -GNinja
```