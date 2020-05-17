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

#ifndef MMVP_UTILS_H
#define MMVP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

uint32_t mmvp_get_data_real_start_address(uint32_t page_size, uint32_t address);
uint32_t mmvp_get_data_real_size(uint32_t page_size, uint32_t address, uint32_t size);
bool mmvp_is_regions_overlap(uint32_t adr1, uint32_t size1, uint32_t adr2, uint32_t size2);
bool mmvp_is_power_of_two(uint32_t);
uint32_t mmvp_get_mirror_size(uint32_t total_size, uint32_t wear_leveling_factor);

#ifdef __cplusplus
}
#endif

#endif /* MMVP_UTILS_H */
