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
#include <stdbool.h>

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

struct mmvp_partition_descriptor {
        uint32_t address;
        uint32_t size;
        void *data;
};

struct mmvp_partition_header {
        uint32_t size;
        uint32_t version;
        uint32_t counter;
        uint32_t crc;
};

struct mmvp_partition {
        const struct mmvp_partition_descriptor *desc;
        struct mmvp_partition *next;

        struct mmvp_partition_header device_header;
        struct mmvp_partition_header local_header;

        int mirror_index;
        bool write_enable;
};

typedef int (*mmvp_read_write_handler)(uint32_t address, uint8_t *data, uint32_t size);

struct mmvp_device_descriptor {
        mmvp_read_write_handler read;
        mmvp_read_write_handler write;

        uint32_t total_size;
        uint32_t page_size;

        int wear_leveling_factor;
};

struct mmvp_controller {
        const struct mmvp_device_descriptor *device;
        struct mmvp_partition *first;
};

mmvp_error mmvp_init(struct mmvp_controller *self, const struct mmvp_device_descriptor *device);
mmvp_error mmvp_register_partition(struct mmvp_controller *self, struct mmvp_partition *partition, const struct mmvp_partition_descriptor *desc);
mmvp_error mmvp_unregister_partition(struct mmvp_controller *self, struct mmvp_partition *partition);
mmvp_error mmvp_start(struct mmvp_controller *self);
mmvp_error mmvp_confirm_write(struct mmvp_controller *self);
mmvp_error mmvp_service(struct mmvp_controller *self);

#ifdef __cplusplus
}
#endif

#endif /* MMVP_H */
