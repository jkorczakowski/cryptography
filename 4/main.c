#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hex_to_bin (char *hex, char *bin)
{
    for (int i = 0; hex[i] != '\0'; ++i)
    {
        switch (hex[i])
        {
            case '0':
                strcat(bin, "0000");
                break;
            case '1':
                strcat(bin, "0001");
                break;
            case '2':
                strcat(bin, "0010");
                break;
            case '3':
                strcat(bin, "0011");
                break;
            case '4':
                strcat(bin, "0100");
                break;
            case '5':
                strcat(bin, "0101");
                break;
            case '6':
                strcat(bin, "0110");
                break;
            case '7':
                strcat(bin, "0111");
                break;
            case '8':
                strcat(bin, "1000");
                break;
            case '9':
                strcat(bin, "1001");
                break;
            case 'a':
            case 'A':
                strcat(bin, "1010");
                break;
            case 'b':
            case 'B':
                strcat(bin, "1011");
                break;
            case 'c':
            case 'C':
                strcat(bin, "1100");
                break;
            case 'd':
            case 'D':
                strcat(bin, "1101");
                break;
            case 'e':
            case 'E':
                strcat(bin, "1110");
                break;
            case 'f':
            case 'F':
                strcat(bin, "1111");
                break;
            default:
                printf("Invalid hexadecimal input.");
        }
    }
}

int diff_count (char *bin_one, char *bine_two)
{
    if (strlen (bin_one) != strlen (bine_two)) {
        printf ("BYTES LENGHT NOT EQUAL!");
        exit (1);
    }

    int diff = 0;
    int length = strlen (bin_one);


    for (int i = 0; i < length; ++i)
    {
        if (bin_one[i] != bine_two[i])
            ++diff;
    }

    return diff;
}

int main ()
{
    
    int diff;
    int lenght;
    double perc;

    FILE *handler_read = fopen ("hex.txt", "r");
    FILE *handler_write = fopen ("diff.txt", "w");
    for (int i = 0; i < 6; ++i)
    {
    char *buffer_hex_one = calloc (1024, sizeof(char));
    char *buffer_hex_two = calloc (1024, sizeof(char));
    char *buffer_bytes_one = calloc (1024, sizeof(char));
    char *buffer_bytes_two = calloc (1024, sizeof(char));

        fscanf(handler_read, "%s", buffer_hex_one);
        fscanf(handler_read, "%s", buffer_hex_two);
        //printf("%s\n%s\n", buffer_hex_one, buffer_hex_two);

        hex_to_bin(buffer_hex_one, buffer_bytes_one);
        hex_to_bin(buffer_hex_two, buffer_bytes_two);
        lenght = strlen(buffer_bytes_one);
       // printf("%s\n%s\n", buffer_bytes_one, buffer_bytes_two);
       // printf("bytes count %d\n", lenght);

        diff = diff_count(buffer_bytes_one, buffer_bytes_two);
        perc = ((double) diff/lenght) * 100;
       // printf("diff = %d\n", diff);

        if (i == 0) {
            fprintf(handler_write, "cat hash.pdf personal.txt | md5sum\ncat hash.pdf personal_.txt | md5sum\n%s\n%s\nLiczba bitów różniąca wyniki: %d tj. %.2lf%% z %d\n\n", buffer_hex_one, buffer_hex_two, diff, perc, lenght);
        }

        else if (i == 1) {
            fprintf(handler_write, "cat hash.pdf personal.txt | sha1sum\ncat hash.pdf personal_.txt | sha1sum\n%s\n%s\nLiczba bitów różniąca wyniki: %d tj. %.2lf%% z %d\n\n", buffer_hex_one, buffer_hex_two, diff, perc, lenght);
        }

        else if (i == 2) {
            fprintf(handler_write, "cat hash.pdf personal.txt | sha224sum\ncat hash.pdf personal_.txt | sha224sum\n%s\n%s\nLiczba bitów różniąca wyniki: %d tj. %.2lf%% z %d\n\n", buffer_hex_one, buffer_hex_two, diff, perc, lenght);
        }

        else if (i == 3) {
            fprintf(handler_write, "cat hash.pdf personal.txt | sha256sum\ncat hash.pdf personal_.txt | sha256sum\n%s\n%s\nLiczba bitów różniąca wyniki: %d tj. %.2lf%% z %d\n\n", buffer_hex_one, buffer_hex_two, diff, perc, lenght);
        }

        else if (i == 4) {
            fprintf(handler_write, "cat hash.pdf personal.txt | sha384sum\ncat hash.pdf personal_.txt | sha384sum\n%s\n%s\nLiczba bitów różniąca wyniki: %d tj. %.2lf%% z %d\n\n", buffer_hex_one, buffer_hex_two, diff, perc, lenght);
        }

        else if (i == 5) {
            fprintf(handler_write, "cat hash.pdf personal.txt | sha512sum\ncat hash.pdf personal_.txt | sha512sum\n%s\n%s\nLiczba bitów różniąca wyniki: %d tj. %.2lf%% z %d\n\n", buffer_hex_one, buffer_hex_two, diff, perc, lenght);
        }

    free(buffer_bytes_two);
    free(buffer_bytes_one);
    free(buffer_hex_two);
    free(buffer_hex_one);
    }


    fclose(handler_write);
    fclose(handler_read);
    
    return 0;

}
