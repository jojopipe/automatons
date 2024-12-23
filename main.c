#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    int states_count;
    state_t start;
    transition_t *transitions;
    int transitions_count;
    state_t *final_states;
    int final_states_count;
} nfa_t;

int char_match(char c, char *s) {
    char *end = &s[strlen(s)];
    for (char *p = s; p < end; ++p) { //pointer magic
        if (*p == c) {
            return 1;
        }
    }
    return 0;
}

void string_split(char **output, int *outputc, char *input, int inptuc, char split[]) {
    assert(output);
    assert(input);
    *outputc = 0;
    output[*outputc] = input;
    for (int i = 1; i < inptuc; ++i) {
        if (input[i] == '\0') break;
        if (char_match(input[i], split)) {
            input[i] = '\0';
            /* look for next non-split char / skip over every split char */
            for (int j = i + 1; j < inptuc; ++j) {
                if (char_match(input[j], split)) continue;
                if (input[j] == '\0') break;
                output[++(*outputc)] = &input[j];
                i += (j - i - 1);
                break;
            }
        }
    }
    output[++(*outputc)] = NULL;
}

int nfa_from_file(nfa_t *nfa, char *file_path) {
    FILE *f = fopen(file_path,"r");
    if (!f) return 1;

    int buf_len = 1024;
    int actual_buf_size = 0;
    char *buf = (char*) malloc(sizeof(char) * buf_len);
    char *working_buf;
    size_t lines_read = 0;
    char **split;

    while(fgets(buf, buf_len, f)) {
        actual_buf_size = (int) strlen(buf);
        working_buf = (char*) malloc(sizeof(char) * buf_len);
        memcpy(working_buf, buf, actual_buf_size);
        ++lines_read;
        if (working_buf[actual_buf_size-1] == '\n') \
            working_buf[actual_buf_size-1] = '\0';
        switch(lines_read) {
            case 1: { /* alphabet */
                nfa->alphabet = (char *) malloc(strlen(working_buf));
                assert(nfa->alphabet);
                memcpy(nfa->alphabet, working_buf, actual_buf_size);
                //nfa->alphabet = buf;
                break;
            }
            case 2: { /* states */
                split = (char **) malloc(sizeof(char *) * actual_buf_size);
                assert(split);
                int splitc = 0;
                string_split(split, &splitc, working_buf, actual_buf_size, (char[]) {','});
                //buf unusable now!!!
                state_t *states = (state_t*) malloc(sizeof(state_t*) * splitc);
                assert(states);
                int i;
                for (i = 0; i < splitc; ++i) {
                    state_t *curr = (state_t*) malloc(sizeof(state_t));
                    assert(curr);
                    curr->name = split[i];
                    memcpy(curr->name, split[i], strlen(split[i]));
                    curr->id = i;
                    states[i] = *curr;
                }
                nfa->states_count = splitc;
                nfa->states = states;
                break;
            }
            case 3: { /* starting state */
                assert(nfa->states);
                for (int i = 0; i < nfa->states_count-1; ++i) {
                    if (strcmp(working_buf, nfa->states[i].name) != 0) continue;
                    nfa->start = nfa->states[i];
                }
                break;
            }
            case 4: { /* final states */
                break;
            }
            default: {
                if (*working_buf == '#') continue;
                ++nfa->transitions_count;
                break;
            }
        }
    }

    fclose(f);
    return 0;
}

int main(void) {
    fprintf(stdout, "Hello, pookie!\n");
    nfa_t *a1 = (nfa_t*) malloc(sizeof(nfa_t));
    assert(a1);
    if (nfa_from_file(a1, "/home/joppo/programming/automatons/automaton1") != 0) {
        /* error occured */
        printf("error opening file.\n");
    }

    //use automaton:
    for (int i = 0; i < a1->states_count; ++i) {
        printf("%d: %s\n", a1->states[i].id, a1->states[i].name);
    }

    free(a1);
    return 0;
}
