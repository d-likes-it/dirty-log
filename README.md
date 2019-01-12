# Dirty-log
Async logging with very little overhead in the caller. And some fun...

# Motivation
Logging is used for several reasons in code. One of them is to introspect what is happening in production code that is running. If using log levels the interesting logs are often not available when most useful, i.e. when something unexpected happens. One way to deal with the problem is to always log, which is suitable for some applications. Though this implies that logging is very light weight on the emitting side. 

Several authors proposed how to deal with this problem. Essentially it boils down to not log a string which involves heavy string operations, but to keep the log info gathering as slim as possible. 

# Prior Work

- One interesting talk I heard was how to do logging with one memory copy operation per argument into a preallocated buffer (see 3rdparty/variadicLogging). What is not so nice is that the log formating itself needs to be in the same process (it's a (rather) anonymous dynamic callback).

- llcpp (see 3rdparty/llcpp) came up with a idea to postpone the formatting to when the log message is needed by narrowing down the format possibilities at emit, decoding using a script.

# Idea

With modern c++ features it should be possible to derive the signature from the log call and transport the arguments in a generic format with the same amount of mem copies. The signature descriptor string is known at compile time (constexpr).

The log line is then "sent" into a udp like writer, which drops log messages if resources are exhausted.

# Status (Updated)

The lib has currently no of args + 1 memcopys if my interpretation is correct. The extra memcopy is for the compile time prepared argument descriptor and length.

Clang assembly output for the example writing 1 double, 1 int:

```assembly
      std::memcpy(pointer, &format_sig::value[0], format_sig::size);
  400c47:	44 88 44 34 2a       	mov    BYTE PTR [rsp+rsi*1+0x2a],r8b
  400c4c:	66 89 4c 34 28       	mov    WORD PTR [rsp+rsi*1+0x28],cx

_ZN4dlog8copy_argIdEEPcS1_RT_():
  *reinterpret_cast<Arg *>(buffer) = t;
  400c77:	66 0f 13 5c 34 2b    	movlpd QWORD PTR [rsp+rsi*1+0x2b],xmm3
_ZN4dlog8copy_argImEEPcS1_RT_():
  400c7d:	48 c7 44 34 33 40 42 	mov    QWORD PTR [rsp+rsi*1+0x33],0xf4240
```


# Outlook

- [done] It should be possible to combine the format and format length into 1 memcopy (assembled into a constexpr string up front).
- [done] It should also be possible to collapse fromat and signature string into a constexpr buffer.
- It should be possible to create a compile time list of format strings. When logging starts that list could be sent as a special log message and later on only indices to the format strings need to be published with every write_log call.
- Integrate it into one of the known logging frameworks and see if performance increases there.
  - emit site is relived of work
  - at the other end of ring buffer some optimisations can be done, such as omitting duplicated log lines

