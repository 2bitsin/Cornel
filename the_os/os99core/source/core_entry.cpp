

extern "C" 
{
  volatile const char hello [] = "A sunny HELLO!";
  __attribute__((cdecl))
  void core_entry()
  {  
    auto p = (volatile unsigned long long*)0x400u;
    for(auto i = 0; i < 100000; ++i)
      *p = *p + hello[i % sizeof(hello)];

  }
}