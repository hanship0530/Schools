//
//  main.c
//  HomeWork2
//
//  Created by Hangeul Bae on 3/28/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//  when bitwise Little endian to Big endian

#include <stdio.h>
#include <string.h>

#define SIZE(x) (sizeof(x)/sizeof(x[0]))-1
#define RR(x,n) (x>>n | x<<(32-n))
#define SS(x, n) (x >> n)
#define Sigma0(x) (RR(x,  2) ^ RR(x, 13) ^ RR(x, 22))
#define Sigma1(x) (RR(x,  6) ^ RR(x, 11) ^ RR(x, 25))
#define Theta0(x) (RR(x,  7) ^ RR(x, 18) ^ SS(x,  3))
#define Theta1(x) (RR(x, 17) ^ RR(x, 19) ^ SS(x, 10))
#define Ch(x,y,z) ((x & y) ^ ((~x) & z))
#define Maj(x,y,z) ((x & y) ^ (x & z) ^ (y & z))


unsigned int constK[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };


void intTochar(unsigned int num, unsigned char* byte) { //trans unsigned integer to char array
    //little endian
    /*byte[0] = num & 0xFF;
    byte[1] = (num>>8) & 0xFF;
    byte[2] = (num>>16) & 0xFF;
    byte[3] = (num>>24) & 0xFF;*/
    //big endian
    byte[0] = (num>>24) & 0xFF;
    byte[1] = (num>>16) & 0xFF;
    byte[2] = (num>>8) & 0xFF;
    byte[3] = num & 0xFF;
}
void charToInt(unsigned int block, unsigned char word[4]) {
    // Little edian
    block = (word[3]<<24) | (word[2]<<16) | (word[1]<<8) | (word[0]);
    
}
void longTochar(unsigned long num, unsigned char byte[8]) { // Little edian, making last padding
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
void sha_256(unsigned char* word, unsigned int startPoint, unsigned int initialWord[8]) {
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
void sha256Print(unsigned int initialWord[8]) {
    printf("Result= ");
   for(int i=0; i<8; i++) {
        printf("%x",initialWord[i]);
    }
    printf("\n");
}
int main(int argc, const char * argv[]) {
    FILE *fp;
    int num=0;
    char* filename = "/Users/Hangeulbae/Desktop/Univ/Cryptocraphy/SHA256/input.txt";
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
//        if(ch == '\n'){
//            num++;
//        }
        num++;
    }
    fclose(fp);
    fp = fopen(filename, "r");
    unsigned char plainText[num];
    int num2=0;
    while(1){
        ch = fgetc(fp);
        if( ch == EOF){
            break;
        }
//        if(ch == '\n'){
//            plainText[num2++] = '\n';
//        }
        plainText[num2++] = ch;
    }
    fclose(fp);
    //unsigned char plainText[] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n"; // sha message
    //unsigned char plainText[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    
    unsigned int initialWord[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    
    unsigned int pltByteSize = num-1;
    int padBitSize = (-pltByteSize*8-64) % 512;
    if(padBitSize < 0) padBitSize += 512;
    if(padBitSize == 0) padBitSize += 512;
    unsigned int padByteSize = padBitSize/8;
    unsigned long pltLongSize = pltByteSize*8; // for length of original message padding
    unsigned int messageByteSize = pltByteSize + padByteSize + 8;
    unsigned int index = 0,i=0,j=0;
    
    
    unsigned char padArr[padByteSize]; // sha message
    unsigned char lastPadArr[8]; // sha message
    if(padByteSize != 0) {
        makePadding(padByteSize, padArr);
    }
    longTochar(pltLongSize, lastPadArr);
    unsigned char mergedM[messageByteSize];
    // Merge!
    for(;index<pltByteSize;index++){
        mergedM[index] = plainText[index];
    }

    for(;index<pltByteSize+padByteSize;index++){
        mergedM[index] = padArr[i++];
    }

    for(;index<messageByteSize;index++){
        mergedM[index] = lastPadArr[j++];
    }
    //SHA-256
    for(unsigned int i=0; i<messageByteSize; i=i+64){
        sha_256(mergedM, i, initialWord);
    }
    sha256Print(initialWord);

    return 0;
}
