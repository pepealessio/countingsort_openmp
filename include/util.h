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
 * @file    util.h
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   This file contains utility function prototypes used to work with 
 *          random array and the declaration of the type you want use in the 
 *          counting sort algorithm.  
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _INC_TYPEH_
#define _INC_TYPEH_


#include <omp.h>
#include <stdlib.h>


#define STARTTIME(id)                           \
   double start_time_42_##id, end_time_42_##id; \
   start_time_42_##id = omp_get_wtime();

#define ENDTIME(id, x)                 \
   end_time_42_##id = omp_get_wtime(); \
   x = (end_time_42_##id - start_time_42_##id)


#define ELEMENT_TYPE int


void init_rand_vector(ELEMENT_TYPE **A, size_t A_len, long min_value, long max_value, int threads);
void deinit_rand_vector(ELEMENT_TYPE *A);


#endif /*_INC_TYPEH_*/
