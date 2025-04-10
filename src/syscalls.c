#include <errno.h>
#include <stdio.h>

register char *stack_ptr asm("sp");
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

__attribute__((weak)) int _read(int file, char *ptr, int len) {
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    *ptr++ = __io_getchar();
  }

  return len;
}

__attribute__((weak)) int _write(int file, char *ptr, int len) {
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    __io_putchar(*ptr++);
  }
  return len;
}
char *_sbrk(int incr) {
  extern char end asm("end");
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0) {
    heap_end = &end;
  }

  prev_heap_end = heap_end;

  if (heap_end + incr > stack_ptr) {
    _write(1, "Heap and stack collision\r\n", 26);
    errno = ENOMEM;
    return (char *)-1;
  }

  heap_end += incr;

  return (char *)prev_heap_end;
}