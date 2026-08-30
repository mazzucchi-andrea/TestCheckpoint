#!/bin/bash

# SPDX-FileCopyrightText: 2026 Andrea Mazzucchi <andrea.mazzucchi@tutamail.com>
# SPDX-FileCopyrightText: 2026 Francesco Quaglia <francesco.quaglia@uniroma2.it>
#
# SPDX-License-Identifier: GPL-3.0-or-later

set -e

declare -a cache_flush=(0)
declare -a ops=(1000)
declare -a size=(0x100000 0x400000)
declare -a mods=(32 256)
declare -a chunks=(32 256)
declare -a writes=(0.95 0.90 0.85 0.80 0.75 0.70 0.65 0.60 0.55 0.50 0.45 0.40 0.35 0.30)

usage() {
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  (none)      Run experiments, then generate figures"
    echo "  experiments Only run the experiments and generate CSV data"
    echo "  figures     Only (re)generate figures from existing CSV data"
    echo "  clean       Delete all CSV results in subdirectories and the plots directory"
    exit 1
}

check_bc() {
    if ! command -v bc &> /dev/null; then
        echo "Error: bc could not be found."
        echo "Please install bc to run this script."
        exit 1
    fi
}

check_gnuplot() {
    if ! command -v gnuplot &> /dev/null; then
        echo "Error: Gnuplot could not be found."
        echo "Please install gnuplot to run this script."
        exit 1
    fi

    if [ ! -f "plot.gp" ]; then
        echo "Error: Gnuplot template 'plot.gp' not found!"
        echo "Please make sure it's in the same directory as this script."
        exit 1
    fi
}

run_experiments() {
    check_bc

    # Tests with MVM_GRID_CKPT
    cd MVM_GRID_CKPT_OLD
    rm -f ckpt_test_results.csv
    rm -f ckpt_repeat_test_results.csv
    echo "size,cache_flush,mod,ops,writes,reads,ckpt_time,ckpt_ci,restore_time,restore_ci" > ckpt_test_results.csv
    echo "size,cache_flush,mod,ops,writes,reads,repetitions,ckpt_time,ckpt_ci,restore_time,restore_ci" > ckpt_repeat_test_results.csv

    for mod in ${mods[@]};
    do
        for s in ${size[@]};
        do
            for cf in ${cache_flush[@]};
            do
                for o in ${ops[@]};
                do
                    for w in ${writes[@]};
                    do
                        w_ops=$(echo "$o * $w" | bc)
                        w_ops=${w_ops%.*}
                        r_ops=$(echo "$o - $w_ops" | bc)
                        make ALLOCATOR_AREA_SIZE=$s WRITES=$w_ops READS=$r_ops CF=$cf MOD=$mod
                        taskset -c 0 ./application/prog
                    done
                done
            done
        done
    done

    make clean
    cd ..

    cd MVM_GRID_CKPT_NEW
    rm -f ckpt_test_results.csv
    rm -f ckpt_repeat_test_results.csv
    echo "size,cache_flush,mod,ops,writes,reads,ckpt_time,ckpt_ci,restore_time,restore_ci" > ckpt_test_results.csv
    echo "size,cache_flush,mod,ops,writes,reads,repetitions,ckpt_time,ckpt_ci,restore_time,restore_ci" > ckpt_repeat_test_results.csv

    for mod in ${mods[@]};
    do
        for s in ${size[@]};
        do
            for cf in ${cache_flush[@]};
            do
                for o in ${ops[@]};
                do
                    for w in ${writes[@]};
                    do
                        w_ops=$(echo "$o * $w" | bc)
                        w_ops=${w_ops%.*}
                        r_ops=$(echo "$o - $w_ops" | bc)
                        make ALLOCATOR_AREA_SIZE=$s WRITES=$w_ops READS=$r_ops CF=$cf MOD=$mod
                        taskset -c 0 ./application/prog
                    done
                done
            done
        done
    done

    make clean
    cd ..

    # Tests with MVM chunk patch
    cd MVM_CHUNK
    rm -f chunk_test_results.csv
    rm -f chunk_repeat_test_results.csv
    echo "size,cache_flush,chunk,ops,writes,reads,ckpt_time,ckpt_ci,restore_time,restore_ci" > chunk_test_results.csv
    echo "size,cache_flush,chunk,ops,writes,reads,repetitions,ckpt_time,ckpt_ci,restore_time,restore_ci" > chunk_repeat_test_results.csv

    for chunk in ${chunks[@]};
    do
        for s in ${size[@]};
        do
            for cf in ${cache_flush[@]};
            do
                for o in ${ops[@]};
                do
                    for w in ${writes[@]};
                    do
                        w_ops=$(echo "$o * $w" | bc)
                        w_ops=${w_ops%.*}
                        r_ops=$(echo "$o - $w_ops" | bc)
                        make ALLOCATOR_AREA_SIZE=$s WRITES=$w_ops READS=$r_ops CF=$cf CHUNK=$chunk
                        taskset -c 0 ./application/prog
                    done
                done
            done
        done
    done

    make clean
    cd ..
}

generate_figures() {
    check_gnuplot

    rm -rf plots
    mkdir plots

    gcc -O3 get_data.c -o get_plot_data

    for chunk in ${chunks[@]}
    do
        for mod in ${mods[@]};
        do
            for s in ${size[@]};
            do
                for cf in ${cache_flush[@]};
                do
                    for o in ${ops[@]};
                    do
                        ./get_plot_data $s $cf $mod $chunk $o
                        gnuplot plot.gp
                    done
                done
            done
        done
    done

    rm -f get_plot_data
    rm -f ckpt_plot_data.csv
    rm -f restore_plot_data.csv
}

clean_all() {
    rm -f MVM_GRID_CKPT_OLD/ckpt_test_results.csv
    rm -f MVM_GRID_CKPT_OLD/ckpt_repeat_test_results.csv

    rm -f MVM_GRID_CKPT_NEW/ckpt_test_results.csv
    rm -f MVM_GRID_CKPT_NEW/ckpt_repeat_test_results.csv

    rm -f MVM_CHUNK/chunk_test_results.csv
    rm -f MVM_CHUNK/chunk_repeat_test_results.csv

    rm -f ckpt_plot_data.csv
    rm -f restore_plot_data.csv
    rm -f get_plot_data

    rm -rf plots

    echo "Cleaned all CSV results and plots."
}

case "${1:-}" in
    "")
        check_bc
        run_experiments
        check_gnuplot
        generate_figures
        ;;
    experiments)
        check_bc
        run_experiments
        ;;
    figures)
        check_gnuplot
        generate_figures
        ;;
    clean)
        clean_all
        ;;
    -h|--help)
        usage
        ;;
    *)
        echo "Unknown command: $1"
        usage
        ;;
esac