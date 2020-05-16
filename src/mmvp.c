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

#include <string.h>
#include <assert.h>

mmvp_error mmvp_init(struct mmvp_object *self, const struct mmvp_device_descriptor *device)
{
        assert(self != NULL);
        assert(device != NULL);

        assert(device->block_size > 0);
        assert(device->total_size > 0);

        assert(device->block_size <= device->total_size);

        self->device = device;
        self->first = NULL;

        return MMVP_ERROR_NO_ERROR;
}

mmvp_error mmvp_register_partition(struct mmvp_object *self, struct mmvp_partition *partition, const struct mmvp_partition_descriptor *desc)
{
        assert(self != NULL);
        assert(partition != NULL);
        assert(desc != NULL);

        partition->desc = desc;
        partition->next = self->first;
        self->first = partition;

        return MMVP_ERROR_NO_ERROR;
}

mmvp_error mmvp_unregister_partition(struct mmvp_object *self, struct mmvp_partition *partition)
{
        struct mmvp_partition *curr_part = self->first;
        struct mmvp_partition *prev_part = NULL;

        while (curr_part != NULL) {
                if (curr_part == partition) {
                        if (prev_part == NULL) {
                                self->first = curr_part->next;
                        } else {
                                prev_part->next = curr_part->next;
                        }
                        return MMVP_ERROR_NO_ERROR;
                }
                prev_part = curr_part;
                curr_part = curr_part->next;
        }

        return MMVP_ERROR_PARTITION_NOT_EXIST;
}