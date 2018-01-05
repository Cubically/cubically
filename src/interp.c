#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <locale.h>
#include "rubiks.h"
#include "lang.h"

int32_t mem    = 0;
int32_t solved = 1;
int32_t input  = -1;
int pos;
int do_else;
int cur_depth;

extern int DEBUG, codepage;

struct {
    int pos;
    int faces[9];
} jumps[999];
int jumpnum;

#define CPC commands[pos].command
#define CPA commands[pos].arg

int call_command(void)
{
    if (commands[pos].argc == 0) {
        int ret = execute(CPC, -1);
        return ret;
    }

    int i, exec;
    for (i = exec = 0; i < commands[pos].argc; i++) {
        int arg = CPA[i];

        if (arg >= 100) {
            arg -= 100;

            if (issubscript(arg)) {
                cur_depth *= 10;
                cur_depth += numberize(arg);
                continue;
            }
            else if (issuperscript(arg)) {
                cur_depth *= 10;
                cur_depth += _faceval(numberize(arg));
                continue;
            }
            else {
                arg = _faceval(numberize(arg));
            }
        }

        exec++;
        int ret = execute(CPC, arg);
        if (ret)
            return ret;
    }

    if (CPC == '(' || CPC == ')' || !exec)
        execute(CPC, -1);

    cur_depth = 0;
}

int interp(void)
{
    while (commands[pos].command) {
        if (DEBUG) {
            printf("interpreting %c(",commands[pos].command);
            int i;
            for (i = 0; i < commands[pos].argc; i++) {
                printf("%d",commands[pos].arg[i]);
                if (i != (commands[pos].argc - 1))
                    putchar(',');
            }
            puts(")");
        }

        if (call_command())
            break;

        pos++;
    }

    fprintf(stderr, "Notepad: %d\n\n", mem);
    printcube();

    return 0;
}

int execute(char command, int arg)
{
    DEBUG && printf("Command %d=%c, arg %d, depth %d\n", command, command, arg, cur_depth);

    if (rubiksnotation(command)+1) {
        int face  = rubiksnotation(command);
        int turns = abs(arg);
        turncube(face,turns,cur_depth);
    }
    else if (command == 'M') {
        turncube(LEFT,abs(arg),(CUBESIZE-1)/2);
    }
    else if (command == 'E') {
        turncube(DOWN,abs(arg),(CUBESIZE-1)/2);
    }
    else if (command == 'S') {
        turncube(FRONT,abs(arg),(CUBESIZE-1)/2);
    }
    else if (command == '+') {
        mem += faceval;
    }
    else if (command == '-') {
        mem -= faceval;
    }
    else if (command == '/') {
        if (arg == -1)
            input && (mem /= input);
        else
            faceval && (mem /= faceval);
    }
    else if (command == '*') {
        mem *= faceval;
    }
    else if (command == '=') {
        if (arg == -1)
            mem = (mem == input);
        else
            mem = (mem == faceval);
    }
    else if (command == '$') {
        int x = abs(arg);
        while (x--)
        {
            int retval = scanf("%d",&input);
            if (retval < 0)
                input = 0;
        }
    }
    else if (command == '~') {
        int x = abs(arg);
        while (x--)
            input = getchar();
    }
    else if (command == '%') {
        printf("%d",faceval);
        fflush(stdout);
    }
    else if (command == '@') {
        putchar(faceval);
        fflush(stdout);
    }
    else if (command == ':') {
        if (arg == -1)
            mem = input;
        else
            mem = faceval;
    }
    else if (command == '^') {
        mem = pow(mem,faceval);
    }
    else if (command == '<') {
        mem = (mem < faceval);
    }
    else if (command == '>') {
        mem = (mem > faceval);
    }
    else if (command == '_') {
        faceval && (mem %= faceval);
    }
    else if (command == '|') {
        mem |= faceval;
    }
    else if (command == 0x80) {
        mem ^= faceval;
    }
    else if (command == 0x81) {
        mem <<= faceval;
    }
    else if (command == 0x82) {
        mem >>= faceval;
    }
    else if (command == 0x83) {
        mem &= faceval;
    }
    else if (command == '&') {
        if (arg == -1 || faceval)
            return -1;
    }
    else if (command == '(') {
        if (arg == -1) {
            jumps[jumpnum++].pos = pos;
            DEBUG && fprintf(stderr,"jumps[jumpnum-1].pos = %d\n", pos);
        } else {
            jumps[jumpnum].faces[arg] = 1;
        }
    }
    else if (command == ')') {
        if (arg == -1)
            do_jump();
        else
            jumps[jumpnum].faces[arg] = 1;
    }
    else if (command == '!') {
        if (faceval)
            do_skip();
    }
    else if (command == '?') {
        if (!faceval)
            do_skip();
    }
    else if (command == 0x86) {
        cubically_evaluate();
    }

    return 0;
}

int rubiksnotation(char x)
{
    switch (x) {
        case 'U': return 0;
        case 'L': return 1;
        case 'F': return 2;
        case 'R': return 3;
        case 'B': return 4;
        case 'D': return 5;
        default:  return -1;
    }
}

/* this logic is flawless but I hate this function with a burning passion */
int do_jump(void)
{
    int i, count, _do_jump1, _do_jump2;
    for (i = count = _do_jump1 = 0; i < 9; i++)
    {
        if (jumps[jumpnum-1].faces[i]) {
            count++;
            if (_faceval(i))
                _do_jump1 = 1;
        }
    }
    if (!count)
        _do_jump1 = 1;
    for (i = count = _do_jump2 = 0; i < 9; i++)
    {
        if (jumps[jumpnum].faces[i]) {
            count++;
            if (_faceval(i))
                _do_jump2 = 1;
        }
    }
    if (!count)
        _do_jump2 = 1;

    DEBUG && fprintf(stderr, "Jumping if %d&&%d (pos %d)\n", _do_jump1, _do_jump2, pos);

    if (_do_jump1 && _do_jump2) {
        pos = jumps[jumpnum-1].pos;
        DEBUG && fprintf(stderr, "Jumped (pos %d)\n", pos);
        clear_jump(0);
        return 1;
    }
    else {
        clear_jump(-1);
        clear_jump(0);
        jumpnum--;
        return 0;
    }
}

int32_t _faceval(int face)
{
    face = face == 9 ? 0 : face;
    if (face == 8)
        return !issolved();
    if (face == 7)
        return input;
    else if (face == 6 || face == -1)
        return mem;
    else {
        int32_t retval = 0;
        size_t i, j;
        for (i = 0; i < CUBESIZE; i++)
            for (j = 0; j < CUBESIZE; j++)
                retval += (int32_t) CUBE(face,i,j);
        return retval;
    }
}

void do_skip(void)
{
    char c = commands[++pos].command;

    if (c == '{') {
        int loop = 1, level = 0;
        while (loop) {
            c = commands[++pos].command;
            if (c == '{')
                level++;
            else if (c == '}')
                loop = level--;
        }
    }
}

void cubically_evaluate(void)
{
    char *buf = malloc(1024);
    memset(buf, 0, 1024);

    if (!fgets(buf,1024,stdin))
        return;

    if (buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = 0;

    int *str = parse_string(buf);
    DEBUG && printf("Read: <%s>\n", (char*) str);
    command *cmds = parse_commands(str);
    int l; // number of new commands
    int r; // number of old commands remaining in source
    int i; // used for final insert loop

    for (l = 0; cmds[l].command; l++);// { DEBUG && printf("Read: %c%d\n", cmds[l].command, cmds[l].arg); }
    l--; // no clue

    // FIXME copying args

    for (r = pos; commands[r].command; r++);
    for (; r > pos; r--) {
        commands[r+l].command = commands[r].command;
        commands[r+l].argc = commands[r].argc;
    }

    for (i = 0; cmds[i].command; i++) {
        commands[pos+i].command = cmds[i].command;
        commands[pos+i].argc = cmds[i].argc;
    }

    pos--;
}
