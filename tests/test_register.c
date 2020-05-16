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

#include "mmvp.h"

#include "common.h"

static struct mmvp_object mmvp;
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
}

int main(int argc, char **argv)
{
        mmvp_error e;

        prepare_test();
        e = mmvp_register_partition(NULL, NULL, NULL);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_register_partition(&mmvp, NULL, NULL);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_register_partition(&mmvp, &partition1, NULL);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        partition1_descriptor.data = NULL;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        partition1_descriptor.size = 0;
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_ERROR_WRONG_SIZE);

        prepare_test();
        e = mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        assert(e == MMVP_ERROR_OK);
        assert(partition1.desc == &partition1_descriptor);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);
        e = mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        assert(e == MMVP_ERROR_OK);
        assert(partition2.desc == &partition2_descriptor);
        assert(partition2.next == &partition1);
        assert(mmvp.first == &partition2);

	return 0;
}
