# JOS port

## Build

The project is added into JOS as submodule, and should be built from the JOS root:

```bash
cd /path/to/oscourse
git submodule update --init --recursive
python3 -m pip install --user Jinja2
make qemu   # Will do `cd into micropython/ports/jos && make`
```

## Test

JOS is configured to launch shell, where micropython can be found in root with several testing scripts:

```bash
$ ls
...
testshell.sh
test_hello.py
test_math.py
test_class.py
test_all.py
init
...
micropython
...
$
```

The common help flag can be used to learn the available options:

```bash
$ micropython -h
usage: /micropython [<opts>] [-X <implopt>] [-c <command> | -m <module> | <filename>]
Options:
...
$
```

Any of existing python scripts can be run as common:

```bash
$ micropython test_hello.py
Hello world from python!
$
```

Or, if shebang line is provided (all the default scripts have it), a script can be run by prompt:

```bash
$ test_hello.py
Hello world from python!
$
```

For running all available tests, do:
```bash
cd /path/to/oscourse
make grade
```

## Features

* Main syntax features (arithmetic, functions, classes, etc.)
* Console output
* Running file by shebang prompt: `#!/micropython` (no interpreter arguments though)
* Supported `time.sleep()` via HPET timer (the most precise results are on milliseconds scale)
* Supported `os.urandom()`
* Supported `os.system()`
* Supported `time.time()` and `time.gmtime()`
* Auto-generated grade tests

## Limitations

* Only `x86-64` micropython is supported for JOS.
* No floating-point arithmetic
* No threading
* No `-c` mode (shell need to be updated)
* No interactive mode
