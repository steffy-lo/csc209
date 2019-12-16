#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXTSIZE 200

struct node {
    char text[TEXTSIZE];  /* either the question or the animal name */
    struct node *ifyes, *ifno;  /* if both NULL, text is animal name, else q */
};

struct node frog = { "frog", NULL, NULL };
struct node squirrel = { "squirrel", NULL, NULL };
struct node top = { "Is your animal green?", &frog, &squirrel };


int main()
{
    extern void doit();
    while (1)
	doit();
}


void doit()
{
    struct node *p = &top, *old, *new;
    extern int getyesno();
    extern void mygetline(char *);

    printf("Think of an animal.\n");
    while (p->ifyes) {
	/* question */
	printf("%s\n", p->text);
	if (getyesno())
	    p = p->ifyes;
	else
	    p = p->ifno;
    }

    /* animal */
    printf("Is your animal a %s?\n", p->text);
    if (getyesno()) {
	printf("I got it!\n\n");
	return;
    }

    /* new nodes are animals; old node is the question */
    old = (struct node *)malloc(sizeof(struct node));
    new = (struct node *)malloc(sizeof(struct node));
    if (old == NULL || new == NULL) {
	printf("out of memory\n");
	exit(1);
    }

    strcpy(old->text, p->text);
    old->ifyes = old->ifno = NULL;

    printf("Aw, I lose.  What animal were you thinking of?\n");
    mygetline(new->text);
    new->ifyes = new->ifno = NULL;

    printf("Tell me a question which would distinguish a %s from a %s.\n", old->text, new->text);
    mygetline(p->text);
    printf("What would be the answer for a %s?\n", new->text);
    if (getyesno()) {
	p->ifyes = new;
	p->ifno = old;
    } else {
	p->ifyes = old;
	p->ifno = new;
    }

    printf("I'll remember that.\n\n");
}


void mygetline(char *s)
{
    char *p;
    if (!fgets(s, TEXTSIZE, stdin))
	exit(1);
    if ((p = strchr(s, '\n')))
	*p = '\0';
}


int getyesno()
{
    char buf[80];
    extern void dump(struct node *p, int level);
    while (fgets(buf, sizeof buf, stdin)) {
	if (buf[0] == 'y')
	    return 1;
	if (buf[0] == 'n')
	    return 0;
	if (strcmp(buf, "DUMP\n") == 0)
	    dump(&top, 0);
	printf("Please answer the question.\n");
    }
    exit(1);
}


void dump(struct node *p, int level)
{
    extern void space(int i);
    space(level);
    if (p->ifyes) {
	printf("Question: %s\n", p->text);
	space(level);
	printf("If yes:\n");
	dump(p->ifyes, level + 1);
	space(level);
	printf("If no:\n");
	dump(p->ifno, level + 1);
    } else {
	printf("Animal: %s\n", p->text);
    }
}


void space(int i)
{
    while (i-- > 0)
	printf("  ");
}
