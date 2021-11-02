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
 * @file    minmax.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   This file contains the function to get the maximum and the minimum
 *          of an array.  
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "minmax.h"


/**
 * @brief Get the min and the max value in the array A.
 * 
 * @param A         The pointer to the array.
 * @param A_len     The lenght of the array.
 * @param min       The pointer were store the minimum. 
 * @param max       The pointer were store the maximum.
 */
void get_min_max(ELEMENT_TYPE *A, size_t A_len, ELEMENT_TYPE* min, ELEMENT_TYPE* max)
{
    *min = A[0];
    *max = A[0];

    for (size_t i = 1; i < A_len; i++)
    {
        if (A[i] < *min)
        {
            *min = A[i];
        }
        else if (A[i] > *max)
        {
            *max = A[i];
        }
    }
}

/**
 * @brief Get the min and the max value in the array A.
 * 
 * @param A         The pointer to the array.
 * @param A_len     The lenght of the array.
 * @param tnum      The thread to use to run this function. If you want use 
 *                  default number use 0.
 * @param min       The pointer were store the minimum. 
 * @param max       The pointer were store the maximum.
 */
void get_min_max_paral(ELEMENT_TYPE *A, size_t A_len, int tnum, ELEMENT_TYPE* min, ELEMENT_TYPE* max)
{
    *min = A[0];
    *max = A[0];

    #pragma omp parallel default(none) firstprivate(A_len, A) shared(max, min) num_threads(tnum) 
    {
        // We made a local variable for max and min for each thread
        ELEMENT_TYPE l_min = A[0];
        ELEMENT_TYPE l_max = A[0];

        // Each thread compute max and min on his part of the array. That don't need
        // to wait because the next part depends just on the local min and max of the thread.
        #pragma omp for nowait
        for (size_t i = 1; i < A_len; i++)
        {
            if (A[i] < l_min)
            {
                l_min = A[i];
            }
            else if (A[i] > l_max)
            {
                l_max = A[i];
            }
        }

        // Each thread update the global min and max.
        #pragma omp critical
        {
            if (l_min < *min)
            {
                *min = l_min;
            }
            if (l_max > *max)
            {
                *max = l_max;
            }
        }
    }
}
