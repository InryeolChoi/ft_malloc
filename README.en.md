# ft_malloc

`ft_malloc` is a memory allocator project for implementing the behavior of `malloc`, `free`, and `realloc` directly in C.

## Goals

- Understand the core structure of a dynamic memory allocator.
- Manage metadata for allocated and free memory blocks.
- Work directly with the flow between system memory and user allocation requests.

## Current Structure

- `new.h`: Defines the memory block metadata structure
- `new.c`: Implementation file
- `Makefile`: Build rules to be added

## Status

The project is currently in its initial setup phase. Memory region management, block splitting and coalescing, and allocation/free APIs will be added step by step.
