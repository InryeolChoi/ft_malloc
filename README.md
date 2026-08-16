# ft_malloc

[영문 README](README.en.md)

`ft_malloc`은 C로 `malloc`, `free`, `realloc`, `show_alloc_mem`,
`show_alloc_mem_ex`를 직접 구현하는 42 과제 프로젝트입니다.

## 목표

- `mmap`과 `munmap`을 이용한 동적 메모리 할당 흐름 구현
- TINY, SMALL, LARGE 크기 구간별 메모리 영역 관리
- 사용자 영역 앞의 메타데이터를 이용한 할당 상태 추적
- `free`와 `realloc`을 위한 포인터, box, tag 탐색 분리
- 프로젝트 내부 동작에 필요한 유틸리티를 `libft`로 관리

## 프로젝트 구성

- `includes/ft_malloc.h`: 공통 타입, 상수, 전역 상태, 함수 원형
- `src/malloc.c`: 해제 tag 탐색, box 생성, tag 할당과 분할
- `src/free.c`: 포인터 검증, tag 해제, 인접 해제 tag 병합
- `src/realloc.c`: 제자리 확장과 축소, 공간 분할, 데이터 복사
- `src/show_alloc_mem.c`: 주소순 box 탐색과 기본 할당 상태 출력
- `src/show_alloc_mem_ex.c`: 사용 중인 사용자 영역의 16진수 덤프 출력
- `src/boxes.c`: zone 분류, box 목록 접근, box 탐색
- `src/support_malloc.c`: box 연결, 최초 tag 생성, tag 분할
- `src/support_tags.c`: tag와 사용자 영역 사이의 주소 변환과 tag 탐색
- `src/support_size.c`: 정렬, 오버플로 검사, box 크기 계산
- `src/utils_macos.c`: macOS의 페이지 크기 조회
- `src/utils_linux.c`: Linux의 페이지 크기 조회
- `src/support_thread.c`: zone별 pthread mutex 제어
- `src/support_debug.c`: 디버그 환경변수와 Scribble 처리
- `src/support_history.c`: 할당 기록 저장과 출력
- `libft/`: 과제에서 사용하는 libft
- `Makefile`: libft와 allocator 공유 라이브러리의 빌드 규칙
- `.clangd`: clangd의 include 경로 설정

## 빌드

루트 `Makefile`은 먼저 `libft/libft.a`를 빌드한 뒤 allocator를 공유
라이브러리로 연결합니다. 컴파일과 링크에는 pthread 사용을 위한
`-pthread`가 적용됩니다.

`HOSTTYPE`이 비어 있거나 정의되지 않았으면 `uname -m`과 `uname -s`를
조합해 자동으로 지정합니다. Apple Silicon macOS에서는 다음과 같은
파일이 생성됩니다.

- `libft_malloc_arm64_Darwin.so`: 실제 공유 라이브러리
- `libft_malloc.so`: 실제 라이브러리를 가리키는 심볼릭 링크

Makefile은 macOS에서 `utils_macos.c`와 `-dynamiclib`을 선택하고, Linux에서는
`utils_linux.c`와 `-shared`를 선택합니다. 지원하지 않는 운영체제에서는
빌드를 중단합니다.

```sh
make          # libft와 allocator 빌드
make clean    # allocator와 libft의 오브젝트 정리
make fclean   # 오브젝트, libft.a, 공유 라이브러리와 링크 제거
make re       # 전체 정리 후 다시 빌드
```

`make HOSTTYPE=custom`처럼 호스트 이름을 직접 지정할 수도 있습니다.
`make HOSTTYPE=`처럼 빈 값을 전달하면 자동 감지를 사용합니다.

`libft/Makefile`은 `-MMD -MP`로 `.d` 의존성 파일을 만들고 포함합니다.
따라서 libft 헤더가 변경되면 관련 object를 다시 빌드할 수 있습니다.

## 플랫폼 호환성 검증

Linux 전용 빌드는 Docker의 각 환경에서 `make re`로 완전히 다시
컴파일했습니다. 생성된 공유 라이브러리를 `LD_PRELOAD`로 주입한 뒤
`malloc`, `malloc(0)`, `realloc`, `free`와 4개 스레드의 반복 할당을
검사했습니다. 각 작업 스레드는 2,000회의 할당 작업을 수행했습니다.

| 환경 | 아키텍처 / 컴파일러 | 결과 |
| --- | --- | --- |
| Alpine Linux | ARM64 / GCC 15.2, musl | 통과 |
| Ubuntu 24.04.4 LTS | ARM64 / GCC 13.3 | 통과 |
| Debian 12 Bookworm | ARM64 / GCC 12.2 | 통과 |
| Kali Linux Rolling | ARM64 / GCC 15.3 | 통과 |
| Red Hat Enterprise Linux 9.8 UBI | ARM64 / GCC 11.5 | 통과 |
| Linux Mint 22.3 커뮤니티 이미지 | AMD64 / GCC 13.3 | 제한과 함께 통과 |

Mint 커뮤니티 이미지는 컨테이너 내부에서 Ubuntu 24.04로 식별되며
`/etc/linuxmint/info` 파일이 없었습니다. 따라서 해당 결과는 Mint가
사용하는 Ubuntu 기반과의 호환성을 확인한 참고 결과이며, 실제 Mint 설치
환경의 독립적인 검증으로 간주하지 않습니다. Arch Linux는 아직 직접
실행하지 않았습니다.

## 메모리 모델

현재 allocator는 `box`와 `tag`를 중심으로 구성됩니다.

- `t_box`: `mmap`으로 확보한 메모리 영역
- `t_tag`: 사용자 영역 앞에 놓이는 메타데이터
- `capacity`: 정렬 후 tag가 관리하는 사용자 영역의 실제 크기
- `origin_size`: 사용자가 원래 요청한 크기
- `t_malloc_state`: TINY, SMALL, LARGE box 목록을 관리하는 전역 상태
- `t_thread_state`: zone과 할당 기록용 mutex를 보관하는 상태
- `TAG_MAGIC`: tag의 유효성 확인에 사용하는 magic 값
- `ALIGNMENT`: 사용자 영역과 메타데이터 배치에 사용하는 16바이트 정렬 기준
- `TINY_MAX`: 128바이트 이하 요청의 분류 기준
- `SMALL_MAX`: 1024바이트 이하 요청의 분류 기준

TINY와 SMALL은 하나의 box 안에 여러 할당을 저장합니다. LARGE는
요청마다 별도의 box를 만들고, 하나의 할당이 해제되면 box 전체를
`munmap`합니다.

## 구현된 기능

- `ZONE_TINY`, `ZONE_SMALL`, `ZONE_LARGE` 분류
- zone별 box 목록 접근과 box 탐색
- 사용자 포인터가 할당자의 box에 속하는지 확인
- 사용자 포인터와 tag 사이의 주소 변환
- 해제 tag 재사용과 tag 분할
- 정렬된 크기와 원래 요청 크기의 분리 저장
- 덧셈과 곱셈의 오버플로 검사
- 각 zone의 최소 100개 할당을 수용하는 box 계산
- LARGE 전용 box 생성과 split 방지
- 빈 LARGE box의 목록 제거와 `munmap`
- 조건을 만족하는 빈 TINY/SMALL box의 `munmap`
- 인접 해제 tag의 병합을 통한 파편화 감소
- `realloc(NULL, size)`를 `malloc(size)`와 같은 흐름으로 처리
- capacity 안의 realloc 축소와 해제 tag 분할
- 인접 해제 tag를 이용한 같은 zone의 realloc 제자리 확장
- 공간이 부족하거나 zone이 바뀌는 realloc의 새 할당과 데이터 복사
- 확장용 새 할당 실패 시 기존 할당 보존
- `realloc(ptr, 0)`의 최소 크기 할당 처리
- TINY, SMALL, LARGE box의 주소 오름차순 `show_alloc_mem` 출력
- 사용 중인 사용자 영역을 16바이트 단위로 출력하는 `show_alloc_mem_ex`
- `FT_MALLOC_DEBUG=1` 기반 잘못된 해제와 중복 해제 진단
- `FT_MALLOC_SCRIBBLE=1` 기반 `0xAA`와 `0x55` 패턴 처리
- `FT_MALLOC_HISTORY=1` 기반 256개 원형 배열 할당 기록

## 스레드 안전성

각 zone을 독립적인 mutex로 보호해 TINY, SMALL, LARGE 작업이 서로
불필요하게 기다리지 않도록 구성했습니다.

- `malloc`, `free`, `realloc`은 접근하는 zone만 잠금
- `find_box_and_lock`이 얻은 mutex의 소유권을 호출 경로가 이어받음
- 모든 정상 및 오류 경로에서 소유한 mutex를 해제
- 중첩된 `malloc`이나 `free` 호출 전 기존 mutex를 해제해 재귀 잠금 방지
- 잠금 상태에서 `copy_size`를 저장해 잠금 해제 후 기존 tag를 재참조하지 않음
- `show_alloc_mem`과 `show_alloc_mem_ex`는 세 zone을 함께 잠가 일관된 상태를 순회
- `unmap_box`는 `munmap` 성공을 확인한 뒤 box 목록을 변경
- 할당 기록은 zone mutex와 분리된 기록 전용 mutex로 보호

## 검증 현황

필수 기능과 제출 검증을 완료했습니다. `unmap_box`는 첫 번째, 중간,
마지막 box 제거와 `munmap` 실패 시 목록 보존을 포함해 176/176 케이스를
통과했습니다. macOS의 `DYLD` interposition에서 `malloc`, `free`,
`realloc`, `show_alloc_mem` 심볼과 경계값 동작을 확인했습니다.

공식 Norminette 3.3.59는 오류 0건과 전역변수 감지 알림 4건을 보고했습니다.
`-Wall -Wextra -Werror -Wmissing-prototypes` 검사와 공유 라이브러리 빌드도
통과했습니다.

realloc 기본 동작은 60/60 케이스를 통과했습니다. 멀티스레드 스트레스
검사에서는 allocator 작업 54,000회, `show_alloc_mem` 호출 720회,
무결성 검사 179,910회를 오류 없이 완료했습니다.

`find_next_box`의 빈 목록, 1개, 5개, 혼합, 마지막 항목 케이스와 결정적 주소
오름차순 출력은 각각 5회 반복해 통과했습니다. zone 분류, split, 재사용,
병합, LARGE `munmap`, realloc 회귀 검사도 반복 검증했습니다.

## `show_alloc_mem` 검증

TINY, SMALL, LARGE별 사용 중인 tag의 시작 주소, 끝 주소, `origin_size`,
전체 요청 크기 합계를 출력합니다. 10, 200, 2000, 0바이트 요청과 여러 box,
부분 해제, 전체 해제, 빈 목록, 64비트 `size_t` 출력을 검사했습니다.

현재 `malloc(0)`은 내부에서 1바이트 요청으로 정규화됩니다. `free`는
`origin_size`를 0으로 만들고 같은 box 안의 인접 해제 tag를 병합합니다.
box 출력은 `find_next_box`가 전체 목록에서 다음 주소를 다시 찾으므로
생성 순서와 관계없이 주소 오름차순을 유지합니다.

## `show_alloc_mem_ex` 검증

해제 tag와 정렬 여유 공간을 제외하고 사용 중인 사용자 영역을
`origin_size`만큼 두 자리 16진수 값으로 출력합니다. 각 행은 최대 16바이트이며 행의
시작 주소를 함께 표시합니다.

1, 15, 16, 17바이트 경계, TINY/SMALL/LARGE 동시 출력, 해제된 할당
제외, realloc 전후 데이터 보존을 검사했습니다. 6개 작업 스레드가 실행당
2,400회의 malloc/realloc/free를 수행하면서 출력을 호출하는 스트레스도
10회 반복해 통과했습니다.

## 할당 기록

`FT_MALLOC_HISTORY`의 값이 정확히 `1`일 때 할당 기록을 저장합니다.
환경변수가 없거나 다른 값이면 기록과 출력은 수행하지 않습니다.

```sh
FT_MALLOC_HISTORY=1 ./program
```

할당 기록은 동적 할당 없이 `g_malloc` 내부의 256개 고정 크기 원형 배열에
저장됩니다. 각 항목은 순번, 이벤트 종류, 이전 포인터, 새 포인터,
요청 크기를 보관합니다. 배열이 가득 차면 가장 오래된 기록부터 덮어쓰고,
출력할 때는 남아 있는 기록을 오래된 순서부터 순회합니다.

`malloc`, `free`, `realloc` 성공 경로가 각각 이벤트를 기록합니다. 이동하는
`realloc`은 내부에서 `malloc`과 `free`를 호출하므로 상세 이벤트와 최종
`realloc` 이벤트가 함께 남을 수 있습니다.

환경변수 활성화와 비활성화, realloc의 제자리 변경·이동·크기 0, 600개
이벤트의 원형 배열 순환, 8개 작업 스레드의 동시 기록을 검사했습니다.

## malloc 디버그 환경변수

`FT_MALLOC_DEBUG`가 정확히 `1`일 때만 `free` 오류를 표준 오류로 출력합니다.

```sh
FT_MALLOC_DEBUG=1 ./program
```

할당자 box 밖의 주소와 할당 시작점이 아닌 주소는 `invalid free`로,
이미 해제된 tag를 다시 해제하면 `double free`로 출력합니다.

`FT_MALLOC_SCRIBBLE=1`은 새 할당을 `0xAA`로 채우고 정상 `free`에서
사용자 영역을 `0x55`로 채웁니다. 제자리 realloc 확장 영역은 `0xAA`, 축소로
소유권을 잃은 영역은 `0x55`로 채웁니다.

```sh
FT_MALLOC_SCRIBBLE=1 ./program
```

17바이트 malloc/free, realloc 확장과 축소, 새 할당을 사용하는 확장,
DEBUG와 SCRIBBLE 환경변수의 독립성을 공유 라이브러리로 확인했습니다.

향후 디버그 후보는 다음과 같습니다.

- `FT_MALLOC_ABORT_ON_ERROR=1`: `invalid free` 또는 `double free`를 출력한 뒤
  `abort()`해 디버거가 실패 지점에서 멈추도록 하는 기능
