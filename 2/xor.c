#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

long long convertDecimalToBinary(int n)
{
    long long binaryNumber = 0;
    int remainder, i = 1;

    while (n!=0)
    {
        remainder = n%2;
        n /= 2;
        binaryNumber += remainder*i;
        i *= 10;
    }
    return binaryNumber;
}



char* read_file (char *filename)
{
  char *buffer = NULL;
  int string_size, read_size;
  FILE *handler = fopen(filename, "r");
    if(handler == NULL) {
        printf("ERROR! CANNOT OPEN %s\n",filename);
        exit(1);
    }
  if (handler)
  {
    fseek(handler, 0, SEEK_END);
    string_size = ftell(handler);
    rewind(handler);

    buffer = (char*)malloc(sizeof(char)*(string_size+1));
    read_size = fread(buffer, sizeof(char), string_size, handler);
    buffer[string_size] = '\0';

    if (string_size != read_size)
    {
      free(buffer);
      buffer=NULL;
    }

    fclose(handler);
  }
  return buffer;
  }


 void create_plain_text(char *buffer)
 {
     
     FILE *handler = fopen("plain.txt", "w");

        if(handler == NULL) {
            printf("ERROR! CANNOT OPEN plain.txt"); 
            exit(1);
            }
     
     for(int i = 0; i < strlen(buffer); i++)
     {
         if(buffer[i] != '\n')
            fprintf(handler,"%c",buffer[i]);

         if(((i+1)%25 == 0) && (i+1 != strlen(buffer)))
            fprintf(handler, "%c", '\n');
     }

     fclose(handler);
     
 }

 char* getkey(){

        FILE *key;
        char c;
        static char keytab[32];
        int i=0;

        if((key=fopen("key.txt", "r")) == NULL)
        {
          printf("ERROR! CANNOT OPEN key.txt\n");
          exit(1);
        }
        c = fgetc(key);
          while(c != EOF){
            keytab[i] = c;
            c = fgetc(key);
            i++;
          }

        fclose(key);
        return keytab;
}

 void xor_encrypt(char *key, char *buffer)
 {
     int k = 0;
     FILE *handler = fopen("crypto.txt", "w");
     if(handler == NULL){
        printf("ERROR! CANNOT OPEN crypto.txt"); exit(1);}

     for(int i = 0; i < strlen(buffer); i++)
     {
         if(buffer[i] == '\n' && i != strlen(buffer)) {
             i++;
             k = 0;
             fprintf(handler,"%c",'\n');
         }

         if(i+1%4 == 0)
            k = 0;

          if(buffer[i+1] == '\n' || buffer[i+1] == '\0')
            fprintf(handler, "%d", buffer[i]^key[k]);
          
          else
            fprintf(handler, "%d ", buffer[i]^key[k]);

     }

     fclose(handler);
 }


void xor_decrypt(){

 FILE *crypto, *decrypt;
  char c, k, *keyt=getkey();
  int j=0, i=strlen(keyt);
  int cryptarray[i];
  char decryptarray[i],pass[i];


  for(int n = 0; n < i; n++){
    pass[n] = '0';

  }

  if((decrypt=fopen("decrypt.txt", "wb")) == NULL)
  {
    printf("ERROR! CANNOT OPEN decrypt.txt\n");
    exit(1);
  }

  if((crypto=fopen("crypto.txt", "rb")) == NULL)
  {
    printf("ERROR! CANNOT OPEN crypto.txt\n");
    exit(1);
  }

  while((fscanf(crypto, "%d ", &cryptarray[j])) != EOF){
    if(j != i)
    {
      if(pass[j] != '0') fprintf(decrypt, "%c", (cryptarray[j]^pass[j]));
      else{
          if(cryptarray[j] >= 64)
          {
            decryptarray[j] = (char)(cryptarray[j]^32);
            fprintf(decrypt,"%c", decryptarray[j]);
            if(pass[j] == '0') pass[j]=decryptarray[j];
          }
          else fprintf(decrypt,"*");

        }
        j++;
    }
    if(j == i)
    {
      j = 0;
      fprintf(decrypt,"\n");
    }
  }

  fclose(decrypt);
  fclose(crypto);
}

 void xor_crack(int line_length)
 {

     //30 - text lines
     //25 - line length
     unsigned int buffer[line_length];
     unsigned int text_line = 30;
     unsigned int a, b, c;
     unsigned int space =  32;
     unsigned int spaceMask =  64;
     unsigned int *password = calloc(line_length, sizeof(unsigned int));
     unsigned int *key = malloc(line_length * sizeof(unsigned int));
     unsigned int ab_s;
     unsigned int ac_s;
     unsigned int bc_s;


     FILE *handler = fopen("crypto.txt", "r");
     

     for(unsigned int j = 0; j < 1; j++) {
         
         for(int i = 0; i < line_length; i++) {
            fscanf(handler,"%u ", &buffer[i]);
         }
         for(int w = 0; w < line_length; w++)
            printf("%u", buffer[w]);
         
         printf("\n");
      //   exit(1);
     
        for(unsigned int k = 0; k < 1; k++) {
            
           unsigned int cl = j * (line_length);
             a = k + 0;
             b = k + 1;
             c = k + 2;
            

           ab_s = (unsigned int) ((buffer[a + cl] ^ buffer[b + cl]) & spaceMask);
           ac_s = (unsigned int) ((buffer[a + cl] ^ buffer[c + cl]) & spaceMask);
           bc_s = (unsigned int) ((buffer[b + cl] ^ buffer[c + cl]) & spaceMask);
          // printf("%d %d %d\n",ab_s, ac_s, bc_s);
           


            if(ab_s != 0)
            {
                if(ac_s != 0 && bc_s == 0) {
                    password[a] = (unsigned int) (buffer[a + cl] ^ (unsigned int) space); printf("1. %u\n",password[a]); }
                
                else if(ac_s == 0 && bc_s != 0) {
                    password[b] = (unsigned int) (buffer[b + cl] ^ (unsigned int) space);printf("2. %u\n",password[b]); }
            }

            if(ac_s != 0)
            {
                if(ab_s != 0 && bc_s == 0) {
                    password[a] = (unsigned int) (buffer[a + cl] ^ (unsigned int) space);printf("3. %u\n",password[a]); } 
                
                else if(ab_s == 0 && bc_s != 0) {
                    password[c] = (unsigned int) (buffer[c + cl] ^ (unsigned int) space); printf("4. %u\n",password[a]); }
            }

            if(bc_s != 0)
            {
                if(ab_s != 0 && ac_s == 0) {
                    password[b] = (unsigned int) (buffer[b + cl] ^ (unsigned int) space);printf("5. %u\n",password[a]); }
                
                else if(ab_s == 0 && ac_s != 0) {
                    password[c] = (unsigned int) (buffer[c + cl] ^ (unsigned int) space); printf("6. %u\n",password[a]); }
            }
            
        }
     }
    
       
       // printf("%u", password[0]);
       // printf("\n");
         
        
         fclose(handler);
         free(key);
         free(password);
     }
 

int main(int argc, char **argv)
{

    if(argc != 2) {
        printf("No arguments.\n-p to prepare\n-e to encrypt\n-k to crack\n");
        exit(1);
    }

    
    if(strcmp(argv[1],"-p") == 0)
    {
        char* original_text = read_file("orig.txt");
        create_plain_text(original_text);
        free(original_text);

    }

    if(strcmp(argv[1], "-e") == 0)
    {
        char* plain_text = read_file("plain.txt");
        char* key = read_file("key.txt");
        xor_encrypt(key, plain_text);
        free(key);
        free(plain_text);

    }

    if(strcmp(argv[1], "-k") == 0)
    {
        char* crypted = read_file("crypto.txt");
        xor_decrypt();
        free(crypted);
    }
    

    



    /*printf("%s\n",original_text);
    printf("%s\n",plain_text);
    printf("%s\n",key);
    printf("%s\n",crypted);*/

    return 0;
}