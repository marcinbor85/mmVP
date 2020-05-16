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

static int read_page(uint8_t *data, uint32_t size_to_read, uint32_t *read_size)
{
        return 0;
}

static int write_page(uint8_t *data, uint32_t size_to_write, uint32_t *write_size)
{
        return 0;
}

static const struct mmvp_device_descriptor _test_device_descriptor = {
        .read_page = read_page,
        .write_page = write_page,

        .total_size = TEST_COMMON_DEVICE_MEMORY_TOTAL_SIZE,
        .page_size = TEST_COMMON_DEVICE_MEMORY_PAGE_SIZE
};

const struct mmvp_device_descriptor *test_device_descriptor = &_test_device_descriptor;

static uint8_t data1[TEST_COMMON_PARTITION1_SIZE];

static const struct mmvp_partition_descriptor _test_partition1_descriptor = {
        .address = TEST_COMMON_PARTITION1_ADDRESS,
        .size = sizeof(data1),
        .data = data1
};

static uint8_t data2[TEST_COMMON_PARTITION2_SIZE];

static const struct mmvp_partition_descriptor _test_partition2_descriptor = {
        .address = TEST_COMMON_PARTITION2_ADDRESS,
        .size = sizeof(data2),
        .data = data2
};

const struct mmvp_partition_descriptor *test_partition1_descriptor = &_test_partition1_descriptor;
const struct mmvp_partition_descriptor *test_partition2_descriptor = &_test_partition2_descriptor;
