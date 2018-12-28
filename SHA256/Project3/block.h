//
//  block.h
//  Projec3
//
//  Created by Hangeul Bae on 6/6/18.
//  Copyright © 2018 Hangeul Bae. All rights reserved.
//

#ifndef block_h
#define block_h
void makeHeader(unsigned int* header, unsigned int version, unsigned int* prevBlockHash, unsigned int* contentHash, unsigned int timeStamp
                , unsigned int level, unsigned int nans, unsigned int* comment);
#endif /* block_h */
