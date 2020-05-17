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

static uint8_t _test_device_memory_data[TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE];
uint8_t *test_device_memory_data = _test_device_memory_data;

static int read_page(uint32_t address, uint8_t *data, uint32_t size)
{
        memcpy(data, &_test_device_memory_data[address], size);
        return size;
}

static int write_page(uint32_t address, uint8_t *data, uint32_t size)
{
        memcpy(&_test_device_memory_data[address], data, size);
        return size;
}

static const struct mmvp_device_descriptor _test_device_descriptor = {
        .read = read_page,
        .write = write_page,

        .total_size = TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE,
        .page_size = TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE,
        .wear_leveling_factor = TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR
};

const struct mmvp_device_descriptor *test_device_descriptor = &_test_device_descriptor;

static void restore_default1(void *data, uint32_t size, bool dirty)
{
        uint8_t i = (dirty == false) ? 0 : 0x10;
        uint8_t *data_ptr = data;

        while (size-- > 0)
                *data_ptr++ = i++;
}

static uint8_t data1[TEST_COMMON_PARTITION1_SIZE];

static const struct mmvp_partition_descriptor _test_partition1_descriptor = {
        .version = TEST_COMMON_PARTITION1_VERSION,
        .address = TEST_COMMON_PARTITION1_ADDRESS,
        .size = sizeof(data1),
        .data = data1,
        .restore_default = restore_default1
};

static void restore_default2(void *data, uint32_t size, bool dirty)
{
        uint8_t i = (dirty == false) ? 0 : 0x10;
        uint8_t *data_ptr = data;

        while (size-- > 0)
                *data_ptr++ = i--;
}

static uint8_t data2[TEST_COMMON_PARTITION2_SIZE];

static const struct mmvp_partition_descriptor _test_partition2_descriptor = {
        .version = TEST_COMMON_PARTITION2_VERSION,
        .address = TEST_COMMON_PARTITION2_ADDRESS,
        .size = sizeof(data2),
        .data = data2,
        .restore_default = restore_default2
};

const struct mmvp_partition_descriptor *test_partition1_descriptor = &_test_partition1_descriptor;
const struct mmvp_partition_descriptor *test_partition2_descriptor = &_test_partition2_descriptor;

void test_common_print_hex_buf(uint8_t *buf, uint32_t size)
{
        uint8_t cntr = 0;

        while (size-- > 0) {
                printf("%02X ", *buf++);
                cntr %= 16;
                if ((++cntr % 16) == 0)
                        printf("\n");
        }
}

void test_common_prepare_device_memory(void)
{
        struct mmvp_partition_header *header; 
        uint8_t* data;
        uint32_t adr;

        memset(_test_device_memory_data, 0x00, TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE);

        adr = TEST_COMMON_PARTITION1_ADDRESS;
        header = (struct mmvp_partition_header*)&_test_device_memory_data[adr];
        data = &_test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, adr)];
        
        data[0] = 0xDE;
        data[1] = 0xAD;
        data[2] = 0xBE;
        data[3] = 0xEF;
        data[TEST_COMMON_PARTITION1_SIZE - 4] = 0xCA;
        data[TEST_COMMON_PARTITION1_SIZE - 3] = 0xFE;
        data[TEST_COMMON_PARTITION1_SIZE - 2] = 0xBA;
        data[TEST_COMMON_PARTITION1_SIZE - 1] = 0xBE;

        header->counter = 0x00000001;
        header->size = TEST_COMMON_PARTITION1_SIZE;
        header->version = TEST_COMMON_PARTITION1_VERSION;
        header->crc = mmvp_get_crc32(MMVP_PARTITION_CRC_INIT_VALUE, data, TEST_COMMON_PARTITION1_SIZE);

        adr = TEST_COMMON_PARTITION2_ADDRESS + TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE / TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR;
        header = (struct mmvp_partition_header*)&_test_device_memory_data[adr];
        data = &_test_device_memory_data[mmvp_get_data_real_start_address(TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE, adr)];

        data[0] = 0xBA;
        data[1] = 0xAD;
        data[2] = 0xC0;
        data[3] = 0xDE;
        data[TEST_COMMON_PARTITION2_SIZE - 4] = 0xBA;
        data[TEST_COMMON_PARTITION2_SIZE - 3] = 0xBE;
        data[TEST_COMMON_PARTITION2_SIZE - 2] = 0xFA;
        data[TEST_COMMON_PARTITION2_SIZE - 1] = 0xCE;

        header->counter = 0x00000001;
        header->size = TEST_COMMON_PARTITION2_SIZE;
        header->version = TEST_COMMON_PARTITION2_VERSION;
        header->crc = mmvp_get_crc32(MMVP_PARTITION_CRC_INIT_VALUE, data, TEST_COMMON_PARTITION2_SIZE);
}