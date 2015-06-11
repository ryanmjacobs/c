# c

[![Build Status](https://travis-ci.org/ryanmjacobs/c.svg)](https://travis-ci.org/ryanmjacobs/c)

> "There isn't much that's special about C. That's one of the reasons why it's fast."

I love C for its raw speed (although it does have its drawbacks). We
should all write more C.

With this shell script, you can compile and execute C "scripts" in
one go!

(Oh yeah, and it works for C++ too).

![GIF Demo](http://i.imgur.com/aWnG03r.gif)

Here's a simple example:
```c
#include <stdio.h>

int main(void) {
    printf("Hello World!\n");
    return 0;
}
```

Run it by typing:
```bash
$ c hello.c
Hello World!
```

Or, call it from the shebang!
```c
#!/usr/bin/c
#include <stdio.h>

int main(void) {
    printf("Hello World!\n");
    return 0;
}
```
```bash
$ chmod +x hello.c
$ ./hello.c
Hello World!
```

## Hooked? Here's how to install it:
Use a package manager? [Check here](https://github.com/ryanmjacobs/c#packages).

For all users:
```bash
$ git clone https://github.com/ryanmjacobs/c
$ sudo cp ./c/c /usr/bin/c
```

Just for a local user:
```bash
$ git clone https://github.com/ryanmjacobs/c
$ mkdir -p ~/.bin
$ cp ./c/c ~/.bin/c
$ echo 'PATH=$PATH:$HOME/.bin' >> ~/.bashrc
```

Note: if you install it somewhere other than `/usr/bin/c`, then your shebang will be different.
For example it may be something more similar to `#!/home/ryan/.bin/c`.

## Okay, how do I use it?
c will use whatever $CC is set to. You can change this with:
```bash
$ export CC=clang
$ export CC=tcc
$ # etc...
```
## CLI
### Multiple Files - CLI
Anything you want passed to the compiler, put in quotes as the first argument.
Whether they're flags (`-Wall`, `-O2`, etc.) or file names (`file.c`,
`main.c`, etc.).

```bash
$ c "main.c other.c" arg1 arg2
$ c "main.c other.c -O3 -Wall -lncurses" arg1 arg2
```
### Single File - CLI
With only one file, omit the quotes:
```bash
$ c hello.c
$ c main.c arg1 arg2
```

## Shebang!
After adding a shebang, just set the file to executable and it's ready to run.
```bash
$ chmod +x file.c
$ ./file.c
```

### Single File - Shebang
Add this to the top of your C file:
```c
#!/usr/bin/c
```

### Multiple Files - Shebang
Just tack on any extra flags, options, or files you want passed to the compiler.
Then be sure to add the terminating `--` characters.
```c
#!/usr/bin/c file1.c file2.c -lncurses -lm --
```

## Compiling from stdin
```c
$ cat hello.c | c
# ...or...
$ c < hello.c
```
```c
$ c "" arg1 arg2 < hello.c
$ c "other.c -lncurses" arg1 arg2 < hello.c
```

## Caching
The default cache size is set to 5 MB. You can change this with:
```bash
$ export C_CACHE_SIZE=$((10*1024)) # 10 MB
```
The default cache path is set to `$TMPDIR/c.cache`. You can change this with:
```bash
$ export C_CACHE_PATH="/tmp/the_cache"
```

# Contributing
Feel free to submit any ideas, questions, or problems by reporting an issue.
Or, if you're feeling bit brave, submit a pull request. :grimacing:

Just hack away and make sure that all of the tests pass.
```bash
$ cd tests
$ ./test.sh
```

## Why?
First of all, I want to clarify why this is **not** the same as `tcc -run`.
TCC is a compiler. We all know that. TCC will preform its own set of
optimizations, just as GCC will preform its own and Clang will preform its own.
The purpose of this script is to give a simple front-end to your favorite
compiler.

Whether it's GCC, Clang, or something else entirely, **you** get to choose
your compiler.

Second reason: it's simply satisfying to type `c hello.c` and see it run instantly.

Third reason: I'm a fan of speed, and C definitely offers it. Being able to
write a small, fast, and portable C "script" is great. You can pass around a
C "script" just like you would a BASH script.

## zsh
If you're using `zsh`, then you can take advantage of `zsh`'s suffix aliases:
```bash
$ alias -s c='c'
$ alias -s cc='c'
$ alias -s cpp='c'
```
Then you can run files with `./file.c` without `chmod +x`.

## Packages
Use a package manager? You've come to the right place.

AUR: https://aur.archlinux.org/packages/c/<br>
bpkg: `bpkg install ryanmjacobs/c`<br>
brew: `brew install https://raw.githubusercontent.com/ryanmjacobs/c/master/c.rb`
(shebang path is `/usr/local/bin/c`)<br>

## Todo
~~Maybe later we can implement caching.~~ Done!

## License
```
The MIT License (MIT)

Copyright (c) 2015 Ryan Jacobs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
