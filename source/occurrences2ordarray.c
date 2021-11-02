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
 * @file    occurrences2ordarray.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   This file contains the function to populate an array 
 *          basing on the pdf array.  
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "counting_occurrence.h"


/**
 * @brief Populate an array in an ordinate way based on a pdf array.
 * 
 * @param A     The array to populate.
 * @param min   The minimum value in a.
 * @param C     The pdf array.
 * @param C_len The pdf array lenght.
 */
void occurrence2ordarray(ELEMENT_TYPE *A, ELEMENT_TYPE min, size_t *C, size_t C_len)
{
    size_t k = 0;

    for (size_t i = 0; i < C_len; i++)
    {
        for (size_t j = 0; j < C[i]; j++)
        {
            A[k++] = i + min;
        }
    }
}

/**
 * @brief Populate an array in an ordinate way based on a pdf array.
 * 
 * @param A     The array to populate.
 * @param min   The minimum value in a.
 * @param C     The pdf array.
 * @param C_len The pdf array lenght.
 * @param tnum  The thread to use to run this function. If you want use 
 *              default number use 0.
 */
void occurrence2ordarray_paral_1(ELEMENT_TYPE *A, ELEMENT_TYPE min, size_t *C, size_t C_len, int tnum)
{
    // Compose an index based locator to populate the vector v.
    for (size_t i = 1; i < C_len; i++)
    {
        C[i] += C[i-1];
    }
    
    // Ordering based on the frequency array.
    #pragma omp parallel for default(none) firstprivate(A, C, C_len, min) num_threads(tnum)
    for (size_t i = 0; i < C_len; i++)
    {
        size_t start = (i != 0 ? C[i-1] : 0);
        for (size_t j = start; j < C[i]; j++)
        {
            A[j] = i + min;
        }
    }
}

/**
 * @brief Populate an array in an ordinate way based on a pdf array.
 * 
 * @param A     The array to populate.
 * @param min   The minimum value in a.
 * @param C     The pdf array.
 * @param C_len The pdf array lenght.
 * @param tnum  The thread to use to run this function. If you want use 
 *              default number use 0.
 */
void occurrence2ordarray_paral_2(ELEMENT_TYPE *A, ELEMENT_TYPE min, size_t *C, size_t C_len, int tnum)
{
    // Compose an index based locator to populate the vector v.
    #pragma omp parallel for ordered default(none) firstprivate(C_len) shared(C) 
    for (size_t i = 1; i < C_len; i++)
    {
        #pragma omp ordered
        {
            C[i] += C[i-1];
        }
    }
    
    // Ordering based on the frequency array.
    #pragma omp parallel for default(none) firstprivate(A, C, C_len, min) num_threads(tnum)
    for (size_t i = 0; i < C_len; i++)
    {
        size_t start = (i != 0 ? C[i-1] : 0);
        for (size_t j = start; j < C[i]; j++)
        {
            A[j] = i + min;
        }
    }
}
