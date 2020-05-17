/*
MIT License

Copyright (c) 2020 Marcin Borowicz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MMVP_ERROR_H
#define MMVP_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
        MMVP_ERROR_WRONG_FACTOR = -8,
        MMVP_ERROR_PARTITION_EXIST = -7,
        MMVP_ERROR_PARTITION_OVERLAP = -6,
        MMVP_ERROR_OUT_OF_MEMORY = -5,
        MMVP_ERROR_NULL_POINTER = -4,
        MMVP_ERROR_WRONG_SIZE = -3,
        MMVP_ERROR_ADDRESS_ALIGNMENT = -2,
        MMVP_ERROR_PARTITION_NOT_EXIST = -1,
        MMVP_ERROR_OK = 0
} mmvp_error;

#ifdef __cplusplus
}
#endif

#endif /* MMVP_ERROR_H */
