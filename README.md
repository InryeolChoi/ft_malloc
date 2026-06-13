# ft_malloc
[English version](README.en.md)

`ft_malloc`는 C로 `malloc`, `free`, `realloc`, `show_alloc_mem`을 직접 구현하는 메모리 할당자 프로젝트입니다.

## 목표

- `mmap`/`munmap` 기반으로 동적 메모리 할당 흐름을 구현합니다.
- TINY, SMALL, LARGE 크기 구간별 메모리 영역을 관리합니다.
- 사용자 영역 앞에 붙는 메타데이터 태그를 통해 할당 상태를 추적합니다.
- `libft`를 함께 포함해 프로젝트 내부 유틸리티로 사용할 수 있게 구성합니다.
- 포인터 검증, 영역 탐색, 태그 탐색을 분리해 `free`와 `realloc` 구현 기반을 만듭니다.

## 현재 구성

- `includes/ft_malloc.h`: 공통 타입, 상수, 전역 상태, 함수 프로토타입 정의
- `src/malloc.c`: 전역 malloc 상태, box 생성, free tag 탐색, tag 할당/분할 기반 `malloc` 구현
- `src/free.c`: 포인터가 속한 box/tag를 찾아 free 상태로 표시하는 기본 `free`
- `src/realloc.c`: 현재 `realloc` stub
- `src/show_alloc_mem.c`: 할당 상태 출력 구현 예정 파일
- `src/boxes.c`: zone type 판별, box list 접근, 포인터가 속한 box 탐색 헬퍼
- `src/tags.c`: tag와 user area 사이의 주소 변환, box 내부 tag 탐색 헬퍼
- `src/utils.c`: 페이지 크기 조회, box 크기 계산, overflow 방지 계산 헬퍼
- `libft/`: 과제에서 허용된 libft 복사본
- `.clangd`: `includes/`, `libft/` include path 설정

## 메모리 모델

현재 구조는 `box`와 `tag`를 중심으로 잡혀 있습니다.

- `t_box`: mmap으로 확보한 메모리 영역을 나타냅니다.
- `t_tag`: 사용자에게 반환되는 영역 앞에 위치하는 메타데이터입니다.
- `t_malloc_state`: TINY, SMALL, LARGE box list를 전역으로 관리합니다.
- `TAG_MAGIC`: tag 무결성을 확인하기 위한 magic value입니다.
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
- `free(ptr)`에서 유효한 tag를 찾아 `is_free` 표시
- OS별 페이지 크기 조회와 fallback 처리
- overflow를 피하기 위한 덧셈/곱셈 helper
- 요청 크기와 zone type에 따른 mmap box 크기 계산
- `create_box`를 통한 mmap 영역 생성 및 첫 free tag 초기화
- box를 전역 box list에 연결하는 흐름
- `find_available_tag`를 통한 재사용 가능한 free tag 탐색
- `allocate_tag`를 통한 tag 할당 처리와 남은 공간 분할

## 진행 상태

현재까지 저장소 초기 설정, `libft` 포함, 헤더 구조 설계, `src/` 파일 분리, box/tag 탐색 헬퍼, 기본 `free` 흐름, `malloc`의 box 생성 및 tag 할당 초안이 진행되었습니다.

아직 전체 allocator 동작은 구현 중입니다. 이후 작업은 새 box 연결 기준 정리, tag split edge case 보강, free tag 병합, prototype 정리, `malloc`, `free`, `realloc`, `show_alloc_mem` 완성 순서로 이어질 예정입니다.
