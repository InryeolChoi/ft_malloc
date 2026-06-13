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
- `src/malloc.c`: Global malloc state, box creation, free-tag lookup, and tag allocation/splitting groundwork for `malloc`
- `src/free.c`: Basic `free` flow that finds the owning box/tag and marks the tag as free
- `src/realloc.c`: Current `realloc` stub
- `src/show_alloc_mem.c`: Placeholder for allocation-state output
- `src/boxes.c`: Helpers for zone classification, box-list access, and pointer-to-box lookup
- `src/tags.c`: Helpers for converting between tag addresses and user-area addresses, plus tag lookup inside a box
- `src/utils.c`: Helpers for page-size lookup, box-size calculation, and overflow-safe arithmetic
- `libft/`: Copied libft dependency allowed by the assignment
- `.clangd`: Include path configuration for `includes/` and `libft/`

## Memory Model

The current design is centered around `box` and `tag` metadata.

- `t_box`: Represents a memory area obtained with mmap.
- `t_tag`: Metadata stored before the user-visible allocation area.
- `t_malloc_state`: Global state that tracks TINY, SMALL, and LARGE box lists.
- `TAG_MAGIC`: Magic value used to validate tag metadata.
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
- Basic `free(ptr)` behavior that marks a valid tag as free
- OS-specific page-size lookup with a fallback
- Overflow-safe addition and multiplication helpers
- mmap box-size calculation based on request size and zone type
- mmap-backed box creation with an initial free tag
- Flow for connecting a new box to the global box list
- Lookup for reusable free tags
- Tag allocation with splitting when enough space remains

## Status

The repository setup, `libft` import, header design, `src/` file split, box/tag lookup helpers, the basic `free` flow, and an initial `malloc` box/tag allocation path are now in place.

The full allocator behavior is still in progress. Next steps include tightening new-box linking rules, hardening tag split edge cases, coalescing free tags, cleaning up prototypes, and completing `malloc`, `free`, `realloc`, and `show_alloc_mem`.
