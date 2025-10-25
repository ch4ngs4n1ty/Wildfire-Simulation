# Wildfire Simulation — Wildfire Page

This document describes the "Wildfire" page of the Wildfire-Simulation project: what it does, how to use it, configurable parameters, outputs, and contribution notes. Drop this section into your project's README or link it from the site page for users who want detailed usage and troubleshooting help.

## Overview

The Wildfire page provides an interactive simulation and visualization of wildfire spread across a landscape. It lets users configure environmental conditions (wind, humidity, fuel types), set ignition points, run deterministic or stochastic spread models, and visualize results in real time. The page is designed for experimentation, teaching, and basic scenario planning.

Key capabilities:
- Configure environmental parameters (wind speed/direction, humidity, terrain slope).
- Choose simulation model and resolution.
- Add, remove, and edit ignition points.
- Start/pause/step/reset the simulation.
- Export simulation outputs (GeoJSON/CSV/image snapshots).

## Quick start (local)

1. Clone the repository and install dependencies:
   - git clone https://github.com/ch4ngs4n1ty/Wildfire-Simulation.git
   - cd Wildfire-Simulation
   - npm install

2. Run the development server:
   - npm run dev
   - Open the app (usually http://localhost:3000 or the port shown in console).

3. Open the Wildfire page from the app navigation.

(If your project uses different commands or Docker, replace the steps above with the project's actual start instructions.)

## Page layout & controls

- Map/Canvas: Main visualization of the landscape and fire front.
- Controls panel:
  - Run / Pause / Step / Reset
  - Simulation speed / Time step
  - Resolution (grid cell size)
  - Export (GeoJSON/CSV/PNG)
- Parameters panel:
  - Wind: speed and direction
  - Humidity: relative humidity percentage
  - Fuel map: select or upload a fuel/vegetation raster
  - Slope/terrain toggle: enable slope effects if DEM available
  - Spread model: select model (cellular automata / probabilistic / physics-based)
  - Random seed: set for reproducible stochastic runs
- Ignition editor:
  - Click to add ignition points
  - Edit location and ignition time
  - Multiple simultaneous ignition support

## Configurable parameters (common)

- wind.speed: numeric (e.g., 0–30 m/s)
- wind.direction: degrees (0–360)
- humidity: 0–100 (%)
- fuel.type or fuel.raster: categorical/continuous fuel load
- slope.effect: boolean or coefficient
- spread.model: "cellular", "probabilistic", or "custom"
- time.step: seconds per simulation step
- grid.resolution: meters per cell
- stochasticity: probability scaling for random spread

Tip: Save common parameter sets as "scenarios" to quickly repeat experiments.

## Running simulations

- Start: press "Run" to advance simulation in real time.
- Pause/Step: Pause and use "Step" to advance one time step for careful observation.
- Reset: Reset to initial conditions or load another scenario.
- Reproducibility: Set the random seed before starting to reproduce stochastic results.

## Outputs & export

- GeoJSON: burned-area polygons and timestamps per feature.
- CSV: per-cell or per-step tabular output (time, cell coords, state).
- PNG / SVG: map snapshots for reporting.
- Optional: time-lapse animation export (webm/mp4) if enabled.

When exporting GeoJSON, the geometry will include the timestamped properties so you can build playback visualizations in other tools.

## Data sources & format

- Fuel maps: raster (GeoTIFF) or simplified categorical grid. Coordinate systems should be consistent with the map view.
- DEM (optional): used to compute slope if slope effects are enabled.
- If uploading custom rasters, ensure they are correctly georeferenced and aligned to the simulation grid.

## Implementation notes (for maintainers)

- Simulation is grid-based (cellular automata style) with optional probabilistic spread rules.
- For deterministic behavior, use physics-based spread formulas or disable stochasticity.
- Performance:
  - Larger grids and smaller time steps will increase CPU/GPU and memory usage.
  - Consider Web Workers or GPU acceleration for large simulations.
- Visualization:
  - Map overlay uses interactive map libraries (e.g., Leaflet, deck.gl, or canvas), with layers for fuel, fire front, and burned area.

## Examples

- Small test scenario:
  - Wind: 5 m/s, direction 90°
  - Humidity: 20%
  - Single ignition at center
  - Grid resolution: 10 m
- Multi-ignition test:
  - Add three ignition points spaced across the map to see fire confluence behavior.

Include saved scenario JSON to reproduce these quickly.

## Troubleshooting

- Simulation runs too slowly:
  - Increase grid cell size (reduce resolution) or increase time step.
  - Disable real-time rendering or lower visualization update frequency.
- Ignition points not starting:
  - Verify ignition time is set and not in the past relative to the simulation clock.
- Exports empty/invalid:
  - Confirm fuel and DEM layers are loaded and aligned; inspect console for errors.

## Contributing

Contributions welcome. If you add a new spread model, visualization, or export format:
- Add unit tests where appropriate.
- Update the Wildfire page docs to describe the new feature.
- Open a pull request with a clear description and example scenarios.

Please follow the repository's contribution guidelines and code style.
