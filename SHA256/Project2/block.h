//
//  block.h
//  Homework2
//
//  Created by Hangeul Bae on 5/10/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//

#ifndef block_h
#define block_h
void makeBlock(unsigned int blockSize, unsigned int* blockHeader, unsigned int* plainText, unsigned int* block);
void makeHeader(unsigned int* header, unsigned int* contentsHash, unsigned int timeStamp);
#endif /* block_h */
