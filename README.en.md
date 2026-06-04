# ft_malloc

`ft_malloc` is a memory allocator project for implementing `malloc`, `free`, `realloc`, and `show_alloc_mem` directly in C.

## Goals

- Implement dynamic allocation flow with `mmap` and `munmap`.
- Manage memory areas by TINY, SMALL, and LARGE size classes.
- Track allocation state with metadata tags placed before user areas.
- Include `libft` as the project utility library allowed by the assignment.

## Current Structure

- `includes/ft_malloc.h`: Shared types, constants, global state, and function prototypes
- `src/malloc.c`: Global malloc state and the starting point for `malloc`
- `src/free.c`: Placeholder for the `free` implementation
- `src/realloc.c`: Starting point for the `realloc` implementation
- `src/show_alloc_mem.c`: Placeholder for allocation-state output
- `src/boxes.c`: Helpers for zone classification, box-list access, and pointer-to-box lookup
- `src/tags.c`: Helpers for converting between tag addresses and user-area addresses
- `libft/`: Copied libft dependency allowed by the assignment

## Memory Model

The current design is centered around `box` and `tag` metadata.

- `t_box`: Represents a memory area obtained with mmap.
- `t_tag`: Metadata stored before the user-visible allocation area.
- `t_malloc_state`: Global state that tracks TINY, SMALL, and LARGE box lists.
- `TINY_MAX`: Requests up to 128 bytes are classified as TINY.
- `SMALL_MAX`: Requests up to 1024 bytes are classified as SMALL.

## Status

The repository setup, `libft` import, header design, `src/` file split, and part of the box/tag helper implementation are now in place.

The full allocation and deallocation flow is still in progress. Next steps include mmap area creation, free-tag lookup, tag splitting and coalescing, and completing `malloc`, `free`, `realloc`, and `show_alloc_mem`.
