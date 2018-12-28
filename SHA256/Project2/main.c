//
//  main.c
//  Homework2
//
//  Created by Hangeul Bae on 5/6/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "block.h"
void initiate(unsigned int* initialWord) {
    initialWord[0] = 0x6a09e667;
    initialWord[1] = 0xbb67ae85;
    initialWord[2] = 0x3c6ef372;
    initialWord[3] = 0xa54ff53a;
    initialWord[4] = 0x510e527f;
    initialWord[5] = 0x9b05688c;
    initialWord[6] = 0x1f83d9ab;
    initialWord[7] = 0x5be0cd19;
    
}
int main(int argc, const char * argv[]) {
    unsigned int initialWord[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    unsigned int blockHeader[25];
    unsigned int result[8];
    int num=0;
    FILE *fp;
    char* filename = "/Users/Hangeulbae/Desktop/Univ/Cryptography and Network Security/project2/input.txt";
    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Could not open file %s",filename);
        return 1;
    }
    char ch;
    while(1){
        ch = fgetc(fp);
        if( ch == EOF){
            break;
        }
        num++;
    }
    fclose(fp);
    fp = fopen(filename, "r");
    unsigned char plainText[num-3];
    int num2=0;
    while(1){
        ch = fgetc(fp);
        if( ch == EOF | ch == '\n' | ch == ' ' | ch == NULL){
            break;
        }
        if(num2<num-2){
            plainText[num2++] = ch;
        }
    }
    fclose(fp);
    //unsigned char plainText[] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // sha message
    unsigned int blockSize = num2+4+100;
    unsigned int block[blockSize];
    unsigned int timeStamp = (unsigned int)time(NULL);
    proceedSha256(num2, plainText, initialWord);
    makeHeader(blockHeader, initialWord, timeStamp);
    makeBlock(blockSize/4, blockHeader, plainText, block);
    initiate(initialWord);
    proceedSha256(100,blockHeader,initialWord);
    for(int i=0;i<8;i++){
        result[i] = initialWord[i];
    }
    initiate(initialWord);
    proceedSha256(8,result,initialWord);
    printf("Result:        \n");
    for(int i=0; i<8; i++) {
        printf("%x",initialWord[i]);
    }
    printf("\n");
    return 0;
}

