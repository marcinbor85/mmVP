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

#ifndef MMVP_H
#define MMVP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmvp_partition.h"
#include "mmvp_device.h"
#include "mmvp_controller.h"
#include "mmvp_error.h"

mmvp_error mmvp_init(struct mmvp_controller *self, const struct mmvp_device_descriptor *device);
mmvp_error mmvp_register_partition(struct mmvp_controller *self, struct mmvp_partition *partition, const struct mmvp_partition_descriptor *desc);
mmvp_error mmvp_unregister_partition(struct mmvp_controller *self, struct mmvp_partition *partition);
mmvp_error mmvp_start(struct mmvp_controller *self);
mmvp_error mmvp_confirm_write(struct mmvp_controller *self);
mmvp_error mmvp_confirm_read(struct mmvp_controller *self, uint32_t address, uint8_t *data, uint32_t size);
mmvp_error mmvp_service(struct mmvp_controller *self);

#ifdef __cplusplus
}
#endif

#endif /* MMVP_H */
