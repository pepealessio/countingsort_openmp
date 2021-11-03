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
 * @file    occurrences2ordarray.h
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   This file contains the function prototypes to populate an array 
 *          basing on the pdf array.  
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _INC_OCCURRENCE2ORDARRAY_
#define _INC_OCCURRENCE2ORDARRAY_

#include <stdlib.h>
#include "util.h"

#define occurrence2ordarray_paral occurrence2ordarray_paral_2

void occurrence2ordarray(ELEMENT_TYPE *A, ELEMENT_TYPE min, size_t *C, size_t C_len);
void occurrence2ordarray_paral_1(ELEMENT_TYPE *A, ELEMENT_TYPE min, size_t *C, size_t C_len, int tnum);
void occurrence2ordarray_paral_2(ELEMENT_TYPE *A, ELEMENT_TYPE min, size_t *C, size_t C_len, int tnum);

#endif /*_INC_OCCURRENCE2ORDARRAY_*/