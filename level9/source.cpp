#include <string.h>

class N {

	private:

	int		nb;
	char	s[100];

	public:

	N(int n) : nb(n) {
	}
	void	setAnnotation(char *s) {
		memcpy(this->s, s, strlen(s));
	}
	int	operator+(N& n) {
		return this->nb + n.nb;
	}
	int	operator-(N& n) {
		return this->nb - n.nb;
	}
};

int		main(int ac, char **av) {
	if (ac < 1) {
		_exit(1);
	}
	N	*a = new N(5);
	N	*b = new N(6);
	a->setAnnotation(av[1]);
	return (*a + *b);
}