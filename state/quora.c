#include <stdio.h>

struct state;
typedef void state_fun(struct state *);

struct state
{
    state_fun * next;
    int i; // data
};

state_fun foo, bar;

void foo(struct state * state)
{
    printf("%s %i\n", __func__, ++state->i);
    state->next = bar;
}

void bar(struct state * state)
{
    printf("%s %i\n", __func__, ++state->i);
    state->next = state->i < 5 ? foo : 0;
}

int main(void)
{
    struct state fbg = { foo, 100 };
    while(fbg.next) fbg.next(&fbg);
}