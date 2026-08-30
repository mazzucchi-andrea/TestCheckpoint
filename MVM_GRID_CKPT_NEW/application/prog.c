/*
 * SPDX-FileCopyrightText: 2026 Andrea Mazzucchi <andrea.mazzucchi@tutamail.com>
 * SPDX-FileCopyrightText: 2026 Francesco Quaglia
 * <francesco.quaglia@uniroma2.it>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <emmintrin.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

#include "ckpt_setup.h"

#ifndef MOD
#define MOD 8
#endif

#ifndef ALLOCATOR_AREA_SIZE
#define ALLOCATOR_AREA_SIZE 0x100000
#endif

#ifndef WRITES
#define WRITES 950
#endif

#ifndef READS
#define READS 50
#endif

#ifndef CF
#define CF 0
#endif

#define LIMIT_8BIT ALLOCATOR_AREA_SIZE - 1
#define LIMIT_16BIT ALLOCATOR_AREA_SIZE - 2
#define LIMIT_32BIT ALLOCATOR_AREA_SIZE - 4
#define LIMIT_64BIT ALLOCATOR_AREA_SIZE - 8

double restore_area(uint8_t *area) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    _restore_area(area);
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
}

double test_checkpoint(uint8_t* area, int8_t value_8bit, int16_t value_16bit,
                       int32_t value_32bit, int64_t value_64bit) {
    int offset;
    volatile uint8_t read_8bit_value;
    volatile uint16_t read_16bit_value;
    volatile uint32_t read_32bit_value;
    volatile uint64_t read_64bit_value;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    _set_ckpt(area);
    offset = 0;
    for (int i = 0; i < WRITES / 4; i++) {
        if (offset >= LIMIT_8BIT) {
            offset = 0;
        }
        *(int8_t*)(area + offset) = value_8bit;
        offset += 1;
    }
    offset = (offset + (MOD >> 1)) & ~(MOD - 1);
    for (int i = 0; i < WRITES / 4; i++) {
        if (offset >= LIMIT_16BIT) {
            offset = 0;
        }
        *(int16_t*)(area + offset) = value_16bit;
        offset += 1;
    }
    offset = (offset + (MOD >> 1)) & ~(MOD - 1);
    for (int i = 0; i < WRITES / 4; i++) {
        if (offset >= LIMIT_32BIT) {
            offset = 0;
        }
        *(int32_t*)(area + offset) = value_32bit;
        offset += 2;
    }
    offset = (offset + (MOD >> 1)) & ~(MOD - 1);
    for (int i = 0; i < WRITES / 4; i++) {
        if (offset >= LIMIT_64BIT) {
            offset = 0;
        }
        *(int64_t*)(area + offset) = value_64bit;
        offset += 4;
    }

    offset = 0;
    for (int i = 0; i < READS / 4; i++) {
        if (offset >= LIMIT_8BIT) {
            offset = 0;
        }
        read_8bit_value = *(int8_t*)(area + offset);
        offset += 1;
    }
    offset = (offset + (MOD >> 1)) & ~(MOD - 1);
    for (int i = 0; i < READS / 4; i++) {
        if (offset >= LIMIT_16BIT) {
            offset = 0;
        }
        read_16bit_value = *(int16_t*)(area + offset);
        offset += 1;
    }
    offset = (offset + (MOD >> 1)) & ~(MOD - 1);
    for (int i = 0; i < READS / 4; i++) {
        if (offset >= LIMIT_32BIT) {
            offset = 0;
        }
        read_32bit_value = *(int32_t*)(area + offset);
        offset += 2;
    }
    offset = (offset + (MOD >> 1)) & ~(MOD - 1);
    for (int i = 0; i < READS / 4; i++) {
        if (offset >= LIMIT_64BIT) {
            offset = 0;
        }
        read_64bit_value = *(int64_t*)(area + offset);
        offset += 4;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
}

double test_checkpoint_repeat(uint8_t* area, int8_t value_8bit,
                              int16_t value_16bit, int32_t value_32bit,
                              int64_t value_64bit, int rep) {
    int offset;
    volatile uint8_t read_8bit_value;
    volatile uint16_t read_16bit_value;
    volatile uint32_t read_32bit_value;
    volatile uint64_t read_64bit_value;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    _set_ckpt(area);
    for (int r = 0; r < rep; r++) {
        offset = 0;
        for (int i = 0; i < WRITES / 4; i++) {
            if (offset >= LIMIT_8BIT) {
                offset = 0;
            }
            *(int8_t*)(area + offset) = value_8bit;
            offset += 1;
        }
        offset = (offset + (MOD >> 1)) & ~(MOD - 1);
        for (int i = 0; i < WRITES / 4; i++) {
            if (offset >= LIMIT_16BIT) {
                offset = 0;
            }
            *(int16_t*)(area + offset) = value_16bit;
            offset += 1;
        }
        offset = (offset + (MOD >> 1)) & ~(MOD - 1);
        for (int i = 0; i < WRITES / 4; i++) {
            if (offset >= LIMIT_32BIT) {
                offset = 0;
            }
            *(int32_t*)(area + offset) = value_32bit;
            offset += 2;
        }
        offset = (offset + (MOD >> 1)) & ~(MOD - 1);
        for (int i = 0; i < WRITES / 4; i++) {
            if (offset >= LIMIT_64BIT) {
                offset = 0;
            }
            *(int64_t*)(area + offset) = value_64bit;
            offset += 4;
        }

        offset = 0;
        for (int i = 0; i < READS / 4; i++) {
            if (offset >= LIMIT_8BIT) {
                offset = 0;
            }
            read_8bit_value = *(int8_t*)(area + offset);
            offset += 1;
        }
        offset = (offset + (MOD >> 1)) & ~(MOD - 1);
        for (int i = 0; i < READS / 4; i++) {
            if (offset >= LIMIT_16BIT) {
                offset = 0;
            }
            read_16bit_value = *(int16_t*)(area + offset);
            offset += 1;
        }
        offset = (offset + (MOD >> 1)) & ~(MOD - 1);
        for (int i = 0; i < READS / 4; i++) {
            if (offset >= LIMIT_32BIT) {
                offset = 0;
            }
            read_32bit_value = *(int32_t*)(area + offset);
            offset += 2;
        }
        offset = (offset + (MOD >> 1)) & ~(MOD - 1);
        for (int i = 0; i < READS / 4; i++) {
            if (offset >= LIMIT_64BIT) {
                offset = 0;
            }
            read_64bit_value = *(int64_t*)(area + offset);
            offset += 4;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
}

void clean_cache(uint8_t* area) {
    int cache_line_size = __builtin_cpu_supports("sse2") ? 64 : 32;
    for (int i = 0; i < (2 * ALLOCATOR_AREA_SIZE + BITMAP_SIZE);
         i += (cache_line_size / 8)) {
        _mm_clflush((void*)(area + i));
    }
}

void mean_ci_95(double* samples, double* mean, double* ci) {
    double sum = 0.0;
    for (int i = 0; i < 1000; i++) {

        sum += samples[i];
    }
    *mean = sum / 1000;

    double var = 0.0;
    for (int i = 0; i < 1000; i++) {

        double d = samples[i] - *mean;
        var += d * d;
    }

    double sd = sqrt(var / (1000 - 1)); // sample SD
    double sem = sd / sqrt(1000);       // standard error

    const double t95 = 1.962;

    *ci = t95 * sem;
}

int main(void) {
    double ckpt_samples[1000], restore_samples[1000];
    double ckpt_mean, ckpt_ci, restore_mean, restore_ci;
    unsigned long base_addr;
    clock_t begin, end;
    uint8_t* area;
    uint8_t value_8bit;
    uint16_t value_16bit;
    uint32_t value_32bit;
    uint64_t value_64bit;
    size_t size;
    FILE* file;

    _tls_setup();

    srand(42);
    value_8bit = rand() % UINT8_MAX;
    value_16bit = rand() % UINT16_MAX;
    value_32bit = rand() % UINT32_MAX;
    value_64bit = rand() % UINT64_MAX;

    base_addr = 8UL * 1024UL * ALLOCATOR_AREA_SIZE;
    size = 2 * ALLOCATOR_AREA_SIZE + BITMAP_SIZE;
    area = (uint8_t*)mmap((void*)base_addr, size, PROT_READ | PROT_WRITE,
                          MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0);
    if (area == MAP_FAILED) {
        perror("mmap failed");
        return errno;
    }

    for (int i = 0; i < 1000; i++) {
#if CF == 1
        clean_cache(area);
#endif
        ckpt_samples[i] = test_checkpoint(area, value_8bit, value_16bit,
                                          value_32bit, value_64bit);
#if CF == 1
        clean_cache(area);
#endif
        restore_samples[i] = restore_area(area);
    }

    file = fopen("ckpt_test_results.csv", "a");
    if (file == NULL) {

        fprintf(stderr, "Error opening ckpt_test_results.csv\n");
        return errno;
    }
    mean_ci_95(ckpt_samples, &ckpt_mean, &ckpt_ci);
    mean_ci_95(restore_samples, &restore_mean, &restore_ci);
    fprintf(file, "0x%x,%d,%d,%d,%d,%d,%.20f,%.20f,%.20f,%.20f\n",
            ALLOCATOR_AREA_SIZE, CF, MOD, WRITES + READS, WRITES, READS,
            ckpt_mean, ckpt_ci, restore_mean, restore_ci);
    fclose(file);

    file = fopen("ckpt_repeat_test_results.csv", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening ckpt_repeat_test_results.csv\n");
        return errno;
    }

    for (int i = 0; i < 1000; i++) {
#if CF == 1
        clean_cache(area);
#endif
        ckpt_samples[i] = test_checkpoint_repeat(area, value_8bit, value_16bit,
                                                 value_32bit, value_64bit, 10);
#if CF == 1
        clean_cache(area);
#endif
        restore_samples[i] = restore_area(area);
    }
    mean_ci_95(ckpt_samples, &ckpt_mean, &ckpt_ci);
    mean_ci_95(restore_samples, &restore_mean, &restore_ci);
    fprintf(file, "0x%x,%d,%d,%d,%d,%d,%d,%.20f,%.20f,%.20f,%.20f\n",
            ALLOCATOR_AREA_SIZE, CF, MOD, WRITES + READS, WRITES, READS, 10,
            ckpt_mean, ckpt_ci, restore_mean, restore_ci);
    fclose(file);

    return EXIT_SUCCESS;
}
