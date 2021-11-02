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
 * @file    counting_occurrence.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   This file contains the function to get the occurrane of each
 *          element in an array.  
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "counting_occurrence.h"
 

/**
 * @brief Count the occurrance of each element in teh array and return a pdf array.
 * 
 * @param A         The array to compute pdf.
 * @param A_len     The lenght of the A array.
 * @param min       The minimum value in the array.
 * @param C         A zeroes array that have as lenght the range of the elements of A.
 * @param C_len     The lenght of C array.
 */
void count_occurrence(ELEMENT_TYPE *A, size_t A_len, ELEMENT_TYPE min, size_t *C, size_t C_len)
{
    for (size_t i = 0; i < A_len; i++)
    {
        C[A[i] - min]++;
    }
}

/**
 * @brief Count the occurrance of each element in teh array and return a pdf array.
 * 
 * @param A         The array to compute pdf.
 * @param A_len     The lenght of the A array.
 * @param min       The minimum value in the array.
 * @param C         A zeroes array that have as lenght the range of the elements of A.
 * @param C_len     The lenght of C array.
 * @param tnum      The thread to use to run this function. If you want use 
 *                  default number use 0.
 */
void count_occurrence_paral_1(ELEMENT_TYPE *A, size_t A_len, ELEMENT_TYPE min, size_t *C, size_t C_len, int tnum)
{
    // Count the element frequency.
    #pragma omp parallel default(none) firstprivate(A, A_len, C_len, min) shared(C) num_threads(tnum)
    {
        // Each thread have a local C array inizialized to 0.
        size_t *C_loc = (size_t *) calloc(C_len, sizeof(size_t));

        // Each thread count frequency on a part of the array. They do not need to wait
        // the barrier. Otherwise thay can continue because they work only on their local
        // variable scope.
        #pragma omp for nowait
        for (size_t j = 0; j < A_len; j++)
        {
            C_loc[A[j] - min]++;
        }

        // Just one thread for time store his results in the global C array.
        #pragma omp critical
        {
            for (size_t k = 0; k < C_len; k++)
            {
                C[k] += C_loc[k];
            }
        }
        
        // Each thread dealloc him local temporary vector
        free(C_loc);
    }
}

/**
 * @brief Count the occurrance of each element in teh array and return a pdf array.
 * 
 * @param A         The array to compute pdf.
 * @param A_len     The lenght of the A array.
 * @param min       The minimum value in the array.
 * @param C         A zeroes array that have as lenght the range of the elements of A.
 * @param C_len     The lenght of C array.
 * @param tnum      The thread to use to run this function. If you want use 
 *                  default number use 0.
 */
void count_occurrence_paral_2(ELEMENT_TYPE *A, size_t A_len, ELEMENT_TYPE min, size_t *C, size_t C_len, int tnum)
{
    #pragma omp parallel for default(none) shared(C) firstprivate(A, A_len, C_len, min) num_threads(tnum)
    for (size_t i = 0; i < A_len; i++)
    {   
        #pragma omp atomic
        C[A[i] - min]++;
    }
}
