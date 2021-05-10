void __assert_rtn(
    const char* function, const char* file, unsigned int line,
    const char* assertion);

void __assert_fail(
    const char* assertion, const char* file, unsigned int line,
    const char* function)
{
  __assert_rtn(function, file, line, assertion);
}
