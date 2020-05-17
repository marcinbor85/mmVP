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

#include "common.h"

static struct mmvp_controller mmvp;
static struct mmvp_partition partition1;
static struct mmvp_partition partition2;

static struct mmvp_device_descriptor device_descriptor;

static struct mmvp_partition_descriptor partition1_descriptor;
static struct mmvp_partition_descriptor partition2_descriptor;

static void prepare_test(void)
{
        memcpy((uint8_t*)&device_descriptor, (uint8_t*)test_device_descriptor, sizeof(device_descriptor));
        mmvp_init(&mmvp, &device_descriptor);
        memcpy((uint8_t*)&partition1_descriptor, (uint8_t*)test_partition1_descriptor, sizeof(partition1_descriptor));
        memcpy((uint8_t*)&partition2_descriptor, (uint8_t*)test_partition2_descriptor, sizeof(partition2_descriptor));
        mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
}

int main(int argc, char **argv)
{
	mmvp_error e;

        prepare_test();
        e = mmvp_start(NULL);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint8_t*)partition1.desc->data)[0] == 0x00);
        assert(((uint8_t*)partition1.desc->data)[1] == 0x01);
        assert(((uint8_t*)partition1.desc->data)[2] == 0x02);
        assert(partition1.dirty == true);
        assert(partition1.local_crc == 0);
        assert(partition1.mirror_index == 0);
        assert(((uint8_t*)partition2.desc->data)[0] == 0x00);
        assert(((uint8_t*)partition2.desc->data)[1] == 0xFF);
        assert(((uint8_t*)partition2.desc->data)[2] == 0xFE);
        assert(partition2.dirty == true);
        assert(partition2.local_crc == 0);
        assert(partition2.mirror_index == 0);

        test_common_prepare_device_memory();
        prepare_test();
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 1);

	return 0;
}
