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

        mmvp_register_partition(&mmvp, &partition1, test_partition1_descriptor);
        assert(partition1.desc == test_partition1_descriptor);
        assert(partition1.next == NULL);
        assert(mmvp.first == &partition1);
        mmvp_register_partition(&mmvp, &partition2, test_partition2_descriptor);
        assert(partition2.desc == test_partition2_descriptor);
        assert(partition2.next == &partition1);
        assert(mmvp.first == &partition2);

	return 0;
}
