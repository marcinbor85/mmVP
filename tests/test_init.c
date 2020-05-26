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

static struct mmvp_device_descriptor device_descriptor;
static struct mmvp_controller_config controller_config;

static void prepare_test(void)
{
        memcpy((uint8_t*)&device_descriptor, (uint8_t*)test_device_descriptor, sizeof(device_descriptor));
        memcpy((uint8_t*)&controller_config, (uint8_t*)test_controller_config, sizeof(controller_config));
}

int main(int argc, char **argv)
{
        mmvp_status e;

        prepare_test();
        e = mmvp_init(&mmvp, NULL, &controller_config);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_init(NULL, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_init(&mmvp, &device_descriptor, NULL);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        device_descriptor.read = NULL;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        device_descriptor.write = NULL;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

         prepare_test();
        device_descriptor.is_read_completed = NULL;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        device_descriptor.is_write_completed = NULL;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_NULL_POINTER);

        prepare_test();
        device_descriptor.page_size = 0;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_WRONG_SIZE);

        prepare_test();
        device_descriptor.total_size = 0;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_WRONG_SIZE);

        prepare_test();
        device_descriptor.total_size = device_descriptor.page_size - 1;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_WRONG_SIZE);

        prepare_test();
        device_descriptor.total_size = device_descriptor.page_size;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        prepare_test();
        device_descriptor.total_size = device_descriptor.page_size / 2;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_OUT_OF_MEMORY);

        prepare_test();
        controller_config.wear_leveling_factor = 3;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_WRONG_FACTOR);

        prepare_test();
        controller_config.wear_leveling_factor = 24;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_ERROR_WRONG_FACTOR);

        prepare_test();
        controller_config.wear_leveling_factor = 8;
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_OK);

        prepare_test();
        e = mmvp_init(&mmvp, &device_descriptor, &controller_config);
        assert(e == MMVP_STATUS_OK);
        assert(mmvp.first == NULL);
        assert(mmvp.device == &device_descriptor);

	return 0;
}
