#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char	c[68];

void	m() {
	printf("%s - %d\n", c, time(0));
}

int		main(int ac, char **av) {
	int	*a;
	int	*b;

	a = malloc(sizeof(int) * 2);
	a[0] = 1;
	a[1] = malloc(8);

	b = malloc(sizeof(int) * 2);
	b[0] = 2;
	b[1] = malloc(8);

	strcpy((char *)a[1], av[1]);
	strcpy((char *)b[1], av[2]);
	
	fgets(c, 68, fopen("/home/user/level8/.pass", "r"));

	puts("~~");
	return (0);
}