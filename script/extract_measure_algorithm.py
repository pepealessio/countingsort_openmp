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
# ----------------------------------------------

# ------------- Don't edit below here ----------

RAW_FIELDS = ['t_seq', 't_par1_1th', 't_par1_2th', 't_par1_4th', 't_par1_8th', 't_par1_16th', 't_par2_1th', 't_par2_2th', 't_par2_4th', 't_par2_8th', 't_par2_16th'] 


import matplotlib.pyplot as plt
import os
import shutil
import sys
import numpy as np


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
if not( os.path.exists("measures/min_max/raw") ) or not(len(os.listdir("measures/min_max/raw")) == len(SIZE) + 1):
        #or os.path.exists("measures/occurrance/raw") ) or not(len(os.listdir("measures/occurrance/raw")) == len(SIZE) * len(RANGE) + 1)
        #or os.path.exists("measures/populate/raw") ) or not(len(os.listdir("measures/populate/raw")) == len(SIZE) * len(RANGE) + 1):
    print("Raw measures not present. Generate it with 'bash script/measure_algorithm.bash'")
    exit(1)

# Create the folder containing all the processed data
if os.path.exists("measures/min_max/processed"):
    shutil.rmtree("measures/min_max/processed")
os.mkdir("measures/min_max/processed")

if os.path.exists("measures/occurrance/processed"):
    shutil.rmtree("measures/occurrance/processed")
os.mkdir("measures/occurrance/processed")

if os.path.exists("measures/populate/processed"):
    shutil.rmtree("measures/populate/processed")
os.mkdir("measures/populate/processed")

# Create the folder containing all the plots
if os.path.exists("measures/min_max/plots"):
    shutil.rmtree("measures/min_max/plots")
os.mkdir("measures/min_max/plots")

if os.path.exists("measures/occurrance/plots"):
    shutil.rmtree("measures/occurrance/plots")
os.mkdir("measures/occurrance/plots")

if os.path.exists("measures/populate/plots"):
    shutil.rmtree("measures/populate/plots")
os.mkdir("measures/populate/plots")

print("---------- Elaborating for min_max ----------")

measures = np.zeros(shape=(len(SIZE), len(RAW_FIELDS[:6]), 3))

# Loop for each size
for size_index, size in enumerate(SIZE):

    # Print help
    print("Elaborating measured for size={}. #{} of {}.".format(size, 1+size_index, len(SIZE)))

    # Now open files and evaluate results
    measure = np.genfromtxt(f"measures/min_max/raw/size_{size}.csv", delimiter=',', invalid_raise = False)
    measure = np.sum(measure[1:, :], axis=0) / (measure.shape[0] - 1)
    
    measures[size_index, :, 0] = measure

    for i in range(len(RAW_FIELDS[:6])):
        # Compute speedup and efficiency for the current size
        measures[size_index, i, 1] = compute_speedup(measures[size_index, 0, 0], measures[size_index, i, 0])
        measures[size_index, i, 2] = compute_efficiency(measures[size_index, 0, 0], measures[size_index, i, 0], 2**(i-1) if i!= 0 else 1)

# Save processed measures
np.save(f"measures/min_max/processed/min_max.npy", measures)


# ------------- Generate plot plot -----------------
measure = np.load(f"measures/min_max/processed/min_max.npy")

t = [1,2,4,8,16]
fig, (ax1, ax2) = plt.subplots(2)

fig.suptitle(f'Min_Max', fontsize=6)

ax1.plot(t, t, 'g^-', label="Ideal Sp.")

for i, size in enumerate(SIZE):
    c = [float(i)/float(len(SIZE)), 0.0, float(len(SIZE)-i)/float(len(SIZE))] #R,G,B
    ax1.plot(t, measure[i, 1:, 1], label=f"Size {size}", color=c, marker='.')
    ax2.plot(t, measure[i, 1:, 2], label=f"Efficiency Size {size}", color=c, marker='.')

ax1.set_xlabel("#CPU")
ax1.set_ylabel("Speedup")

ax1.legend(loc='upper left', ncol=2, fontsize=6)

ax2.set_xlabel("#CPU")
ax2.set_ylabel("Efficiency")

ax1.label_outer()
fig.tight_layout()
        
plt.savefig(f"measures/min_max/plots/minmax.png")
plt.close()


print("---------- Elaborating for occurrance ----------")

# Loop for each size
for range_index, rang in enumerate(RANGE):

    measures = np.zeros(shape=(len(SIZE), len(RAW_FIELDS), 3))

    for size_index, size in enumerate(SIZE):
        # Print help
        print("Elaborating measured for size={}. #{} of {}.".format(size, 1+size_index+range_index*len(SIZE), len(SIZE)*len(RANGE)))

        # Now open files and evaluate results
        measure = np.genfromtxt(f"measures/occurrance/raw/size_{size}_range_{rang}.csv", delimiter=',', invalid_raise = False)
        measure = np.sum(measure[1:, :], axis=0) / (measure.shape[0] - 1)

        measures[size_index, :, 0] = measure

        for i in range(len(RAW_FIELDS)):
            # Compute speedup and efficiency for the current size
            measures[size_index, i, 1] = compute_speedup(measures[size_index, 0, 0], measures[size_index, i, 0])
            if i < 6:
                measures[size_index, i, 2] = compute_efficiency(measures[size_index, 0, 0], measures[size_index, i, 0], round(2**(i-1)) if i!= 0 else 1)
            else:
                measures[size_index, i, 2] = compute_efficiency(measures[size_index, 0, 0], measures[size_index, i, 0], round(2**(i-6)) if i!= 0 else 1)

        # Save processed measures
        np.save(f"measures/occurrance/processed/range_{rang}.npy", measures)


for range_index, rang in enumerate(RANGE):
    for size_index, size in enumerate(SIZE):
        measure = np.load(f"measures/occurrance/processed/range_{rang}.npy")

        # ------------- Generate plot plot -----------------
        t = [1,2,4,8,16]
        fig, ax = plt.subplots(2, 2)

        fig.suptitle(f'Occurrance range={rang}', fontsize=6)

        ax[0][0].plot(t, t, 'g^-', label="Ideal Sp.")
        ax[0][1].plot(t, t, 'g^-', label="Ideal Sp.")

        for i, size in enumerate(SIZE):
            c = [float(i)/float(len(SIZE)), 0.0, float(len(SIZE)-i)/float(len(SIZE))] #R,G,B
            ax[0][0].plot(t, measure[i, 1:6, 1], label=f"Size {size}", color=c, marker='.')
            ax[1][0].plot(t, measure[i, 1:6, 2], label=f"Efficiency Size {size}", color=c, marker='.')

            ax[0][1].plot(t, measure[i, 6:, 1], label=f"Exp.Sp. Size {size}", color=c, marker='.')
            ax[1][1].plot(t, measure[i, 6:, 2], label=f"Efficiency Size {size}", color=c, marker='.')

        ax[0][0].set_xlabel("#CPU")
        ax[0][1].set_xlabel("#CPU")
        ax[0][0].set_ylabel("Speedup")
        ax[1][0].set_ylabel("Efficiency")

        ax[0][0].legend(loc='upper left', fontsize=6)
        
        for ax in fig.get_axes():
            ax.label_outer()

        fig.tight_layout()

            
    plt.savefig(f"measures/occurrance/plots/occurrance_range{rang}.png")
    plt.close()


print("---------- Elaborating for populate ----------")

# Loop for each size
for range_index, rang in enumerate(RANGE):

    measures = np.zeros(shape=(len(SIZE), len(RAW_FIELDS), 3))

    for size_index, size in enumerate(SIZE):
        # Print help
        print("Elaborating measured for size={}. #{} of {}.".format(size, 1+size_index+range_index*len(SIZE), len(SIZE)*len(RANGE)))

        # Now open files and evaluate results
        measure = np.genfromtxt(f"measures/populate/raw/size_{size}_range_{rang}.csv", delimiter=',', invalid_raise = False)
        measure = np.sum(measure[1:, :], axis=0) / (measure.shape[0] - 1)

        measures[size_index, :, 0] = measure

        for i in range(len(RAW_FIELDS)):
            # Compute speedup and efficiency for the current size
            measures[size_index, i, 1] = compute_speedup(measures[size_index, 0, 0], measures[size_index, i, 0])
            if i < 6:
                measures[size_index, i, 2] = compute_efficiency(measures[size_index, 0, 0], measures[size_index, i, 0], round(2**(i-1)) if i!= 0 else 1)
            else:
                measures[size_index, i, 2] = compute_efficiency(measures[size_index, 0, 0], measures[size_index, i, 0], round(2**(i-6)) if i!= 0 else 1)

        # Save processed measures
        np.save(f"measures/populate/processed/range_{rang}.npy", measures)


for range_index, rang in enumerate(RANGE):
    for size_index, size in enumerate(SIZE):
        measure = np.load(f"measures/populate/processed/range_{rang}.npy")

        # ------------- Generate plot plot -----------------
        t = [1,2,4,8,16]
        fig, ax = plt.subplots(2, 2)

        fig.suptitle(f'Occurrance range={rang}', fontsize=6)

        ax[0][0].plot(t, t, 'g^-', label="Ideal Sp.")
        ax[0][1].plot(t, t, 'g^-', label="Ideal Sp.")

        for i, size in enumerate(SIZE):
            c = [float(i)/float(len(SIZE)), 0.0, float(len(SIZE)-i)/float(len(SIZE))] #R,G,B
            ax[0][0].plot(t, measure[i, 1:6, 1], label=f"Size {size}", color=c, marker='.')
            ax[1][0].plot(t, measure[i, 1:6, 2], label=f"Efficiency Size {size}", color=c, marker='.')

            ax[0][1].plot(t, measure[i, 6:, 1], label=f"Exp.Sp. Size {size}", color=c, marker='.')
            ax[1][1].plot(t, measure[i, 6:, 2], label=f"Efficiency Size {size}", color=c, marker='.')

        ax[0][0].set_xlabel("#CPU")
        ax[0][1].set_xlabel("#CPU")
        ax[0][0].set_ylabel("Speedup")
        ax[1][0].set_ylabel("Efficiency")

        ax[0][0].legend(loc='upper left', fontsize=6)
        
        for ax in fig.get_axes():
            ax.label_outer()

        fig.tight_layout()

            
    plt.savefig(f"measures/populate/plots/occurrance_range{rang}.png")
    plt.close()



print("Script ended successfully.")


# # Loop for each (size, optimization) couple
# for size_index, size in enumerate(SIZE):
#     for range_index, range in enumerate(RANGE):

#         # Print help
#         print("Elaborating measured for size={}, range={} and optimization={} - #{} of {}.".format(size, range, opt, 1+size_index*len(RANGE)*len(OPT_LEVEL)+range_index*len(OPT_LEVEL)+opt_index, len(SIZE)*len(RANGE)*len(OPT_LEVEL)))

#         # Compute refined measure for current size
#         measures = np.zeros(shape=(len(N_THREAD), len(RAW_FIELDS)+2))

#         for n_tx_index, n_th in enumerate(N_THREAD):
#             # Now open files and evaluate results
#             measure = np.genfromtxt(f"measures/counting_sort/raw/size_{size}_range_{range}_threads_{n_th}_opt_{opt}.csv", delimiter=',', invalid_raise = False)
#             measure = reject_outliers(measure[1:, :], MEASURE_ON_INDEX)
#             measure = np.sum(measure[:, :], axis=0) / (measure.shape[0])
            
#             measures[n_tx_index, :-2] = measure

#             # Compute speedup and efficiency for the current size
#             measures[n_tx_index, -2] = compute_speedup(measures[0, MEASURE_ON_INDEX], measures[n_tx_index, MEASURE_ON_INDEX])
#             measures[n_tx_index, -1] = compute_efficiency(measures[0, MEASURE_ON_INDEX], measures[n_tx_index, MEASURE_ON_INDEX], n_th)

#         # Save processed measures
#         np.savetxt(f"measures/counting_sort/processed/size_{size}_range_{range}_opt_{opt}.csv", measures, delimiter=',', header=','.join([*RAW_FIELDS, "speedup", "efficiency"]))

# # Loop for each problem size
# for size_index, size in enumerate(SIZE):
#     for range_index, range in enumerate(RANGE):
#         for opt_index, opt in enumerate(OPT_LEVEL):

#             # Print help
#             print("Plotting for size={}, range={}, and optimization={} - #{} of {}.".format(size, range, opt, 1+size_index*len(RANGE)*len(OPT_LEVEL)+range_index*len(OPT_LEVEL)+opt_index, len(SIZE)*len(RANGE)*len(OPT_LEVEL)))

#             measure = np.genfromtxt(f"measures/counting_sort/processed/size_{size}_range_{range}_opt_{opt}.csv", delimiter=',')

#             # ------------- Generate plot plot -----------------
#             t = N_THREAD[1:]
#             fig, ax1 = plt.subplots()
#             ax2 = ax1.twinx()

#             fig.suptitle(f'Size:{size} Range:{range} Opt:O{opt}', fontsize=15)

#             ax2.plot(t, measure[:, -1][1:], 'b.--', label="Efficiency")
#             ax2.set_xlabel("#CPU")
#             ax2.set_ylabel("Efficiency", color="blue")
#             ax2.tick_params(axis='y', colors="blue")

#             ax1.plot(t, t, 'r^-', label="Ideal Sp.")
#             ax1.plot(t, measure[:, -2][1:], 'g^-', label="Experimental Sp.")
#             ax1.set_xlabel("#CPU")
#             ax1.set_ylabel("Speedup")

#             fig.legend(loc='upper left', fontsize=8)
#             fig.tight_layout()

#             plt.savefig(f"measures/counting_sort/plots/size_{size}_range_{range}_opt_{opt}.png")
#             plt.close()

# print("Script ended successfully.")
