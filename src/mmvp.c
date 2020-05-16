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
#include "mmvp_assert.h"

#include <string.h>

mmvp_error mmvp_init(struct mmvp_controller *self, const struct mmvp_device_descriptor *device)
{
        mmvp_check_param(self != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(device != NULL, MMVP_ERROR_NULL_POINTER);

        mmvp_check_param(device->read_page != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(device->write_page != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(device->page_size > 0, MMVP_ERROR_WRONG_SIZE);
        mmvp_check_param(device->total_size > 0, MMVP_ERROR_WRONG_SIZE);

        mmvp_check_param(device->page_size <= device->total_size, MMVP_ERROR_WRONG_SIZE);

        self->device = device;
        self->first = NULL;

        return MMVP_ERROR_OK;
}

mmvp_error mmvp_register_partition(struct mmvp_controller *self, struct mmvp_partition *partition, const struct mmvp_partition_descriptor *desc)
{
        mmvp_check_param(self != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(partition != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(desc != NULL, MMVP_ERROR_NULL_POINTER);

        mmvp_check_param(desc->data != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(desc->size > 0, MMVP_ERROR_WRONG_SIZE);
        mmvp_check_param((desc->address % self->device->page_size) == 0, MMVP_ERROR_ADDRESS_PADDING);

        partition->desc = desc;
        partition->next = self->first;
        self->first = partition;

        return MMVP_ERROR_OK;
}

mmvp_error mmvp_unregister_partition(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        mmvp_check_param(self != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(partition != NULL, MMVP_ERROR_NULL_POINTER);

        struct mmvp_partition *curr_part = self->first;
        struct mmvp_partition *prev_part = NULL;

        while (curr_part != NULL) {
                if (curr_part == partition) {
                        if (prev_part == NULL) {
                                self->first = curr_part->next;
                        } else {
                                prev_part->next = curr_part->next;
                        }
                        return MMVP_ERROR_OK;
                }
                prev_part = curr_part;
                curr_part = curr_part->next;
        }

        return MMVP_ERROR_PARTITION_NOT_EXIST;
}

uint32_t get_partition_data_start_address(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        mmvp_check_param(self != NULL, MMVP_ERROR_NULL_POINTER);
        mmvp_check_param(partition != NULL, MMVP_ERROR_NULL_POINTER);
        
        uint32_t adr;
        uint32_t padding;

        adr = partition->desc->address + sizeof(struct mmvp_partition_header);
        padding = adr % self->device->page_size;

        return adr + padding;
}

static void load_partition_data(struct mmvp_controller *self, struct mmvp_partition *partition)
{
        (void)self;
        (void)partition;
}

mmvp_error mmvp_start(struct mmvp_controller *self)
{
        mmvp_check_param(self != NULL, MMVP_ERROR_NULL_POINTER);

        struct mmvp_partition *curr_part = self->first;

        while (curr_part != NULL) {
                load_partition_data(self, curr_part);

                curr_part = curr_part->next;
        }

        return MMVP_ERROR_OK;
}
