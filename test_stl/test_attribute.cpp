
#include <iostream>
#include <cstdarg>
// disable __attribute__ if not use GNU compiler
#ifndef __GNUC__
#define __attribute__(x)
#endif

void myprint(const char *format...) __attribute__((format(printf, 1, 2)));

void myexit() __attribute__((noreturn));

//^ use two or more attribute together
void die(const char *format...) __attribute__((noreturn)) __attribute__((format(printf, 1, 2)));
void reborn(const char *format...) __attribute__((noreturn, format(printf, 1, 2)));

int test_myexit(const int &i);

int main(int argc, char **argv)
{
  //^ 1. __attribute__((format()))
  myprint("i:%d\n", 6);
  // report warning if use  __attribute__(format())
  myprint("i:%s, %d\n", "abc", 6);
  myprint("i:%d\n", 6);
  // report warning if use  __attribute__(format(()))
  // myprint("%s, %d, %d\n", 1, 2);
  //^ 2. __attribute__((noreturn))
  // no report about noreturn if use __attribute__((noreturn))
  test_myexit(0);

  int old_var __attribute__((deprecated)) = 10;
  myprint("old_var:%d\n", old_var);

  return 0;
}

void myprint(const char *format...)
{
  std::va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

void myexit()
{
  std::exit(-1);
}

int test_myexit(const int &i)
{
  if (i < 0)
  {
    myexit();
  }
  else
  {
    return i;
  }
}