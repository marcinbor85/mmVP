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

#ifndef MMVP_H
#define MMVP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {
        MMVP_ERROR_PARTITION_NOT_EXIST = -2,
        MMVP_ERROR_GENERAL = -1,
        MMVP_ERROR_NO_ERROR = 0
} mmvp_error;

typedef enum {
        MMVP_PARTITION_TYPE_READONLY = 0,
        MMVP_PARTITION_TYPE_READWRITE,
} mmvp_partition_type;

struct mmvp_partition_descriptor {
        uint32_t address;
        uint32_t size;
        void *data;
};

struct mmvp_partition {
        const struct mmvp_partition_descriptor *desc;
        struct mmvp_partition *next;
        int mirror_index;
        uint32_t counter;
        uint32_t crc;
};

typedef int (*mmvp_read_data)(uint8_t *data, uint32_t size_to_read, uint32_t *read_size);
typedef int (*mmvp_write_data)(uint8_t *data, uint32_t size_to_write, uint32_t *write_size);

struct mmvp_device_descriptor {
        mmvp_read_data read;
        mmvp_write_data write;

        uint32_t total_size;
        uint32_t block_size;
};

struct mmvp_object {
        const struct mmvp_device_descriptor *device;
        struct mmvp_partition *first;
};

void mmvp_init(struct mmvp_object *self, const struct mmvp_device_descriptor *device);
void mmvp_register_partition(struct mmvp_object *self, struct mmvp_partition *partition, const struct mmvp_partition_descriptor *desc);
mmvp_error mmvp_unregister_partition(struct mmvp_object *self, struct mmvp_partition *partition);
void mmvp_start(struct mmvp_object *self);

#ifdef __cplusplus
}
#endif

#endif /* MMVP_H */
