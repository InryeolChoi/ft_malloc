# ft_malloc

`ft_malloc` is a memory allocator project for implementing `malloc`, `free`, `realloc`, and `show_alloc_mem` directly in C.

## Goals

- Implement dynamic allocation flow with `mmap` and `munmap`.
- Manage memory areas by TINY, SMALL, and LARGE size classes.
- Track allocation state with metadata tags placed before user areas.
- Include `libft` as the project utility library allowed by the assignment.
- Separate pointer validation, area lookup, and tag lookup as the foundation for `free` and `realloc`.

## Current Structure

- `includes/ft_malloc.h`: Shared types, constants, global state, and function prototypes
- `src/malloc.c`: Free-tag lookup, box creation, and tag allocation/splitting for `malloc`
- `src/free.c`: `free` flow that finds the owning box/tag, releases it, and merges adjacent free tags
- `src/realloc.c`: Current `realloc` stub
- `src/show_alloc_mem.c`: Per-zone allocation ranges, requested sizes, and total allocation output
- `src/boxes.c`: Helpers for zone classification, box-list access, and pointer-to-box lookup
- `src/support_malloc.c`: Helpers for box linking, initial tag creation, tag splitting, and tag linking
- `src/support_tags.c`: Tag/user address conversion and tag lookup inside a box
- `src/support_size.c`: Alignment, page-size, zone-payload, and box-size calculation
- `src/utils.c`: Overflow-safe addition and multiplication helpers
- `libft/`: Assignment libft extended with `ft_printf`, get_next_line, and related utilities
- `.clangd`: Include path configuration for `includes/` and `libft/`

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
- Aligned tag allocation and splitting when enough space remains
- Tag splitting is limited to TINY/SMALL-sized requests
- Separate storage for original request size and aligned capacity
- TINY/SMALL/LARGE allocation ranges and total requested-size output
- Allocation-free output through the extended libft `ft_printf`

## Status

Box creation, TINY/SMALL free-tag reuse, TINY/SMALL tag splitting, dedicated LARGE box allocation, alignment, basic `free`, adjacent free-tag coalescing, and `show_alloc_mem` output are now implemented. The current sources pass a `-Wall -Wextra -Werror` syntax check.

The allocator is still in progress. Next steps include releasing unused boxes, including LARGE boxes, with `munmap`, implementing `realloc`, adding concurrency protection, and completing the build and test environment.

## show_alloc_mem Checks

The current implementation prints each active tag's start and end address, `origin_size`, and the total requested size grouped by TINY, SMALL, and LARGE. Basic checks covered `malloc(10)`, `malloc(200)`, `malloc(2000)`, `malloc(0)`, consecutive allocations in the same zone, multiple boxes, partial and full frees, empty lists, and 64-bit `size_t` output.

`malloc(0)` is normalized internally to a 1-byte request, so its `origin_size` is also printed as 1. `free()` resets `origin_size` to 0 and coalesces adjacent free tags inside the same box. LARGE requests are currently excluded from reusable tag lookup, allocate a fresh box each time, and are not split. The latest checks printed addresses in increasing order, but box lists still follow creation order, so sorted output when `mmap` returns non-sequential addresses remains something to revisit.
