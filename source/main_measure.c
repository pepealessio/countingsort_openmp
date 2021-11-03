// Course:           High Performance Computing
// A.Y:              2021/22
// Lecturer:         Francesco Moscato           fmoscato@unisa.it

// Team:
// Alessio Pepe          0622701463      a.pepe108@studenti.unisa.it
// Teresa Tortorella     0622701507      t.tortorella3@studenti.unisa.it
// Paolo Mansi           0622701542      p.mansi5@studenti.unisa.it

// Copyright (C) 2021 - All Rights Reserved

// This file is part of Counting_Sort.

// Counting_Sort is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Counting_Sort is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Counting_Sort.  If not, see <http://www.gnu.org/licenses/>.

/**
 * @file    main_measure.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it) 
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "util.h"
#include "minmax.h"
#include "counting_occurrence.h"
#include "occurrences2ordarray.h"


int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        printf("USAGE: %s threads algo_num array_len element_range\n", argv[0]);
        exit(1);
    }

    ELEMENT_TYPE min, max;
    double time_min_max, time_occurrance, time_populate, time_algo, start, end;
    size_t nth = atoi(argv[1]);
    size_t algo_num = atoi(argv[2]);
    size_t len = atoi(argv[3]);
    size_t range = atoi(argv[4]);

    ELEMENT_TYPE *A;
    
    // Init a random vector
    init_rand_vector(&A, len, -56, range-56);

    STARTTIME(3); // Get a measure of all counting sort (and the additional if then of this script)

    // Compute min and max
    get_min_max(A, len, &min, &max);

    // ----- Measure min_max -------
    if (nth == 0)  // Measure for seuential algorithm
    {
        STARTTIME(0);
        get_min_max(A, len, &min, &max);
        ENDTIME(0, time_min_max); 
    }
    else  // Measure for parallel algorithm
    {
        STARTTIME(0);
        get_min_max_paral(A, len, nth, &min, &max);
        ENDTIME(0, time_min_max);
    }

    // ------ Measure count_occurrence --------

    // Allocate C array
    size_t C_len = max - min + 1;
    size_t *C = (size_t *) calloc(C_len, sizeof(size_t));

    if (nth == 0)  // Measure for seuential algorithm
    {
        STARTTIME(1);
        count_occurrence(A, len, min, C, C_len);
        ENDTIME(1, time_occurrance); 
    }
    else  // Measure for parallel algorithm
    {
        if (algo_num == 1)
        {
            STARTTIME(1);
            count_occurrence_paral_1(A, len, min, C, C_len, nth);
            ENDTIME(1, time_occurrance);
        }
        else if (algo_num == 2)
        {
            STARTTIME(1);
            count_occurrence_paral_2(A, len, min, C, C_len, nth);
            ENDTIME(1, time_occurrance);
        }
        else
        {
            printf("algo_num can be 1 or 2");
        }
    }

    // ------- Measure populate --------------

    
    if (nth == 0)  // Measure for seuential algorithm
    {
        STARTTIME(2);
        occurrence2ordarray(A, min, C, C_len);
        ENDTIME(2, time_populate); 
    }
    else  // Measure for parallel algorithm
    {
        if (algo_num == 1)
        {
            STARTTIME(2);
            occurrence2ordarray_paral_1(A, min, C, C_len, nth);
            ENDTIME(2, time_populate);
        }
        else if (algo_num == 2)
        {
            STARTTIME(2);
            occurrence2ordarray_paral_2(A, min, C, C_len, nth);
            ENDTIME(2, time_populate);
        }
        else
        {
            printf("algo_num can be 1 or 2");
        }
    }

    free(C);  // Deallocate frequence vector

    ENDTIME(3, time_algo);

    deinit_rand_vector(A);  // Deallocate array A

    // Expected in output:
    // size, range, n_th, t_min_max, t_count_occurrance, t_populate, t_algo 
    printf("%d, %d, %d, %f, %f, %f, %f\n", (int) len, (int) range, (int) nth, time_min_max, time_occurrance, time_populate, time_algo);

    return EXIT_SUCCESS;   
}