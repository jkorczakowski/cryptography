#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include <assert.h>

#define BUFFER_SIZE 1024
#define RANDOM_MAX 100

void generate_keys(char *filename);

int main ()
{
    generate_keys ("elgamal.txt");
    return EXIT_SUCCESS;
}

void generate_keys (char *filename)
{
    mpz_t n;
    char *prime, *generator;
    int alice_key;
    prime = (char *) calloc (BUFFER_SIZE, sizeof (char));
    generator = (char *) calloc (BUFFER_SIZE, sizeof (char));

    FILE *handler = fopen (filename, "r");

    fscanf(handler, "%[^\n] %[^\n]", prime, generator);

    fclose (handler);

    srand (time (NULL));
    alice_key = rand () % RANDOM_MAX + 1;

    mpz_init (n);
    mpz_set_ui (n, 0);


    printf("%s\n", prime);
    printf("%s\n", generator);
    printf("%d\n", alice_key);
    
    free (generator);
    free (prime);
}
