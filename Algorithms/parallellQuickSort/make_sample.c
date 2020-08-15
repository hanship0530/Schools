#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]) {
   FILE *fp;
   int  i;
   //Inializes random number generator
   srand(time(0));
   char *p;
   //const int num = (int)strtol(argv[1],&p,10);
  const int num = 70000000;
   long *S = (long *)malloc(sizeof(long) * (long)num);
   for(i=0; i<num; i++) {
      S[i] = rand() % 2000000000 + 100;
   }
   fp = fopen("sample.7KM","w");
   for(i=0; i<num; i+=5) {
      fprintf(fp,"%ld %ld %ld %ld %ld\n",S[i],S[i+1],S[i+2],S[i+3],S[i+4]);
   }
   fclose(fp);
   free(S);
   return 0;
}