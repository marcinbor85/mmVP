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

int main(int argc, char **argv)
{
	mmvp_error e;

        mmvp_init(&mmvp, test_device_descriptor);

        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_PARTITION_NOT_EXIST);

        mmvp_register_partition(&mmvp, &partition1, test_partition1_descriptor);
        mmvp_register_partition(&mmvp, &partition2, test_partition2_descriptor);

        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition2.next == NULL);
        assert(mmvp.first == &partition2);
        e = mmvp_unregister_partition(&mmvp, &partition2);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(mmvp.first == NULL);

        mmvp_register_partition(&mmvp, &partition1, test_partition1_descriptor);
        mmvp_register_partition(&mmvp, &partition2, test_partition2_descriptor);

        e = mmvp_unregister_partition(&mmvp, &partition2);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);
        e = mmvp_unregister_partition(&mmvp, &partition1);
        assert(e == MMVP_ERROR_NO_ERROR);
        assert(mmvp.first == NULL);

	return 0;
}
