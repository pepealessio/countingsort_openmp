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
 * @file    occurrance2ordarray_getmeasures.c
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
#include "minmax.h"
#include "counting_occurrence.h"
#include "occurrences2ordarray.h"


int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("USAGE: %s array_len element_range\n", argv[0]);
        exit(1);
    }

    ELEMENT_TYPE min, max;
    ELEMENT_TYPE *A;
    double t[11];
    double start, end;
    size_t *C, *C_cpy;
    size_t C_len;
    size_t len = atoi(argv[1]);
    size_t range = atoi(argv[2]);

    
    // Init a random vector
    srand(time(NULL));
    init_rand_vector(&A, len, -56, range-56);

    // Get min and max
    get_min_max_paral(A, len, 0, &min, &max);

    // Allocate C array
    C_len = max - min + 1;
    C = (size_t *) calloc(C_len, sizeof(size_t));

    // Compute C array
    count_occurrence_paral(A, len, min, C, C_len, 0);

    // Made a copy of C array so all alghorithm can work on the same data
    C_cpy = (size_t *) malloc(C_len * sizeof(size_t));

    // Measure for seuential algorithm
    memcpy(C_cpy, C, C_len * sizeof(size_t));
    STARTTIME(0);
    occurrence2ordarray(A, min, C_cpy, C_len);
    ENDTIME(0, t[0]); 

    // Measure for parallel algorithm 1
    for (size_t i = 0; i <= 4; i++)
    {
        memcpy(C_cpy, C, C_len * sizeof(size_t));
        STARTTIME(0);
        occurrence2ordarray_paral_1(A, min, C_cpy, C_len, (int)(pow(2,i)));
        ENDTIME(0, t[i+1]);
    }

    // Measure for parallel algorithm 2
    for (size_t i = 0; i <= 4; i++)
    {
        memcpy(C_cpy, C, C_len * sizeof(size_t));
        STARTTIME(0);
        occurrence2ordarray_paral_2(A, min, C_cpy, C_len, (int)(pow(2,i)));
        ENDTIME(0, t[6+i]);
    }
    
    deinit_rand_vector(A);
    free(C);
    free(C_cpy);

    for (size_t i = 0; i < 11; i++)
    {
        if(i == 10)
        {
            printf("%f\n", t[i]);
        }
        else{
            printf("%f,", t[i]);
        }
    }

    return 0;   
}
