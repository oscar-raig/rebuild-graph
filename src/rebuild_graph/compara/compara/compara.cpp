/********************************* LLIBRERIES *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <memory.h>
#include "compara.h"


#include <matrix/gsl_matrix.h>  //Per la descomposicio en valors singulars


/******************************************************************************/
/* Carrega en una estructura TGraf la matriu descrita en el fitxer *nom
   Omple l'ordre, el grau_max, conn[][] i grau[]. Torna el nombre de posicions que
   no s