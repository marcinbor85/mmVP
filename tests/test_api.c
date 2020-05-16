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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <assert.h>

#include "../src/mmvp.h"

static int read_block(uint8_t *data, uint32_t size_to_read, uint32_t *read_size)
{
        return 0;
}

static int write_block(uint8_t *data, uint32_t size_to_write, uint32_t *write_size)
{
        return 0;
}

static const struct mmvp_device_descriptor device = {
        .read = read_block,
        .write = write_block,

        .total_size = 8192,
        .block_size = 32
};

static uint8_t data1[256];

static const struct mmvp_partition_descriptor partition_descriptor1 = {
        .address = 0x0000,
        .size = sizeof(data1),
        .data = data1
};

static struct mmvp_partition partition1;

static uint8_t data2[512];

static const struct mmvp_partition_descriptor partition_descriptor2 = {
        .address = 0x1000,
        .size = sizeof(data2),
        .data = data2
};

static struct mmvp_partition partition2;

int main(int argc, char **argv)
{
	struct mmvp_object mmvp;

        mmvp_error e;

        e = mmvp_init(&mmvp, &device);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(mmvp.first == NULL);
        assert(mmvp.device == &device);

        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_PARTITION_NOT_EXIST);

        e = mmvp_register_partition(&mmvp, &partition1, &partition_descriptor1);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition1.desc == &partition_descriptor1);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);

        e = mmvp_register_partition(&mmvp, &partition2, &partition_descriptor2);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition2.desc == &partition_descriptor2);
        assert(partition2.next == &partition1);
        assert(mmvp.first == &partition2);

        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition2.next == NULL);
        assert(mmvp.first == &partition2);

        e = mmvp_unregister_partition(&mmvp, &partition2);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(mmvp.first == NULL);

        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_PARTITION_NOT_EXIST);

        e = mmvp_register_partition(&mmvp, &partition1, &partition_descriptor1);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition1.desc == &partition_descriptor1);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);

        e = mmvp_register_partition(&mmvp, &partition2, &partition_descriptor2);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition2.desc == &partition_descriptor2);
        assert(partition2.next == &partition1);
        assert(mmvp.first == &partition2);

        e = mmvp_unregister_partition(&mmvp, &partition2);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);

        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(mmvp.first == NULL);

	return 0;
}
