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
 * @file    minmax_getmeasures.c
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
#include "util.h"


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("USAGE: %s array_len\n", argv[0]);
        exit(1);
    }

    ELEMENT_TYPE *A;
    ELEMENT_TYPE min, max;
    double t[6];
    double start, end;
    size_t len = atoi(argv[1]);
    
    // Init a random vector
    srand(time(NULL));
    init_rand_vector(&A, len, 0, 1000, 0);

    // Measure for seuential algorithm
    STARTTIME(1);
    get_min_max(A, len, &min, &max);
    ENDTIME(1, t[0]); 

    // Measure for parallel algorithm 1
    for (size_t i = 0; i <= 4; i++)
    {
        STARTTIME(0);
        get_min_max_paral(A, len, (int)(pow(2,i)), &min, &max);
        ENDTIME(0, t[i+1]);
    }

    deinit_rand_vector(A);
    
    for (size_t i = 0; i < 6; i++)
    {
        if(i == 5)
        {
            printf("%f\n", t[i]);
        }
        else{
            printf("%f,", t[i]);
        }
    }

    return 0;   
}
