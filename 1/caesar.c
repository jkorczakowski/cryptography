/*
Juliusz Korczakowski
Indeks - 246817
Tester Programista
Kryptografia - Szyfr Cezara i Szyfr Afiniczny
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<math.h>
#define M 26
#define M_GCD_COUNT 12

int modInverse(int a) 
{ 
    int m = M;
    a = a%m; 
    for (int x=1; x<m; x++) 
       if ((a*x) % m == 1) 
          return x;
    return 1; 
} 

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int gcd(int a, int b)
{
    while (b != 0)
    {
        a %= b;
        a ^= b;
        b ^= a;
        a ^= b;
    }

    return a;
}

int *gcd_table ()
{
  int *table = calloc(M_GCD_COUNT, sizeof(int));
  int i,j;

  for (i = 0, j = 0; i < M; i++) {
    if (gcd(i, M) == 1) {
    table[j] = i;
    j++;
    }
  }

  return table;
}

char* read_file (char *filename)
{
  char *buffer = NULL;
  int string_size, read_size;
  FILE *handler = fopen(filename, "r");

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

  int read_key_caesar (char *filename)
  {
    int key;
    FILE *handler = fopen(filename, "r");
    fscanf(handler,"%d",&key);
    fclose(handler);
    return key;
  }

  char read_helper_caesar (char *filename)
  {
    char helper;
    FILE *handler = fopen(filename,"r");
    fscanf(handler, "%c", &helper);
    fclose(handler);

    return helper;
  }

void save_file (char *filename, char *string)
{
  FILE *handler = fopen(filename, "w");
  fprintf(handler, "%s", string);
  fclose(handler);
}

char* caesar_encryption (char *string, int key)
{
  if (key > 25 || key < 1) {
    printf("ERROR! INCORRECT KEY!");
    exit(1);
  }

  char *result = calloc(strlen(string+1), sizeof(char));

  for (int i = 0; i < strlen(string); i++) {
    if(isalpha(string[i])) {
      if (isupper(string[i])) {
        result[i] += (char)((string[i] + key - 65) % M + 65);
      }
      else if (islower(string[i])) {
        result[i] += (char)((string[i] + key - 97) % M + 97);
      }
    }
    else {
        result[i] += string[i];
      }
  }

  /*

  char tmp;
  
  for(int i = 0; string[i] != '\0'; i++) {
    tmp = string[i];

    if(tmp >= 'a' && tmp <= 'z') {
      tmp += key;

      if(tmp > 'z') {
        tmp = tmp - 'z' + 'a' - 1;
      }

      string[i] = tmp;
    }

    else if (tmp >= 'A' && tmp <= 'Z') {
      tmp += key;
      
      if (tmp > 'Z') {
        tmp = tmp - 'Z' + 'A' - 1;
      }

      string[i] = tmp;
    }
  }
  */

  return result;
}

char *caesar_decryption (char *string, int key)
{
  if (key > 25 || key < 1) {
    printf("ERROR! INCORRECT KEY!");
    exit(1);
  }
  char *result = calloc(strlen(string+1), sizeof(char));

  for (int i = 0; i < strlen(string); i++) {
    if(isalpha(string[i])) {
      if (isupper(string[i])) {
        result[i] += (char)((string[i] - key + 65) % M + 65);
      }
      if (islower(string[i])) {
        result[i] += (char)(mod((string[i] - key - 97), 26) + 97);
       // printf("%d %d\n",string[i], result[i]);
      }
    }
    else {
        result[i] += string[i];
      }
  }

  
  
  /*

  char tmp;
  
  for(int i = 0; string[i] != '\0'; i++) {
    tmp = string[i];

    if(tmp >= 'a' && tmp <= 'z') {
      tmp -= key;

      if(tmp < 'a') {
        tmp = tmp + 'z' - 'a' + 1;
      }

      string[i] = tmp;
    }

    else if (tmp >= 'A' && tmp <= 'Z') {
      tmp -= key;
      
      if (tmp < 'A') {
        tmp = tmp + 'Z' - 'A' + 1;
      }

      string[i] = tmp;
    }
  }
*/
  return result;
}

void crack_caesar_brute (char *string)
{
  int key_count = 25;
  int key = 1;
  int i;

  FILE *handler = fopen("decrypt.txt", "w");
  for (i = 0; i < key_count; i++) {
    fprintf(handler,"%s\n", caesar_decryption(string,key));
    key++;
  }
  
  fclose(handler);
}

 void crack_caesar_helper (char helper, char *string)
  {
    int key = abs(helper - string[0]);
    char *decrypted = caesar_decryption(string,key);
    FILE *handler = fopen ("key-new.txt","w");
    fprintf(handler,"%d",key);
    fclose(handler);

    handler = fopen("decrypt.txt", "w");
    fprintf(handler,"%s",decrypted);
    fclose(handler);
  }

  void read_affine_key(char *filename, int *a, int *b)
  {
    FILE *handler = fopen("key.txt","r");
    fscanf(handler,"%d %d",b,a);
    fclose(handler);
  }

  char *affine_encryption(char *string, int a, int b)
  {
     if(gcd(a,b) != 1) {
      printf("ERROR! NWD(%d, %d) == %d != 1!\n",a,b,gcd(a,b));
      exit(1);
    }

    char *result = calloc(strlen(string+1), sizeof(char));
    int i;
    for (i = 0; i < strlen(string); i++) {
      if (isalpha(string[i]) != 0) {
        if(islower(string[i]))
          result[i] = result[i] + (char) ((((a * (string[i] - 'a') ) + b) % M) + 'a');
        if(isupper(string[i]))
          result[i] = result[i] + (char) ((((a * (string[i] - 'A') ) + b) % M) + 'A');
      }
        
      else
        result[i] += string[i];
        }
        return result;
  }

  

  char *affine_decryption (char *string, int a, int b)
  {
    char *result = calloc(strlen(string+1), sizeof(char));
    int i;
    int gcd = 0;
    int a_inverted = 0;

    for (i = 0; i < M; i++) {
      gcd = (a * i) % M;

      if(gcd == 1)
        a_inverted = i;
    }

    if(a_inverted == 0) {
      printf("ERROR! NO GREATEST COMMON DIVISOR = 1!\n");
      exit(1);
    }

    for (i = 0; i < strlen(string); i++) {
      if (isalpha(string[i])) {
        if(islower(string[i]))
          result[i] = result[i] + (char) (a_inverted * (M + string[i] - 'a' - b) % M + 'a');
        if(isupper(string[i]))
          result[i] = result[i] + (char) (a_inverted * (M + string[i] - 'A' - b) % M + 'A');
      }
        
      else
        result[i] += string[i];
        }
        return result;


  }

  void affine_crack_brute (char *string)
  {
    int i,j;
    int k = 0;

    int *table = gcd_table();

    FILE *handler = fopen("decrypt.txt", "w");
    for (i = 0; i < M_GCD_COUNT; i++) {
      for (j = 0; j < M; j++) {
        fprintf(handler, "%s\n", affine_decryption(string, table[i], j));
        k++;
      }
    }
  }

  char *read_helper_affini (char *filename)
  {
    char *helper = calloc(2, sizeof(char));
    FILE *handler = fopen(filename,"r");
    fscanf(handler, "%*s %s", helper);
    fclose(handler);

    return helper;
  }

  void affine_crack_helper (char *helper, char *string)
  {
    int y_1,y_2,x_1,x_2;
    int a,b;

    if (islower(helper[0])) {
      x_1 = helper[0] - 'a';
    }
    else {
      x_1 = helper[0] - 'A';
    }
    if (islower(helper[1])) {
      x_2 = helper[1] - 'a';
    }
    else {
      x_2 = helper[1] - 'A';
    }
    if (islower(string[0])) {
      y_1 = string[0] - 'a';
    }
    else {
      y_1 = string[0] - 'A';
    }
    if (islower(string[1])) {
      y_2 = string[1] - 'a';
    }
    else {
      y_2 = string[1] - 'A';
    }
    
    a = ((mod(y_1-y_2, M) * modInverse(mod(x_1-x_2,M))) % M);
    b = mod(y_1 - a*x_1, M);
    //printf("a = %d b = %d\n",a,b);

    FILE *handler = fopen ("key-new.txt","w");
    fprintf(handler,"%d %d", b, a);
    fclose(handler);

    handler = fopen("decrypt.txt", "w");
    char *decrypted = affine_decryption(string,a,b);
    fprintf(handler,"%s",decrypted);
    fclose(handler);
    free(decrypted);

  }


  int main (int argc, char **argv)
  {
    
    if(argc < 3) {
      printf("ERROR! NOT ENOUGH ARGUMENTS!\n");
      exit(1);
    }
    
    char *plain_text = read_file("plain.txt");
    int a,b;
  /*
    read_affine_key("key.txt",&a,&b);
    char *affine_encrypted_text = affine_encryption(plain_text,a,b);
    //printf("%s\n",affine_encrypted_text);
    save_file("crypto.txt",affine_encrypted_text);
    char *affine_decrypted_text = affine_decryption(affine_encrypted_text,a,b);
    //printf("%s\n",affine_decrypted_text);
    //affine_crack_brute(affine_encrypted_text);
    char *helper = read_helper_affini("extra.txt");
    //printf("%s\n",helper);
    affine_crack_helper(helper,affine_encrypted_text);

*/



    
    if (strcmp(argv[1],"-c") == 0 && strcmp(argv[2],"-e") == 0) {
      if(access("key.txt", F_OK ) == -1 ) {
        printf("ERROR! KEY FILE DOES NOT EXISTS!\n");
        exit(1);
      }
      int key = read_key_caesar("key.txt");
      char *caesar_encrypted_text = caesar_encryption(plain_text,key);
      save_file("crypto.txt",caesar_encrypted_text);
      free(caesar_encrypted_text);
    }

    else if (strcmp(argv[1],"-c") == 0 && strcmp(argv[2],"-d") == 0) {
      if(access("crypto.txt", F_OK ) == -1 ) {
        printf("ERROR! FILE TO DECRYPT DOES NOT EXISTS!\n");
        exit(1);
      }

      int key = read_key_caesar("key.txt");
      char *caesar_encrypted_text = read_file("crypto.txt");
      char *caesar_decrypted_text = caesar_decryption(caesar_encrypted_text, key);
      save_file("decrypt.txt",caesar_decrypted_text);
      free(caesar_decrypted_text);
      free(caesar_encrypted_text);
    }

    else if (strcmp(argv[1],"-c") == 0 && strcmp(argv[2],"-j") == 0) {
      if(access("crypto.txt", F_OK ) == -1 || access("extra.txt", F_OK == -1)) {
        printf("ERROR! FILE TO CRACK OR EXTRA DOES NOT EXISTS!\n");
        exit(1);
      }

      char *caesar_encrypted_text = read_file("crypto.txt");
      char helper = read_helper_caesar("extra.txt");
      crack_caesar_helper(helper, caesar_encrypted_text); 
      free(caesar_encrypted_text);
    }

    else if (strcmp(argv[1],"-c") == 0 && strcmp(argv[2],"-k") == 0) {
      if(access("crypto.txt", F_OK ) == -1 ) {
        printf("ERROR! FILE TO CRACK DOES NOT EXISTS!\n");
        exit(1);
      }

      char *caesar_encrypted_text = read_file("crypto.txt");
      crack_caesar_brute(caesar_encrypted_text);
      free(caesar_encrypted_text);
    }





    else if (strcmp(argv[1],"-a") == 0 && strcmp(argv[2],"-e") == 0) {
      if(access("key.txt", F_OK ) == -1 ) {
        printf("ERROR! KEY FILE DOES NOT EXISTS!\n");
        exit(1);
      }
      read_affine_key("key.txt",&a,&b);
      char *affine_encrypted_text = affine_encryption(plain_text,a,b);
      save_file("crypto.txt",affine_encrypted_text);
      free(affine_encrypted_text);
    }

    else if (strcmp(argv[1],"-a") == 0 && strcmp(argv[2],"-d") == 0) {
      if(access("crypto.txt", F_OK ) == -1 ) {
        printf("ERROR! FILE TO DECRYPT DOES NOT EXISTS!\n");
        exit(1);
      }

      read_affine_key("key.txt",&a,&b);
      char *affine_encrypted_text = read_file("crypto.txt");
      char *affine_decrypted_text = affine_decryption(affine_encrypted_text, a, b);
      save_file("decrypt.txt",affine_decrypted_text);
      free(affine_decrypted_text);
      free(affine_encrypted_text);
    }

    else if (strcmp(argv[1],"-a") == 0 && strcmp(argv[2],"-j") == 0) {
      if(access("crypto.txt", F_OK ) == -1 || access("extra.txt", F_OK == -1)) {
        printf("ERROR! FILE TO CRACK OR EXTRA DOES NOT EXISTS!\n");
        exit(1);
      }

      char *affine_encrypted_text = read_file("crypto.txt");
      char *helper = read_helper_affini("extra.txt");
      affine_crack_helper(helper, affine_encrypted_text); 
      free(affine_encrypted_text);
    }

    else if (strcmp(argv[1],"-a") == 0 && strcmp(argv[2],"-k") == 0) {
      if(access("crypto.txt", F_OK ) == -1 ) {
        printf("ERROR! FILE TO CRACK DOES NOT EXISTS!\n");
        exit(1);
      }

      char *affine_encrypted_text = read_file("crypto.txt");
      affine_crack_brute(affine_encrypted_text);
      free(affine_encrypted_text);
    }

    else {
      free(plain_text);
      printf("ERROR! WRONG ARGUMENTS!\n");
      printf("USE\n-c -e / -c -d / -c -j / -c -k\nOR\n-a -e / -a -d / -a -j / -a -k\n");
      exit(1);

    }

    free(plain_text);
    return 0;
  }


