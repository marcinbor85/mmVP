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
static struct mmvp_controller_config controller_config;

static struct mmvp_partition_descriptor partition1_descriptor;
static struct mmvp_partition_descriptor partition2_descriptor;

static void prepare_test(void)
{
        memcpy((uint8_t*)&device_descriptor, (uint8_t*)test_device_descriptor, sizeof(device_descriptor));
        memcpy((uint8_t*)&controller_config, (uint8_t*)test_controller_config, sizeof(controller_config));
        mmvp_init(&mmvp, &device_descriptor, &controller_config);
        memcpy((uint8_t*)&partition1_descriptor, (uint8_t*)test_partition1_descriptor, sizeof(partition1_descriptor));
        memcpy((uint8_t*)&partition2_descriptor, (uint8_t*)test_partition2_descriptor, sizeof(partition2_descriptor));
}

int main(int argc, char **argv)
{
        mmvp_status e;

        prepare_test();
        e = mmvp_register_partition(NULL, NULL, NULL);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_register_partition(&mmvp, NULL, NULL);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_register_partition(&mmvp, &partition1, NULL);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        partition1_descriptor.data = NULL;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        partition1_descriptor.size = 0;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_WRONG_SIZE);

        prepare_test();
        device_descriptor.page_size = 16;
        partition1_descriptor.address = 24;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_ADDRESS_ALIGNMENT);

        prepare_test();
        device_descriptor.page_size = 16;
        partition1_descriptor.address = 4;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_ADDRESS_ALIGNMENT);

        prepare_test();
        device_descriptor.page_size = 16;
        partition1_descriptor.address = 0;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_OK);

        prepare_test();
        device_descriptor.page_size = 16;
        partition1_descriptor.address = 16;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_OK);

        prepare_test();
        device_descriptor.page_size = 16;
        partition1_descriptor.address = 32;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_OK);

        prepare_test();
        device_descriptor.total_size = 64;
        device_descriptor.page_size = 16;
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 32;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        prepare_test();
        device_descriptor.total_size = 128;
        device_descriptor.page_size = 16;
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 32;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_OK);

        prepare_test();
        device_descriptor.total_size = 128;
        device_descriptor.page_size = 16;
        controller_config.wear_leveling_factor = 4;
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 32;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        prepare_test();
        device_descriptor.total_size = 128;
        device_descriptor.page_size = 32;
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 32;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_OK);

        prepare_test();
        device_descriptor.total_size = 128;
        device_descriptor.page_size = 32;
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 33;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        prepare_test();
        device_descriptor.total_size = 128;
        device_descriptor.page_size = 32;
        partition1_descriptor.address = 32;
        partition1_descriptor.size = 32;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        prepare_test();
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 32;
        partition2_descriptor.address = 32;
        partition2_descriptor.size = 32;
        mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        e = mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        assert(e == MMVP_STATUS_ERROR_PARTITION_OVERLAP);

        prepare_test();
        partition1_descriptor.address = 32;
        partition1_descriptor.size = 32;
        partition2_descriptor.address = 0;
        partition2_descriptor.size = 32;
        mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        e = mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        assert(e == MMVP_STATUS_ERROR_PARTITION_OVERLAP);

        prepare_test();
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 32;
        partition2_descriptor.address = 64;
        partition2_descriptor.size = 32;
        mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        e = mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        assert(e == MMVP_STATUS_OK);

        prepare_test();
        partition1_descriptor.address = 0;
        partition1_descriptor.size = 256;
        partition2_descriptor.address = 64;
        partition2_descriptor.size = 32;
        mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        e = mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        assert(e == MMVP_STATUS_ERROR_PARTITION_OVERLAP);

        prepare_test();
        memset((uint8_t*)&partition1, 0xFF, sizeof(partition1));
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_OK);
        assert(partition1.dirty == false);
        assert(partition1.local_crc == 0);
        assert(partition1.mirror_index == 0);
        assert(partition1.header.counter == 0);
        assert(partition1.header.crc == 0);
        assert(partition1.header.size == 0);
        assert(partition1.header.version == 0);

        prepare_test();
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_OK);
        assert(partition1.desc == &partition1_descriptor);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);
        e = mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        assert(e == MMVP_STATUS_OK);
        assert(partition2.desc == &partition2_descriptor);
        assert(partition2.next == &partition1);
        assert(mmvp.first == &partition2);
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_STATUS_ERROR_PARTITION_EXIST);
        e = mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        assert(e == MMVP_STATUS_ERROR_PARTITION_EXIST);

	return 0;
}
