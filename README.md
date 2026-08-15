# ft_malloc
[English version](README.en.md)

`ft_malloc`는 C로 `malloc`, `free`, `realloc`, `show_alloc_mem`,
`show_alloc_mem_ex`를 직접 구현하는 메모리 할당자 프로젝트입니다.

## 목표

- `mmap`/`munmap` 기반으로 동적 메모리 할당 흐름을 구현합니다.
- TINY, SMALL, LARGE 크기 구간별 메모리 영역을 관리합니다.
- 사용자 영역 앞에 붙는 메타데이터 태그를 통해 할당 상태를 추적합니다.
- `libft`를 함께 포함해 프로젝트 내부 유틸리티로 사용할 수 있게 구성합니다.
- 포인터 검증, 영역 탐색, 태그 탐색을 분리해 `free`와 `realloc`에 사용합니다.

## 현재 구성

- `includes/ft_malloc.h`: 공통 타입, 상수, 전역 상태와 의미별로 그룹화한 함수 프로토타입 정의
- `src/malloc.c`: free tag 탐색, box 생성, tag 할당/분할 기반 `malloc` 구현
- `src/free.c`: 포인터가 속한 box/tag를 찾아 해제하고 인접 free tag를 병합하는 `free`
- `src/realloc.c`: 포인터 검증, 같은 zone의 제자리 확장과 남은 공간 분할,
  zone 이동 시 재할당 및 데이터 복사를 처리하는 `realloc`
- `src/show_alloc_mem.c`: zone별 box를 주소순으로 선택해 할당 범위, 요청 크기, 전체 할당량 출력
- `src/show_alloc_mem_ex.c`: 사용 중인 user area를 16바이트 단위 hexadecimal dump로 출력
- `src/boxes.c`: zone type 판별, box list 접근, 포인터가 속한 box 탐색 헬퍼
- `src/support_malloc.c`: box 연결, 최초 tag 생성, tag split 및 연결 헬퍼
- `src/support_tags.c`: tag와 user area 사이의 주소 변환, box 내부 tag 탐색
- `src/support_size.c`: 정렬, 페이지 크기, box content와 최종 box 크기 계산
- `src/utils.c`: overflow 방지 덧셈/곱셈 헬퍼
- `src/support_thread.c`: zone별 pthread mutex 초기화와 lock/unlock 제어
- `src/support_debug.c`: malloc debug 환경변수, `free` 진단 및 Scribble 처리
- `libft/`: `ft_printf`, get_next_line 등을 포함한 과제용 libft
- `Makefile`: libft와 allocator shared library 빌드 및 정리 규칙
- `.clangd`: `includes/`, `libft/` include path 설정

## Build

루트 Makefile은 먼저 `libft/libft.a`를 빌드한 뒤 allocator object를 shared library로 연결합니다. pthread 기반 thread safety 작업을 위해 allocator의 컴파일과 링크에는 `-pthread`를 사용합니다. `HOSTTYPE`이 설정되지 않았거나 환경 변수 또는 명령행에서 빈 값으로 전달되면 `uname -m`과 `uname -s`를 조합한 값으로 대체합니다. 예를 들어 Apple Silicon macOS에서는 다음 산출물이 생성됩니다.

- `libft_malloc_arm64_Darwin.so`: host별 실제 shared library
- `libft_malloc.so`: 위 파일을 가리키는 심볼릭 링크

macOS에서는 `-dynamiclib`, 그 외 운영체제에서는 `-shared`로 연결합니다. 기본 사용법은 다음과 같습니다.

```sh
make          # libft와 allocator 빌드
make clean    # allocator/libft object 정리
make fclean   # object, libft.a, shared library와 symlink 정리
make re       # 전체 정리 후 다시 빌드
```

`make HOSTTYPE=custom`처럼 host 이름을 직접 지정할 수도 있습니다. 빈 값을 명시한 `make HOSTTYPE=`은 자동 감지 fallback을 사용합니다. 변경 사항이 없는 상태에서 `make`를 다시 실행하면 산출물을 재빌드하지 않습니다.

`libft/Makefile`은 `-MMD -MP`로 `.d` 의존성 파일을 생성하고 포함합니다. 따라서 libft 헤더가 바뀌면 관련 object를 다시 빌드할 수 있으며, `clean`은 `.o`와 `.d` 파일을 함께 정리합니다.

## 메모리 모델

현재 구조는 `box`와 `tag`를 중심으로 잡혀 있습니다.

- `t_box`: mmap으로 확보한 메모리 영역을 나타냅니다.
- `t_tag`: 사용자에게 반환되는 영역 앞에 위치하는 메타데이터입니다.
- `capacity`: 정렬 후 실제로 tag가 관리하는 user area 크기입니다.
- `origin_size`: 사용자가 원래 요청한 크기로, 출력과 통계에 사용합니다.
- `t_malloc_state`: TINY, SMALL, LARGE box list를 전역으로 관리합니다.
- `t_thread_state`: TINY, SMALL, LARGE별 pthread mutex를 보유합니다.
- `TAG_MAGIC`: tag 무결성을 확인하기 위한 magic value입니다.
- `ALIGNMENT`: user area와 메타데이터 배치에 사용하는 16-byte 정렬 기준입니다.
- `TINY_MAX`: 128 bytes 이하 요청을 TINY로 분류합니다.
- `SMALL_MAX`: 1024 bytes 이하 요청을 SMALL로 분류합니다.

## 구현된 부분

- `g_malloc` 전역 상태 초기화
- 요청 크기에 따른 `ZONE_TINY`, `ZONE_SMALL`, `ZONE_LARGE` 분류
- zone type에 맞는 box list 접근
- 포인터가 어떤 box 범위 안에 있는지 확인
- 전체 box pool에서 포인터가 속한 box 탐색
- tag와 user area 사이의 주소 변환
- box 내부에서 user pointer에 대응되는 tag 탐색
- `free(ptr)`에서 유효한 tag를 찾아 요청 크기를 초기화하고 `is_free` 표시
- 해제된 tag의 이전/다음 tag가 free 상태라면 같은 box 안에서 병합
- OS별 페이지 크기 조회와 fallback 처리
- overflow를 피하기 위한 덧셈/곱셈 helper
- 요청 크기와 zone type에 따른 mmap box 크기 계산
- `create_box`를 통한 mmap 영역 생성 및 첫 free tag 초기화
- box를 전역 box list에 연결하는 흐름
- 기존 box list에서 재사용 가능한 free tag 탐색
- LARGE 요청은 기존 free tag 재사용 없이 요청마다 별도 box 생성
- LARGE box가 완전히 free 상태가 되면 box list에서 제거하고 `munmap`
- TINY/SMALL box가 완전히 free 상태이고 같은 zone에 다른 box가 남아 있으면 box list에서 제거하고 `munmap`
- `unmap_box`는 zone lock을 유지한 채 `next_box`와 type을 보존하고
  `munmap`을 먼저 호출하며, 성공한 경우에만 head 또는 이전 box의 link를 갱신
- `munmap`이 실패하면 box list를 변경하지 않아 allocator가 해당 box를 계속 추적
- 정렬된 요청 크기를 기준으로 tag 할당 및 남은 공간 분할
- tag split은 TINY/SMALL 크기 요청에서만 수행
- 원래 요청 크기와 정렬된 capacity를 분리해 저장
- `show_alloc_mem`에서 TINY/SMALL/LARGE별 box를 `uintptr_t` 주소 오름차순으로 선택하고 주소 범위와 전체 요청 크기 출력
- `find_next_box`가 직전에 출력한 box보다 주소가 큰 box 중 최솟값을 매번 다시 찾아 생성 순서와 무관하게 출력
- 임시 allocation이나 `t_box` 출력 플래그 없이 box 정렬에 O(n^2) 시간과 O(1) 추가 메모리 사용
- 확장된 libft의 `ft_printf`를 동적 할당 없이 출력에 사용
- `realloc(NULL, size)`를 `malloc(size)`와 같은 흐름으로 처리
- 유효하지 않거나 이미 해제된 포인터에 대해 `NULL` 반환
- 기존 capacity 안의 축소 요청은 같은 포인터를 유지하며 요청 크기 갱신
- capacity를 넘더라도 같은 TINY/SMALL zone의 바로 다음 tag가 free이고 합친
  공간이 충분하면 해당 tag를 흡수해 포인터와 기존 데이터를 유지하며 제자리 확장
- 병합 후 `t_tag`와 최소 user area를 만들 공간이 남으면 이를 다시 free tag로 분할
- 다른 zone으로 커지거나 인접 공간이 부족하면 새 영역을 할당하고 기존 데이터를
  복사한 뒤 이전 영역 해제
- 확장용 새 할당이 실패하면 `NULL`을 반환하고 기존 영역 보존
- `realloc(ptr, 0)`은 현재 1 byte 영역을 새로 할당한 뒤 기존 영역 해제
- `control_mutex(type, action)`로 zone mutex의 `MUTEX_LOCK` 또는
  `MUTEX_UNLOCK` 동작을 선택하고 잘못된 type/action은 거부
- `malloc`의 box/tag 탐색과 변경 경로를 해당 zone mutex로 보호
- `free`와 `realloc`은 `find_box_and_lock`으로 box를 찾고 해당 zone
  mutex의 소유권을 호출 경로가 이어받아 해제
- `realloc`은 중첩된 `malloc`/`free` 호출 전에 기존 zone mutex를 해제하고,
  잠금 상태에서 `copy_size`를 저장해 해제 후 tag를 다시 읽지 않음
- 새 영역 할당에 실패한 `realloc`은 기존 allocation을 그대로 보존
- 더 이상 사용하지 않는 `find_box_pool`을 제거하고 box 탐색을
  `find_box_and_lock`으로 통일
- `show_alloc_mem`은 출력 중 세 zone mutex를 모두 잠가 일관된 상태를 순회
- `show_alloc_mem_ex`는 사용 중인 tag의 user area를 `origin_size`만큼
  16바이트 단위 hexadecimal dump로 출력
- `show_alloc_mem_ex`도 세 zone mutex를 잠가 free 또는 `munmap`과 동시에
  메모리를 읽지 않도록 보호
- `FT_MALLOC_DEBUG=1`일 때 allocator 밖의 포인터, allocation 시작점이 아닌
  포인터를 invalid free로 출력하고 탐지 가능한 재해제를 double free로 출력
- `FT_MALLOC_SCRIBBLE=1`일 때 새 user area를 `0xAA`, 해제되는 user area를
  `0x55`로 채우고 제자리 realloc의 확장·축소 영역에도 같은 정책 적용

## 진행 상태

현재 `malloc`의 box 생성, TINY/SMALL free tag 재사용과 split, LARGE 전용 box 할당, 비어 있는 box의 조건부 `munmap`, 정렬 처리, 기본 `free`, 인접 free tag 병합, 기본 `realloc`, 주소순 `show_alloc_mem` 출력 흐름이 구현된 상태입니다. 전체 소스는 `-Wall -Wextra -Werror -Wmissing-prototypes` 문법 검사를 통과하며 루트 Makefile로 shared library를 빌드할 수 있습니다.

Mandatory 구현과 제출 검증을 완료했습니다. 새 `unmap_box`는 첫 번째, 중간,
마지막 box 제거와 주입된 `munmap` 실패 시 list 보존을 포함해 176/176
케이스를 통과했습니다. macOS `DYLD` flat namespace interposition에서
`malloc`, `free`, `realloc`, `show_alloc_mem` export와 경계값 동작을
확인했습니다. 공식 Norminette 3.3.59 결과는 Error 0건이며,
`GLOBAL_VAR_DETECTED` Notice 4건만 남았습니다. Bonus는 zone별 thread
safety, free tag 병합, `show_alloc_mem_ex` hexadecimal dump까지 구현했으며,
기본 malloc debug 진단과 Scribble 환경변수도 구현했습니다.

`find_next_box`의 empty/one/five/mixed/tail 케이스와 `print_boxes`/`show_alloc_mem`의 결정적 주소 오름차순 출력을 각각 5회 반복해 통과했습니다. malloc/free/realloc의 zone 분류, split, 재사용, 병합, LARGE `munmap`, realloc 회귀 검사도 각각 5회 반복해 통과했습니다. shared library의 `malloc`, `free`, `realloc`, `show_alloc_mem` 필수 심볼 export를 확인했으며 timeout 발생은 0건이었습니다.

`includes/ft_malloc.h`의 include와 원형은 public API, size/overflow, box/list, tag/allocation, display, free/coalescing, realloc helper 순으로 정리되어 있으며 문법 검사와 `-Wmissing-prototypes` 검사를 통과했습니다.

`includes/ft_malloc.h`, `src/*.c`, 루트 `Makefile`에는 표준 42 파일 헤더가 추가된 상태입니다. `libft/Makefile`은 `.d` 의존성 파일을 사용하도록 개선했습니다. bonus thread-safety 구현은 zone별 mutex를 사용하며, `malloc`, `free`, `realloc`은 필요한 zone만 잠급니다. `realloc`은 `find_box_and_lock`이 넘긴 잠금 소유권을 각 반환 경로에서 정리하고, 중첩된 `malloc`/`free` 호출 전에는 잠금을 해제합니다. 같은 TINY/SMALL zone에서는 인접 free tag를 흡수해 제자리 확장하고, 병합 후 충분한 공간이 남으면 free tag로 다시 분할합니다. 다른 zone으로 커지는 요청은 새 allocation으로 이동하며 기존 데이터와 `FT_MALLOC_SCRIBBLE` 동작을 보존합니다. `show_alloc_mem`은 세 zone을 모두 잠근 동안 출력합니다. `realloc` 기본 동작은 60/60 케이스를 통과했고, 6회의 멀티스레드 스트레스 검사에서 allocator 작업 54,000회, `show_alloc_mem` 호출 720회, 무결성 검사 179,910회를 오류 없이 완료했습니다. 축소 후 남은 capacity split은 향후 범위입니다.

## Thread Safety Status

- `malloc`, `free`, and `realloc` protect only the zone they access.
- `free` and `realloc` use `find_box_and_lock`, then release the acquired zone
  mutex on every exit path that owns it.
- `realloc` unlocks before nested `malloc` or `free` calls to avoid recursive
  locking. It captures `copy_size` while locked and does not read the old tag
  after releasing the mutex.
- If a new allocation fails, `realloc` returns `NULL` and preserves the original
  allocation.
- `show_alloc_mem` locks all three zones while traversing and printing allocator
  state.
- `show_alloc_mem_ex` uses the same all-zone lock while dumping live user areas.
- `unmap_box` keeps the zone locked, saves the next box and zone type, and calls
  `munmap` before changing the list. On failure the list remains unchanged; on
  success it updates either the list head or the previous box link.

The `realloc` locking flow passed all 60 basic semantic cases and six stress
runs totaling 54,000 allocator operations, 720 `show_alloc_mem` calls, and
179,910 integrity checks, with no deadlocks, crashes, corruption, or stale
locks.

## Project Progress

Mandatory implementation and submission verification are complete. The new
`unmap_box` path passed 176/176 cases covering first, middle, and last box
removal, plus injected `munmap` failure with list preservation. Actual macOS
`DYLD` flat-namespace interposition passed with exported `malloc`, `free`,
`realloc`, and `show_alloc_mem` symbols and boundary-size checks. Official
Norminette 3.3.59 reported zero Errors and four `GLOBAL_VAR_DETECTED` Notices.
Bonus work currently includes zone-level thread safety, free-tag coalescing,
the `show_alloc_mem_ex` hexadecimal dump, and basic invalid/double-free reports
controlled by `FT_MALLOC_DEBUG=1`. `FT_MALLOC_SCRIBBLE=1` fills newly exposed
bytes with `0xAA` and released bytes with `0x55`. Within the same TINY/SMALL
zone, `realloc` can absorb an adjacent free tag, split reusable excess capacity,
and preserve both the pointer and existing data. Cross-zone growth uses a new
allocation.

## show_alloc_mem 검증

기초 구현은 TINY/SMALL/LARGE별 사용 중인 tag의 시작·끝 주소와 `origin_size`, 전체 요청 크기 합계를 출력합니다. `malloc(10)`, `malloc(200)`, `malloc(2000)`, `malloc(0)`, 같은 zone의 연속 할당, 여러 box, 중간 및 전체 `free`, 빈 목록, 64-bit `size_t` 출력을 검사했으며 컴파일과 런타임 검증을 통과했습니다.

현재 `malloc(0)`은 내부에서 1 byte 요청으로 정규화되어 `origin_size`도 1로 출력됩니다. `free()`는 tag를 free 상태로 바꾸면서 `origin_size`를 0으로 되돌리고, 같은 box 안에서 앞뒤로 맞닿은 free tag를 병합합니다. LARGE 요청은 현재 재사용 가능한 tag 탐색 대상에서 제외되어 매번 새 box를 확보하고, split도 수행하지 않으며, 해제 후 box 전체가 비면 box list에서 제거한 뒤 `munmap`합니다. TINY/SMALL box도 전체가 비고 같은 zone에 다른 box가 남아 있으면 `munmap` 대상이 됩니다.

각 zone의 box 출력은 `find_next_box`가 직전에 선택한 주소보다 큰 box 중 `uintptr_t` 기준 최솟값을 전체 box list에서 다시 찾는 방식입니다. 따라서 box 생성 순서나 `mmap` 반환 순서와 관계없이 주소 오름차순으로 선택하며, 해당 zone의 box 수를 n이라 할 때 O(n^2) 시간과 O(1) 추가 메모리를 사용합니다. 이 과정은 임시 allocation을 만들거나 `t_box`에 출력 여부 플래그를 추가하지 않습니다. empty/one/five/mixed/tail 선택과 결정적 주소 오름차순 출력은 각각 5회 반복 검사를 통과했습니다.

## show_alloc_mem_ex 검증

`show_alloc_mem_ex`는 free tag와 정렬 여유 공간을 제외하고, 사용 중인
user area를 `origin_size`만큼 두 자리 hexadecimal 값으로 출력합니다.
각 행은 최대 16바이트이며 행의 시작 주소를 함께 표시합니다.

1, 15, 16, 17바이트 경계, TINY/SMALL/LARGE 동시 출력, free된 allocation
제외, realloc 전후 데이터 보존과 총합을 검사했습니다. 6개 worker가 실행당
2,400회의 malloc/realloc/free를 수행하는 동안 확장 출력을 80회 호출하는
스트레스를 10회 반복했고, 교착·손상·stale lock 없이 10/10 통과했습니다.
공식 Norminette 3.3.59는 Error 0건과 전역변수 Notice 4건을 보고했습니다.

## malloc debug 환경변수

`FT_MALLOC_DEBUG`의 값이 정확히 `1`일 때만 `free` 오류를 표준 오류로
출력합니다. 환경변수가 없거나 다른 값이면 기존처럼 오류 포인터를 조용히
무시합니다.

```sh
FT_MALLOC_DEBUG=1 ./program
```

allocator의 box 밖 주소와 box 안의 잘못된 중간 주소는 `invalid free`로,
아직 관리 중인 free tag를 다시 해제하면 `double free`로 출력합니다. Debug
비활성 상태의 출력 0바이트와 활성 상태의 두 invalid free 및 한 double free를
실제 shared library로 확인했습니다.

`FT_MALLOC_SCRIBBLE=1`은 새 allocation을 `0xAA`로 채우고 정상 free에서
metadata를 지우기 전에 user area를 `0x55`로 채웁니다. 제자리 realloc은
확장된 꼬리를 `0xAA`, 축소로 소유권을 잃은 꼬리를 `0x55`로 채웁니다.
새 allocation이 필요한 realloc은 먼저 전체를 `0xAA`로 채운 뒤 기존 내용을
복사하므로 새로 늘어난 부분에는 `0xAA`가 남습니다.

```sh
FT_MALLOC_SCRIBBLE=1 ./program
```

17바이트 malloc/free의 `0xAA`/`0x55` 패턴, capacity 안의 realloc 확장과
축소, 새 allocation을 사용하는 확장, DEBUG와 SCRIBBLE 환경변수의 독립성을
실제 shared library로 확인했습니다.

향후 debug 후보는 다음과 같습니다.

- `FT_MALLOC_ABORT_ON_ERROR=1`: invalid free 또는 double free를 출력한 뒤
  `abort()`하여 debugger가 정확한 실패 지점에서 멈추게 합니다.
