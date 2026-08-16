# ft_malloc

`ft_malloc` is a memory allocator project for implementing `malloc`, `free`,
`realloc`, `show_alloc_mem`, and `show_alloc_mem_ex` directly in C.

## Goals

- Implement dynamic allocation flow with `mmap` and `munmap`.
- Manage memory areas by TINY, SMALL, and LARGE size classes.
- Track allocation state with metadata tags placed before user areas.
- Include `libft` as the project utility library allowed by the assignment.
- Separate pointer validation, area lookup, and tag lookup for use by `free` and `realloc`.

## Current Structure

- `includes/ft_malloc.h`: Shared types, constants, global state, and semantically grouped function prototypes
- `src/malloc.c`: Free-tag lookup, box creation, and tag allocation/splitting for `malloc`
- `src/free.c`: `free` flow that finds the owning box/tag, releases it, and merges adjacent free tags
- `src/realloc.c`: Pointer validation, in-place growth/shrink splitting, plus
  reallocation and data copying when moving between zones
- `src/show_alloc_mem.c`: Per-zone boxes selected by address, allocation ranges, requested sizes, and total allocation output
- `src/show_alloc_mem_ex.c`: Hexadecimal dumps of live user areas in 16-byte rows
- `src/boxes.c`: Helpers for zone classification, box-list access, and pointer-to-box lookup
- `src/support_malloc.c`: Helpers for box linking, initial tag creation, tag splitting, and tag linking
- `src/support_tags.c`: Tag/user address conversion and tag lookup inside a box
- `src/support_size.c`: Alignment, overflow checks, box-content, and final box-size calculation
- `src/utils_macos.c`: Page-size lookup for macOS
- `src/utils_linux.c`: Page-size lookup for Linux
- `src/support_thread.c`: Per-zone pthread mutex initialization and lock/unlock control
- `src/support_debug.c`: Malloc debug variables, `free` diagnostics, and Scribble handling
- `src/support_history.c`: Allocation-history recording, ring traversal, and output
- `libft/`: Assignment libft extended with `ft_printf`, get_next_line, and related utilities
- `Makefile`: Build and cleanup rules for libft and the allocator shared library
- `.clangd`: Include path configuration for `includes/` and `libft/`

## Build

The root Makefile builds `libft/libft.a` first, then links the allocator objects into a shared library. Allocator compilation and linking use `-pthread` for the ongoing pthread-based thread-safety work. When `HOSTTYPE` is unset or passed as an empty environment or command-line value, it falls back to a value composed from `uname -m` and `uname -s`. On Apple Silicon macOS, for example, the build produces:

- `libft_malloc_arm64_Darwin.so`: Host-specific shared library
- `libft_malloc.so`: Symbolic link to the host-specific file

The Makefile selects `utils_macos.c` with `-dynamiclib` on macOS and
`utils_linux.c` with `-shared` on Linux. Unsupported operating systems stop
the build. The standard targets are:

```sh
make          # Build libft and the allocator
make clean    # Remove allocator and libft objects
make fclean   # Remove objects, libft.a, shared library, and symlink
make re       # Clean everything and rebuild
```

A host name can also be supplied explicitly, as in `make HOSTTYPE=custom`. Passing an empty value with `make HOSTTYPE=` invokes automatic host detection. Running `make` again with no changes does not rebuild the outputs.

`libft/Makefile` now generates and includes `.d` dependency files with `-MMD -MP`. This allows related objects to rebuild when libft headers change, and `clean` removes both `.o` and `.d` files.

## Platform Compatibility Checks

Each Linux build was compiled from scratch with `make re` inside its Docker
distribution. The resulting shared library was injected with `LD_PRELOAD` and
tested with `malloc`, `malloc(0)`, `realloc`, `free`, and four concurrent
workers performing 2,000 allocation cycles each.

| Environment | Architecture / compiler | Result |
| --- | --- | --- |
| Alpine Linux | ARM64 / GCC 15.2, musl | Passed |
| Ubuntu 24.04.4 LTS | ARM64 / GCC 13.3 | Passed |
| Debian 12 Bookworm | ARM64 / GCC 12.2 | Passed |
| Kali Linux Rolling | ARM64 / GCC 15.3 | Passed |
| Red Hat Enterprise Linux 9.8 UBI | ARM64 / GCC 11.5 | Passed |
| Linux Mint 22.3 community image | AMD64 / GCC 13.3 | Passed with the limitation below |

Despite its name, the Mint community image identifies itself as Ubuntu 24.04
inside the container and does not provide `/etc/linuxmint/info`. Its result is
therefore only a compatibility reference for the Ubuntu base used by Mint, not
an independent check on an installed Mint system. Arch Linux has not yet been
run directly.

## Memory Model

The current design is centered around `box` and `tag` metadata.

- `t_box`: Represents a memory area obtained with mmap.
- `t_tag`: Metadata stored before the user-visible allocation area.
- `capacity`: Aligned user-area capacity managed by a tag.
- `origin_size`: Original user-requested size used for output and statistics.
- `t_malloc_state`: Global state that tracks TINY, SMALL, and LARGE box lists.
- `t_thread_state`: Holds pthread mutexes for TINY, SMALL, LARGE, and allocation history.
- `TAG_MAGIC`: Magic value used to validate tag metadata.
- `ALIGNMENT`: 16-byte alignment used for user-area and metadata placement.
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
- OS-specific page-size lookup with `getpagesize` on macOS and
  `sysconf(_SC_PAGESIZE)` on Linux
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
- TINY/SMALL/LARGE boxes selected in ascending `uintptr_t` address order, with allocation ranges and total requested-size output
- `find_next_box` repeatedly selects the lowest-address box above the previously printed box, independent of creation order
- O(n^2) time and O(1) extra memory for box ordering, without temporary allocations or a printed flag in `t_box`
- Allocation-free output through the extended libft `ft_printf`
- `realloc(NULL, size)` follows the same path as `malloc(size)`
- Invalid or already freed pointers return `NULL`
- Shrinks within the existing capacity retain the same pointer; TINY/SMALL
  allocations split sufficient aligned remainder into a free tag and coalesce it
  with the following free tag
- LARGE shrink keeps the dedicated box as a single-tag allocation
- Growth beyond capacity can retain the pointer and existing data by absorbing
  an adjacent free tag when the request remains in the same TINY/SMALL zone
- After an in-place merge, enough unused capacity for a `t_tag` and minimum user
  area is split back into a free tag
- Cross-zone growth or insufficient adjacent space allocates a new area, copies
  the existing data, then frees the old area
- A failed growth allocation returns `NULL` and preserves the old allocation
- `realloc(ptr, 0)` currently allocates a new 1-byte area before freeing the old area
- `control_mutex(type, action)` selects `MUTEX_LOCK` or `MUTEX_UNLOCK` for a zone mutex and rejects invalid types or actions
- The box/tag lookup and mutation path in `malloc` is protected by its zone mutex
- `free` and `realloc` acquire the owning zone mutex through `find_box_and_lock`
- `realloc` unlocks before nested `malloc` or `free` calls and preserves the
  original allocation when growth fails
- `show_alloc_mem` and `show_alloc_mem_ex` lock all three zones while reading
  allocator state
- `show_alloc_mem_ex` dumps only live user areas, limited to `origin_size`, in
  rows of at most 16 two-digit hexadecimal bytes
- `FT_MALLOC_DEBUG=1` reports pointers outside the allocator or not at an
  allocation start as invalid frees and reports detectable repeated frees
- `FT_MALLOC_SCRIBBLE=1` fills new user areas with `0xAA`, released user areas
  with `0x55`, and applies the same policy to in-place realloc resizing
- `FT_MALLOC_HISTORY=1` records `malloc`, `free`, and `realloc` events in a
  fixed 256-entry ring buffer and prints retained entries in oldest-first order
  at the end of `show_alloc_mem_ex`

## Status

Mandatory implementation and submission verification are complete. The
allocator implements box creation, TINY/SMALL reuse and splitting, dedicated
LARGE mappings, transactional `munmap`, alignment, coalescing, `realloc`, and
address-ordered `show_alloc_mem` output. Bonus work includes zone-level thread
safety, free-tag defragmentation, and `show_alloc_mem_ex` hexadecimal dumps.
Basic invalid-free and double-free diagnostics controlled by
`FT_MALLOC_DEBUG=1` and Scribble patterns controlled by
`FT_MALLOC_SCRIBBLE=1` are also implemented. Allocation-history recording and
output are protected by a dedicated mutex.

The empty/one/five/mixed/tail `find_next_box` cases and deterministic ascending-address output from `print_boxes`/`show_alloc_mem` each passed five repeated runs. The malloc/free/realloc regression checks for zone classification, splitting, reuse, coalescing, LARGE `munmap`, and realloc also passed five repeated runs each. The shared library exports the required `malloc`, `free`, `realloc`, and `show_alloc_mem` symbols, and there were zero timeouts.

The includes and prototypes in `includes/ft_malloc.h` are organized as public API, size/overflow, box/list, tag/allocation, display, free/coalescing, and realloc helpers. The header passes the syntax and `-Wmissing-prototypes` checks.

Standard 42 headers are present in allocator files, and `libft/Makefile` uses
`.d` dependency files. Official Norminette 3.3.59 reports zero Errors and four
global-variable Notices. Same-zone in-place `realloc` growth absorbs an adjacent
free tag and splits reusable excess capacity while preserving existing data and
`FT_MALLOC_SCRIBBLE` behavior. TINY/SMALL shrink splits reusable aligned
capacity and coalesces adjacent free tags, while LARGE keeps its dedicated
single-tag box. Cross-zone growth falls back to a new allocation. Dedicated
shrink checks covered splitting, coalescing/reuse, LARGE behavior, Scribble, and
20 repeated runs.

## show_alloc_mem Checks

The current implementation prints each active tag's start and end address, `origin_size`, and the total requested size grouped by TINY, SMALL, and LARGE. Basic checks covered `malloc(10)`, `malloc(200)`, `malloc(2000)`, `malloc(0)`, consecutive allocations in the same zone, multiple boxes, partial and full frees, empty lists, and 64-bit `size_t` output.

`malloc(0)` is normalized internally to a 1-byte request, so its `origin_size` is also printed as 1. `free()` resets `origin_size` to 0 and coalesces adjacent free tags inside the same box. LARGE requests are currently excluded from reusable tag lookup, allocate a fresh box each time, are not split, and are removed from the box list and released with `munmap` once freed. TINY/SMALL boxes are also eligible for `munmap` when the whole box is free and another box remains in the same zone.

For each zone, `find_next_box` scans the full box list for the lowest `uintptr_t` address above the previously selected box. This selects boxes in ascending address order regardless of box creation order or the order of addresses returned by `mmap`. For n boxes in that zone, ordering takes O(n^2) time and O(1) extra memory, without temporary allocations or a printed flag in `t_box`. The empty/one/five/mixed/tail selection cases and deterministic ascending-address output each passed five repeated runs.

## show_alloc_mem_ex Checks

`show_alloc_mem_ex` skips free tags and alignment slack, then prints exactly
`origin_size` bytes from each live user area. Every byte uses two hexadecimal
digits, and each row contains at most 16 bytes with its starting address.

Checks covered 1, 15, 16, and 17-byte boundaries, concurrent TINY/SMALL/LARGE
allocations, removal of freed allocations from output, and data preservation
after `realloc`. A stress run used six workers performing 2,400 combined
malloc/realloc/free operations while calling `show_alloc_mem_ex` 80 times. Ten
repeated runs completed 10/10 without deadlocks, corruption, or stale locks.

## Allocation History

Allocation history is enabled only when `FT_MALLOC_HISTORY` is exactly `1`.
If the variable is unset or has another value, history recording and history
output are skipped.

```sh
FT_MALLOC_HISTORY=1 ./program
```

Without allocating additional memory, the allocator stores history in a fixed
256-entry ring buffer inside `g_malloc`. Each entry contains an increasing
sequence number, the event type, the old pointer, the new pointer, and the
user-requested size. Once the buffer is full, new events overwrite the oldest
entries. Retained entries are printed from oldest to newest.

Successful `malloc`, `free`, and `realloc` paths record their corresponding
events. A moving `realloc` calls `malloc` and `free` internally, so their
detailed events may appear alongside the final `realloc` event. This is the
current policy for preserving internal allocator activity.

The ring buffer uses a dedicated history mutex, separate from the zone mutexes.
After finishing its hexadecimal dump and releasing the zone mutexes,
`show_alloc_mem_ex` prints the history at the end while holding the history
mutex.

Dedicated checks covered the environment variable's enabled and disabled
states, in-place and moving realloc, realloc with size zero, a 600-event wrap
into the 256-entry ring, and concurrent recording by eight workers. Each case
passed ten runs; retained output stayed ordered from `#344` through `#599`
without deadlocks, timeouts, or tag-link corruption.

## Malloc Debug Environment

`free` diagnostics are written to standard error only when
`FT_MALLOC_DEBUG` is exactly `1`. If the variable is unset or has another
value, invalid pointers are silently ignored as before.

```sh
FT_MALLOC_DEBUG=1 ./program
```

Addresses outside allocator boxes and interior addresses that are not exact
allocation starts produce `invalid free`. Releasing a free tag that is still
tracked produces `double free`. Tests against the shared library confirmed
zero output while disabled and two invalid-free messages plus one double-free
message while enabled.

`FT_MALLOC_SCRIBBLE=1` fills a new allocation with `0xAA` and fills its user
area with `0x55` before normal `free` clears the metadata. In-place realloc
growth fills the newly exposed tail with `0xAA`; shrinking fills the released
tail with `0x55`. A realloc that moves to a new allocation fills the new area
first, then copies the old contents, leaving the extended portion as `0xAA`.

```sh
FT_MALLOC_SCRIBBLE=1 ./program
```

Shared-library tests covered the `0xAA`/`0x55` patterns for a 17-byte
allocation, in-capacity realloc growth and shrink, moved realloc growth, and
independence between the DEBUG and SCRIBBLE variables.

One possible follow-up debug option is intentionally not implemented yet:

- `FT_MALLOC_ABORT_ON_ERROR=1`: call `abort()` after reporting an invalid or
  double free so a debugger stops at the failure.
