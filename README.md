# Nuclear-Emulsion Muon Detector - Geant4 Geometry Model

A Geant4 (C++) model of the geometry and materials of a six-plate nuclear-emulsion detector of the type used for cosmic-ray muography.

## Overview

The model reproduces the layered structure of a compact emulsion detector: six double-sided emulsion plates, each sealed in a polyethylene bag, pressed between two rubber mats inside a stainless-steel frame. All dimensions and materials are defined in [`src/DetectorConstruction.cc`](src/DetectorConstruction.cc).

## Scientific motivation

Muography uses the attenuation of atmospheric muons to image the interior of large objects. Nuclear emulsions are compact, passive, power-free detectors in which charged-particle tracks become visible as chains of silver grains after chemical development. A Monte Carlo model of the detector is a prerequisite for studying energy deposition and track registration in its different components. This repository covers the first step: a faithful geometric and material model.

The detector design modelled here follows the assembly described in the muography literature cited in the [References](#references) (six double-sided emulsion plates of 10.0 × 12.5 cm², each vacuum-packed in a light- and water-tight polyethylene bag, fixed in a holding structure).

## Detector geometry

All volumes are `G4Box` solids. The stack runs along the **z axis**; the beam direction of the example code is +z. Sizes below are taken from `DetectorConstruction.cc` (x × y × z).

| Component | Material (Geant4) | Size (cm) | Count |
|---|---|---|---|
| World | `G4_AIR` | 100 × 100 × 100 | 1 |
| Steel box | `G4_STAINLESS-STEEL` | 12.808 × 10.308 × 1.1238 | 1 |
| Rubber mat | `NaturalRubber`: polyisoprene C₅H₈, 0.92 g/cm³ | 12.508 × 10.008 × 0.3 | 2 |
| Packet (bag) | `G4_POLYETHYLENE` | 12.508 × 10.008 × 0.0373 | 6 |
| Emulsion layer | `AgBr_Gelatin`: 85 % AgBr + 15 % gelatin by mass, ≈ 5.70 g/cm³ | 12.5 × 10.0 × 0.0044 | 2 per packet |
| Plastic base | `G4_POLYSTYRENE` | 12.5 × 10.0 × 0.0205 | 1 per packet |

Layer stack along z (not to scale), as built in code:

```
 steel (0.15 cm) | rubber (0.3) | packet 1 … packet 6 | rubber (0.3) | steel (0.15)

 one packet (0.0373 cm) = PE 0.004 | emulsion 0.0044 | plastic base 0.0205 | emulsion 0.0044 | PE 0.004
```

Notes on the construction:

- The rubber mats and the six packets are placed as daughter volumes inside the steel box. The steel box is a solid slab, so the steel is the mother material that fills the space not occupied by the daughters (the frame walls).
- The sizes are self-consistent: the inner depth of the box (1.1238 − 2 × 0.15 = 0.8238 cm) equals two rubber mats (0.6 cm) plus six packets (0.2238 cm), and one packet equals 2 × 0.0044 + 0.0205 + 2 × 0.004 = 0.0373 cm.
- The emulsion density follows from the mass-weighted mixture in the code, `0.85 × 6.473 + 0.15 × 1.35` g/cm³. Gelatin is approximated by the simplified formula C₅H₈O₂N (1.35 g/cm³).
- The same emulsion logical volume is placed twice in each packet; the placements are named `Emulsion1_<i>` and `Emulsion2_<i>` (`i` = 0–5). Overlap checking is enabled for all placements (`checkOverlaps = true`).
- The packet bags are named `Packet_1` … `Packet_6`.

## Technologies

- C++17-capable compiler
- [Geant4](https://geant4.web.cern.ch/) — developed with **11.3.p02** (the version used for the author's build); other 11.x versions are expected to work but have not been tested
- CMake ≥ 3.16

## Repository structure

```
.
├── CMakeLists.txt
├── exampleB1.cc            # main program (from Geant4 example B1)
├── include/                # headers (from example B1)
├── src/
│   ├── DetectorConstruction.cc   # <- the detector model (project-specific)
│   ├── PrimaryGeneratorAction.cc # default example generator (gamma, 6 MeV, +z)
│   ├── RunAction.cc              # example B1, with a null-pointer guard (see below)
│   ├── EventAction.cc
│   ├── SteppingAction.cc
│   └── ActionInitialization.cc
└── macros/                 # init_vis.mac, vis.mac, run1.mac, run2.mac (from example B1)
```

## Build and run

Requires a working Geant4 installation with its environment set (`source <geant4-install>/bin/geant4.sh`).

```bash
cmake -S . -B build
cmake --build build -j
cd build
./exampleB1            # interactive mode: reads init_vis.mac, opens the viewer
```

To inspect the geometry, run the executable without arguments and use the viewer opened by `init_vis.mac`. `vis.mac` is the standard B1 visualisation macro.

Batch mode, using one of the example macros:

```bash
./exampleB1 run1.mac
```

## References

The references below are taken from the original course report and were not re-verified.

1. S. Cecchini, M. Spurio, *Atmospheric muons: experimental aspects*, Geoscientific Instrumentation, Methods and Data Systems **1**, 185–196 (2012).
2. A. Nishio et al., *Development of Nuclear Emulsion Detector for Muon Radiography*, Physics Procedia **80**, 74–77 (2015).
3. A. B. Aleksandrov et al., *Muography of the cave church of the Holy Dormition Pskov-Pechory Monastery*.
4. S. Agostinelli et al., *Geant4 — a simulation toolkit*, Nucl. Instrum. Methods A **506**, 250–303 (2003).

