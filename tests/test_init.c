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

static struct mmvp_controller mmvp;

static struct mmvp_device_descriptor device_descriptor;

static void prepare_test(void)
{
        memcpy((uint8_t*)&device_descriptor, (uint8_t*)test_device_descriptor, sizeof(device_descriptor));
}

int main(int argc, char **argv)
{
        mmvp_error e;

        prepare_test();
        e = mmvp_init(&mmvp, NULL);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        e = mmvp_init(NULL, &device_descriptor);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        device_descriptor.read_page = NULL;
        e = mmvp_init(&mmvp, &device_descriptor);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        device_descriptor.write_page = NULL;
        e = mmvp_init(&mmvp, &device_descriptor);
        assert(e == MMVP_ERROR_NULL_POINTER);

        prepare_test();
        device_descriptor.page_size = 0;
        e = mmvp_init(&mmvp, &device_descriptor);
        assert(e == MMVP_ERROR_WRONG_SIZE);

        prepare_test();
        device_descriptor.total_size = 0;
        e = mmvp_init(&mmvp, &device_descriptor);
        assert(e == MMVP_ERROR_WRONG_SIZE);

        prepare_test();
        device_descriptor.total_size = device_descriptor.page_size - 1;
        e = mmvp_init(&mmvp, &device_descriptor);
        assert(e == MMVP_ERROR_WRONG_SIZE);

        prepare_test();
        e = mmvp_init(&mmvp, &device_descriptor);
        assert(e == MMVP_ERROR_OK);
        assert(mmvp.first == NULL);
        assert(mmvp.device == &device_descriptor);

	return 0;
}
