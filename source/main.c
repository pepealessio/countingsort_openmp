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
 * @file    main.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)  
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "counting_sort.h"


int main(int argc, char const *argv[])
{

	ELEMENT_TYPE *v;
	double time_init = 0.0, time_sort = 0.0;
	int len, range, threads;	

	if(argc < 4){
		printf("ERROR! Usage: %s len range threads", argv[0]);	
		exit(1);
	}

	len = atoi(argv[1]);
	range = atoi(argv[2]);
	threads = atoi(argv[3]);

	STARTTIME(1);
	init_rand_vector(&v, len, -1003, range-1003);
	ENDTIME(1, time_init);
		
	
	if (threads == 0)
	{
		STARTTIME(2);
		counting_sort(v, len);
		ENDTIME(2, time_sort);
	}
	else
	{
		STARTTIME(2);
		counting_sort_parall(v, len, threads);
		ENDTIME(2, time_sort);
	}

	deinit_rand_vector(v);

    printf("%d;%d;%d;%f;%f\n", len, range, threads, time_init, time_sort);

	return 0;
}
