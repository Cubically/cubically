#pragma once

#include <wchar.h>

#define clear_jump(x)  for (i = 0; i < 9; i++)            \
                           jumps[jumpnum+x].faces[i] = 0; \
                       jumps[jumpnum+x].pos = 0;

#define faceval _faceval(arg)

#define special(x)  (x == L'(' || x == L')')
int execute(wint_t,int);
int do_jump(void);
int32_t _faceval(int);
void do_skip(void);

typedef struct {
  wint_t command;
  int arg;
} command;
    
wint_t *parse_file(FILE *);
wint_t *parse_string(char *);
command *parse_commands(wint_t *);

int interp(void);
void cubically_evaluate(void);

command *commands;
