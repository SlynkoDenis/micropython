/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Fabian Vogt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "helpers.h"

#include "py/mpstate.h"
#include "py/gc.h"

#if MICROPY_EMIT_NATIVE || (MICROPY_PY_FFI && MICROPY_FORCE_PLAT_ALLOC_EXEC)

// The memory allocated here is not on the GC heap (and it may contain pointers
// that need to be GC'd) so we must somehow trace this memory.  We do it by
// keeping a linked list of all mmap'd regions, and tracing them explicitly.

typedef struct _mmap_region_t {
    void *ptr;
    size_t len;
    struct _mmap_region_t *next;
} mmap_region_t;

void mp_unix_alloc_exec(size_t min_size, void **ptr, size_t *size) {
    // size needs to be a multiple of the page size
    *size = (min_size + 0xfff) & (~0xfff);
    int r = sys_alloc_region(CURENVID, *ptr, *size, PROT_RW | PROT_X);
    if (r < 0) {
        *ptr = NULL;
    }

    // add new link to the list of mmap'd regions
    mmap_region_t *rg = m_new_obj(mmap_region_t);
    rg->ptr = *ptr;
    rg->len = min_size;
    rg->next = MP_STATE_VM(mmap_region_head);
    MP_STATE_VM(mmap_region_head) = rg;
}

void mp_unix_free_exec(void *ptr, size_t size) {
    // munmap(ptr, size);
    sys_unmap_region(CURENVID, ptr, size);

    // unlink the mmap'd region from the list
    for (mmap_region_t **rg = (mmap_region_t **)&MP_STATE_VM(mmap_region_head); *rg != NULL; *rg = (*rg)->next) {
        if ((*rg)->ptr == ptr) {
            mmap_region_t *next = (*rg)->next;
            m_del_obj(mmap_region_t, *rg);
            *rg = next;
            return;
        }
    }
}

void mp_unix_mark_exec(void) {
    for (mmap_region_t *rg = MP_STATE_VM(mmap_region_head); rg != NULL; rg = rg->next) {
        gc_collect_root(rg->ptr, rg->len / sizeof(mp_uint_t));
    }
}

MP_REGISTER_ROOT_POINTER(void *mmap_region_head);

#endif // MICROPY_EMIT_NATIVE || (MICROPY_PY_FFI && MICROPY_FORCE_PLAT_ALLOC_EXEC)