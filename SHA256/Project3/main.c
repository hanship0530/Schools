//
//  main.c
//  Projec3
//
//  Created by Hangeul Bae on 6/6/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "block.h"
//#include "sha256.h"
struct Block{
    unsigned int version;
    unsigned int header[25];
    unsigned int prevHeaderBlockHash[8];
    unsigned int contentHash[8];
    unsigned int timeStamp;
    unsigned int level;
    unsigned int nans;
    unsigned int comment[5];
    unsigned int headerHash[8];
};
void integersToChar(unsigned int integer[], unsigned char chars[], int charsLeng) { //trans unsigned integer to char array
    for(int i=0;i<charsLeng;i=i+4){
        chars[i] =(integer[i/4]>>24) & 0xFF;
        chars[i+1] = (integer[i/4]>>16) & 0xFF;
        chars[i+2] = (integer[i/4]>>8) & 0xFF;
        chars[i+3] = integer[i/4] & 0xFF;
    }
}
void charsToIntegers(unsigned int* integers, unsigned char* chars, int charsLeng) {
    // Little edian
    for(int i=0;i<charsLeng;i=i+4){
        integers[i/4]=(chars[i+3]<<24) | (chars[i+2]<<16) | (chars[i+1]<<8) | (chars[i]);
    }
}
void printInfo(unsigned int try,unsigned int version, unsigned int* prevBlockHash, unsigned int* contentHash, unsigned int timeStamp
               , unsigned int level, unsigned int nans, unsigned int* comment, unsigned int* headerHash){
    printf("Try: %u\n",try);
    printf("------Header Info------\n");
    printf("Version: %08x\n",version);
    printf("Previou Header Block Hash: ");
    for(int j=0;j<8;j++){
        printf("%08x",prevBlockHash[j]);
    }
    printf("\n");
    printf("Content Block Hash: ");
    for(int j=0;j<8;j++){
        printf("%08x",contentHash[j]);
    }
    printf("\n");
    printf("TimeStamp: %08x\n",timeStamp);
    printf("Level: %08x\n",level);
    printf("Nans: %08x\n",nans);
    printf("Comment: ");
    for(int j=0;j<5;j++){
        printf("%08x",comment[j]);
    }
    printf("\n");
    printf("Header Hash!: ");
    for(int j=0;j<8;j++){
        printf("%08x",headerHash[j]);
    }
    printf("\n\n");
    
}
int main(int argc, const char * argv[]) {
    int pltLength=0;
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
        pltLength++;
    }
    fclose(fp);
    fp = fopen(filename, "r");
    unsigned char plainText[pltLength-3];
    int pltLength_c=0;
    while(1){
        ch = fgetc(fp);
        if( ch == EOF | ch == '\n' | ch == ' ' | ch == NULL){
            break;
        }
        if(pltLength_c<pltLength-2){
            plainText[pltLength_c++] = ch;
        }
    }
    fclose(fp);
    time_t t = time(NULL);
    //printf("%08x",comment[2]); // print hexa with zero there is no omission
    unsigned int version=0;
    unsigned int prevBlockHash[8]={0};
    unsigned int contentHash[8];
    unsigned int timeStamp=(unsigned int)t;
    unsigned int level=0;
    unsigned int comment[5];
    
    struct Block prev;
    doSha256(pltLength,plainText,contentHash);
    unsigned char temp[32];
    integersToChar(contentHash, temp, 32);
    doSha256(32,temp,contentHash);
    unsigned char com[20]={"Hangeul Bae"};
    charsToIntegers(comment, com, 20);
    int index=4;
    int next=0;
    
    clock_t start, middle;
    
    start=clock();
    prev.version=version;
    memcpy(prev.prevHeaderBlockHash, prevBlockHash, sizeof(prevBlockHash));
    memcpy(prev.contentHash, contentHash, sizeof(contentHash));
    prev.timeStamp=timeStamp;
    prev.level=level;
    prev.nans=0;
    memcpy(prev.comment, comment, sizeof(comment));
    makeHeader(prev.header, prev.version, prev.prevHeaderBlockHash, prev.contentHash, prev.timeStamp, prev.level, prev.nans, prev.comment);
    doSha256(100,prev.header,prev.headerHash);
    doSha256(32,prev.headerHash,prev.headerHash);
    
    //printInfo(0,prev.version, prev.prevBlockHash, prev.contentHash, prev.timeStamp, prev.level, prev.nans, prev.comment, prev.headerHash);
    //for(int i=0;i<1000;i++){
    while(next<2) {
        struct Block curr;
        if((prev.headerHash[next]>>(32-index))==0 && next==0) {
            middle=clock();
            printf("%d's first bit zero: \n",index);
            printf("Taking time: %.3f\n",(float)(middle-start)/CLOCKS_PER_SEC);
            printInfo(prev.nans,prev.version, prev.prevHeaderBlockHash, prev.contentHash, prev.timeStamp, prev.level, prev.nans, prev.comment, prev.headerHash);
            index=index+4;
            if(index==36){
                index=4;
                next++;
            }
        }else if((((prev.headerHash[next])==0) && (prev.headerHash[next]>>(32-index))==0) && next==1) {
            middle=clock();
            printf("%d's first bit zero: \n",index+32);
            printf("Taking time: %.3f\n",(float)(middle-start)/CLOCKS_PER_SEC);
            printInfo(prev.nans,prev.version, prev.prevHeaderBlockHash, prev.contentHash, prev.timeStamp, prev.level, prev.nans, prev.comment, prev.headerHash);
            index=index+4;
            if(index==36){
                break;
            }
        }
        curr.version = prev.version;
        memcpy(curr.prevHeaderBlockHash, prev.headerHash, sizeof(prev.headerHash));
        memcpy(curr.contentHash, prev.contentHash, sizeof(prev.contentHash));
        curr.timeStamp = prev.timeStamp;
        curr.level = prev.level;
        curr.nans = prev.nans+1;
        memcpy(curr.comment, prev.comment, sizeof(prev.comment));
        makeHeader(curr.header, curr.version, curr.prevHeaderBlockHash, curr.contentHash, curr.timeStamp, curr.level, curr.nans, curr.comment);
        doSha256(100,curr.header,curr.headerHash);
        doSha256(32,curr.headerHash,curr.headerHash);
        //printInfo(curr.nans,curr.version, curr.prevBlockHash,curr.contentHash, curr.timeStamp, curr.level, curr.nans, curr.comment, curr.headerHash);
        prev=curr;
    }
}
