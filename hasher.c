// vim: softtabstop=4 :shiftwidth=4 :expandtab
/*****************************************************************
 *
 * Hashing strings
 *
 * u@sansculotte.net 2018
 *
**/
#include <string.h>
#include <stdlib.h>
#include "m_pd.h"

#define VERSION "v0.0.1"

#define TRUE 1
#define FALSE 0


typedef enum {MD5, SHA1, SHA256} t_algorithm;

static t_class *hasher_class;

typedef struct _hasher {
    t_object  x_obj;
    t_outlet  *s_out;
    t_algorithm algorithm; 
} t_hasher;

void _hasher_about(t_hasher *x);
void _hasher_algorithm(t_hasher *x, t_symbol *s);
void _hasher_hash(t_hasher *x, t_symbol *s);

t_algorithm select_algorithm(t_symbol *algorithm);
char* algorithm_name(t_algorithm algorithm);


/**
 * Select algorithm from user input
 * default to md5
 */
t_algorithm select_algorithm(t_symbol *algorithm) {

    char* a = algorithm->s_name;

    if (strcmp(a, "sha-256") == 0 || strcmp(a, "sha256") == 0) {
        return SHA256;
    }
    if (strcmp(a, "sha-1") == 0 || strcmp(a, "sha1") == 0) {
        return SHA1;
    }
    return MD5;
}

/**
 * get human readable name for algorithm
 */
char* algorithm_name(t_algorithm a) {
    char* algorithms[] = {"md5", "sha1", "sha256"};
    return algorithms[a];
}

/**
 * constructor
 */
void* hasher_new(t_symbol* s)
{
    t_hasher *x = (t_hasher *)pd_new(hasher_class);

    x->algorithm = select_algorithm(s);
    x->s_out = outlet_new(&x->x_obj, &s_symbol);
    
    return (void *)x;
}

/**
 * info and pointers
 */
void _hasher_about(t_hasher *x)
{
    char v[64];
    sprintf(v, "[hasher] version: %s\nalgorithm: %s", VERSION, algorithm_name(x->algorithm));
    post(v);
    post("https://github.com/sansculotte/pd-hasher");
}

/**
 * change the hashering algorithm
 */
void _hasher_algorithm(t_hasher *x, t_symbol *s) {
    x->algorithm = select_algorithm(s);
}

/**
 * hash a string 
 */
void _hasher_hash(t_hasher *x, t_symbol *s) {
    outlet_symbol(x->s_out, s);
}

/**
 * create object
 */
void hasher_setup(void)
{
    hasher_class = class_new(
        gensym("hasher"),
        (t_newmethod)hasher_new,
        0,
        sizeof(t_hasher),
        CLASS_DEFAULT,
        A_DEFSYMBOL,
        0
    );
    class_addmethod(hasher_class, (t_method)_hasher_about, gensym("about"), 0);
    class_addmethod(hasher_class, (t_method)_hasher_algorithm, gensym("algorithm"), A_SYMBOL, 0);
    class_addmethod(hasher_class, (t_method)_hasher_hash, gensym("hash"), A_SYMBOL, 0);
}