//
//  block.c
//  Homework2
//
//  Created by Hangeul Bae on 5/10/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//

#include "block.h"

void makeBlock(unsigned int blockSize, unsigned int* blockHeader, unsigned int* plainText, unsigned int* block){
    int i=0;
    block[i++]=blockSize*4;
//    unsigned char temp[4];
//    temp[0] = blockSize & 0xFF;
//    temp[1] = (blockSize>>8) & 0xFF;
//    temp[2] = (blockSize>>16) & 0xFF;
//    temp[3] = (blockSize>>24) & 0xFF;
//    unsigned int b = (temp[0]<<24) | (temp[1]<<16) | (temp[2]<<8) | (temp[3]); big endian
    printf("BlockSize hexcode: %x\n",blockSize);
    for(;i<25;i++){
        block[i]=blockHeader[i];
    }
    for(;i<(int)blockSize;i++){
        block[i]=(plainText[i-22]<<24) | (plainText[i-23]<<16) | (plainText[i-24]<<8) | (plainText[i-25]);
    }
}

void makeHeader(unsigned int* header, unsigned int* contentsHash, unsigned int timeStamp) {
    int i=0;
    //Version
    header[i++]=0x00000000;
    //Previous content's Hash
    for(; i<9; i++){
        header[i]=0x00000000;
    }
    //Content's Hash
    for(;i<17;i++){
        header[i]=contentsHash[i-9];
    }
    //TimeStamp
    printf("TimeStamp hexcode: %x\n",timeStamp);
    header[i++]=timeStamp;
    //Class
    header[i++]=0xFFFFFFFF;
    //Nans
    header[i++]=0x00000000;
    //Coments
    for (; i<25; i++) {
        header[i]=0x00000000;
    }
}

