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

int main(int argc, char **argv)
{
        assert(mmvp_get_data_real_start_address(8, 0) == 16);
        assert(mmvp_get_data_real_start_address(16, 0) == 16);
        assert(mmvp_get_data_real_start_address(32, 0) == 32);
        assert(mmvp_get_data_real_start_address(64, 0) == 64);

        assert(mmvp_get_data_real_start_address(8, 16) == 32);
        assert(mmvp_get_data_real_start_address(16, 16) == 32);
        assert(mmvp_get_data_real_start_address(32, 32) == 64);

        assert(mmvp_get_data_real_start_address(8, 64) == 80);
        assert(mmvp_get_data_real_start_address(16, 64) == 80);
        assert(mmvp_get_data_real_start_address(32, 64) == 96);
        assert(mmvp_get_data_real_start_address(64, 64) == 128);

        assert(mmvp_is_regions_overlap(0, 10, 100, 10) == false);
        assert(mmvp_is_regions_overlap(0, 100, 100, 10) == false);
        assert(mmvp_is_regions_overlap(0, 101, 100, 10) == true);
        assert(mmvp_is_regions_overlap(0, 10, 10, 10) == false);
        assert(mmvp_is_regions_overlap(0, 10, 9, 10) == true);
        assert(mmvp_is_regions_overlap(0, 10, 2, 2) == true);
        assert(mmvp_is_regions_overlap(2, 2, 0, 10) == true);

        assert(mmvp_get_data_real_size(8, 0, 12) == 28);
        assert(mmvp_get_data_real_size(16, 0, 12) == 28);
        assert(mmvp_get_data_real_size(32, 0, 12) == 44);
        assert(mmvp_get_data_real_size(64, 0, 12) == 76);

        assert(mmvp_get_data_real_size(8, 16, 12) == 28);
        assert(mmvp_get_data_real_size(16, 16, 12) == 28);
        assert(mmvp_get_data_real_size(32, 32, 12) == 44);

        assert(mmvp_get_data_real_size(8, 64, 12) == 28);
        assert(mmvp_get_data_real_size(16, 64, 12) == 28);
        assert(mmvp_get_data_real_size(32, 64, 12) == 44);
        assert(mmvp_get_data_real_size(64, 64, 12) == 76);

        assert(mmvp_is_power_of_two(0) == false);
        assert(mmvp_is_power_of_two(1) == true);
        assert(mmvp_is_power_of_two(2) == true);
        assert(mmvp_is_power_of_two(3) == false);
        assert(mmvp_is_power_of_two(4) == true);
        assert(mmvp_is_power_of_two(6) == false);
        assert(mmvp_is_power_of_two(8) == true);

        assert(mmvp_get_mirror_size(64, 2) == 32);
        assert(mmvp_get_mirror_size(128, 4) == 32);
        assert(mmvp_get_mirror_size(256, 1) == 256);

        int array_int[4];

        array_int[0] = 0;
        array_int[1] = 1;
        array_int[2] = 2;
        array_int[3] = 3;

        assert(mmvp_is_num_in_array(0, array_int, 4) == true);
        assert(mmvp_is_num_in_array(1, array_int, 4) == true);
        assert(mmvp_is_num_in_array(2, array_int, 4) == true);
        assert(mmvp_is_num_in_array(3, array_int, 4) == true);

        assert(mmvp_is_num_in_array(0, array_int, 3) == true);
        assert(mmvp_is_num_in_array(1, array_int, 3) == true);
        assert(mmvp_is_num_in_array(2, array_int, 3) == true);
        assert(mmvp_is_num_in_array(3, array_int, 3) == false);

        assert(mmvp_is_num_in_array(0, array_int, 2) == true);
        assert(mmvp_is_num_in_array(1, array_int, 2) == true);
        assert(mmvp_is_num_in_array(2, array_int, 2) == false);
        assert(mmvp_is_num_in_array(3, array_int, 2) == false);

        assert(mmvp_is_num_in_array(0, array_int, 1) == true);
        assert(mmvp_is_num_in_array(1, array_int, 1) == false);
        assert(mmvp_is_num_in_array(2, array_int, 1) == false);
        assert(mmvp_is_num_in_array(3, array_int, 1) == false);

        assert(mmvp_is_num_in_array(0, array_int, 0) == false);
        assert(mmvp_is_num_in_array(1, array_int, 0) == false);
        assert(mmvp_is_num_in_array(2, array_int, 0) == false);
        assert(mmvp_is_num_in_array(3, array_int, 0) == false);

        uint8_t array_uint8[8];

        assert(mmvp_get_crc32(0, array_uint8, 0) == 0x00000000);
        assert(mmvp_get_crc32(0xFFFFFFFF, array_uint8, 0) == 0xFFFFFFFF);

        array_uint8[0] = 0x78;
        array_uint8[1] = 0x56;
        array_uint8[2] = 0x34;
        array_uint8[3] = 0x12;

        assert(mmvp_get_crc32(0, array_uint8, 1) == 0xC6BCF05F);
        assert(mmvp_get_crc32(0, array_uint8, 2) == 0x99ED649E);
        assert(mmvp_get_crc32(0, array_uint8, 3) == 0x2D864EDD);
        assert(mmvp_get_crc32(0, array_uint8, 4) == 0x6A330D2D);

        array_uint8[0] = 0x00;
        array_uint8[1] = 0x01;
        array_uint8[2] = 0x02;
        array_uint8[3] = 0x03;

        assert(mmvp_get_crc32(0, array_uint8, 1) == 0x00000000);
        assert(mmvp_get_crc32(0, array_uint8, 2) == 0x04C11DB7);
        assert(mmvp_get_crc32(0, array_uint8, 3) == 0xDB9BFAB2);
        assert(mmvp_get_crc32(0, array_uint8, 4) == 0xAC691451);

        assert(mmvp_get_crc32(0xFFFFFFFF, array_uint8, 1) == 0x4E08BFB4);
        assert(mmvp_get_crc32(0xFFFFFFFF, array_uint8, 2) == 0x047679CA);
        assert(mmvp_get_crc32(0xFFFFFFFF, array_uint8, 3) == 0x6CFF87B2);
        assert(mmvp_get_crc32(0xFFFFFFFF, array_uint8, 4) == 0x6B6DC92A);

	return 0;
}
