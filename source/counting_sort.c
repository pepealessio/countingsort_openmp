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
 * @file    counting_sort.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   Counting sort alghorithm in serial and parallelized (OpenMP) version. Serial
 *          version are inspired at https://it.wikipedia.org/wiki/Counting_sort.   
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include "counting_sort.h"
#include "util.h"
#include "min_max.h"
#include "counting_occurrence.h"
#include "occurrences2ordarray.h"


/**
 * @brief   Reorder the array using a counter-sort alghorithm.
 * 
 * @param v     The pointer to the array to reorder.
 * @param len   The lenght to the array to reorder.
 */
void counting_sort(ELEMENT_TYPE *A, size_t len) 
{
    ELEMENT_TYPE min, max;
    size_t *C;
    size_t C_len, k;

    //Compute max and min of v.
    get_min_max(A, len, &min, &max);

    // Construct a zeros array of lenght max-min+1.
    C_len = max - min + 1;
    C = (size_t *) calloc(C_len, sizeof(size_t));

    // Count the element frequency.
    count_occurrence(A, len, min, C, C_len);
    
    // Ordering based on the frequency array.
    occurrence2ordarray(A, min, C, C_len);
    
    free(C);    // Dealloc the frequency vector.
}

/**
 * @brief   Reorder the array using a counter-sort alghorithm parallelized.
 * 
 * @param v             The pointer to the array to reorder.
 * @param len           The lenght to the array to reorder.
 * @param threads       The thread to use to run this function. If you want use 
 *                      default number use 0.
 */
void counting_sort_parall(ELEMENT_TYPE *A, size_t len, int threads) 
{
    ELEMENT_TYPE min, max;
    size_t *C;
    size_t C_len;

    // Compute min and max
    get_min_max_paral(A, len, threads, &min, &max);

    // Construct a zeros array of lenght max-min+1.
    C_len = max - min + 1;
    C = (size_t *) calloc(C_len, sizeof(size_t));

    // Count the element frequency.
    count_occurrence_paral(A, len, min, C, C_len, threads);
    
    // Compose an index based locator to populate the vector v in a parallel way.
    occurrence2ordarray_paral(A, min, C, C_len, threads);
    
    // Dealloc the frequency vector.
    free(C); 
}

