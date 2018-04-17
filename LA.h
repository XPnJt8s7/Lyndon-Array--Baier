#ifndef LA_H
#define LA_H

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[93m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

int gsaca_phase_1(const unsigned char *S, unsigned int *LA, unsigned int *SA, unsigned int *ISA , unsigned int *PREV, void *GSIZE, unsigned int n);

int newLA(const unsigned char *S, unsigned int *LA, unsigned int *SA, unsigned int *ISA , void *GSIZE, unsigned int n);

#endif /* end of include guard: AC_H */
