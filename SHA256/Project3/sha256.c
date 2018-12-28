//
//  sha256.c
//  Projec3
//
//  Created by Hangeul Bae on 6/6/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//

#include "sha256.h"

void intToChar(unsigned int* digested, unsigned char* hashed) { //trans unsigned integer to char array
    //little endian
    /*byte[0] = num & 0xFF;
     byte[1] = (num>>8) & 0xFF;
     byte[2] = (num>>16) & 0xFF;
     byte[3] = (num>>24) & 0xFF;*/
    //big endian
    for(int i=0;i<32;i=i+4){
        hashed[i] =(digested[i/4]>>24) & 0xFF;
        hashed[i+1] = (digested[i/4]>>16) & 0xFF;
        hashed[i+2] = (digested[i/4]>>8) & 0xFF;
        hashed[i+3] = digested[i/4] & 0xFF;
    }
}
void charToInt(unsigned int block, unsigned char* word) {
    // Little edian
    block = (word[3]<<24) | (word[2]<<16) | (word[1]<<8) | (word[0]);
    
}
void longToChar(unsigned long num, unsigned char* byte) { // Little edian, making last padding
    // little endian
    /*byte[0] = num & 0xFF;
     byte[1] = (num>>8) & 0xFF;
     byte[2] = (num>>16) & 0xFF;
     byte[3] = (num>>24) & 0xFF;
     byte[4] = (num>>32) & 0xFF;
     byte[5] = (num>>40) & 0xFF;
     byte[6] = (num>>48) & 0xFF;
     byte[7] = (num>>56) & 0xFF;*/
    // big endian
    byte[0] = (num>>56) & 0xFF;
    byte[1] = (num>>48) & 0xFF;
    byte[2] = (num>>40) & 0xFF;
    byte[3] = (num>>32) & 0xFF;
    byte[4] = (num>>24) & 0xFF;
    byte[5] = (num>>16) & 0xFF;
    byte[6] = (num>>8) & 0xFF;
    byte[7] = num & 0xFF;
}
void makePadding(unsigned int padByteSize, unsigned char* padArr) {
    padArr[0] = 0x80; //0b10000000
    if(padByteSize >= 2){
        for(unsigned int i=1; i<padByteSize; i++) {
            padArr[i] = 0x00;
        }
    }
}
void roundFunc(unsigned char* word, unsigned int startPoint, unsigned int* initialWord) {
    unsigned int i;
    unsigned int block[64];
    
    for(i=0; i<16; ++i, startPoint+=4){ // intial words
        block[i] = (word[startPoint]<<24) | (word[startPoint+1]<<16) | (word[startPoint+2]<<8) | (word[startPoint+3]);
        //block[i] = (word[startPoint+3]<<24) | (word[startPoint+2]<<16) | (word[startPoint+1]<<8) | (word[startPoint]); little endian
    }
    for( ; i<64; ++i) { // i start from 16, sha 256's rounds are 64 round
        block[i] = Theta0(block[i-15]) + block[i-16] + Theta1(block[i-2]) + block[i-7];
    }
    //SHA-256 Compression
    unsigned int t, temp1=0, temp2=0, a, b, c, d, e, f, g, h;
    a = initialWord[0];
    b = initialWord[1];
    c = initialWord[2];
    d = initialWord[3];
    e = initialWord[4];
    f = initialWord[5];
    g = initialWord[6];
    h = initialWord[7];
    
    // Round
    for(t=0; t<64; t++) {
        temp1 = Sigma1(e) + h + Ch(e, f, g) + constK[t] + block[t];
        temp2 = Sigma0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    initialWord[0] += a;
    initialWord[1] += b;
    initialWord[2] += c;
    initialWord[3] += d;
    initialWord[4] += e;
    initialWord[5] += f;
    initialWord[6] += g;
    initialWord[7] += h;
}
void doSha256(int contentsByteSize, unsigned char* plainText, unsigned int* hashed) {
    unsigned int initialWord[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    //Variables!
    unsigned int pltByteSize = contentsByteSize;
    int padBitSize = (-pltByteSize*8-64) % 512;
    if(padBitSize < 0) padBitSize += 512;
    if(padBitSize == 0) padBitSize += 512;
    unsigned int padByteSize = padBitSize/8;
    unsigned long pltLongSize = pltByteSize*8; // for length of original message padding
    unsigned int msgByteSize = pltByteSize + padByteSize + 8;
    unsigned int index = 0,i=0,j=0;
    //Message Concat or Padding
    unsigned char padding[padByteSize]; // sha message
    unsigned char msgLength[8]; // sha message
    if(padByteSize != 0) {
        makePadding(padByteSize, padding);
    }
    longToChar(pltLongSize, msgLength);
    unsigned char shaBlock[msgByteSize];
    
    //Merge!
    for(;index<pltByteSize;index++){
        shaBlock[index] = plainText[index];
    }
    
    for(;index<pltByteSize+padByteSize;index++){
        shaBlock[index] = padding[i++];
    }
    
    for(;index<msgByteSize;index++){
        shaBlock[index] = msgLength[j++];
    }
    
    //Round Function!
    for(unsigned int i=0; i<msgByteSize; i=i+64){
        roundFunc(shaBlock, i, initialWord);
    }
    for(int i=0;i<8;i++){
        hashed[i]=initialWord[i];
    }
}