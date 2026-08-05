# ft_malloc

`ft_malloc` is a memory allocator project for implementing `malloc`, `free`, `realloc`, and `show_alloc_mem` directly in C.

## Goals

- Implement dynamic allocation flow with `mmap` and `munmap`.
- Manage memory areas by TINY, SMALL, and LARGE size classes.
- Track allocation state with metadata tags placed before user areas.
- Include `libft` as the project utility library allowed by the assignment.
- Separate pointer validation, area lookup, and tag lookup for use by `free` and `realloc`.

## Current Structure

- `includes/ft_malloc.h`: Shared types, constants, global state, and function prototypes
- `src/malloc.c`: Free-tag lookup, box creation, and tag allocation/splitting for `malloc`
- `src/free.c`: `free` flow that finds the owning box/tag, releases it, and merges adjacent free tags
- `src/realloc.c`: Basic `realloc` implementation with pointer validation, shrinking, reallocation, and data copying
- `src/show_alloc_mem.c`: Per-zone allocation ranges, requested sizes, and total allocation output
- `src/boxes.c`: Helpers for zone classification, box-list access, and pointer-to-box lookup
- `src/support_malloc.c`: Helpers for box linking, initial tag creation, tag splitting, and tag linking
- `src/support_tags.c`: Tag/user address conversion and tag lookup inside a box
- `src/support_size.c`: Alignment, page-size, zone-payload, and box-size calculation
- `src/utils.c`: Overflow-safe addition and multiplication helpers
- `libft/`: Assignment libft extended with `ft_printf`, get_next_line, and related utilities
- `Makefile`: Build and cleanup rules for libft and the allocator shared library
- `.clangd`: Include path configuration for `includes/` and `libft/`

## Build

The root Makefile builds `libft/libft.a` first, then links the allocator objects into a shared library. When `HOSTTYPE` is unset or passed as an empty environment or command-line value, it falls back to a value composed from `uname -m` and `uname -s`. On Apple Silicon macOS, for example, the build produces:

- `libft_malloc_arm64_Darwin.so`: Host-specific shared library
- `libft_malloc.so`: Symbolic link to the host-specific file

The linker uses `-dynamiclib` on macOS and `-shared` on other operating systems. The standard targets are:

```sh
make          # Build libft and the allocator
make clean    # Remove allocator and libft objects
make fclean   # Remove objects, libft.a, shared library, and symlink
make re       # Clean everything and rebuild
```

A host name can also be supplied explicitly, as in `make HOSTTYPE=custom`. Passing an empty value with `make HOSTTYPE=` invokes automatic host detection. Running `make` again with no changes does not rebuild the outputs.

## Memory Model

The current design is centered around `box` and `tag` metadata.

- `t_box`: Represents a memory area obtained with mmap.
- `t_tag`: Metadata stored before the user-visible allocation area.
- `capacity`: Aligned payload capacity managed by a tag.
- `origin_size`: Original user-requested size used for output and statistics.
- `t_malloc_state`: Global state that tracks TINY, SMALL, and LARGE box lists.
- `TAG_MAGIC`: Magic value used to validate tag metadata.
- `ALIGNMENT`: 16-byte alignment used for payload and metadata placement.
- `TINY_MAX`: Requests up to 128 bytes are classified as TINY.
- `SMALL_MAX`: Requests up to 1024 bytes are classified as SMALL.

## Implemented So Far

- Global `g_malloc` state initialization
- `ZONE_TINY`, `ZONE_SMALL`, and `ZONE_LARGE` classification by requested size
- Access to the box list for a given zone type
- Pointer range checks against a box
- Lookup of the box that owns a user pointer across the whole box pool
- Address conversion between a tag and its user area
- Lookup of the tag that corresponds to a user pointer inside a box
- Basic `free(ptr)` behavior that clears the requested size and marks a valid tag as free
- Adjacent free-tag coalescing with the previous and next tags inside the same box
- OS-specific page-size lookup with a fallback
- Overflow-safe addition and multiplication helpers
- mmap box-size calculation based on request size and zone type
- mmap-backed box creation with an initial free tag
- Flow for connecting a new box to the global box list
- Reusable free-tag lookup across existing box lists
- LARGE requests skip free-tag reuse and allocate a dedicated box per request
- Fully freed LARGE boxes are removed from the box list and released with `munmap`
- Fully freed TINY/SMALL boxes are released with `munmap` when another box remains in the same zone
- Aligned tag allocation and splitting when enough space remains
- Tag splitting is limited to TINY/SMALL-sized requests
- Separate storage for original request size and aligned capacity
- TINY/SMALL/LARGE allocation ranges and total requested-size output
- Allocation-free output through the extended libft `ft_printf`
- `realloc(NULL, size)` follows the same path as `malloc(size)`
- Invalid or already freed pointers return `NULL`
- Shrinks within the existing capacity retain the same pointer and update the requested size
- Growth beyond capacity allocates a new area, copies existing data, then frees the old area
- A failed growth allocation returns `NULL` and preserves the old allocation
- `realloc(ptr, 0)` currently allocates a new 1-byte area before freeing the old area

## Status

Box creation, TINY/SMALL free-tag reuse and splitting, dedicated LARGE box allocation, conditional `munmap` for empty boxes, alignment, basic `free`, adjacent free-tag coalescing, basic `realloc`, and `show_alloc_mem` output are now implemented. The current sources pass a `-Wall -Wextra -Werror` syntax check, and the root Makefile builds the shared library.

The `realloc` checks cover NULL, shrinking within capacity, growth with data copying, size 0, preservation of the old allocation when a `SIZE_MAX` request fails, invalid pointers, and TINY/SMALL/LARGE boundaries. Each case passed five repeated runs. The `make`, `clean`, `fclean`, `re`, and no-change incremental build flows were also verified, along with the shared library exports for the required `malloc`, `free`, `realloc`, and `show_alloc_mem` symbols.

The main remaining work is thread safety and bonus functionality. In-place `realloc` growth and splitting unused capacity after a shrink remain optimization tasks. The official `norminette` is not installed yet. Manual checks pass the C/H 80-column limit, function length, argument/variable counts, and functions-per-file limits, but `includes/ft_malloc.h` and `src/*.c` do not yet have standard 42 file headers.

## show_alloc_mem Checks

The current implementation prints each active tag's start and end address, `origin_size`, and the total requested size grouped by TINY, SMALL, and LARGE. Basic checks covered `malloc(10)`, `malloc(200)`, `malloc(2000)`, `malloc(0)`, consecutive allocations in the same zone, multiple boxes, partial and full frees, empty lists, and 64-bit `size_t` output.

`malloc(0)` is normalized internally to a 1-byte request, so its `origin_size` is also printed as 1. `free()` resets `origin_size` to 0 and coalesces adjacent free tags inside the same box. LARGE requests are currently excluded from reusable tag lookup, allocate a fresh box each time, are not split, and are removed from the box list and released with `munmap` once freed. TINY/SMALL boxes are also eligible for `munmap` when the whole box is free and another box remains in the same zone. The latest checks printed addresses in increasing order, but box lists still follow creation order, so sorted output when `mmap` returns non-sequential addresses remains something to revisit.
