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

#ifndef MMVP_PARTITION_H
#define MMVP_PARTITION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "mmvp_controller.h"

#define MMVP_PARTITION_HEADER_SIZE                  ((uint32_t)(sizeof(struct mmvp_partition_header)))
#define MMVP_PARTITION_MEMORY_DEFAULT_BYTE_VALUE    (0x00)
#define MMVP_PARTITION_CRC_INIT_VALUE               (0xFFFFFFFFUL)

typedef void (*mmvp_partition_restore_default_handler)(void *data, uint32_t size);

struct mmvp_partition_descriptor {
        uint32_t version;
        uint32_t address;
        uint32_t size;
        void *data;

        mmvp_partition_restore_default_handler restore_default;
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

        struct mmvp_partition_header header;

        uint32_t local_crc;
        int mirror_index;
        bool write_enable;
        bool dirty;
};

void mmvp_partition_load_data(struct mmvp_controller *self, struct mmvp_partition *partition);

#ifdef __cplusplus
}
#endif

#endif /* MMVP_PARTITION_H */
