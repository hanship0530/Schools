//
//  block.c
//  Projec3
//
//  Created by Hangeul Bae on 6/6/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//

#include "block.h"
void makeHeader(unsigned int* header, unsigned int version, unsigned int* prevBlockHash, unsigned int* contentHash, unsigned int timeStamp
                , unsigned int level, unsigned int nans, unsigned int* comment){
    int i=0;
    unsigned int prevHashCopied[8];
    unsigned int contentHashCopied[8];
    unsigned int commentCopied[5];
    memcpy(prevHashCopied,prevBlockHash,sizeof(prevBlockHash));
    memcpy(commentCopied,contentHash,sizeof(contentHash));
    memcpy(commentCopied,comment,sizeof(comment));
    //Version
    header[i++]=version;
    //Previous content's Hash
    for(;i<9;i++){
        header[i]=prevHashCopied[i-1];
    }
    //Content's Hash
    for(;i<17;i++){
        header[i]=contentHashCopied[i-9];
    }
    //TimeStamp
    header[i++]=timeStamp;
    //Class
    header[i++]=level;
    //Nans
    header[i++]=nans;
    //Coments
    for(;i<25;i++){
        header[i]=commentCopied[i-20];
    }
}
