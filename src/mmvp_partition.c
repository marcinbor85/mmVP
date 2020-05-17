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

#include "mmvp_partition.h"
#include "mmvp_utils.h"

#include <stddef.h>
#include <string.h>

#include <assert.h>

static int find_mirror_with_highest_counter(struct mmvp_controller *self, struct mmvp_partition *partition, int *mirrors_error, int mirrors_error_num)
{
        int m, i;
        int mirror_index = -1;

        uint32_t cntr = 0;
        uint32_t adr;
        uint32_t mirror_size;

        assert(self != NULL);
        assert(partition != NULL);
        assert(mirrors_error != NULL);
        assert(mirrors_error_num >= 0);

        mirror_size = mmvp_get_mirror_size(self->device->total_size, self->device->wear_leveling_factor);

        for (m = 0; m < self->device->wear_leveling_factor; m++) {

                if (mmvp_is_num_in_array(m, mirrors_error, mirrors_error_num) != false)
                        continue;

                adr = partition->desc->address + m * mirror_size;

                i = self->device->read(adr, (uint8_t*)&partition->header, MMVP_PARTITION_HEADER_SIZE);
                if (i != MMVP_PARTITION_HEADER_SIZE)
                        continue;

                if (partition->header.counter > cntr)
                        mirror_index = m;
        }

        partition->mirror_index = mirror_index;
        return mirror_index;
}

static uint32_t read_data_and_check_crc_partition(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        uint8_t buf[self->device->page_size];
        uint32_t adr;
        uint32_t mirror_size;

        uint32_t act_size;
        uint32_t copy_act_size;
        uint32_t chunk_size;
        uint32_t crc;

        int i;

        assert(self != NULL);
        assert(partition != NULL);

        mirror_size = mmvp_get_mirror_size(self->device->total_size, self->device->wear_leveling_factor);

        adr = mmvp_get_data_real_start_address(self->device->page_size, partition->desc->address);
        adr = adr + partition->mirror_index * mirror_size;

        copy_act_size = 0;
        act_size = 0;
        crc = MMVP_PARTITION_CRC_INIT_VALUE;

        while (act_size < partition->header.size) {

                chunk_size = partition->header.size - act_size;
                if (chunk_size > self->device->page_size)
                        chunk_size = self->device->page_size;
                
                i = self->device->read(adr, buf, chunk_size);
                if ((uint32_t)i != chunk_size)
                        return crc;
                
                crc = mmvp_get_crc32(crc, buf, chunk_size);
                act_size += chunk_size;

                chunk_size = partition->desc->size - copy_act_size;
                if (chunk_size > self->device->page_size)
                        chunk_size = self->device->page_size;

                memcpy(partition->desc->data, buf, chunk_size);

                copy_act_size += chunk_size;
        }

        return crc;
}

static bool load_latest_valid_mirror(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        int mirrors_with_error[self->device->wear_leveling_factor];
        int mirrors_with_error_num = 0;

        int mirror_index;

        uint32_t crc;

        assert(self != NULL);
        assert(partition != NULL);

        while (mirrors_with_error_num < self->device->wear_leveling_factor) {
                mirror_index = find_mirror_with_highest_counter(self, partition, mirrors_with_error, mirrors_with_error_num);
                if (mirror_index < 0) {
                        break;
                }

                if (partition->header.version != partition->desc->version) {
                        mirrors_with_error[mirrors_with_error_num++] = mirror_index;
                        continue;
                }

                crc = read_data_and_check_crc_partition(self, partition);
                if (partition->header.crc != crc) {
                        mirrors_with_error[mirrors_with_error_num++] = mirror_index;
                        continue;
                }

                return true;
        }

        return false;
}

static void fill_local_data_with_defaults(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        assert(self != NULL);
        assert(partition != NULL);

        if (partition->desc->restore_default == NULL) {
                memset(partition->desc->data, MMVP_PARTITION_MEMORY_DEFAULT_BYTE_VALUE, partition->desc->size);
        } else {
                partition->desc->restore_default(partition->desc->data, partition->desc->size);
        }
}

void mmvp_load_partition_data(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        bool is_loaded;

        assert(self != NULL);
        assert(partition != NULL);

        fill_local_data_with_defaults(self, partition);
        is_loaded = load_latest_valid_mirror(self, partition);

        if (is_loaded == false) {
                fill_local_data_with_defaults(self, partition);
                partition->mirror_index = 0;
                partition->local_crc = 0;
        }
}
