#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct state state_t;
typedef struct transition transition_t;
typedef struct nfa nfa_t;
int nfa_from_file(nfa_t *nfa, char *file_path);

typedef struct state {
    char *name;
    uint32_t id;
} state_t;

typedef struct transition {
    state_t *from;
    state_t *to;
    char symbols[]; /* allow multiple symbols in one transition */
} transition_t;

typedef struct nfa {
    char *alphabet;
    state_t *states;
    state_t start;
    transition_t *transitions;
    state_t *final_states;
} nfa_t;

int nfa_from_file(nfa_t *nfa, char *file_path) {
    FILE *f = fopen(file_path,"r");
    if (!f) return 1;

    int buf_len = 1024;
    char buf[buf_len];
    size_t lines_read = 0;

    while(fgets(buf, buf_len, f)) {
        switch(++lines_read) {
            case 1:
                nfa->alphabet = (char*) malloc(strlen(buf));
                *nfa->alphabet = *buf;
                break;
        }
    }

    fclose(f);
    return 0;
}

int main(void) {
    fprintf(stdout, "Hello, pookie!\n");
    nfa_t *a1 = (nfa_t*) malloc(sizeof(nfa_t));
    if(nfa_from_file(a1, "/home/joppo/programming/automatons/automaton1") != 0) {
        /* error occured */
        printf("error opening file.\n");
    }

    //use automaton:

    free(a1);
    return 0;
}
