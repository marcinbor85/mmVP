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

static void prepare_device_memory_mirror(uint32_t address, uint32_t size, uint32_t mirror_index, uint32_t version, uint32_t cntr, bool error, uint32_t tag_head, uint32_t tag_tail)
{
        struct mmvp_partition_header *header; 
        uint8_t* data;
        uint32_t adr;

        adr = address + mirror_index * mmvp_get_mirror_size(TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE, TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR);
        header = (struct mmvp_partition_header*)&test_device_memory_data[adr];
        data = &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, adr)];

        memset((uint8_t*)header, 0x00, MMVP_PARTITION_HEADER_SIZE);
        memset(data, 0x00, size);
        
        data[0] = tag_head;
        data[1] = tag_head >> 8;
        data[2] = tag_head >> 16;
        data[3] = tag_head >> 24;
        data[size - 4] = tag_tail;
        data[size - 3] = tag_tail >> 8;
        data[size - 2] = tag_tail >> 16;
        data[size - 1] = tag_tail >> 24;

        header->counter = cntr;
        header->size = size;
        header->version = version;
        header->crc = mmvp_get_crc32(MMVP_PARTITION_CRC_INIT_VALUE, data, size) + error;
}

static void prepare_device_memory_partition(uint8_t partition_num, uint32_t mirror_index, uint32_t version, uint32_t cntr, bool error, uint32_t tag_head, uint32_t tag_tail)
{
        uint32_t adr;
        uint32_t size;

        switch (partition_num) {
        case 0:
                adr = TEST_COMMON_PARTITION1_ADDRESS;
                size = TEST_COMMON_PARTITION1_SIZE;
                break;
        case 1:
                adr = TEST_COMMON_PARTITION2_ADDRESS;
                size = TEST_COMMON_PARTITION2_SIZE;
                break;
        default:
                adr = 0;
                size = 0;
                break;
        }
        prepare_device_memory_mirror(
                adr,
                size,
                mirror_index,
                version,
                cntr,
                error,
                tag_head,
                tag_tail
        );
}

static void prepare_test(void)
{
        memset(test_device_memory_data, 0x00, TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE);
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
        assert(((uint32_t*)partition1.desc->data)[0] == 0x03020100);
        assert(partition1.dirty == true);
        assert(partition1.local_crc == 0);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xFDFEFF00);
        assert(partition2.dirty == true);
        assert(partition2.local_crc == 0);
        assert(partition2.mirror_index == 0);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000001, false, 0xBAADC0DE, 0xBABEFACE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xDEADBEEF);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xCAFEBABE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xBAADC0DE);
        assert(((uint32_t*)partition2.desc->data)[TEST_COMMON_PARTITION2_SIZE/4 - 1] == 0xBABEFACE);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 1);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000001, true, 0xBAADC0DE, 0xBABEFACE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xDEADBEEF);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xCAFEBABE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xFDFEFF00);
        assert(partition2.dirty == true);
        assert(partition2.local_crc == 0);
        assert(partition2.mirror_index == 0);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000001, true, 0xBAADC0DE, 0xBABEFACE);
        prepare_device_memory_partition(1, 0, TEST_COMMON_PARTITION2_VERSION, 0x00000002, false, 0xBAADBABE, 0xDEADCAFE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xDEADBEEF);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xCAFEBABE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xBAADBABE);
        assert(((uint32_t*)partition2.desc->data)[TEST_COMMON_PARTITION2_SIZE/4 - 1] == 0xDEADCAFE);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 0);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000001, false, 0xBAADC0DE, 0xBABEFACE);
        prepare_device_memory_partition(1, 0, TEST_COMMON_PARTITION2_VERSION, 0x00000002, false, 0xBAADBABE, 0xDEADCAFE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xDEADBEEF);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xCAFEBABE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xBAADBABE);
        assert(((uint32_t*)partition2.desc->data)[TEST_COMMON_PARTITION2_SIZE/4 - 1] == 0xDEADCAFE);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 0);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000001, false, 0xBAADC0DE, 0xBABEFACE);
        prepare_device_memory_partition(1, 0, TEST_COMMON_PARTITION2_VERSION, 0x00000002, true, 0xBAADBABE, 0xDEADCAFE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xDEADBEEF);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xCAFEBABE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xBAADC0DE);
        assert(((uint32_t*)partition2.desc->data)[TEST_COMMON_PARTITION2_SIZE/4 - 1] == 0xBABEFACE);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 1);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000003, false, 0xBAADC0DE, 0xBABEFACE);
        prepare_device_memory_partition(1, 0, TEST_COMMON_PARTITION2_VERSION, 0x00000002, false, 0xBAADBABE, 0xDEADCAFE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xDEADBEEF);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xCAFEBABE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xBAADC0DE);
        assert(((uint32_t*)partition2.desc->data)[TEST_COMMON_PARTITION2_SIZE/4 - 1] == 0xBABEFACE);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 1);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(0, 1, TEST_COMMON_PARTITION1_VERSION, 0x00000002, true, 0xBEEFDEAD, 0xBABECAFE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000003, false, 0xBAADC0DE, 0xBABEFACE);
        prepare_device_memory_partition(1, 0, TEST_COMMON_PARTITION2_VERSION, 0x00000002, false, 0xBAADBABE, 0xDEADCAFE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xDEADBEEF);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xCAFEBABE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 0);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xBAADC0DE);
        assert(((uint32_t*)partition2.desc->data)[TEST_COMMON_PARTITION2_SIZE/4 - 1] == 0xBABEFACE);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 1);

        prepare_test();
        prepare_device_memory_partition(0, 0, TEST_COMMON_PARTITION1_VERSION, 0x00000001, false, 0xDEADBEEF, 0xCAFEBABE);
        prepare_device_memory_partition(0, 1, TEST_COMMON_PARTITION1_VERSION, 0x00000002, false, 0xBEEFDEAD, 0xBABECAFE);
        prepare_device_memory_partition(1, 1, TEST_COMMON_PARTITION2_VERSION, 0x00000003, false, 0xBAADC0DE, 0xBABEFACE);
        prepare_device_memory_partition(1, 0, TEST_COMMON_PARTITION2_VERSION, 0x00000002, false, 0xBAADBABE, 0xDEADCAFE);
        e = mmvp_start(&mmvp);
        assert(e == MMVP_ERROR_OK);
        assert(((uint32_t*)partition1.desc->data)[0] == 0xBEEFDEAD);
        assert(((uint32_t*)partition1.desc->data)[TEST_COMMON_PARTITION1_SIZE/4 - 1] == 0xBABECAFE);
        assert(memcmp((uint8_t*)partition1.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION1_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR)], TEST_COMMON_PARTITION1_SIZE) == 0);
        assert(partition1.dirty == false);
        assert(partition1.mirror_index == 1);
        assert(((uint32_t*)partition2.desc->data)[0] == 0xBAADC0DE);
        assert(((uint32_t*)partition2.desc->data)[TEST_COMMON_PARTITION2_SIZE/4 - 1] == 0xBABEFACE);
        assert(memcmp((uint8_t*)partition2.desc->data, &test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, TEST_COMMON_PARTITION2_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR)], TEST_COMMON_PARTITION2_SIZE) == 0);
        assert(partition2.dirty == false);
        assert(partition2.mirror_index == 1);

	return 0;
}
