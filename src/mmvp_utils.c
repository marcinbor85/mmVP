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

#include "mmvp_utils.h"
#include "mmvp.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

uint32_t mmvp_get_data_real_start_address(uint32_t page_size, uint32_t address)
{
        assert(page_size > 0);
        
        uint32_t adr;
        uint32_t padding;

        adr = address + sizeof(struct mmvp_partition_header);
        padding = (page_size - adr % page_size) % page_size;

        return adr + padding;
}

uint32_t mmvp_get_data_real_size(uint32_t page_size, uint32_t address, uint32_t size)
{
        assert(page_size > 0);
        assert(size > 0);

        uint32_t adr;

        adr = mmvp_get_data_real_start_address(page_size, address);

        return size + (adr - address);
}

bool mmvp_is_regions_overlap(uint32_t adr1, uint32_t size1, uint32_t adr2, uint32_t size2)
{
        assert(size1 > 0);
        assert(size2 > 0);

        uint32_t adr1_end = adr1 + size1;
        uint32_t adr2_end = adr2 + size2;

        if ((adr1_end > adr2) && (adr1_end < adr2_end))
                return true;

        if ((adr2_end > adr1) && (adr2_end < adr1_end))
                return true;

        if ((adr1 > adr2) && (adr1_end < adr2_end))
                return true;

        if ((adr2 > adr1) && (adr2_end < adr1_end))
                return true;
        
        return false;
}

bool mmvp_is_power_of_two(uint32_t num)
{
        return (!(num & (num - 1)) && num);
}

uint32_t mmvp_get_mirror_size(uint32_t total_size, uint32_t wear_leveling_factor)
{
        return total_size / wear_leveling_factor;
}
