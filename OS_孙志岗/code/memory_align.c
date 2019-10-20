#include <stdio.h>

struct a
{
	int i;
	char c;
	int j;
};

struct b
{
	int i;
	char c;
	char d;
	int j;
};

struct c
{
	int i;
	char c;
	int j;
	char d;
};



int main() {
	printf("%lu\n", sizeof(struct a));
	printf("%lu\n", sizeof(struct b));
	printf("%lu\n", sizeof(struct c));
	return 0;
}