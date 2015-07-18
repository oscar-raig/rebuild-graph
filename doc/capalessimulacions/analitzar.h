


typedef struct {
	int     grau;           	/* Grau del vertex                              */
	int     *veins;         	/* Veins del vertex                             */
	double	distancia_mitja;	/* Grup del vertex (per calcular fractalitat)	*/
	int 	excentricitat;
	double  bc;
	double clustering;
	char *nom;
} vertex;


typedef struct {
	int     Nvertexs;   	/* Numero de vertexs del graf                   */
	vertex  *llistaVertexs; /* Vertexs que componen el graf					*/
	int 	Nbranques;
	int		grau_minim;
	int		grau_maxim;
	double 	grau_mig;
	int 	radi;
	int 	diametre;
	double	distancia_mitja;
	double  clustering;
	double	correlacio;
	int 	conexe;
} grafStruct;

int llegir_parametres(int argc, char* argv[]);
void print_us();

// graf_calculs.h
int es_conexe(grafStruct* g);
int nombre_branques(grafStruct *g);
void calcular_graus(grafStruct *g);
void calcular_distancies(grafStruct *g);
int	distancies_node(int origen, grafStruct *g);
void correlacio(grafStruct *g);
void clustering(grafStruct *g);
void betweenness_centrality(grafStruct *g);


// graf_funcions
void init_graf_struct ( grafStruct *graf ) ;
void reserva_memoria_graf (grafStruct **graf);
void reserva_memoria_llistaVertexs (grafStruct *a);
void allibera_memoria_graf ( grafStruct *a );
int a_vei_de_b (grafStruct *graf, int a,int b);
void elimina_vei (int node,int eliminat, grafStruct *graf);
void elimina_branca (int i, int j,grafStruct *graf);
void reserva_memoria_veins( grafStruct * a, int i , int num );
void afegeix_vei (int vertexd, int i, grafStruct *graf);
void afegeix_branca (  int i, int j, grafStruct *graf );
int afegeix_node( grafStruct *graf);

#ifndef gml_scanner_h
#define gml_scanner_h



/*
 * start-size of buffers for reading strings. If too small it will be enlarged
 * dynamically
 */

#define INITIAL_SIZE 1024

typedef enum {
    GML_KEY, GML_INT, GML_DOUBLE, GML_STRING, GML_L_BRACKET, 
    GML_R_BRACKET, GML_END, GML_LIST, GML_ERROR
} GML_value; 


typedef enum {
    GML_UNEXPECTED, GML_SYNTAX, GML_PREMATURE_EOF, GML_TOO_MANY_DIGITS,
    GML_OPEN_BRACKET, GML_TOO_MANY_BRACKETS, GML_OK
} GML_error_value;


struct GML_error {
    GML_error_value err_num;
    int line;
    int column;
};


union GML_tok_val {
    long integer;
    double floating;
    char* string;
    struct GML_error err;
};


struct GML_token { 
    GML_value kind;
    union GML_tok_val value;
};

/*
 * global variables
 */

extern unsigned int GML_line;
extern unsigned int GML_column;

/*
 * if you are interested in the position where an error occured it is a good
 * idea to set GML_line and GML_column back. 
 * This is what GML_init does.
 */

void GML_init ();

/*
 * returns the next token in file. If an error occured it will be stored in 
 * GML_token.
 */

struct GML_token GML_scanner (FILE*);

#endif


// gmlparser
#ifndef gml_parser_h
#define gml_parser_h


union GML_pair_val {
    long integer;
    double floating;
    char* string;
    struct GML_pair* list;
};

struct GML_pair {
    char* key;
    GML_value kind;
    union GML_pair_val value;
    struct GML_pair* next;
};

struct GML_list_elem {
    char* key;
    struct GML_list_elem* next;
};

struct GML_stat {
    struct GML_error err;
    struct GML_list_elem* key_list;
};

/*
 * returns list of KEY - VALUE pairs. Errors and a pointer to a list
 * of key-names are returned in GML_stat. Previous information contained
 * in GML_stat, i.e. the key_list, will be *lost*. 
 */

struct GML_pair* GML_parser (FILE*, struct GML_stat*, int);

/*
 * free memory used in a list of GML_pair
 */

void GML_free_list (struct GML_pair*, struct GML_list_elem*);


/*
 * debugging 
 */

struct GML_token GML_find_value_for_key (struct GML_pair* list, char *key );

void GML_print_list (struct GML_pair*, int);

void GML_read_graph (struct GML_pair* list, grafStruct *g);

void GML_parse_file (FILE* file, grafStruct *g);

int GML_token_cmp(const void *x, const void *y);

unsigned GML_hash(const void *a);

#endif




int recuperar_terme(FILE *fp, char *buf, int size) ;

void txt_read_graf(FILE *fp, grafStruct *g) ;

#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED
#include <setjmp.h>
#define T Except_T
typedef struct T {
	const char *reason;
} T;
typedef struct Except_Frame Except_Frame;
struct Except_Frame {
	Except_Frame *prev;
	jmp_buf env;
	const char *file;
	int line;
	const T *exception;
};
enum { Except_entered=0, Except_raised,
	Except_handled,   Except_finalized };
extern Except_Frame *Except_stack;
extern const Except_T Assert_Failed;
void Except_raise(const T *e, const char *file,int line);
#ifdef WIN32
#include <windows.h>

extern int Except_index;
extern void Except_init(void);
extern void Except_push(Except_Frame *fp);
extern void Except_pop(void);
#endif
#ifdef WIN32
/* $Id: except.h 6 2007-01-22 00:45:22Z drhanson $ */
#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
#define RERAISE Except_raise(Except_frame.exception, \
Except_frame.file, Except_frame.line)
#define RETURN switch (Except_pop(),0) default: return
#define TRY do { \
volatile int Except_flag; \
Except_Frame Except_frame; \
if (Except_index == -1) \
Except_init(); \
Except_push(&Except_frame);  \
Except_flag = setjmp(Except_frame.env); \
if (Except_flag == Except_entered) {
#define EXCEPT(e) \
if (Except_flag == Except_entered) Except_pop(); \
} else if (Except_frame.exception == &(e)) { \
Except_flag = Except_handled;
#define ELSE \
if (Except_flag == Except_entered) Except_pop(); \
} else { \
Except_flag = Except_handled;
#define FINALLY \
if (Except_flag == Except_entered) Except_pop(); \
} { \
if (Except_flag == Except_entered) \
Except_flag = Except_finalized;
#define END_TRY \
if (Except_flag == Except_entered) Except_pop(); \
} if (Except_flag == Except_raised) RERAISE; \
} while (0)
#else
#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
#define RERAISE Except_raise(Except_frame.exception, \
Except_frame.file, Except_frame.line)
#define RETURN switch (Except_stack = Except_stack->prev,0) default: return
#define TRY do { \
volatile int Except_flag; \
Except_Frame Except_frame; \
Except_frame.prev = Except_stack; \
Except_stack = &Except_frame;  \
Except_flag = setjmp(Except_frame.env); \
if (Except_flag == Except_entered) {
#define EXCEPT(e) \
if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
} else if (Except_frame.exception == &(e)) { \
Except_flag = Except_handled;
#define ELSE \
if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
} else { \
Except_flag = Except_handled;
#define FINALLY \
if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
} { \
if (Except_flag == Except_entered) \
Except_flag = Except_finalized;
#define END_TRY \
if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
} if (Except_flag == Except_raised) RERAISE; \
} while (0)
#endif
#undef T
#endif


#undef assert
#ifdef NDEBUG
#define assert(e) ((void)0)
#else

extern void assert(int e);
#define assert(e) ((void)((e)||(RAISE(Assert_Failed),0)))
#endif

#ifndef MEM_INCLUDED
#define MEM_INCLUDED

extern const Except_T Mem_Failed;
extern void *Mem_alloc (long nbytes,
						const char *file, int line);
extern void *Mem_calloc(long count, long nbytes,
						const char *file, int line);
extern void Mem_free(void *ptr,
					 const char *file, int line);
extern void *Mem_resize(void *ptr, long nbytes,
						const char *file, int line);
#define ALLOC(nbytes) \
Mem_alloc((nbytes), __FILE__, __LINE__)
#define CALLOC(count, nbytes) \
Mem_calloc((count), (nbytes), __FILE__, __LINE__)
#define  NEW(p) ((p) = ALLOC((long)sizeof *(p)))
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof *(p)))
#define FREE(ptr) ((void)(Mem_free((ptr), \
__FILE__, __LINE__), (ptr) = 0))
#define RESIZE(ptr, nbytes) 	((ptr) = Mem_resize((ptr), \
(nbytes), __FILE__, __LINE__))
#endif

#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED
#define T Table_T
typedef struct T *T;
extern T    Table_new (int hint,
					   int cmp(const void *x, const void *y),
					   unsigned hash(const void *key));
extern void Table_free(T *table);
extern int Table_length(T table);
extern int Table_put   (T table, const void *key,
						int value);
extern int Table_get   (T table, const void *key);
extern int Table_remove(T table, const void *key);

#undef T
#endif

