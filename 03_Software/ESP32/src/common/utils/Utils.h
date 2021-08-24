#ifndef _UTILS_H_
#define _UTILS_H_

enum {
    FALSE = 0,
    TRUE = 1
};

#define MERGE_BYTES_TO_WORD(byte_H, byte_L)  ( (((unsigned short)(byte_H)) << 8) | ((unsigned short)(byte_L)) )

#endif // _UTILS_H_



