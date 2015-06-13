/******************************************************************************
 * @file     hgpu_md5.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           MD5 submodule for HGPU package
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013-2015 Vadim Demchik
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/

#include "../include/hgpu_md5.h"

            unsigned char HGPU_md5_buffer[HGPU_MD5_BLOCKSIZE];
            unsigned int  HGPU_md5_t[HGPU_MD5_BLOCKSIZE];
            unsigned int  HGPU_md5_s[HGPU_MD5_BLOCKSIZE];
            unsigned int  HGPU_md5_state[4];
            unsigned int  HGPU_md5_bytes[2];
           unsigned char  HGPU_md5_result[16];
                    bool  HGPU_md5_finalized;

inline unsigned int
HGPU_md5_rol(unsigned int x, int s){
    return ((x << s) | (x >> (32-s)));
}

void
HGPU_md5_init(void){
    HGPU_md5_finalized = false;
    HGPU_md5_bytes[0] = 0;
    HGPU_md5_bytes[1] = 0;

    HGPU_md5_state[0] = 0x67452301;  HGPU_md5_state[1] = 0xefcdab89;
    HGPU_md5_state[2] = 0x98badcfe;  HGPU_md5_state[3] = 0x10325476;

    HGPU_md5_s[ 0] = 7;     HGPU_md5_s[ 1] = 12;    HGPU_md5_s[ 2] = 17;    HGPU_md5_s[ 3] = 22;
    HGPU_md5_s[16] = 5;     HGPU_md5_s[17] =  9;    HGPU_md5_s[18] = 14;    HGPU_md5_s[19] = 20;
    HGPU_md5_s[32] = 4;     HGPU_md5_s[33] = 11;    HGPU_md5_s[34] = 16;    HGPU_md5_s[35] = 23;
    HGPU_md5_s[48] = 6;     HGPU_md5_s[49] = 10;    HGPU_md5_s[50] = 15;    HGPU_md5_s[51] = 21;

    for (int i=0; i<4; i++)
        for (int j=1; j<4; j++) {
            HGPU_md5_s[16 * i + 4 * j + 0] = HGPU_md5_s[16 * i    ];
            HGPU_md5_s[16 * i + 4 * j + 1] = HGPU_md5_s[16 * i + 1];
            HGPU_md5_s[16 * i + 4 * j + 2] = HGPU_md5_s[16 * i + 2];
            HGPU_md5_s[16 * i + 4 * j + 3] = HGPU_md5_s[16 * i + 3];
        }

    for (int i=0; i<64; i++)
        HGPU_md5_t[i] = (unsigned int) floor(abs(sin((double) i + 1)) * pow(2.0,32));

    memset(HGPU_md5_buffer, 0, sizeof(HGPU_md5_buffer));
}

void
HGPU_md5_finalize(void){
    unsigned char HGPU_md5_padding[64];
    HGPU_md5_padding[0] = 128;
    for (int i=1; i<64; i++) HGPU_md5_padding[i]=0;

    if (!HGPU_md5_finalized) {
        unsigned char bits[8];
        HGPU_md5_setword(HGPU_md5_bytes, bits, 8);

    unsigned int index = HGPU_md5_bytes[0] / 8 % 64;
    unsigned int padLen = (index < 56) ? (56 - index) : (120 - index);
    HGPU_md5_update(HGPU_md5_padding, padLen);
    HGPU_md5_update(bits, 8);
    HGPU_md5_setword(HGPU_md5_state, HGPU_md5_result, 16);

    memset(HGPU_md5_buffer, 0, sizeof(HGPU_md5_buffer));
    memset(HGPU_md5_bytes,  0, sizeof(HGPU_md5_bytes));

    HGPU_md5_finalized=true;
  }
}

void
HGPU_md5_getword(const unsigned char* buffer, unsigned int* x, unsigned int len){
    unsigned int i = 0;
    for (unsigned int j = 0; j < len; j += 4) {
        x[i] = ((unsigned int) buffer[j    ]) |
              (((unsigned int) buffer[j + 1]) <<  8) |
              (((unsigned int) buffer[j + 2]) << 16) |
              (((unsigned int) buffer[j + 3]) << 24);
        i++;
    }
}

void
HGPU_md5_setword(const unsigned int* x, unsigned char* buffer, unsigned int len){
    unsigned int i = 0;
    for (unsigned int j = 0; j < len; j += 4) {
        buffer[j    ] = (x[i]      ) & 0xff;
        buffer[j + 1] = (x[i] >>  8) & 0xff;
        buffer[j + 2] = (x[i] >> 16) & 0xff;
        buffer[j + 3] = (x[i] >> 24) & 0xff;
        i++;
    }
}

void
HGPU_md5_step(const unsigned char* MD5_block){
    unsigned int a = HGPU_md5_state[0];
    unsigned int b = HGPU_md5_state[1];
    unsigned int c = HGPU_md5_state[2];
    unsigned int d = HGPU_md5_state[3];

    unsigned int f,g,temp;
    unsigned int w[HGPU_MD5_BLOCKSIZE];

    HGPU_md5_getword(MD5_block, w, HGPU_MD5_BLOCKSIZE);

    for(int i=0; i<HGPU_MD5_BLOCKSIZE; i++){
        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if ((16 <= i) && (i < 32)) {
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        } else if ((32 <= i) && (i < 48)) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else if ((48 <= i) && (i < 64)) {
            f = c ^ ((~d) | b);
            g = (7 * i) % 16;
        } else {
            f = 0;
            g = 0;
        }
        temp = d;
        d = c;
        c = b;
        b = b + HGPU_md5_rol((a + f + HGPU_md5_t[i] + w[g]) , HGPU_md5_s[i]);
        a = temp;
    }

    HGPU_md5_state[0] += a;
    HGPU_md5_state[1] += b;
    HGPU_md5_state[2] += c;
    HGPU_md5_state[3] += d;

    memset(w, 0, sizeof w);
}

void
HGPU_md5_update(const unsigned char* input, unsigned int len){
    unsigned int index = HGPU_md5_bytes[0] / 8 % HGPU_MD5_BLOCKSIZE;

    if ((HGPU_md5_bytes[0] += (len << 3)) < (len << 3)) HGPU_md5_bytes[1]++;
    HGPU_md5_bytes[1] += (len >> 29);

    unsigned int firstpart = 64 - index;
    unsigned int i;

    if (len >= firstpart) {
        memcpy(&HGPU_md5_buffer[index], input, firstpart);
        HGPU_md5_step(HGPU_md5_buffer);

        for (i = firstpart; i + HGPU_MD5_BLOCKSIZE <= len; i += HGPU_MD5_BLOCKSIZE) HGPU_md5_step(&input[i]);
        index = 0;
    } else
        i = 0;

    memcpy(&HGPU_md5_buffer[index], &input[i], len-i);
}

void
HGPU_md5_update(const char* input, unsigned int len){
    HGPU_md5_update((const unsigned char*) input, len);
}

char*
HGPU_md5_getresult(void){
    char* buf = (char*) calloc(34,sizeof(char));
    if (HGPU_md5_finalized)
        for (int i=0; i<16; i++)
            sprintf_s(buf+i*2, sizeof(buf), "%02x", HGPU_md5_result[i]);
    buf[32]=0;
  return buf;
}

char*
HGPU_md5(const char* str){
    unsigned int len = (unsigned int) strlen(str);

    HGPU_md5_init();
    HGPU_md5_update(str, len);
    HGPU_md5_finalize();

    return HGPU_md5_getresult();
}
