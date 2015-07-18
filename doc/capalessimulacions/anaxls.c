#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "analitzar.h"
#include <ctype.h>
#include <limits.h>
#include <stddef.h>

int info_nodes;
int info_distancies;
int escriure_sortida;

unsigned int GML_line;
unsigned int GML_column;

char* GML_table[] = {
    "&nbsp;",     /* 160 */
    "&iexcl;",
    "&cent;",
    "&pound;",
    "&curren;",
    "&yen;",
    "&brvbar;",
    "&sect;",
    "&uml;",
    "&copy;",
    "&ordf;",     /* 170 */
    "&laquo;",
    "&not;",
    "&shy;",
    "&reg;",
    "&macr;",
    "&deg;",
    "&plusmn;",
    "&sup2;",
    "&sup3;",     /* 180 */
    "&acute;",
    "&micro;",
    "&para;",
    "&middot;",
    "&cedil;",
    "&sup1;",
    "&ordm;",
    "&raquo;",
    "&frac14;",
    "&frac12;",   
    "&frac34;",   /* 190 */
    "&iquest;",
    "&Agrave;",
    "&Aacute;",
    "&Acirc;",
    "&Atilde;",
    "&Auml;",
    "&Aring;",
    "&AElig;",
    "&Ccedil;",
    "&Egrave;",   /* 200 */
    "&Eacute;",
    "&Ecirc;",
    "&Euml;",
    "&Igrave;",
    "&Iacute;",
    "&Icirc;",
    "&Iuml;",
    "&ETH;",
    "&Ntilde;",
    "&Ograve;",   /* 210 */
    "&Oacute;",
    "&Ocirc;",
    "&Otilde;",
    "&Ouml;",
    "&times;",
    "&Oslash;",
    "&Ugrave;",
    "&Uacute;",
    "&Ucirc;",
    "&Uuml;",     /* 220 */
    "&Yacute;",
    "&THORN;",
    "&szlig;",
    "&agrave;",
    "&aacute;",
    "&acirc;",
    "&atilde;",
    "&auml;",
    "&aring;",
    "&aelig;",    /* 230 */
    "&ccedil;",
    "&egrave;",
    "&eacute;",
    "&ecirc;",
    "&euml;",
    "&igrave;",
    "&iacute;",
    "&icirc;",
    "&iuml;",
    "&eth;",      /* 240 */
    "&ntilde;",
    "&ograve;",
    "&oacute;",
    "&ocirc;",
    "&otilde;",
    "&ouml;",
    "&divide;",
    "&oslash;",
    "&ugrave;",
    "&uacute;",   /* 250 */
    "&ucirc;",
    "&uuml;",
    "&yacute;",
    "&thorn;",
    "&yuml;"
}; 

#define R Except_T
Except_Frame *Except_stack = NULL;

void Except_raise(const R *e, const char *file,
				  int line) {
#ifdef WIN32
	Except_Frame *p;
	
	if (Except_index == -1)
		Except_init();
	p = TlsGetValue(Except_index);
#else
	Except_Frame *p = Except_stack;
#endif
	assert(e);
	if (p == NULL) {
		fprintf(stderr, "Uncaught exception");
		if (e->reason)
			fprintf(stderr, " %s", e->reason);
		else
			fprintf(stderr, " at 0x%p", e);
		if (file && line > 0)
			fprintf(stderr, " raised at %s:%d\n", file, line);
		fprintf(stderr, "aborting...\n");
		fflush(stderr);
		abort();
	}
	p->exception = e;
	p->file = file;
	p->line = line;
#ifdef WIN32
	Except_pop();
#else
	Except_stack = Except_stack->prev;
#endif
	longjmp(p->env, Except_raised);
}
#ifdef WIN32
_CRTIMP void __cdecl _assert(void *, void *, unsigned);
#undef assert
#define assert(e) ((e) || (_assert(#e, __FILE__, __LINE__), 0))

int Except_index = -1;
void Except_init(void) {
	BOOL cond;
	
	Except_index = TlsAlloc();
	assert(Except_index != TLS_OUT_OF_INDEXES);
	cond = TlsSetValue(Except_index, NULL);
	assert(cond == TRUE);
}

void Except_push(Except_Frame *fp) {
	BOOL cond;
	
	fp->prev = TlsGetValue(Except_index);
	cond = TlsSetValue(Except_index, fp);
	assert(cond == TRUE);
}

void Except_pop(void) {
	BOOL cond;
	Except_Frame *tos = TlsGetValue(Except_index);
	
	cond = TlsSetValue(Except_index, tos->prev);
	assert(cond == TRUE);
}
#endif

const Except_T Assert_Failed = { "Assertion failed" };
void (assert)(int e) {
	assert(e);
}

const Except_T Mem_Failed = { "Allocation Failed" };
void *Mem_alloc(long nbytes, const char *file, int line){
	void *ptr;
	assert(nbytes > 0);
	ptr = malloc(nbytes);
	if (ptr == NULL)
	{
		if (file == NULL)
			RAISE(Mem_Failed);
		else
			Except_raise(&Mem_Failed, file, line);
	}
	return ptr;
}
void *Mem_calloc(long count, long nbytes,
				 const char *file, int line) {
	void *ptr;
	assert(count > 0);
	assert(nbytes > 0);
	ptr = calloc(count, nbytes);
	if (ptr == NULL)
	{
		if (file == NULL)
			RAISE(Mem_Failed);
		else
			Except_raise(&Mem_Failed, file, line);
	}
	return ptr;
}
void Mem_free(void *ptr, const char *file, int line) {
	if (ptr)
		free(ptr);
}
void *Mem_resize(void *ptr, long nbytes,
				 const char *file, int line) {
	assert(ptr);
	assert(nbytes > 0);
	ptr = realloc(ptr, nbytes);
	if (ptr == NULL)
	{
		if (file == NULL)
			RAISE(Mem_Failed);
		else
			Except_raise(&Mem_Failed, file, line);
	}
	return ptr;
}

#define T Table_T
struct T {
	int size;
	int (*cmp)(const void *x, const void *y);
	unsigned (*hash)(const void *key);
	int length;
	unsigned timestamp;
	struct binding {
		struct binding *link;
		const void *key;
		int value;
	} **buckets;
};
static int cmpatom(const void *x, const void *y) {
	return x != y;
}
static unsigned hashatom(const void *key) {
	return (unsigned long)key>>2;
}
T Table_new(int hint,
			int cmp(const void *x, const void *y),
			unsigned hash(const void *key)) {
	T table;
	int i;
	static int primes[] = { 509, 509, 1021, 2053, 4093,
		8191, 16381, 32771, 65521, INT_MAX };
	assert(hint >= 0);
	for (i = 1; primes[i] < hint; i++)
		;
	table = ALLOC(sizeof (*table) +
				  primes[i-1]*sizeof (table->buckets[0]));
	table->size = primes[i-1];
	table->cmp  = cmp  ?  cmp : cmpatom;
	table->hash = hash ? hash : hashatom;
	table->buckets = (struct binding **)(table + 1);
	for (i = 0; i < table->size; i++)
		table->buckets[i] = NULL;
	table->length = 0;
	table->timestamp = 0;
	return table;
}
int Table_get(T table, const void *key) {
	int i;
	struct binding *p;
	assert(table);
	assert(key);
	i = (*table->hash)(key)%table->size;
	for (p = table->buckets[i]; p; p = p->link)
		if ((*table->cmp)(key, p->key) == 0)
			break;
	if(p) return p->value;
	return 0;
	//return p ? p->value : NULL;
}
int Table_put(T table, const void *key, int value) {
	int i;
	struct binding *p;
	int prev;
	assert(table);
	assert(key);
	i = (*table->hash)(key)%table->size;
	for (p = table->buckets[i]; p; p = p->link)
		if ((*table->cmp)(key, p->key) == 0)
			break;
	if (p == NULL) {
		NEW(p);
		p->key = key;
		p->link = table->buckets[i];
		table->buckets[i] = p;
		table->length++;
		prev = 0;
	} else
		prev = p->value;
	p->value = value;
	table->timestamp++;
	return value;
}
int Table_length(T table) {
	assert(table);
	return table->length;
}

int Table_remove(T table, const void *key) {
	int i;
	struct binding **pp;
	assert(table);
	assert(key);
	table->timestamp++;
	i = (*table->hash)(key)%table->size;
	for (pp = &table->buckets[i]; *pp; pp = &(*pp)->link)
		if ((*table->cmp)(key, (*pp)->key) == 0) {
			struct binding *p = *pp;
			int value = p->value;
			*pp = p->link;
			FREE(p);
			table->length--;
			return value;
		}
	return -1;
}

void Table_free(T *table) {
	assert(table && *table);
	if ((*table)->length > 0) {
		int i;
		struct binding *p, *q;
		for (i = 0; i < (*table)->size; i++)
			for (p = (*table)->buckets[i]; p; p = q) {
				q = p->link;
				//free(p->key);
				FREE(p);
			}
	}
	FREE(*table);
}


int recuperar_terme(FILE *fp, char *buf, int size) {
	int c;
	int i=0;
	// Llegim el primer caràcter i anem llegint mentres hi hagi espais en blanc
	c = getc(fp);
	for ( ; c != EOF && isspace(c) && c!='\n'; c = getc(fp));
	if (c == '\n') { return 0;}
	else if (c == EOF) return -1; // retornem -1 (final d'arxiu)
	
	// comprobem si es un comentari, si ho es continuem llegint fins que trobem un
	// retorn de carro o s'acaba l'archiu
	if( c == '#') {
		do { c=getc(fp);} while ( c!= EOF && c!='\n');
		if (c == EOF) return -1; // retornem -1 (final d'arxiu)
		if (c == '\n') return 0; // retornem 0 (final de línea)
	}
	
	if( c == '"') {
		do {if (i < size - 1) buf[i++]=c; c=getc(fp); } while ( c!= EOF && c!='\n' && c!='"');
		
		if (c == EOF || c == '\n')  ungetc(c, fp); 
		else buf[i++]=c; // Afegim el caracter "
		
		if (i < size) buf[i] = '\0';
		return i;
	}
	
	//Tenim un caàcter que no es un espai en blanc, continuem llegint fins trobar un espai en blanc
	
	for ( ; c != EOF && !isspace(c) && c!=','; c = getc(fp))
	{
		if (i < size - 1)
			buf[i++] = c;
	}
	
	if (c == EOF || c == '\n') {
		ungetc(c, fp); 
	}
	
	if (i < size) buf[i] = '\0';
	return i;
}

unsigned strhash(const void *x) {
	const char *str = x;
	unsigned h = 0;
	
	while (*str)
		h = (h<<1) + *str++;
	return h;
}



void txt_read_graf(FILE *fp, grafStruct *g) {
	//	int i;
	char buf[100];
	int num=0;
	//	int cnt=0;
	char *new;
	int nodes,nodet;
	
	printf(" LLegint graf en format txt\n");
	//Creem taula hash
	Table_T identifiers = Table_new(5000, (int (*)(const void *, const void *))strcmp, strhash);
	int linia = 0;
	while (num>=0) {
		
		printf("Tractant linia %d buf(%s)\n",linia++,buf);
		num=recuperar_terme(fp,buf,100);
		if(num>0) {
			if(!(nodes=Table_get(identifiers,buf)))			
			{
				new=(char*)malloc(sizeof(char)*num);
				strcpy(new,buf);
				nodes=Table_put(identifiers, new ,1+afegeix_node(g));
				g->llistaVertexs[nodes-1].nom=new;
				//		free(new);	
				
			}
			while(num>0){
				num=recuperar_terme(fp,buf,100);
				if(num>0) {
					if(!(nodet=Table_get(identifiers,buf))) { 
						new=(char*)malloc(sizeof(char)*num);
						strcpy(new,buf);
						nodet=Table_put(identifiers, new ,1+afegeix_node(g));
						g->llistaVertexs[nodet-1].nom=new;
						//	free(new);
					}
					if(!a_vei_de_b(g,nodes-1,nodet-1)) {
						printf("Afegint node %d a nodet %d\n",nodes-1,nodet-1);
						afegeix_branca(nodes-1,nodet-1,g);
					}
				}
			}
		}
		
	}
	
	Table_free(&identifiers);
	
}




int GML_search_ISO (char* str, int len) {
	
	int i;
	int ret = '&'; 
	
	if (!strncmp (str, "&quot;", len)) {
		return 34;
	} else if (!strncmp (str, "&amp;", len)) {
		return 38;
	} else if (!strncmp (str, "&lt;", len)) {
		return 60;
	} else if (!strncmp (str, "&gt;", len)) {
		return 62;
	}
	
	for (i = 0; i < 96; i++) {
		if (!strncmp (str, GML_table[i], len)) {
			ret = i + 160;
			break;
		}
	}
	
	return ret;
}


void GML_init () {
	
    GML_line = 1;
    GML_column = 1;
}



struct GML_token GML_scanner (FILE* source) {
    
    unsigned int cur_max_size = INITIAL_SIZE;
    static char buffer[INITIAL_SIZE];
    char* tmp = buffer;
    char* ret = tmp;
    struct GML_token token;
    int is_float = 0;
    unsigned int count = 0;
    int next;
    char ISO_buffer[8];
    int ISO_count;
	
    assert (source != NULL);
    
    /* 
     * eliminate preceeding white spaces
     */
    
    do {
		next = fgetc (source);
		GML_column++;
		
		if (next == '\n') {
			GML_line++;
			GML_column = 1;
		}
		
    } while (isspace (next) && next != EOF);
	
    if (next == EOF) {
		
		/*
		 * reached EOF
		 */
		
		token.kind = GML_END;
		return token; 
		
    } else if (isdigit (next) || next == '.' || next == '+' || next == '-') {
		
		/* 
		 * floating point or integer 
		 */
	    
		do {
			if (count == INITIAL_SIZE - 1) {
				token.value.err.err_num = GML_TOO_MANY_DIGITS;
				token.value.err.line = GML_line;
				token.value.err.column = GML_column + count;
				token.kind = GML_ERROR;
				return token;
			}
			
			if (next == '.' || next == 'E') {
				is_float = 1;
			}
			
			buffer[count] = next;
			count++;
			next = fgetc (source);
			
		} while (!isspace(next) && next != ']' && next != EOF);
		
		buffer[count] = 0;
		
		if (next == ']') {
			ungetc (next, source);
		}
		
		if (next == '\n') {
			GML_line++;
			GML_column = 1;
		} else {
			GML_column += count;
		}
	    
		if (is_float) {
			token.value.floating = atof (tmp);
			token.kind = GML_DOUBLE;
		} else {
			token.value.integer = atol (tmp);
			token.kind = GML_INT;
		}
		
		return token;
		
    } else if (isalpha (next) || next == '_') {
		
		/*
		 * key
		 */
		
		do {
			if (count == cur_max_size - 1) {
				*tmp = 0;
				tmp =  (char*) malloc(2 * cur_max_size * sizeof (char));
				strcpy (tmp, ret);
				
				if (cur_max_size > INITIAL_SIZE) {
					free (ret);
				}
				
				ret = tmp;
				tmp += count;
				cur_max_size *= 2;
			}
			
			*tmp++ = next;
			count++;
			next = fgetc (source);
		} while (isalnum (next) || next == '_');
		
		if (next == '\n') {
			GML_line++;
			GML_column = 1;
		} else {
			GML_column += count;
		}
		
		if (next == '[') {
			ungetc (next, source);
		} else if (!isspace (next)) {
			token.value.err.err_num = GML_UNEXPECTED;
			token.value.err.line = GML_line;
			token.value.err.column = GML_column + count;
			token.kind = GML_ERROR;
			
			if (cur_max_size > INITIAL_SIZE) {
				free (ret);
			}
			
			return token;   
		} 
		
		*tmp = 0;
		token.kind = GML_KEY;
		token.value.string = (char*) malloc((count+1) * sizeof (char));
		strcpy (token.value.string, ret);
		
		if (cur_max_size > INITIAL_SIZE) {
			free (ret);
		}
		
		return token;
		
    } else {
		/*
		 * comments, brackets and strings
		 */
		
		switch (next) {
			case '#':
				do {
					next = fgetc (source);
				} while (next != '\n' && next != EOF);
				
				GML_line++;
				GML_column = 1;
				return GML_scanner (source);
				
			case '[':
				token.kind = GML_L_BRACKET;
				return token;
				
			case ']':
				token.kind = GML_R_BRACKET;
				return token;
				
			case '"':
				next = fgetc (source);
				GML_column++;
				
				while (next != '"') {
					
					if (count >= cur_max_size - 8) {
						*tmp = 0;
						tmp = (char*) malloc (2 * cur_max_size * sizeof(char));
						strcpy (tmp, ret);
						
						if (cur_max_size > INITIAL_SIZE) {
							free (ret);
						}
						
						ret = tmp;
						tmp += count;
						cur_max_size *= 2;
					}
					
					if (next == '&') {
						ISO_count = 0;
						
						while (next != ';') {
							if (next == '"' || next == EOF) {
								ungetc (next, source);
								ISO_count = 0;
								break;
							}
							
							if (ISO_count < 8) {
								ISO_buffer[ISO_count] = next;
								ISO_count++;
							}
							
							next = fgetc (source);
						}
						
						if (ISO_count == 8) {
							ISO_count = 0;
						}
						
						if (ISO_count) {
							ISO_buffer[ISO_count] = ';';
							ISO_count++;
							next = GML_search_ISO (ISO_buffer, ISO_count);
							ISO_count = 0;
						} else {
							next = '&';
						}
					} 
					
					*tmp++ = next;
					count++;
					GML_column++;
					
					next = fgetc (source);
					
					if (next == EOF) {
						token.value.err.err_num = GML_PREMATURE_EOF;
						token.value.err.line = GML_line;
						token.value.err.column = GML_column + count;
						token.kind = GML_ERROR;
						
						if (cur_max_size > INITIAL_SIZE) {
							free (ret);
						}
						
						return token;
					}
					
					if (next == '\n') {
						GML_line++;
						GML_column = 1;
					}
				}
				
				*tmp = 0;
				token.kind = GML_STRING;
				token.value.string = (char*) malloc((count+1) * sizeof (char));
				strcpy (token.value.string, ret);
				
				if (cur_max_size > INITIAL_SIZE) {
					free (ret);
				}
				
				return token;
				
			default:
				token.value.err.err_num = GML_UNEXPECTED;
				token.value.err.line = GML_line;
				token.value.err.column = GML_column;
				token.kind = GML_ERROR;
				return token;
		}		
    }
}	    


struct GML_pair* GML_parser (FILE* source, struct GML_stat* stat, int open) {
    
    struct GML_token token;
    struct GML_pair* pair;
    struct GML_pair* list;
    struct GML_pair* tmp = NULL;
    struct GML_list_elem* tmp_elem;
	
    assert (stat);
	
    pair = (struct GML_pair*) malloc (sizeof (struct GML_pair));
    list = pair;
	
    for (;;) {
		token = GML_scanner (source);
		
		if (token.kind == GML_END) {
			if (open) {
				stat->err.err_num = GML_OPEN_BRACKET;
				stat->err.line = GML_line;
				stat->err.column = GML_column;
				free (pair);
				
				if (tmp == NULL) {
					return NULL;
				} else {
					tmp->next = NULL;
					return list;
				}
			}
			
			break;
			
		} else if (token.kind == GML_R_BRACKET) {
			if (!open) {
				stat->err.err_num = GML_TOO_MANY_BRACKETS;
				stat->err.line = GML_line;
				stat->err.column = GML_column;
				free (pair);
				
				if (tmp == NULL) {
					return NULL;
				} else {
					tmp->next = NULL;
					return list;
				}
			}
			
			break;
			
		} else if (token.kind == GML_ERROR) {
			stat->err.err_num = token.value.err.err_num;
			stat->err.line = token.value.err.line;
			stat->err.column = token.value.err.column;
			free (pair);
			
			if (tmp == NULL) {
				return NULL;
			} else {
				tmp->next = NULL;
				return list;
			}
			
		} else if (token.kind != GML_KEY) {
			stat->err.err_num = GML_SYNTAX;
			stat->err.line = GML_line;
			stat->err.column = GML_column;
			free (pair);
			
			if (token.kind == GML_STRING) {
				free (token.value.string);
			}
			
			if (tmp == NULL) {
				return NULL;
			} else {
				tmp->next = NULL;
				return list;
			}
		}
		
		if (!stat->key_list) {
			stat->key_list = (struct GML_list_elem*) 
			malloc (sizeof (struct GML_list_elem));
			stat->key_list->next = NULL;
			stat->key_list->key = token.value.string;
			pair->key = token.value.string;
			
		} else {
			tmp_elem = stat->key_list;
			
			while (tmp_elem) {
				if (!strcmp (tmp_elem->key, token.value.string)) {
					free (token.value.string);
					pair->key = tmp_elem->key;
					
					break;
				}
				
				tmp_elem = tmp_elem->next;
			}
			
			if (!tmp_elem) {
				tmp_elem = (struct GML_list_elem*)
				malloc (sizeof (struct GML_list_elem));
				tmp_elem->next = stat->key_list;
				stat->key_list = tmp_elem;
				tmp_elem->key = token.value.string;
				pair->key = token.value.string;
			}
		}
		
		token = GML_scanner (source);
		
		
		switch (token.kind) {
			case GML_INT:
				pair->kind = GML_INT;
				pair->value.integer = token.value.integer;
				break;
				
			case GML_DOUBLE:
				pair->kind = GML_DOUBLE;
				pair->value.floating = token.value.floating;
				break;
				
			case GML_STRING:
				pair->kind = GML_STRING;
				pair->value.string = token.value.string;
				break;
				
			case GML_L_BRACKET:
				pair->kind = GML_LIST;
				pair->value.list = GML_parser (source, stat, 1);
				
				if (stat->err.err_num != GML_OK) {
					return list;
				}
				
				break;
				
			case GML_ERROR:
				stat->err.err_num = token.value.err.err_num;
				stat->err.line = token.value.err.line;
				stat->err.column = token.value.err.column;
				free (pair);
				
				if (tmp == NULL) {
					return NULL;
				} else {
					tmp->next = NULL;
					return list;
				}
				
			default:    
				stat->err.line = GML_line;
				stat->err.column = GML_column;
				stat->err.err_num = GML_SYNTAX;
				free (pair);
				
				if (tmp == NULL) {
					return NULL;
				} else {
					tmp->next = NULL;
					return list;
				}
		}
		
		tmp = pair;
		pair = (struct GML_pair*) malloc (sizeof (struct GML_pair));
		tmp->next = pair;
    }
	
    stat->err.err_num = GML_OK;
    free (pair);
    
    if (tmp == NULL) {
		return NULL;
    } else {
		tmp->next = NULL;
		return list;
    }
}


int GML_token_cmp(const void *a, const void *b) {
	const struct GML_token *x = a;
	const struct GML_token *y = b;
	
	if(x->kind != y->kind) return -1;
	
	switch (x->kind) {
		case GML_INT:
			if(x->value.integer < y->value.integer) return -1;
			if(x->value.integer > y->value.integer) return 1;
			return 0;
		case GML_DOUBLE:
			if(x->value.floating < y->value.floating) return -1;
			if(x->value.floating > y->value.floating) return 1;
			return 0;
		case GML_STRING:
			return strcmp(x->value.string,y->value.string);
		default:
			return -1;
	}
	return 1; //	return Text_cmp(*(Text_T *)x, *(Text_T *)y);
}

unsigned GML_hash(const void *a) {
	const struct GML_token *x = a;
	int i;
	unsigned h = 0;
	
	if(x->kind == GML_STRING)
	{
		for (i = 0; i < strlen(x->value.string); i++)
			h = (h<<1) + x->value.string[i];
		return h;
	}
	if(x->kind == GML_INT)
		return x->value.integer;
	//if(x->kind == GML_DOUBLE)
	//	return *(double *)x->value.floating;
	
	return h;
}

void GML_free_list (struct GML_pair* list, struct GML_list_elem* keys) {
    
    struct GML_pair* tmp = list;
    struct GML_list_elem* tmp_key;
	
    while (keys) {
		free (keys->key);
		tmp_key = keys->next;
		free (keys);
		keys = tmp_key;
    }
	
    while (list) {
		
		switch (list->kind) {
			case GML_LIST:
				GML_free_list (list->value.list, NULL);
				break;
				
			case GML_STRING:
				free (list->value.string);
				break;
				
			default:
				break;
		}
		
		tmp = list->next;
		free (list);
		list = tmp;
    }
}

void GML_read_graph (struct GML_pair* list, grafStruct *g) {
	struct GML_pair* tmp = list;
	struct GML_token token,tokend;
	struct GML_token* new = NULL;
	Table_T identifiers = Table_new(5000, GML_token_cmp, GML_hash);
	//	int nodes=1;
	int nodes,nodet;
	
	if(strcmp(tmp->key,"graph")==0) {
		tmp = tmp->value.list;
		
		//	printf("nombre vertexs %i\n",g->Nvertexs);
		while(tmp) {
			//printf ("*KEY* : %s\n", tmp->key);
			
			if( strcmp(tmp->key,"node")==0 ) {
				//				printf("Node: ");
				token=GML_find_value_for_key(tmp->value.list,"id");
				//				printf("\n");
				if(!Table_get(identifiers,&token))
				{
					//	printf("Node no afegit\n");
					new = (struct GML_token*) malloc (sizeof (struct GML_token));
					*new = token;
					Table_put(identifiers, new ,1+afegeix_node(g));	
					
				}
			}
			else if ( strcmp(tmp->key,"edge")==0 ) {
				
				token=GML_find_value_for_key(tmp->value.list,"source");
				if(!(nodes=Table_get(identifiers,&token))) { 
					new = (struct GML_token*) malloc (sizeof (struct GML_token));
					*new = token;
					nodes=Table_put(identifiers, new,1+afegeix_node(g));
				}
				
				tokend=GML_find_value_for_key(tmp->value.list,"target");
				if(!(nodet=Table_get(identifiers,&tokend))) { 
					//		printf("Node no afegit\n");
					new = (struct GML_token*) malloc (sizeof (struct GML_token));
					*new = tokend;
					nodet=Table_put(identifiers, new,1+afegeix_node(g));
					//		printf("Nou node afegit id:%i\n",nodet);
				}
				printf("Branca %i-%i\n",nodes-1,nodet-1);
				if(!a_vei_de_b(g,nodes-1,nodet-1)) {
					afegeix_branca(nodes-1,nodet-1,g);
					printf("Adding nodes %d to nodet %d",nodes -1,nodet -1);
				}
			}
			tmp = tmp->next;
		}
		
	}
	
	else {
		printf("Les dades llegides no són un graf\n");
		exit(-1);
	}
	
	Table_free(&identifiers);
}

struct GML_token GML_find_value_for_key (struct GML_pair* list, char *key ) {
	struct GML_token token;
	struct GML_pair* tmp = list;
	
	while(tmp) {
		if(strcmp(tmp->key,key)==0) {
			token.kind = tmp->kind;
			switch (tmp->kind) {
				case GML_INT:
					token.value.integer = tmp->value.integer;
					//	    printf ("token: %ld\n", tmp->value.integer);
					break;
					
				case GML_DOUBLE:
					token.value.floating = tmp->value.floating;
					//	    printf ("  *VALUE* (double) : %f \n", tmp->value.floating);
					break;
					
				case GML_STRING:
					token.value.string = tmp->value.string;
					//	    printf ("  *VALUE* (string) : %s \n", tmp->value.string);
					break;
					
				default:
					break;
			}
		}
		tmp = tmp->next;
	}
	
	return token;
}

void GML_print_list (struct GML_pair* list, int level) {
    
    struct GML_pair* tmp = list;
    int i;
	
    while (tmp) {
		
		for (i = 0; i < level; i++) {
			printf ("    ");
		}
		
		printf ("*KEY* : %s", tmp->key);
		
		switch (tmp->kind) {
			case GML_INT:
				printf ("  *VALUE* (long) : %ld \n", tmp->value.integer);
				break;
				
			case GML_DOUBLE:
				printf ("  *VALUE* (double) : %f \n", tmp->value.floating);
				break;
				
			case GML_STRING:
				printf ("  *VALUE* (string) : %s \n", tmp->value.string);
				break;
				
			case GML_LIST:
				printf ("  *VALUE* (list) : \n");
				GML_print_list (tmp->value.list, level+1);
				break;
				
			default:
				break;
		}
		
		tmp = tmp->next;
    }
}

void GML_parse_file (FILE* file, grafStruct *g) {
	
    struct GML_pair* list;
    struct GML_stat* stat=(struct GML_stat*)malloc(sizeof(struct GML_stat));
    stat->key_list = NULL;
	
	printf(" LLegint graf en format gml\n");
	
    GML_init ();
	list = GML_parser (file, stat, 0);
	
	
    if (stat->err.err_num != GML_OK) {
    	printf ("An error occured while reading line %d column %d:\n", stat->err.line, stat->err.column);
		
        switch (stat->err.err_num) {
			case GML_UNEXPECTED:
				printf ("UNEXPECTED CHARACTER");
				break;
				
			case GML_SYNTAX:
				printf ("SYNTAX ERROR"); 
				break;
				
			case GML_PREMATURE_EOF:
				printf ("PREMATURE EOF IN STRING");
				break;
				
			case GML_TOO_MANY_DIGITS:
				printf ("NUMBER WITH TOO MANY DIGITS");
				break;
				
			case GML_OPEN_BRACKET:
				printf ("OPEN BRACKETS LEFT AT EOF");
				break;
				
			case GML_TOO_MANY_BRACKETS:
				printf ("TOO MANY CLOSING BRACKETS");
				break;
                
			default:
				break;
        }
		
    	printf ("\n");
		exit(-1);
	}      
	
    //GML_print_list (list, 0);
	
	GML_read_graph (list,g);			
	
	GML_free_list (list, stat->key_list);
	
    
}


void init_graf_struct ( grafStruct *graf ) 
{
	printf("graf inicialitzat\n");
	//	reserva_memoria_graf(&graf);
	graf->Nvertexs=0;	
	//printf("%i",graf->Nvertexs);
}

void reserva_memoria_graf (grafStruct **graf )
{
	if ( ( *graf=(grafStruct*)malloc(sizeof(grafStruct)) ) == NULL )
	{
		printf("RESERVA_MEMORIA_GRAF: Error reservant memoria\n");
		exit(-1);
	}
	memset(*graf,0,sizeof(grafStruct));
}

void reserva_memoria_llistaVertexs (grafStruct *a)
{
	if ((a->llistaVertexs =
	     (vertex *) malloc (sizeof (vertex) * a->Nvertexs)) == NULL)
	{
		// Si no podem reservar la memoria necessaria sortim amb error
		printf("RESERVA_MEMORIA_LLISTAVERTEXS:Error reservant memoria");
		exit(-1);
	}
	
}

void allibera_memoria_graf ( grafStruct *a )
{
	int i;
	
	for (i = 0; i < a->Nvertexs; i++)
	{
		if(a->llistaVertexs[i].grau>0)
		{	
			free (a->llistaVertexs[i].veins);
		}
	}
	
	if(a->Nvertexs>0)
	{
		free (a->llistaVertexs);
	}
	
}

int a_vei_de_b (grafStruct *graf, int a,int b)
{
	int i;
	
	for(i=0;i<graf->llistaVertexs[a].grau;i++)
		if(graf->llistaVertexs[a].veins[i]==b) return 1;
	
	return 0;
}

void elimina_vei (int node,int eliminat, grafStruct *graf)
{
	int i,numVei=-1;
	
	for(i=0;i<graf->llistaVertexs[node].grau;i++)
		if (graf->llistaVertexs[node].veins[i]==eliminat)
		{
			numVei=i;
			i=graf->llistaVertexs[node].grau;
		}
	
	if (numVei==-1)
	{
		printf("ELIMINA_VEI: Vei no trobat\n");
		exit(-1);		
	}
	
	if (numVei < graf->llistaVertexs[node].grau-1)
	{
		memcpy(&graf->llistaVertexs[node].veins[numVei],
			   &graf->llistaVertexs[node].veins[numVei+1],
			   (graf->llistaVertexs[node].grau-numVei-1)*sizeof(int));
	}
	//printf("Vector desplaçat...");	
	graf->llistaVertexs[node].grau--;
	if(graf->llistaVertexs[node].grau==0)
	{
		free (graf->llistaVertexs[node].veins);	
	}
	else
	{	
		if ((graf->llistaVertexs[node].veins =
			 (int *) realloc (graf->llistaVertexs[node].veins,
							  graf->llistaVertexs[node].grau *
							  sizeof (int))) == NULL)
		{
			printf("ELIMINA_VEI: Error redimensionan vector de veins\n");
			exit(-1);	
		}
	}
	//printf("Memoria recolocada");
	
}

void elimina_branca (int i, int j,grafStruct *graf)
{
	elimina_vei(i,j,graf);
	elimina_vei(j,i,graf);	
}

void reserva_memoria_veins( grafStruct * a, int i , int num )
{
	if( ( a->llistaVertexs[i].veins = (int*)malloc( sizeof(int)*num )) == NULL )
	{
		printf("RESERVA_MEMORIA_VEINS: Error reservant memoria");
		exit(-1);
	}
	
}

void afegeix_vei (int vertexd, int i, grafStruct *graf)
{
	//	printf("Anem a afegir un vei al node %i amb grau %i\n",vertexd,graf->llistaVertexs[vertexd].grau);
	graf->llistaVertexs[vertexd].grau++;
	
	if (graf->llistaVertexs[vertexd].grau==1)
	{
		reserva_memoria_veins(graf,vertexd,1);
	}
	else 
	{
		if ((graf->llistaVertexs[vertexd].veins =
			 (int *) realloc (graf->llistaVertexs[vertexd].veins,
							  graf->llistaVertexs[vertexd].grau *
							  sizeof (int))) == NULL)
		{
			printf("FRAC_AFEGEIX_VEINS: Error reservant memoria");
			exit(-1);
		}
	}
	
	graf->llistaVertexs[vertexd].veins[graf->llistaVertexs[vertexd].grau - 1] = i;
	
}

void afegeix_branca (  int i, int j, grafStruct *graf )
{
	
	afegeix_vei (i, j,graf); 
	afegeix_vei (j, i,graf);
	
}

int afegeix_node( grafStruct *graf )
{
	if (graf->Nvertexs==0) {
		if ((graf->llistaVertexs =(vertex *) malloc (sizeof(vertex))) == NULL)
		{
			printf("Error redimensionant llista vertexs\n");
			exit(-1);
			
		}
	} else if ((graf->llistaVertexs =(vertex *) realloc (graf->llistaVertexs,(graf->Nvertexs+1) * sizeof (vertex))) == NULL)
	{
		printf("Error redimensionant llistavertexs\n");
		exit(-1);
	}
	
	//	graf->llistaVertexs[graf->Nvertexs].nom=(char*)malloc(sizeof(char)*strlen(nom));
	//	strcpy(graf->llistaVertexs[graf->Nvertexs].nom,nom);
	
	graf->llistaVertexs[graf->Nvertexs].grau=0;
	graf->llistaVertexs[graf->Nvertexs].bc=0.0;
	graf->llistaVertexs[graf->Nvertexs].excentricitat=0;
	graf->llistaVertexs[graf->Nvertexs].clustering=0.0;
	graf->llistaVertexs[graf->Nvertexs].distancia_mitja=0.0;
	return graf->Nvertexs++;
	
}



int es_conexe(grafStruct* g)
{
	
	int *visitats;
	int *distancies;
	int i;
	int j;
	int nodeActual;
	int nodeVei;
	int ultim;
	
	
	
	if (g->Nvertexs==0) {
		g->conexe=0;
		return 0;
	}
	
	distancies=(int*)malloc( sizeof(int)*g->Nvertexs );
	visitats=(int*)malloc( sizeof(int)*g->Nvertexs );
	
	for(i=0;i<g->Nvertexs;i++)
	{ 
		distancies[i]=-1;
		if(g->llistaVertexs[i].grau<=0) {
			g->conexe=0;
			return 0;
		}
	}
	
	distancies[0]=0;
	visitats[0]=0;
	
	i=0;
	ultim = 1;
	
	while(i<ultim)
	{
		nodeActual=visitats[i++];
		
		for(j=0;j<g->llistaVertexs[nodeActual].grau;j++)
		{
			nodeVei=g->llistaVertexs[nodeActual].veins[j];
			if(distancies[nodeVei]==-1)
			{
				distancies[nodeVei]=1;
				visitats[ultim++]=nodeVei;
			}
		}
		
	}
	
	free(distancies);
	free(visitats);
	
	if (i<g->Nvertexs) {
		g->conexe=0;
		return 0;
	}
	g->conexe=1;
	
	return 1;
	
}

int nombre_branques(grafStruct *g)
{
	int i=0, sum=0;
	
	for(i=0;i<g->Nvertexs;sum+=g->llistaVertexs[i++].grau);
	//	printf("nodes %i grau %i\n",i,g->llistaVertexs[i].grau);
	g->Nbranques=sum/2;
	return sum/2;
	
}

void calcular_graus(grafStruct *g)
{
	int i;
	int gmin;
	int gmax;
	int sumg;
	gmin=g->llistaVertexs[0].grau;
	gmax=gmin;
	sumg=gmax;
	
	for(i=1;i<g->Nvertexs;i++)
	{
		sumg+=g->llistaVertexs[i].grau;
		if(g->llistaVertexs[i].grau>gmax) gmax=g->llistaVertexs[i].grau;
		if(g->llistaVertexs[i].grau<gmin) gmin=g->llistaVertexs[i].grau;
	}
	//	printf("Suma de graus:%i\n",sumg);
	g->grau_minim=gmin;
	g->grau_maxim=gmax;
	g->grau_mig=(double)sumg/g->Nvertexs;
	
}


void calcular_distancies(grafStruct *g) {
	int i;
	int radi;
	int diametre;
	double sumdm;
	int ex;
	radi=distancies_node(0,g);
	diametre=radi;
	sumdm=g->llistaVertexs[0].distancia_mitja;
	
	for(i=1;i<g->Nvertexs;i++){
		ex=distancies_node(i,g);
		if(ex<radi) radi=ex;
		if(ex>diametre) diametre=ex;
		sumdm+=g->llistaVertexs[i].distancia_mitja;
	}
	
	g->radi=radi;
	g->diametre=diametre;
	g->distancia_mitja=sumdm/g->Nvertexs;
}

int	distancies_node(int origen, grafStruct *g)
{
	
	int *visitats;
	int *distancies;
	int i;
	int j;
	int nodeActual;
	int nodeVei;
	int ultim;
	
	distancies=(int*)malloc( sizeof(int)*g->Nvertexs );
	visitats=(int*)malloc( sizeof(int)*g->Nvertexs );
	
	for(i=0;i<g->Nvertexs;i++) distancies[i]=-1;
	
	distancies[origen]=0;
	visitats[0]=origen;
	
	i=0;
	ultim = 1;
	
	g->llistaVertexs[origen].distancia_mitja=0.0;
	
	while(i<g->Nvertexs)
	{
		nodeActual=visitats[i++];
		
		for(j=0;j<g->llistaVertexs[nodeActual].grau;j++)
		{
			nodeVei=g->llistaVertexs[nodeActual].veins[j];
			
			
			if(distancies[nodeVei]==-1)
			{
				distancies[nodeVei]=distancies[nodeActual]+1;
				g->llistaVertexs[origen].distancia_mitja+=(double)distancies[nodeVei]/(g->Nvertexs-1);
				visitats[ultim]=nodeVei;
				ultim++;
				
			}
		}
		
	}
	
	g->llistaVertexs[origen].excentricitat=distancies[visitats[i-1]];
	free(distancies);
	free(visitats);
	return g->llistaVertexs[origen].excentricitat;
	
}

void betweenness_centrality(grafStruct *g)
{
	int i,j,s,v,w;
	//int nNodes=g->Nvertexs;
	
	int *pIndexs;
	pIndexs=(int*)malloc(sizeof(int)*g->Nvertexs);
	
	double *CB;	
	CB=(double*)malloc(sizeof(double)*g->Nvertexs);
	
	int *S;
	S=(int*)malloc(sizeof(int)*g->Nvertexs);
	
	int **P;
	P=(int **)malloc(g->Nvertexs*sizeof(int *));
	for (i=0;i<g->Nvertexs;i++)
		P[i]=(int *)malloc(g->Nvertexs*sizeof(int));
	
	int *Q;
	Q=(int*)malloc(sizeof(int)*g->Nvertexs);
	
	
	int *sigma;
	sigma=(int*)malloc(sizeof(int)*g->Nvertexs);
	
	int *d;
	d=(int*)malloc(sizeof(int)*g->Nvertexs);
	
	double *delta;
	delta=(double*)malloc(sizeof(double)*g->Nvertexs);
	
	int qIndex;
	int qLast;
	int sLast;
	
	
	for(i=0;i<g->Nvertexs;i++) CB[i]=0.0;
	
	
	for(s=0;s<g->Nvertexs;s++) //for s ∈ V do
	{
		//	NSLog(@"--BC Node s:%i--",s);
		
		sLast=0; // S ← empty stack;
		
		for(j=0;j<g->Nvertexs;j++) pIndexs[j]=0; // P [w] ← empty list, w ∈ V ;
		
		for(j=0;j<g->Nvertexs;j++)
		{
			sigma[j]=0; //σ[t] ← 0, t ∈ V;
			d[j]=-1; // d[t] ← −1 , t∈V;
		}
		
		sigma[s]=1; // σ[s] ← 1;
		d[s]=0; // d[s] ← 0;
		
		qIndex = 0; // Q ← empty queue;
		Q[qIndex] = s; // enqueue s → Q;
		qLast=1;
		
		
		while (qIndex<g->Nvertexs) { // while Q not empty do
			
			v=Q[qIndex]; // dequeue v ← Q;
			S[sLast]=v; // push v → S;
			sLast++;
			
			for(i=0;i<g->llistaVertexs[v].grau;i++) // foreach neighbor w of v do
			{
				w = g->llistaVertexs[v].veins[i];
				// w found for the first time? 
				if(d[w]<0) // if d[w] < 0 then
				{
					Q[qLast]=w; // enqueue w → Q;
					qLast++;
					d[w]=d[v]+1; // d[w] ← d[v] + 1;
				} // end
				// shortest path to w via v?
				if(d[w]==(d[v]+1)) // if d[w] = d[v] + 1 then
				{
					sigma[w]=sigma[w]+sigma[v]; // σ[w] ← σ[w] + σ[v];
					P[w][pIndexs[w]] = v; //append v → P [w];
					pIndexs[w]++;
				} // end
			} // end
			
			
			qIndex++;
		} // end
		
		for(i=0;i<g->Nvertexs;i++) delta[i]=0.0; // δ[v] ← 0, v ∈ V ;
		//for(i=0;i<nNodes;i++) NSLog(@"sigma %i:%f",i,sigma[i]);
		
		// S returns vertices in order of non-increasing distance from s
		for(i=g->Nvertexs-1;i>=0;i--) // while S not empty do
		{
			w=S[i];
			for(j=0;j<pIndexs[w];j++)
			{
				v=P[w][j]; // for v ∈ P[w] do
				delta[v]+=(double)sigma[v]/sigma[w]*(1+delta[w]); // δ[v]←δ[v]+ σ[v]/σ[w] (1+δ[w]);
			}
			
			if(w!=s) CB[w]+=delta[w]; // if w!=s then CB[w]←CB[w]+δ[w];
			
		} // end
		
	}	
	
	
	for(i=0;i<g->Nvertexs;i++)
		g->llistaVertexs[i].bc=(CB[i]/(g->Nvertexs-1)/(g->Nvertexs-2));	
	
	free(pIndexs);
	free(S);
	free(CB);
	for(i=0;i<g->Nvertexs;i++) free(P[i]);
	free(P);
	free(Q);
	free(sigma);
	free(d);
	free(delta);
}

void correlacio(grafStruct *g)
{
	double sum;
	double prod;
	double sumquads;
	int i,j;
	int nBranques;
	//	int nNodes;
	int cmp;
	int vei;
	int grau;
	int grauVei;
	
	sum=0.0;
	prod=0.0;
	sumquads=0.0;
	cmp=0;
	
	nBranques=nombre_branques(g);
	
	for(i=0;i<g->Nvertexs;i++)
	{
		grau=g->llistaVertexs[i].grau;
		for(j=0;j<grau;j++)
		{
			vei=g->llistaVertexs[i].veins[j];
			if(vei>i)
			{
				cmp++;
				grauVei=g->llistaVertexs[vei].grau;
				sum+=(double)(grau+grauVei)/nBranques/2.0;
				prod+=(double)(grau*grauVei)/nBranques;
				sumquads+=(double)(grau*grau+grauVei*grauVei)/nBranques/2.0;
			}
		}
	}
	if((sumquads-sum*sum)==0.0) g->correlacio=0.0;
	else g->correlacio=(prod-sum*sum)/(sumquads-sum*sum);
	
}

void clustering(grafStruct *g)
{
	double clustering;
	int i,j,k,l,posibles,totals,vert,grau;
	
	clustering = 0.0;
	
	
	
	for ( i=0 ; i<g->Nvertexs ;i++) // i= node per al que calculem el cluster
	{
		grau=g->llistaVertexs[i].grau;
		if (grau>1)
		{
			posibles = (grau) * (grau-1) / 2;
			totals=0;
			
			for ( j=0 ; j<grau ; j++)
			{
				vert=g->llistaVertexs[i].veins[j]; // j= vei del node i
				
				for( k=0 ; k<g->llistaVertexs[vert].grau ; k++)      
				{
					for( l=0 ; l<grau ; l++)
					{
						if ( g->llistaVertexs[i].veins[l]==g->llistaVertexs[vert].veins[k]) totals++;
					}
				}
			}
			
			totals = totals / 2;
			g->llistaVertexs[i].clustering=((double)totals/posibles);
			clustering += (((double)totals/posibles)/g->Nvertexs);
		}      
		
	}
	
	g->clustering=clustering;
}


void print_us()
{
	printf("\n Us: analitzar <tipus> <arxiu> <opcions> <sortida>\n");
	printf("    <tipus> pot ser alguns d'aquests valors:\n");
	printf("       t -- tipus d'arxiu txt (adjacencies,branques)\n");
	printf("       g -- tipus d'arxiu gml\n");
	printf("    <arxiu> nom de l'axiu amb les dades del graf\n");
	printf("    <sortida> (opcional) nom de l'axiu de sortida, si no s'especifica serà 'sortida.txt'\n");
	printf("    <opcions> (opcional) opcions per a la realització dels calculs:\n");
	printf("       n -- no escriure dades dels nodes\n");
	printf("       d -- no calcular distancies\n");
	printf("       o -- guarda els resultats en un arxiu de sortida\n");
	printf("\n Exemples d'us;\n");
	printf("    analitzar t graf.txt  -- Llegeix i analitza el graf de l'arxiu 'graf.txt'\n");
	printf("    analitzar t graf.txt o sortida.txt -- Guarda els resultats a l'arxiu 'sortida.txt'\n");
	printf("    analitzar t graf.txt on sortida.txt -- Guarda els resultats en un arxiu sense la informació dels nodes\n");
	printf("    analitzar t graf.txt d -- Analitza el graf sense calcular distancies\n");
	printf("    analitzar t graf.txt odn sortida.txt -- Guarda els resultats sense la informació dels nodes i distàncies\n");
	
}

int llegir_parametres(int argc, char* argv[])
{
	char c;
	int i;
	
	if (argc < 3) return 0;
	
	if (argv[1][0]!='t' && argv[1][0]!='g') {
		printf("\n Paràmetre %s incorrecte", argv[1]);
		return 0;
	}
	
	
	info_nodes=1;
	info_distancies=1;
	escriure_sortida=0;
	
	if (argc>3){
		for(i=0;i<=strlen(argv[3]);i++)
		{
			c=argv[3][i];
			if(c=='n') {
				printf(" No es calcularà la informació dels nodes\n");
				info_nodes=0;
			}
			else if(c=='d') {
				printf(" No es calcularà la informació de distàncies\n");
				info_distancies=0;
			}
			else if(c=='o') {
				printf(" Es guardarà la informació en un arxiu de sortida\n");
				escriure_sortida=1;
			}
		}
		
	}
	
	return 1;
}
#define BAD_FORMAT_FILE_GRAF 0
#define GOOD_FORMAT_FILE_GRAF 1
#define SPACE_CHAR 32
#define MIN_ASCIII_NUMBER_CODE 48
#define MAX_ASCIII_NUMBER_CODE 58
#define EOL 10
int testGrafFileFormat(char *fileName) {
	int c;
	FILE *file;
	file = fopen(fileName, "r");
	if (file) {
    		while ((c = getc(file)) != EOF) {
			
			if ((( c < MIN_ASCIII_NUMBER_CODE ) || (c > MAX_ASCIII_NUMBER_CODE)) 
				&& c != SPACE_CHAR && c != EOL
				) {
				printf("Invalid char %d\n",c);
				return BAD_FORMAT_FILE_GRAF;
			} 
        		putchar(c);
		}
    	fclose(file);
	}

	return GOOD_FORMAT_FILE_GRAF;
}

int main (int argc, char* argv[]) {
  
	grafStruct *g;
	int i;
//	double cluster;
	FILE* file;
	FILE* sortida;

	if (!llegir_parametres(argc,argv)) print_us();
    else {
		if (!testGrafFileFormat(argv[2])) {
			printf("Bad format graf\n");			
			exit (1);
		}
	
		file = fopen (argv[2], "r");
		if (file == 0){
			printf ("\n No es troba l'arxiu: %s\n\n", argv[2]);
			exit(1);
		}else {
			
			// Inicialitzem el graf
			reserva_memoria_graf(&g);
			g->Nvertexs=0;
			
			// Llegim l'arxiu
			if(argv[1][0]=='t') txt_read_graf(file,g) ;
			else if (argv[1][0]=='g') GML_parse_file (file, g);
			
			fclose(file);

			printf (" Nombre de nodes: %i\n",g->Nvertexs);			
			printf (" Nombre de branques: %i\n",nombre_branques(g));
			
			// Graus
			calcular_graus(g);
			printf (" Grau mínim: %i\n",g->grau_minim);
			printf (" Grau màxim: %i\n",g->grau_maxim);
			printf (" Grau mig: %f\n",g->grau_mig);
			
			// Clustering
			clustering(g);
			printf (" Clustering: %f\n",g->clustering);
			
			// Distancies
			if(!es_conexe(g) || !info_distancies) {
				if(!g->conexe) printf("El graf no és connexe, no es calcularà el diàmetre\n");
			}
			else {
				calcular_distancies(g);
				printf (" Radi: %i\n",g->radi);
				printf (" Diametre: %i\n",g->diametre);
				printf (" Distància Mitja: %f\n",g->distancia_mitja);
			}
			
			// Correlacio
			correlacio(g);
			printf(" Correlació: %f\n", g->correlacio);
			
			printf ("\n %i,%i,%i,%i,%f,%f,%i,%i,%f,%f\n\n",g->Nvertexs,nombre_branques(g),g->grau_minim,g->grau_maxim,g->grau_mig,g->clustering,g->radi,g->diametre,g->distancia_mitja,g->correlacio);
			
			// Escribim la sortida
			if(escriure_sortida)
			{
			if (argc==5) sortida=fopen(argv[4],"w");
			else sortida=fopen("sortida.txt","w");
			
			fprintf (sortida,"# Nombre de nodes: %i\n",g->Nvertexs);			
			fprintf (sortida,"# Nombre de branques: %i\n",g->Nbranques);
			fprintf (sortida,"# Grau minim: %i\n",g->grau_minim);
			fprintf (sortida,"# Grau maxim: %i\n",g->grau_maxim);
			fprintf (sortida,"# Grau mig: %f\n",g->grau_mig);
			fprintf (sortida,"# Clustering: %f\n",g->clustering);
			
			if(g->conexe && info_distancies ) {
				fprintf (sortida,"# Radi: %i\n",g->radi);
				fprintf (sortida,"# Diametre: %i\n",g->diametre);
				fprintf (sortida,"# Distancia Mitja: %f\n",g->distancia_mitja);
			}
			
			fprintf(sortida,"# Correlacio (r): %f\n", g->correlacio);
			
			if(info_nodes){
				
				betweenness_centrality(g);
				fprintf(sortida,"#\n# Informacio dels nodes\n#\n");
				fprintf(sortida,"# id\tnom\tgrau\tclustering\texcent\td mitja\t\tbet. cen.\n");
				
				for(i=0;i<g->Nvertexs;i++)
				{
					fprintf(sortida,"%i\t%s\t%i\t%f\t",i,g->llistaVertexs[i].nom,g->llistaVertexs[i].grau,g->llistaVertexs[i].clustering);
						fprintf(sortida,"%i\t%f\t%f\n",g->llistaVertexs[i].excentricitat,g->llistaVertexs[i].distancia_mitja,g->llistaVertexs[i].bc);
				}
			}
			
			fclose(sortida);
			}
			
		}
		
	
	}
	return 1;

}
