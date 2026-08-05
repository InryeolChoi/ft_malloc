# ft_malloc
[English version](README.en.md)

`ft_malloc`는 C로 `malloc`, `free`, `realloc`, `show_alloc_mem`을 직접 구현하는 메모리 할당자 프로젝트입니다.

## 목표

- `mmap`/`munmap` 기반으로 동적 메모리 할당 흐름을 구현합니다.
- TINY, SMALL, LARGE 크기 구간별 메모리 영역을 관리합니다.
- 사용자 영역 앞에 붙는 메타데이터 태그를 통해 할당 상태를 추적합니다.
- `libft`를 함께 포함해 프로젝트 내부 유틸리티로 사용할 수 있게 구성합니다.
- 포인터 검증, 영역 탐색, 태그 탐색을 분리해 `free`와 `realloc`에 사용합니다.

## 현재 구성

- `includes/ft_malloc.h`: 공통 타입, 상수, 전역 상태, 함수 프로토타입 정의
- `src/malloc.c`: free tag 탐색, box 생성, tag 할당/분할 기반 `malloc` 구현
- `src/free.c`: 포인터가 속한 box/tag를 찾아 해제하고 인접 free tag를 병합하는 `free`
- `src/realloc.c`: 포인터 검증, 축소, 재할당과 데이터 복사를 처리하는 기본 `realloc` 구현
- `src/show_alloc_mem.c`: zone별 할당 주소와 요청 크기, 전체 할당량 출력
- `src/boxes.c`: zone type 판별, box list 접근, 포인터가 속한 box 탐색 헬퍼
- `src/support_malloc.c`: box 연결, 최초 tag 생성, tag split 및 연결 헬퍼
- `src/support_tags.c`: tag와 user area 사이의 주소 변환, box 내부 tag 탐색
- `src/support_size.c`: 정렬, 페이지 크기, zone payload와 box 크기 계산
- `src/utils.c`: overflow 방지 덧셈/곱셈 헬퍼
- `libft/`: `ft_printf`, get_next_line 등을 포함한 과제용 libft
- `Makefile`: libft와 allocator shared library 빌드 및 정리 규칙
- `.clangd`: `includes/`, `libft/` include path 설정

## Build

루트 Makefile은 먼저 `libft/libft.a`를 빌드한 뒤 allocator object를 shared library로 연결합니다. `HOSTTYPE`이 설정되지 않았거나 환경 변수 또는 명령행에서 빈 값으로 전달되면 `uname -m`과 `uname -s`를 조합한 값으로 대체합니다. 예를 들어 Apple Silicon macOS에서는 다음 산출물이 생성됩니다.

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

## 메모리 모델

현재 구조는 `box`와 `tag`를 중심으로 잡혀 있습니다.

- `t_box`: mmap으로 확보한 메모리 영역을 나타냅니다.
- `t_tag`: 사용자에게 반환되는 영역 앞에 위치하는 메타데이터입니다.
- `capacity`: 정렬 후 실제로 tag가 관리하는 payload 크기입니다.
- `origin_size`: 사용자가 원래 요청한 크기로, 출력과 통계에 사용합니다.
- `t_malloc_state`: TINY, SMALL, LARGE box list를 전역으로 관리합니다.
- `TAG_MAGIC`: tag 무결성을 확인하기 위한 magic value입니다.
- `ALIGNMENT`: payload와 메타데이터 배치에 사용하는 16-byte 정렬 기준입니다.
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
- 정렬된 요청 크기를 기준으로 tag 할당 및 남은 공간 분할
- tag split은 TINY/SMALL 크기 요청에서만 수행
- 원래 요청 크기와 정렬된 capacity를 분리해 저장
- `show_alloc_mem`에서 TINY/SMALL/LARGE별 주소 범위와 전체 요청 크기 출력
- 확장된 libft의 `ft_printf`를 동적 할당 없이 출력에 사용
- `realloc(NULL, size)`를 `malloc(size)`와 같은 흐름으로 처리
- 유효하지 않거나 이미 해제된 포인터에 대해 `NULL` 반환
- 기존 capacity 안의 축소 요청은 같은 포인터를 유지하며 요청 크기 갱신
- capacity를 넘는 확장 요청은 새 영역을 할당하고 기존 데이터를 복사한 뒤 이전 영역 해제
- 확장용 새 할당이 실패하면 `NULL`을 반환하고 기존 영역 보존
- `realloc(ptr, 0)`은 현재 1 byte 영역을 새로 할당한 뒤 기존 영역 해제

## 진행 상태

현재 `malloc`의 box 생성, TINY/SMALL free tag 재사용과 split, LARGE 전용 box 할당, 비어 있는 box의 조건부 `munmap`, 정렬 처리, 기본 `free`, 인접 free tag 병합, 기본 `realloc`, `show_alloc_mem` 출력 흐름이 구현된 상태입니다. 전체 소스는 `-Wall -Wextra -Werror` 문법 검사를 통과하며 루트 Makefile로 shared library를 빌드할 수 있습니다.

`realloc`은 NULL, 기존 capacity 안의 축소, 확장과 데이터 복사, size 0, `SIZE_MAX` 할당 실패 시 기존 영역 보존, invalid pointer, TINY/SMALL/LARGE 경계를 대상으로 한 테스트를 각각 5회 반복해 통과했습니다. `make`, `clean`, `fclean`, `re`와 변경 없는 상태의 incremental make도 검증했고, shared library가 `malloc`, `free`, `realloc`, `show_alloc_mem` 필수 심볼을 export하는 것을 확인했습니다.

남은 주요 작업은 thread safety와 bonus 기능입니다. 제자리 `realloc` 확장과 축소 시 남은 capacity를 split하는 동작은 최적화 과제로 남아 있습니다. 공식 `norminette`는 아직 설치되지 않았으며 수동 점검에서는 C/H 코드의 80열 제한, 함수 길이, 인자/변수 수, 파일당 함수 수를 통과했지만 `includes/ft_malloc.h`와 `src/*.c`에는 표준 42 파일 헤더가 추가되지 않았습니다.

## show_alloc_mem 검증

기초 구현은 TINY/SMALL/LARGE별 사용 중인 tag의 시작·끝 주소와 `origin_size`, 전체 요청 크기 합계를 출력합니다. `malloc(10)`, `malloc(200)`, `malloc(2000)`, `malloc(0)`, 같은 zone의 연속 할당, 여러 box, 중간 및 전체 `free`, 빈 목록, 64-bit `size_t` 출력을 검사했으며 컴파일과 런타임 검증을 통과했습니다.

현재 `malloc(0)`은 내부에서 1 byte 요청으로 정규화되어 `origin_size`도 1로 출력됩니다. `free()`는 tag를 free 상태로 바꾸면서 `origin_size`를 0으로 되돌리고, 같은 box 안에서 앞뒤로 맞닿은 free tag를 병합합니다. LARGE 요청은 현재 재사용 가능한 tag 탐색 대상에서 제외되어 매번 새 box를 확보하고, split도 수행하지 않으며, 해제 후 box 전체가 비면 box list에서 제거한 뒤 `munmap`합니다. TINY/SMALL box도 전체가 비고 같은 zone에 다른 box가 남아 있으면 `munmap` 대상이 됩니다. 또한 이번 실행에서는 주소가 증가하는 순서로 출력됐지만 box list는 생성 순서를 사용하므로 mmap이 비순차 주소를 반환할 때의 정렬 보장은 별도 검토가 필요합니다.
