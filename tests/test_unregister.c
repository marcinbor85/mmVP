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
}

int main(int argc, char **argv)
{
	mmvp_error e;

        prepare_test();
        e = mmvp_unregister_partition(NULL, NULL);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_unregister_partition(&mmvp, NULL);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_PARTITION_NOT_EXIST);

        prepare_test();
        mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_OK);
        assert(partition2.next == NULL);
        assert(mmvp.first == &partition2);
        e = mmvp_unregister_partition(&mmvp, &partition2);
        assert(e == MMVP_ERROR_OK);
        assert(mmvp.first == NULL);

        prepare_test();
        mmvp_register_partition(&mmvp, &partition1, &partition1_descriptor);
        mmvp_register_partition(&mmvp, &partition2, &partition2_descriptor);
        e = mmvp_unregister_partition(&mmvp, &partition2);
        assert(e == MMVP_ERROR_OK);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);
        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_OK);
        assert(mmvp.first == NULL);

	return 0;
}
