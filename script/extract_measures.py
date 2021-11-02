#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Course:           High Performance Computing
# A.Y:              2021/22
# Lecturer:         Francesco Moscato           fmoscato@unisa.it

# Team:
# Alessio Pepe          0622701463      a.pepe108@studenti.unisa.it
# Teresa Tortorella     0622701507      t.tortorella3@studenti.unisa.it
# Paolo Mansi           0622701542      p.mansi5@studenti.unisa.it

# Copyright (C) 2021 - All Rights Reserved

# This file is part of Counting_Sort.

# Counting_Sort is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Counting_Sort is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Counting_Sort.  If not, see <http://www.gnu.org/licenses/>.

__author__ = ["Alessio Pepe", "Teresa Tortorella", "Paolo Mansi"]
__copyright__ = "Copyright 2021"
__license__ = "GNU-GPL v3"
__version__ = "1.0.0"
# ------------- Don't edit up here -------------

# --------- You can edit this fields -----------
SIZE = [10000, 100000, 1000000, 1000000]
RANGE = [100, 1000]
OPT_LEVEL = [0, 1, 2, 3]
N_THREAD = [0, 1, 2, 4, 8, 16]
# ----------------------------------------------

# ------------- Don't edit below here ----------

RAW_FIELDS = ['len', 'range', 'threads', 'time_init', 'time_sort' ,'time_user' ,'time_elapsed' ,'time_sys' ,'p_cpu'] 
MEASURE_ON_INDEX = 4


import matplotlib.pyplot as plt
import os
import shutil
import sys
import numpy as np
# from gaussian_outliers import reject_outliers


def reject_outliers(data, index=0):
    """This function remove the outside value from a numpy array considering a gaussian distribution. Remove data 
    points that are not in [mean - std; mean + std] (about 68% of the data).
    
    :param data:    an numpy array representing the data points;
    :param index:   the column to evaluate in order to have all the data filtered. If there's just one column leave 0 as default;
    
    :return:        the filtered data."""

    # Compute mean and std. deviation of the data
    u = np.mean(data[:, index])
    s = np.std(data[:, index])

    # Filter samples out of [mean - std; mean + std]
    filtered = [row for row in data if (u - s < float(row[index]) < u + s)]

    # Return the filtered array
    return np.array(filtered, dtype=np.float64)


def compute_speedup(tser, tpar):
    return tser / tpar


def compute_efficiency(tser, tpar, nth):
    return compute_speedup(tser, tpar) / (nth if nth != 0 else 1)


# ---------------- Data processing --------------------------------------------------

# Check if RAW data are present. If not, exit the script.
if not( os.path.exists("measures/counting_sort/raw") ) or not(len(os.listdir("measures/counting_sort/raw")) == len(SIZE) * len(RANGE) * len(OPT_LEVEL) * len(N_THREAD) + 1) :
    print(f"Raw measures not present. Generate it with 'bash script/measures.bash'")
    exit(1)

# Create the folder containing all the processed data
if os.path.exists("measures/counting_sort/processed"):
    shutil.rmtree("measures/counting_sort/processed")
os.mkdir("measures/counting_sort/processed")

# Create the folder containing all the plots
if os.path.exists("measures/counting_sort/plots"):
    shutil.rmtree("measures/counting_sort/plots")
os.mkdir("measures/counting_sort/plots")

# Loop for each (size, optimization) couple
for size_index, size in enumerate(SIZE):
    for range_index, range in enumerate(RANGE):
        for opt_index, opt in enumerate(OPT_LEVEL):

            # Print help
            print("Elaborating measured for size={}, range={} and optimization={} - #{} of {}.".format(size, range, opt, 1+size_index*len(RANGE)*len(OPT_LEVEL)+range_index*len(OPT_LEVEL)+opt_index, len(SIZE)*len(RANGE)*len(OPT_LEVEL)))

            # Compute refined measure for current size
            measures = np.zeros(shape=(len(N_THREAD), len(RAW_FIELDS)+2))

            for n_tx_index, n_th in enumerate(N_THREAD):
                # Now open files and evaluate results
                measure = np.genfromtxt(f"measures/counting_sort/raw/size_{size}_range_{range}_threads_{n_th}_opt_{opt}.csv", delimiter=',', invalid_raise = False)
                measure = reject_outliers(measure[1:, :], MEASURE_ON_INDEX)
                measure = np.sum(measure[:, :], axis=0) / (measure.shape[0])
                
                measures[n_tx_index, :-2] = measure

                # Compute speedup and efficiency for the current size
                measures[n_tx_index, -2] = compute_speedup(measures[0, MEASURE_ON_INDEX], measures[n_tx_index, MEASURE_ON_INDEX])
                measures[n_tx_index, -1] = compute_efficiency(measures[0, MEASURE_ON_INDEX], measures[n_tx_index, MEASURE_ON_INDEX], n_th)

            # Save processed measures
            np.savetxt(f"measures/counting_sort/processed/size_{size}_range_{range}_opt_{opt}.csv", measures, delimiter=',', header=','.join([*RAW_FIELDS, "speedup", "efficiency"]))

# Loop for each problem size
for size_index, size in enumerate(SIZE):
    for range_index, range in enumerate(RANGE):
        for opt_index, opt in enumerate(OPT_LEVEL):

            # Print help
            print("Plotting for size={}, range={}, and optimization={} - #{} of {}.".format(size, range, opt, 1+size_index*len(RANGE)*len(OPT_LEVEL)+range_index*len(OPT_LEVEL)+opt_index, len(SIZE)*len(RANGE)*len(OPT_LEVEL)))

            measure = np.genfromtxt(f"measures/counting_sort/processed/size_{size}_range_{range}_opt_{opt}.csv", delimiter=',')

            # ------------- Generate plot plot -----------------
            t = N_THREAD[1:]
            fig, ax1 = plt.subplots()
            ax2 = ax1.twinx()

            fig.suptitle(f'Size:{size} Range:{range} Opt:O{opt}', fontsize=15)

            ax2.plot(t, measure[:, -1][1:], 'b.--', label="Efficiency")
            ax2.set_xlabel("#CPU")
            ax2.set_ylabel("Efficiency", color="blue")
            ax2.tick_params(axis='y', colors="blue")

            ax1.plot(t, t, 'r^-', label="Ideal Sp.")
            ax1.plot(t, measure[:, -2][1:], 'g^-', label="Experimental Sp.")
            ax1.set_xlabel("#CPU")
            ax1.set_ylabel("Speedup")

            fig.legend(loc='upper left', fontsize=8)
            fig.tight_layout()

            plt.savefig(f"measures/counting_sort/plots/size_{size}_range_{range}_opt_{opt}.png")
            plt.close()

print("Script ended successfully.")
