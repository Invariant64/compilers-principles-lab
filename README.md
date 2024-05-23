## compilers-principles-lab

BJUT 2024 Compiler Principles Lab

#### Grammatical Analysis Method

- Recursive Subroutine Method
``` bash
make GRAMMAR=RS
```
or
``` bash
make
```
- LR(1) Method
``` bash
make GRAMMAR=LR1
```

#### Compile
``` bash
make
```

#### Run
``` bash
build/main [input file]
```

#### Grammar tree visualization
Need to install graphviz
Only support Recursive Subroutine Method
``` bash
make image
```

#### Debug

On MacOS
``` bash
make lldb ARGS="[input file]"
```

On Linux
``` bash
make gdb ARGS="[input file]"
```

#### Clean
``` bash
make clean
```
