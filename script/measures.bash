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


# ---------- Edit this section ---------------------
NMEASURES=100

ARRAY_RC=(1000 10000 100000 1000000 10000000)
ARRAY_RANGE=(1000 10000 100000)
ARRAY_THS=(0 1 2 4 8)
ARRAY_OPT=(0 1 2 3)
# --------------------------------------------------


TIMEFORMAT='%3U;%3E;%3S;%P'

trap "exit" INT

SCRIPTPATH=$( cd -- "$(dirname "${0}")" >/dev/null 2>&1 ; pwd -P )

if [[ -d ${SCRIPTPATH}/../measures ]]
then
	mkdir -p ${SCRIPTPATH}/../measures ${SCRIPTPATH}/../.oldmeasures/
	mv ${SCRIPTPATH}/../measures ${SCRIPTPATH}/../.oldmeasures/$(date '+%Y-%m-%d_%H-%M-%S')
fi

for opt in "${ARRAY_OPT[@]}"; do
	for size in "${ARRAY_RC[@]}"; do
		for range in "${ARRAY_RANGE[@]}"; do
			for ths in "${ARRAY_THS[@]}"; do
			
				ths_str=$(printf "%02d" $ths)
				
				# Output file
				OUT_FILE=${SCRIPTPATH}/../measures/raw/opt_${opt}_size_${size}_range_${range}_threads_${ths}.csv
			
				mkdir -p $(dirname $OUT_FILE) 2> /dev/null
				
				echo $(basename ${OUT_FILE})
				echo "size,range,n_threads,t_algo" > ${OUT_FILE}
				
				for ((i = 0 ; i < ${NMEASURES}	; i++)); do
					if [[ ${ths} -eq 0 ]]; then
						(./build/main_measures_seq_O${opt} ${ths} ${size} ${range} )2>&1 >> ${OUT_FILE}
					else
						(./build/main_measures_O${opt} ${ths} ${size} ${range} )2>&1 >> ${OUT_FILE}
					fi
					
					# Progress bar
					printf "\r> %d/%d %3.1d%% " $(expr ${i} + 1) ${NMEASURES} $(expr \( \( ${i} + 1 \) \* 100 \) / ${NMEASURES})
					printf "#%.0s" $(seq -s " " 1 $(expr \( ${i} \* 40 \) / ${NMEASURES}))

				done

				# End Progress Bar
				printf "\n"
			done
		done
	done
done
