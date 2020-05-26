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

static int is_read_completed(void)
{
        return 1;
}

static int is_write_completed(void)
{
        return 1;
}

static const struct mmvp_device_descriptor _test_device_descriptor = {
        .read = read_page,
        .write = write_page,
        .is_read_completed = is_read_completed,
        .is_write_completed = is_write_completed,

        .total_size = TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE,
        .page_size = TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE
};

static const struct mmvp_controller_config _test_controller_config = {
        .wear_leveling_factor = TEST_COMMON_DEVICE_MEMORY_WEAR_LEVELING_FACTOR        
};

const struct mmvp_device_descriptor *test_device_descriptor = &_test_device_descriptor;
const struct mmvp_controller_config *test_controller_config = &_test_controller_config;

static void restore_default1(void *data, uint32_t size)
{
        uint8_t i = 0;
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

static void restore_default2(void *data, uint32_t size)
{
        uint8_t i = 0;
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
