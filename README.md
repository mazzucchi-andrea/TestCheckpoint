<!--
SPDX-FileCopyrightText: 2026 Andrea Mazzucchi <andrea.mazzucchi@tutamail.com>
SPDX-FileCopyrightText: 2026 Francesco Quaglia <francesco.quaglia@uniroma2.it>

SPDX-License-Identifier: GPL-3.0-or-later
-->

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) 
[![REUSE status](https://api.reuse.software/badge/github.com/mazzucchi-andrea/Checkpoint)](https://api.reuse.software/info/github.com/mazzucchi-andrea/Checkpoint)

# MVM Checkpoint/Restore Benchmarks

This repository benchmarks checkpoint and restore performance across three
implementations:

- `MVM_GRID_CKPT_OLD`
- `MVM_GRID_CKPT_NEW`
- `MVM_CHUNK`

Each implementation is built and run over a sweep of allocator sizes,
grid/chunk sizes, and read/write ratios. Results are written to CSV
files, and `get_data.c` + `plot.gp` are used to turn those CSVs into
gnuplot figures under `plots/`.

## Prerequisites

- `bc` (used to compute write/read operation counts for each sweep point)
- `gnuplot` (used to render figures)
- `gcc` and `make` (to build `get_plot_data` and the benchmark binaries)
- `plot.gp` must be present in the repository root

The script checks for these and will exit with an explanatory error if
something is missing.

## Usage

```bash
./run.sh             # run all experiments, then generate figures
./run.sh experiments # only run the experiments and generate CSV data
./run.sh figures     # only (re)generate figures from existing CSV data
./run.sh clean       # delete all CSV results and the plots/ directory
```

### `./run.sh` (no arguments)

Runs the full sweep (`cache_flush`, `ops`, `size`, `mods`/`chunks`,
`writes`) for all three implementations, rebuilding each with `make` and
running `./application/prog` under `taskset -c 0` for every parameter
combination. Results are appended to per-implementation CSV files:

- `MVM_GRID_CKPT_OLD/ckpt_test_results.csv`, `ckpt_repeat_test_results.csv`
- `MVM_GRID_CKPT_NEW/ckpt_test_results.csv`, `ckpt_repeat_test_results.csv`
- `MVM_CHUNK/chunk_test_results.csv`, `chunk_repeat_test_results.csv`

Once experiments finish, figures are generated automatically (same as
running `./run.sh figures`).

### `./run.sh experiments`

Runs the full sweep (`cache_flush`, `ops`, `size`, `mods`/`chunks`,
`writes`) for all three implementations, rebuilding each with `make` and
running `./application/prog` under `taskset -c 0` for every parameter
combination. Results are appended to per-implementation CSV files.

Once experiments finish, skips the figures generation.

### `./run.sh figures`

Skips the experiment sweeps entirely and only regenerates `plots/` from
whatever CSV data already exists on disk. Useful for tweaking `plot.gp`
or `get_data.c` without re-running the (slow) benchmarks.

### `./run.sh clean`

Deletes all generated results and figures:

- The CSV result files in `MVM_GRID_CKPT_OLD`, `MVM_GRID_CKPT_NEW`, and
  `MVM_CHUNK`
- Intermediate `ckpt_plot_data.csv` / `restore_plot_data.csv` files
- The compiled `get_plot_data` helper binary
- The entire `plots/` directory

This does not run `make clean` in the implementation subdirectories.

## Sweep Parameters

The parameter sweep is defined at the top of `run.sh`:

| Variable       | Values |
|----------------|--------|
| `cache_flush`  | `0` |
| `ops`          | `1000` |
| `size`         | `0x100000`, `0x400000` |
| `mods`         | `32`, `256` |
| `chunks`       | `32`, `256` |
| `writes`       | `0.95` down to `0.30` in steps of `0.05` |

Edit these arrays directly to change the sweep.

## License

This project is licensed under the GNU General Public License v3.0 or
later (GPL-3.0-or-later). See [`LICENSES/GPL-3.0-or-later.txt`](LICENSES/GPL-3.0-or-later.txt)
for the full license text.

This repository is [REUSE](https://reuse.software/) compliant.
