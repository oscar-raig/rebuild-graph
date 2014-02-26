#include <stdio.h>
#define VERTEXS 5000
#define DEGREE 40
#define MAXTBL 110


/*******************************************************************************
********************************************************************************

DEFINICIO DELS TIPUS I VARIABLES GLOBALS

********************************************************************************
*******************************************************************************/

typedef struct {
int     name;               /* Nom                                            */
int     degree;             /* Grau                                           */
int     nei[DEGREE*3];      /* Veins                                          */
float   BC;                 /* Beetweenness Centrality                        */
int     nllista;            /* Llista per al c�lcul de la                     */
int     llista[VERTEXS];    /* Beetweenness Centrality                        */
} vert;
    
vert vertex[VERTEXS];       /* Vector de Vertexs                              */
int dist[VERTEXS][VERTEXS]; /* Matriu de dist�ncies                           */
int dis[VERTEXS];           /* Vector de dist�ncies                           */
int Nvertexs;               /* Nombre de Vertexs                              */
char *nom;                  /* String on guardem el arxiu d�entrada           */

/*******************************************************************************
********************************************************************************

FUNCI� clustering()

AQUESTA FUNCIO CALCULA I RETORNA LA APORTACIO AL CLUSTERING
DEL NODE PASSAT COM A PARAMETRE (NO ESTA NORMALITZADA)

********************************************************************************
*******************************************************************************/

float clustering(int vert)
{
int i ,j ,a ,k,count=0,max=0;

/* Posibles conexions*/

max=(vertex[vert].degree*(vertex[vert].degree-1))/2; 

/* Contem quantes connexions hi ha de les possibles sempre que el grau del
vertex sigui major que 1                                                      */

if (vertex[vert].degree>1){   
   for(i=0;i<(vertex[vert].degree);i++)
      {
      a=vertex[vert].nei[i];   
      for (k=i+1;k<vertex[vert].degree;k++)
         for(j=0;j<(vertex[a].degree);j++)        
            if(vertex[vert].nei[k]==vertex[a].nei[j]) count ++;
      }   
   }
else
   max=1;    /*    Per que els vertexs amb un sol ve� tinguin clustering=0    */

return((float)count/max);
}


/*******************************************************************************
********************************************************************************

FUNCIO be_ce() 

AQUESTA FUNCI� CALCULA LA BETWEENNESS CENTRALITY DE CADA NODE
DEL GRAF UTILITZANT L�ALGORITME DE ULRIK BRANDES PERAL C�LCUL
R�PID D�AQUESTA

********************************************************************************
*******************************************************************************/

int be_ce()
{
int i,k,j,a,b;
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
            dis[a]=dis[cua[index]]+1;  /* Fiquem la dis�ncia al node a        */
            }
         
         if (dis[a] == dis[cua[index]]+1) /* Si el cam� es m�nim              */
           {
           sig[a]+=sig[cua[index]];
           vertex[a].llista[vertex[a].nllista] = cua[index];
           vertex[a].nllista++;
           }

         }

     stack[n_stack]=cua[index]; /* Fiquem l�ement que estem tractan de la     */
     n_stack++;                 /* cua a l�stack                              */
            
     index ++;  /* Avancem una posici� a la cua                               */
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

FUNCI� distancies()

AQUESTA FUNCI� CALCULA ES DIST�NCIES I EL CLUSTERING DEL GRAF
I GUARDA AL ARXIU clustering.txt EL CLUSTERING DEL NODE
I EL NOMBRE DE VERTEXS QUE HI HA A CADA DIST�NCIA

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
   {printf("ep! no puc crear el fitxer.\n"); exit(1); }

for (i=0;i<Nvertexs;i++)    /* Per a cada node del graf                       */
   {

/* primer calculem l�aportaci� al clustering del vertex                       */

   clus=clustering(i);
   clus_total = clus_total + clus;
   clus=clus/(float)Nvertexs;
   fprintf (sortida , "El Node %i\n\t aporta al clustering %f\n", i, clus);
   fprintf (sortida , "\t te una BC de %f\n", vertex[i].BC/(float)((Nvertexs-1)*(Nvertexs-2)));

/*  Inicialitzem el vector de distancies que usarem per controlar si ja hem
trobat la distancia a cada vertex                                             */

   for (j=0;j<Nvertexs;j++)
      dis[j]=-1;

   dis[i]=0;        /* La dist�ncia d�un node a ell mateix �s 0               */
   count=0;         /* n�mero de nodes a cada dist�ncia                       */
   dis_mitja=0.0;   /* dist�ncia mitja                                        */
   num=0;
   diametre=1;      /* M�xima dist�ncia per a un v�rtex                       */

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

/*          Si el node est� m�s lluny que el �ltim node trobat                */

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

/* calculem la dist�ncia mitja del vertex i ho sumem a la global              */

 dis_mitja=(float) dis_mitja/(Nvertexs-1);
 dis_mitja_total+=dis_mitja;

 /* Comprobem si ha augmentat el diametre que teniem fins ara */

 if (diametre>diametre_max) diametre_max=diametre;

 }

/*  Un cop finalitzat l�algotime calculem el clustering del graf, la dist�ncia
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

FUNCIO distancies_matriu()

AQUESTA FUNCI� INTENTA CALCULAR LA MATRIU DE DIST�NCIES DEL GRAF.
PER FER-HO SEGUEIZ ELS SEG�ENTS PASOS:

1.- INICIALITZA LA MATRIU A VERTEXS+1 I LA DIAGONAL A 0
2.- PER A CADA VERTEX FICA LA DIST�NCIA ALS SEUS VEINS A 1

|   0 1001 1001 1001 1001|     |   0    1 1001 1001 1001|
|1001    0 1001 1001 1001|     |   1    0    1    1 1001|
|1001 1001    0 1001 1001| =>  |1001    1    0    1 1001|
|1001 1001 1001    0 1001|     |1001    1    1    0    1|
|1001 1001 1001 1001    0|     |1001 1001 1001    1    0|

3.-ANEM RECORRENT LA MATRIU REPETIDAMENT COMPROBANT PER A CADA DISTANCIA
ENTRE NODES I-J SI EXISTEIX UN NODE L TAL QUE:

DISTACIA I-L + DISTANCIA L-J < DISTANCIA QUE TENIM GUARDADA DE I-J

4.- ES VA RECORREGUENT LA MATRIU FINS QUE DURANT TOT UN RECORREGUT NO ES
REALITZA CAP CAMBI.

UN COP ROBAT EL FUNCIONAMENT DE LA FUNCI� ENS VAREM ADONAR DE QUE PER
A GRAFS GRANS EL RENDIMENT ERA MASSA POBRE I NO ERA VIABLE UTILITZAR-LA

********************************************************************************
*******************************************************************************/


int distancies_matriu()
{

int repetir,i,j,k,l;
for (i=0;i<Nvertexs;i++)
   {
   dist[i][i]=0;
   for (j=i+1;j<Nvertexs;j++)
      {
      dist[i][j]=VERTEXS+1;
	  dist[j][i]=VERTEXS+1;
	  }
   }

for (i=0;i<Nvertexs;i++)
   for (j=0;j<vertex[i].degree;j++)
      dist[i][vertex[i].nei[j]]=1;

repetir=1;

while(repetir==1)

{
repetir=0;

for (i=0;i<Nvertexs;i++)
   for (j=0;j<Nvertexs;j++)
      for (l=0;l<Nvertexs;l++)
         {
            if ( (dist[i][l] + dist[l][j]) < dist[i][j])
               {
               repetir= 1 ;
               dist[i][j]=dist[i][l]+dist[l][j];
               dist[j][i]=dist[i][l]+dist[l][j];
               }
        }
}
return (1);

}

           
/*******************************************************************************
********************************************************************************

FUNCIO llegir_dades()

AQUESTA FUNCIO LLEGEIX EL GRAF DEL ARXIU PASSAT COM A PARAMETRE  I LES GUARDA
A LA VARIABLE GLOBAL vertex[]

********************************************************************************
*******************************************************************************/

int llegir_dades()
{
FILE * llista, * sortida;
int i,j, val,linies=0, minveins=0,maxveins=0, veins=0, posicio=0;
float mitja=0.0;
char c;


if ((llista=fopen(nom, "rt"))==NULL)
   {printf("ep! no trobo el fitxer.\n"); exit(1); }
                 
/* Aprofitem la primera passada per calcular el nombre de vertexs i el  grau
m�nim, maxim i mitja del graf                                                 */

while (!feof(llista)) {
   c=fgetc(llista); 
   posicio++;
   if (!feof(llista) && c==' ') veins++;
   if (!feof(llista) && c=='\n') {
      veins++;
      if (veins>maxveins) maxveins=veins;
      if (veins<minveins) minveins=veins;
      if (posicio>1){
         if(linies==0) minveins=veins;
         linies++;
         mitja= mitja + (float)veins;
         }
      posicio=0;
      veins=0;
      }
   }

fclose (llista);

mitja =(float) mitja / linies;
Nvertexs=linies;

printf("Caracteristiques del graf:\n\t %i vertexs\n\t grau maxim %i\n\t grau minim %i\n\t grau mitja %f\n", linies, maxveins,minveins,mitja);
                                                                     
/* Obrim de nou el fitxer i guardem la informaci� del graf a l�estructura
vertex[]. Aprofitem la passada pero generar el fitxer graus.txt on es guardar�
el grau de cada node                                                          */

if ((sortida=fopen("graus.txt", "wt"))==NULL)  
   {printf("ep! no puc crear el fitxer.\n"); exit(1); }

if ((llista=fopen(nom, "rt"))==NULL)
   {printf("ep! no trobo el fitxer.\n"); exit(1); }


i=0;
j=0;

while (!feof(llista) && i<linies) {
   fscanf(llista, "%i", &val);
   vertex[i].nei[j]=val;
   j++;        
   fscanf(llista, "%c", &c);
   if (c=='\n') {
      vertex[i].name=i;
      vertex[i].degree=j;
      fprintf (sortida, "Node %i -> %i \n",i,j);
      i++;      
      j=0;
      }   
  }

fclose(llista);
fclose(sortida);

}



/*******************************************************************************
********************************************************************************

FUNCIO PRINCIPAL

AQUESTA FUNCI� L�UNIC QUE FA ES CRIDAR A LES ALTRES FUNCIONS.

1.- LLEGIM LES DADES
2.- CALCULEM LES DIST�NCIES I EL CLUSTERING
3.- CALCULEM LA BETWEENESS CENTRALITY

********************************************************************************
*******************************************************************************/

int main(int argc, char *argv[])

{
int i;

/* llegir el nom del fitxer de la comanda de linia                            */
       
if (argc!=2) { printf("falta el nom del fitxer a llegir\n"); exit(1); }
           
for (i = 1; i < argc; i++)  nom = argv[i];
           
llegir_dades();
i=be_ce();
i=distancies();

}
                     
                                                                                                     
                                                                                                                           
