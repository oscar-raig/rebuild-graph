#include <stdio.h>
#include "analitza.h"

vert vertex[VERTEXS];       /* Vector de Vertexs                              */
int dist[VERTEXS][VERTEXS]; /* Matriu de distncies                           */
int dis[VERTEXS];           /* Vector de distncies                           */
int Nvertexs;               /* Nombre de Vertexs                              */





/*******************************************************************************
********************************************************************************

FUNCI clustering()

AQUESTA FUNCIO CALCULA I RETORNA LA APORTACIO AL CLUSTERING
DEL NODE PASSAT COM A PARAMETRE (NO ESTA NORMALITZADA)

Clustering: how the other nodes are connected between them
********************************************************************************
*******************************************************************************/

float
clustering(int vert){
int i ,j ,a ,k,count=0,max=0;

/* Posibles conexions*/

max=(vertex[vert].degree*(vertex[vert].degree-1))/2;

/* Contem quantes connexions hi ha de les possibles sempre que el grau del
vertex sigui major que 1                                                      */

if (vertex[vert].degree>1){   
   for(i=0;i<(vertex[vert].degree);i++)
      {
      a=vertex[vert].nei[i];   // we got a neighbour
      for (k=i+1;k<vertex[vert].degree;k++)
         for(j=0;j<(vertex[a].degree);j++)        
            if(vertex[vert].nei[k]==vertex[a].nei[j]) count ++;
      }   
   }
else
   max=1;    /*    Per que els vertexs amb un sol vetinguin clustering=0    */

return((float)count/max);
}


/*******************************************************************************
********************************************************************************

FUNCIO be_ce() 

AQUESTA FUNCI CALCULA LA BETWEENNESS CENTRALITY DE CADA NODE
DEL GRAF UTILITZANT LALGORITME DE ULRIK BRANDES PERAL C LCUL
RPID DAQUESTA

********************************************************************************
*******************************************************************************/

int be_ce()
{
int i,k,j,a;
int fi=0,ncua=0,index=0,cua[VERTEXS]; /* Qua                                  */
int n_stack=0,stack[VERTEXS];         /* Stack                                */
int dis[VERTEXS],sig[VERTEXS];
float del[VERTEXS];

for (i=0;i<Nvertexs;i++)
   vertex[i].BC=0.0;    /* Inicialitzam la BC dels vertexs a 0                */

for (i=0;i<Nvertexs;i++)
   {

   for(j=0;j<Nvertexs;j++){   /* Inicialitzem els diferents vectors           */
      dis[j]=-1;
      sig[j]=0;
      vertex[j].nllista=0;
      }
   dis[i]=0;
   sig[i]=1;

   cua[0]=i;
   index=0;
   ncua=1;
   fi=1;
 
   while (ncua>0)
      {
      for(j=0;j<vertex[cua[index]].degree;j++)
         {
         a=vertex[cua[index]].nei[j];

         if (dis[a]<0)    /* Si encara no hem trobat la distancia a a         */
            {
            cua[fi]=a;    /* Fiquem el node a la qua                          */
            fi++;
            ncua++;
            dis[a]=dis[cua[index]]+1;  /* Fiquem la discia al node a        */
            }
         
         if (dis[a] == dis[cua[index]]+1) /* Si el cames mnim              */
           {
           sig[a]+=sig[cua[index]];
           vertex[a].llista[vertex[a].nllista] = cua[index];
           vertex[a].nllista++;
           }

         }

     stack[n_stack]=cua[index]; /* Fiquem lment que estem tractan de la     */
     n_stack++;                 /* cua a lstack                              */
            
     index ++;  /* Avancem una posici a la cua                               */
     ncua --;   /* I elminem el element que acabem de tractar                 */
        
     }    
   
   for(j=0;j<Nvertexs;j++)
      del[j]=0.0;
   
   while(index>0)
      {
      index--;

      for(j=0;j<vertex[cua[index]].nllista;j++)
         {
         k=vertex[cua[index]].llista[j];
         del[k]=del[k]+ ((float)sig[k]/(float)sig[cua[index]])*((float)(1.0+del[cua[index]]));
         }
      
      if(i!=cua[index])
         vertex[cua[index]].BC+=del[cua[index]];
      
      }                

   }

return(i);

}

/*******************************************************************************
********************************************************************************

FUNCI distancies()

AQUESTA FUNCI CALCULA ES DISTNCIES I EL CLUSTERING DEL GRAF
I GUARDA AL ARXIU clustering.txt EL CLUSTERING DEL NODE
I EL NOMBRE DE VERTEXS QUE HI HA A CADA DISTNCIA

********************************************************************************
*******************************************************************************/

int distancies()
{
FILE * sortida;
int i,j,fi,count=0,index=0,ncua=1,cua[VERTEXS],num,diametre_max=0,diametre=0;
float clus ,clus_total=0.0,dis_mitja_total=0.0,dis_mitja;

/* Obrim el arxiu on escriurem el la aportacio al  clustering
i el nombre de vertexs a cada distancia per a cada vertex                     */

if ((sortida=fopen("clustering.txt", "wt"))==NULL)  
   {printf("ep! no puc crear el fitxer.\n"); return -1; }

for (i=0;i<Nvertexs;i++)    /* Per a cada node del graf                       */
   {
       
/* primer calculem laportacial clustering del vertex                       */

   clus=clustering(i);
   clus_total = clus_total + clus;
   clus=clus/(float)Nvertexs;
   fprintf (sortida , "El Node %i\n\t aporta al clustering %f\n", i, clus);
   fprintf (sortida , "\t te una BC de %f\n", vertex[i].BC/(float)((Nvertexs-1)*(Nvertexs-2)));

/*  Inicialitzem el vector de distancies que usarem per controlar si ja hem
trobat la distancia a cada vertex                                             */

   for (j=0;j<Nvertexs;j++)
      dis[j]=-1;

   dis[i]=0;        /* La distncia dun node a ell mateix  0               */
   count=0;         /* nmero de nodes a cada distncia                       */
   dis_mitja=0.0;   /* distcia mitja                                        */
   num=0;
   diametre=1;      /* Mxima distncia per a un vrtex                       */

   index=0;         /* Inicialitzam la qua                                    */
   cua[index]=i;
   ncua=1;
   fi=0; 

   while (ncua!=0)
      {
      for(j=0;j<vertex[cua[index]].degree;j++)
         {
 /* Per a cada vei del primer element de la cua                               */

         if ( dis[vertex[cua[index]].nei[j]]==-1 ) /* Si encara no hem trobat */
            {                                      /* la distancia al node    */

/*          Si el node est ms lluny que el tim node trobat                */

            if(diametre<dis[cua[index]]+1){
               fprintf (sortida,"\t te %i nodes a la distancia %i\n",count,diametre);
               diametre=dis[cua[index]]+1;
               count=0;
               }

            count++;
        
            dis[vertex[cua[index]].nei[j]]=diametre;
            dis_mitja+=(float)diametre;
          
/*         afegim el vertex al final de la cua                                */

            ncua++;
            fi++;
            cua[fi]=vertex[cua[index]].nei[j];
            }

         }

/*    eliminem el primer element de la cua                                    */

      ncua--;
      index++;
      }

 fprintf (sortida,"\t te %i nodes a la distancia %i\n",count,diametre); 

/* calculem la distncia mitja del vertex i ho sumem a la global              */

 dis_mitja=(float) dis_mitja/(Nvertexs-1);
 dis_mitja_total+=dis_mitja;

 /* Comprobem si ha augmentat el diametre que teniem fins ara */

 if (diametre>diametre_max) diametre_max=diametre;

 }

/*  Un cop finalitzat lalgotime calculem el clustering del graf, la distncia
mitja entre nodes i ho mostrem per pantalla                                   */

clus_total =clus_total/(float)Nvertexs;
dis_mitja_total=(float) dis_mitja_total/Nvertexs ;

printf ("\t Clustering %f\n",clus_total);
printf ("\t Diametre %i\n",diametre_max);
printf ("\t Distancia mitja %f\n",dis_mitja_total); 

fclose(sortida);
return(1);
}


/*******************************************************************************
********************************************************************************

FUNCIO llegir_dades()

AQUESTA FUNCIO LLEGEIX EL GRAF DEL ARXIU PASSAT COM A PARAMETRE  I LES GUARDA
A LA VARIABLE GLOBAL vertex[]

********************************************************************************
*******************************************************************************/

int
llegir_dades(const char* nom, int &linies,int &maxveins,int &minveins, float &mitja ){
    
FILE * llista, * sortida;
int i,j, val, veins=0, posicio=0;
char c;


if ((llista=fopen(nom, "rt"))==NULL){
    return -1;
}
                 
/* Aprofitem la primera passada per calcular el nombre de vertexs i el  grau
mnim, maxim i mitja del graf                                                 */

linies=0, minveins=0,maxveins=0,mitja=0.0;
while (!feof(llista)) {
   c=fgetc(llista); 
   posicio++;
   if (!feof(llista) && c==' ') veins++;
   if (!feof(llista) && c=='\n') {
      veins++;
      if (veins > maxveins) maxveins=veins;
      if (veins < minveins) minveins=veins;
      if (posicio > 1){
         if(linies == 0) minveins=veins;
         linies++;
         mitja = mitja + static_cast<float>(veins);
         }
      posicio=0;
      veins=0;
      }
}

fclose(llista);

mitja  = static_cast<float>(mitja) / linies;
Nvertexs = linies;

/* Obrim de nou el fitxer i guardem la informaci del graf a l estructura
vertex[]. Aprofitem la passada pero generar el fitxer graus.txt on es guardar
el grau de cada node                                                          */

if ((sortida=fopen("graus.txt", "wt")) == NULL) {
    printf("ep! no puc crear el fitxer.\n");
    return -1;
}

if ((llista=fopen(nom, "rt")) == NULL) {
    printf("ep! no trobo el fitxer.\n"); return -1;
}


i = 0;
j = 0;

while (!feof(llista) && i < linies) {
        fscanf(llista, "%i", &val);
    vertex[i].nei[j] = val;
    j++;
    fscanf(llista, "%c" , &c);
    if (c == '\n') {
      vertex[i].name = i;
      vertex[i].degree = j;
      fprintf(sortida, "Node %i -> %i \n", i, j);
      i++;
      j = 0;
      }
  }

fclose(llista);
fclose(sortida);

    return 1;
}


