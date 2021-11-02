#!/bin/bash

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

NMEASURES=150

ARRAY_RC=(10000 100000 1000000 1000000)
ARRAY_RANGE=(100 1000)

SCRIPTPATH=$( cd -- "$(dirname "${0}")" >/dev/null 2>&1 ; pwd -P )


if [[ -d ${SCRIPTPATH}/../measures/min_max ]]
then
	mv ${SCRIPTPATH}/../measures/min_max ${SCRIPTPATH}/../measures/min_max_$(date '+%Y-%m-%d_%H-%M-%S')
fi

for size in "${ARRAY_RC[@]}"; do
    # Output file
    OUT_FILE=${SCRIPTPATH}/../measures/min_max/raw/size_${size}.csv

    mkdir -p $(dirname $OUT_FILE) 2> /dev/null
    
    echo "$(date '+%Y-%m-%d_%H-%M-%S')\n${ARRAY_RC[@]}" > "${SCRIPTPATH}/../measures/min_max/raw/info.txt"
    
    echo $(basename ${OUT_FILE})
    echo "t_seq, t_par1_1th, t_par1_2th, t_par1_4th, t_par1_8th, t_par1_16th" > ${OUT_FILE}
    
    for ((i = 0 ; i < ${NMEASURES}	; i++)); do
        (./build/mmg ${size} )2>&1 >> ${OUT_FILE}

        # Progress bar
        printf "\r> %d/%d %3.1d%% " $(expr ${i} + 1) ${NMEASURES} $(expr \( \( ${i} + 1 \) \* 100 \) / ${NMEASURES})
        printf "#%.0s" $(seq -s " " 1 $(expr \( ${i} \* 40 \) / ${NMEASURES}))

    done

    # End Progress Bar
    printf "\n"
done


if [[ -d ${SCRIPTPATH}/../measures/occurrance ]]
then
	mv ${SCRIPTPATH}/../measures/occurrance ${SCRIPTPATH}/../measures/occurrance$(date '+%Y-%m-%d_%H-%M-%S')
fi

for size in "${ARRAY_RC[@]}"; do
    for range in "${ARRAY_RANGE[@]}"; do
        # Output file
        OUT_FILE=${SCRIPTPATH}/../measures/occurrance/raw/size_${size}_range_${range}.csv

        mkdir -p $(dirname $OUT_FILE) 2> /dev/null
        
        echo "$(date '+%Y-%m-%d_%H-%M-%S')\n${ARRAY_RANGE[@]}\n${ARRAY_RC[@]}" > "${SCRIPTPATH}/../measures/occurrance/raw/info.txt"
        
        echo $(basename ${OUT_FILE})
        echo "t_seq, t_par1_1th, t_par1_2th, t_par1_4th, t_par1_8th, t_par1_16th, t_par2_1th, t_par2_2th, t_par2_4th, t_par2_8th, t_par2_16th" > ${OUT_FILE}
        
        for ((i = 0 ; i < ${NMEASURES}	; i++)); do
            (./build/cog ${size} ${range})2>&1 >> ${OUT_FILE}

            # Progress bar
            printf "\r> %d/%d %3.1d%% " $(expr ${i} + 1) ${NMEASURES} $(expr \( \( ${i} + 1 \) \* 100 \) / ${NMEASURES})
            printf "#%.0s" $(seq -s " " 1 $(expr \( ${i} \* 40 \) / ${NMEASURES}))

        done
        # End Progress Bar
        printf "\n"

    done
done


if [[ -d ${SCRIPTPATH}/../measures/populate ]]
then
	mv ${SCRIPTPATH}/../measures/populate ${SCRIPTPATH}/../measures/populate$(date '+%Y-%m-%d_%H-%M-%S')
fi

for size in "${ARRAY_RC[@]}"; do
    for range in "${ARRAY_RANGE[@]}"; do
        # Output file
        OUT_FILE=${SCRIPTPATH}/../measures/populate/raw/size_${size}_range_${range}.csv

        mkdir -p $(dirname $OUT_FILE) 2> /dev/null
        
        echo "$(date '+%Y-%m-%d_%H-%M-%S')\n${ARRAY_RANGE[@]}\n${ARRAY_RC[@]}" > "${SCRIPTPATH}/../measures/populate/raw/info.txt"
        
        echo $(basename ${OUT_FILE})
        echo "t_seq, t_par1_1th, t_par1_2th, t_par1_4th, t_par1_8th, t_par1_16th, t_par2_1th, t_par2_2th, t_par2_4th, t_par2_8th, t_par2_16th" > ${OUT_FILE}
        
        for ((i = 0 ; i < ${NMEASURES}	; i++)); do
            (./build/oog ${size} ${range})2>&1 >> ${OUT_FILE}

            # Progress bar
            printf "\r> %d/%d %3.1d%% " $(expr ${i} + 1) ${NMEASURES} $(expr \( \( ${i} + 1 \) \* 100 \) / ${NMEASURES})
            printf "#%.0s" $(seq -s " " 1 $(expr \( ${i} \* 40 \) / ${NMEASURES}))

        done
        # End Progress Bar
        printf "\n"
        
    done
done
