#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    size_t page_size;
    void   *mem;

    page_size = getpagesize();

    mem = mmap(
        NULL,                       // 주소는 OS가 정하게 함
        page_size,                  // page 하나만큼 요청
        PROT_READ | PROT_WRITE,     // 읽기/쓰기 가능
        MAP_PRIVATE | MAP_ANON,     // 익명 메모리, 프로세스 전용
        -1,                         // 파일 안 씀
        0                           // 파일 offset 안 씀
    );

    if (mem == MAP_FAILED)
    {
        perror("mmap");
        return (1);
    }

    printf("mmap address: %p\n", mem);

    strcpy((char *)mem, "hello mmap");
    printf("stored string: %s\n", (char *)mem);

    if (munmap(mem, page_size) == -1)
    {
        perror("munmap");
        return (1);
    }

    return (0);
}
