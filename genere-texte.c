#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {

    //il faut 2 arguments pour executer le program
    if(argc < 3 || argc > 3){
        printf("Très peu ou beacoup des arguments.\n");
        return 1;
    }
    // printf("%u\n", (unsigned int)time(NULL));

    // unsigned int rng;
    // int urnd = open("/dev/random", O_RDONLY);
    // read(urnd, &rng, sizeof(unsigned int));
    // close(urnd);
    //
    // srand(rng);

    srand((unsigned int)time(NULL));

    size_t longeur = strtoul(argv[1],NULL,10);
    int taille_alph = (int)strtol(argv[2],NULL,10);

    if (taille_alph == 4) {
        int num;
        for (size_t i = 0; i < longeur; i++) {
            num = (rand() % taille_alph);

            switch (num) {
                case 0:
                    printf("a");
                    break;
                case 1:
                    printf("c");
                    break;
                case 2:
                    printf("g");
                    break;
                case 3:
                    printf("t");
                    break;
            }
        }
    } else {
        for (size_t i = 0; i < longeur; i++) {
            printf("%c", (rand() % taille_alph + 33) );
        }
    }


    printf("\n");

    return 0;
}
