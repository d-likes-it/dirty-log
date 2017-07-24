# Dirty-log
Async logging with very little overhead in the caller. And some fun...

# Motivation
Logging is used for several reasons in code. One of them is to introspect what is happening in production code that is running. If using log levels the interesting logs are often not available when most useful, i.e. when something unexpected happens. One way to deal with the problem is to always log, which is suitable for some applications. Though this implies that logging is very light weight on the emitting side. 

Several authors proposed how to deal with this problem. Essentially it boils down to not log a string which involves heavy string operations, but to keep the log info gathering as slim as possible. Onm interesting talk I heard was how to do logging with one memory copy operation per argument into a preallocated buffer (see 3rdparty/variadicLogging).

What is not so nice is that the log formating itself needs to be in the same process (it's a (rather) anonymous dynamic callback).

# Idea

With modern c++ features it should be possible to derive the signature from the log call and transport the arguments in a generic format with the same amount of mem copies. The descriptor string us known at compile time (constexpr).

The log line is then "sent" into a udp like writer, which drops log messages if resources are exhausted.

# Status

The lib has currently no of args + 3 memcopys if my interpretation is correct. (format, format length, signature)

Clang assembly output for the example writing 2 ints:

```assembly
      *reinterpret_cast<uint8_t *>(pointer) = format_length - 1;
  400b59:	c6 44 04 28 11       	mov    BYTE PTR [rsp+rax*1+0x28],0x11
      pointer += sizeof(uint8_t);
      std::memcpy(pointer, format, format_length - 1);
  400b5e:	0f 11 44 04 29       	movups XMMWORD PTR [rsp+rax*1+0x29],xmm0
  400b63:	c6 44 04 39 7d       	mov    BYTE PTR [rsp+rax*1+0x39],0x7d
      pointer += format_length - 1;
      
      std::memcpy(pointer, sig_str::value, sig_str::size);
  400b68:	c6 44 04 3c fe       	mov    BYTE PTR [rsp+rax*1+0x3c],0xfe
  400b6d:	66 c7 44 04 3a 02 fe 	mov    WORD PTR [rsp+rax*1+0x3a],0xfe02
    *reinterpret_cast<Arg*>(buffer) = t;
  400b74:	48 89 4c 04 3d       	mov    QWORD PTR [rsp+rax*1+0x3d],rcx
  400b79:	48 c7 44 04 45 00 04 	mov    QWORD PTR [rsp+rax*1+0x45],0x400
  400b80:	00 00 
    __gnu_cxx::__enable_if<__is_byte<_Tp>::__value, void>::__type
    __fill_a(_Tp* __first, _Tp* __last, const _Tp& __c)
    {
      const _Tp __tmp = __c;
      if (const size_t __len = __last - __first)
	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
  400b82:	0f 11 4c 04 58       	movups XMMWORD PTR [rsp+rax*1+0x58],xmm1
  400b87:	0f 11 4c 04 4d       	movups XMMWORD PTR [rsp+rax*1+0x4d],xmm1
```

# Outlook

It should be possible to combine the format and format length into 1 memcopy (assembled into a constexpr string up frnont).



