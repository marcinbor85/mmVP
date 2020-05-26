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

#include "mmvp.h"
#include "mmvp_utils.h"
#include "mmvp_assert.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

mmvp_status mmvp_init(struct mmvp_controller *self, const struct mmvp_device_descriptor *device, const struct mmvp_controller_config *config)
{
        mmvp_check_param(self != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(device != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(config != NULL, MMVP_STATUS_ERROR_NULL_POINTER);

        mmvp_check_param(device->read != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(device->write != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(mmvp_is_power_of_two(device->page_size) != false, MMVP_STATUS_ERROR_WRONG_SIZE);
        mmvp_check_param(mmvp_is_power_of_two(device->total_size) != false, MMVP_STATUS_ERROR_WRONG_SIZE);
        mmvp_check_param(mmvp_is_power_of_two(config->wear_leveling_factor) != false, MMVP_STATUS_ERROR_WRONG_FACTOR);
        mmvp_check_param(device->page_size <= mmvp_get_mirror_size(device->total_size, config->wear_leveling_factor), MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        self->device = device;
        self->config = config;
        self->first = NULL;

        return MMVP_STATUS_OK;
}

mmvp_status mmvp_register_partition(struct mmvp_controller *self, struct mmvp_partition *partition, const struct mmvp_partition_descriptor *desc)
{
        mmvp_check_param(self != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(partition != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(desc != NULL, MMVP_STATUS_ERROR_NULL_POINTER);

        mmvp_check_param(desc->data != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(desc->size > 0, MMVP_STATUS_ERROR_WRONG_SIZE);
        mmvp_check_param((desc->address % self->device->page_size) == 0, MMVP_STATUS_ERROR_ADDRESS_ALIGNMENT);

        mmvp_check_param(mmvp_get_data_real_start_address(self->device->page_size, desc->address) + desc->size <= mmvp_get_mirror_size(self->device->total_size, self->config->wear_leveling_factor), MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        struct mmvp_partition *curr_part = self->first;
        
        while (curr_part != NULL) {
                mmvp_check_param(partition != curr_part, MMVP_STATUS_ERROR_PARTITION_EXIST);
                mmvp_check_param(mmvp_is_regions_overlap(
                        desc->address,
                        mmvp_get_data_real_size(self->device->page_size, desc->address, desc->size),
                        curr_part->desc->address,
                        mmvp_get_data_real_size(self->device->page_size, curr_part->desc->address, curr_part->desc->size)
                ) == false, MMVP_STATUS_ERROR_PARTITION_OVERLAP);

                curr_part = curr_part->next;
        }

        partition->desc = desc;
        partition->next = self->first;
        partition->write_enable = false;
        partition->mirror_index = 0;
        partition->local_crc = 0;
        partition->dirty = false;
        memset((uint8_t*)&partition->header, 0x00, MMVP_PARTITION_HEADER_SIZE);

        self->first = partition;

        return MMVP_STATUS_OK;
}

mmvp_status mmvp_unregister_partition(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        mmvp_check_param(self != NULL, MMVP_STATUS_ERROR_NULL_POINTER);
        mmvp_check_param(partition != NULL, MMVP_STATUS_ERROR_NULL_POINTER);

        struct mmvp_partition *curr_part = self->first;
        struct mmvp_partition *prev_part = NULL;

        while (curr_part != NULL) {
                if (curr_part == partition) {
                        if (prev_part == NULL) {
                                self->first = curr_part->next;
                        } else {
                                prev_part->next = curr_part->next;
                        }
                        return MMVP_STATUS_OK;
                }
                prev_part = curr_part;
                curr_part = curr_part->next;
        }

        return MMVP_STATUS_ERROR_PARTITION_NOT_EXIST;
}

mmvp_status mmvp_start(struct mmvp_controller *self)
{
        mmvp_check_param(self != NULL, MMVP_STATUS_ERROR_NULL_POINTER);

        struct mmvp_partition *curr_part = self->first;

        while (curr_part != NULL) {
                _mmvp_load_partition_data(self, curr_part);

                curr_part = curr_part->next;
        }

        return MMVP_STATUS_OK;
}
