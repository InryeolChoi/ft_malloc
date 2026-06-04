# ft_malloc
[English version](README.en.md)

`ft_malloc`는 C로 `malloc`, `free`, `realloc`, `show_alloc_mem`을 직접 구현하는 메모리 할당자 프로젝트입니다.

## 목표

- `mmap`/`munmap` 기반으로 동적 메모리 할당 흐름을 구현합니다.
- TINY, SMALL, LARGE 크기 구간별 메모리 영역을 관리합니다.
- 사용자 영역 앞에 붙는 메타데이터 태그를 통해 할당 상태를 추적합니다.
- `libft`를 함께 포함해 프로젝트 내부 유틸리티로 사용할 수 있게 구성합니다.

## 현재 구성

- `includes/ft_malloc.h`: 공통 타입, 상수, 전역 상태, 함수 프로토타입 정의
- `src/malloc.c`: 전역 malloc 상태와 `malloc` 구현 시작점
- `src/free.c`: `free` 구현 예정 파일
- `src/realloc.c`: `realloc` 구현 시작점
- `src/show_alloc_mem.c`: 할당 상태 출력 구현 예정 파일
- `src/boxes.c`: zone type 판별, box list 접근, 포인터가 속한 box 탐색 헬퍼
- `src/tags.c`: tag와 user area 사이의 주소 변환 헬퍼
- `libft/`: 과제에서 허용된 libft 복사본

## 메모리 모델

현재 구조는 `box`와 `tag`를 중심으로 잡혀 있습니다.

- `t_box`: mmap으로 확보한 메모리 영역을 나타냅니다.
- `t_tag`: 사용자에게 반환되는 영역 앞에 위치하는 메타데이터입니다.
- `t_malloc_state`: TINY, SMALL, LARGE box list를 전역으로 관리합니다.
- `TINY_MAX`: 128 bytes 이하 요청을 TINY로 분류합니다.
- `SMALL_MAX`: 1024 bytes 이하 요청을 SMALL로 분류합니다.

## 진행 상태

현재까지 저장소 초기 설정, `libft` 포함, 헤더 구조 설계, `src/` 파일 분리, box/tag 헬퍼 일부 구현이 진행되었습니다.

아직 실제 할당/해제 전체 흐름은 구현 중입니다. 이후 작업은 mmap 영역 생성, 빈 tag 탐색, tag 분할/병합, `malloc`, `free`, `realloc`, `show_alloc_mem` 완성 순서로 이어질 예정입니다.
