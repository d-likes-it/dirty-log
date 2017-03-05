# Dirty-log
Async logging with very little overhead in the caller.

# Motivation
Logging is used for several reasons in code. One of them is to introspect what is happening in production code that is running. If using log levels the interesting logs are often not available when most useful, i.e. when something unexpected happens. One way to deal with the problem is to always log. Though this implies that logging must not hinder the performance of the code that is running. 

Several authors proposed how to doeal with this problem. Essentially it boils down to not log a string which involves heavy string operations, but to keep the log info gathering as slim as possible. Onm interesting talk I heard was how to do logging with one memory copy operation per argument into a preallocated buffer (see 3rdparty/variadicLogging). 

# Discussion

Variadic logging makes many things right:

 * Prepare a buffer 





