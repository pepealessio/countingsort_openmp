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
#include "counting_sort.h"


int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
    printf("USAGE: %s threads array_len element_range\n", argv[0]);
    exit(1);
    }

    ELEMENT_TYPE *A;
    double time_algo=0.0;
    size_t nth = atoi(argv[1]);
    size_t len = atoi(argv[2]);
    size_t range = atoi(argv[3]);

    // Init a random vector
    init_rand_vector(&A, len, -5654, range-5654);

    if(nth==0)
    {
        STARTTIME(0); 
        counting_sort(A, len);
        ENDTIME(0, time_algo);
    }
    else
    {
        STARTTIME(1); 
        counting_sort_parall(A, len, nth);
        ENDTIME(1, time_algo);
    }

    deinit_rand_vector(A);  // Deallocate array A

    // Expected in output:
    // size, range, n_th, t_min_max, t_count_occurrance, t_populate, t_algo 
    printf("%d,%d,%d,%f\n", (int) len, (int) range, (int) nth, time_algo);

    return EXIT_SUCCESS;   
}
