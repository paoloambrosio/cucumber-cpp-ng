# Cucumber-CPP Next Generation (Spike)

## Background

It has become increasingly difficult in [Cucumber-CPP](https://github.com/cucumber/cucumber-cpp)
to support a wide number of compiler versions for all major operating systems.

The purpose of this spike is primarily to offer a precompiled core that can
load step definitions and plugins at runtime from dynamically linked
libraries.

Plugins are written in C++, need to be compiled with a compatible compiler
and linked to the same (dynamic) runtime library.

Step definitions can be written in C or C++, compiled with any compiler and
can use different versions of runtime and other libraries. This is because
C++ objects do not cross the library boundaries.

## Command line

```
$ cucumber-cpp -h
  -h [ --help ]         Help message
  -l [ --load ] arg     Libraries to load
  -i [ --in ] arg       Input format
  -o [ --out ] arg      Output format
```

### Plugins

The spike comes with a simulation of the Wire protocol input, written as
a plugin but linked statically to the executable, and a "Some" plugin that
provides both input and output.

The Wire input plugin allows the user to specify an expression that would
configure socket type (TCP or Unix), host and port if TCP, socket file
if Unix.

Examples of input from Wire protocol:

```
$ cucumber-cpp -i wire
[DEBUG] TCP Wire Input localhost:3902
...
$ cucumber-cpp -i wire:1234
[DEBUG] TCP Wire Input localhost:1234
...
$ cucumber-cpp -i wire:tcp:myhost:1234
[DEBUG] TCP Wire Input myhost:1234
...
$ cucumber-cpp -i wire:unix:my.sock
[DEBUG] UNIX Wire Input my.sock
...
```

The "Some" plugin simulates input from 5 scenarios (as `<< #`) and output of
all scenarios in input (as `>> status #`), not just those from the same
plugin.

```
$ cucumber-cpp -l libsome-cucumber-plugin.so -i some -o some
Loading library from "libsome-cucumber-plugin.so"
<< 5
>> success 5
<< 4
>> success 4
<< 3
>> success 3
<< 2
>> success 2
<< 1
>> success 1
```

It is dynamically loaded so if we don't load the plugin it won't be found:

```
$ cucumber-cpp -i some -o some
[WARN] Can't find input plugin some
```

The Some input plugin, unlike the Wire Protocol input plugin, is not
configurable (but that is not a limitation of the framework).

### Glue code

Step definitions are loaded in the same way as plugins. via the `-l` flag.

```
$ cucumber-cpp -l libglue-example1.so -l libglue-example2.so
Loading library from "libglue-example1.so"
**************** Registering /.../filename.cpp:12
Loading library from "libglue-example2.so"
**************** Registering /.../filename.c:42
**************** Unegistering /.../filename.cpp:12
**************** Unegistering /.../filename.c:42
```
