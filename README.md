# csr

a kcp-based walking simulator written in C.

## tutorial
**1. dependencies**

this may vary for different systems. i'm using ubuntu and these were all the additional things i needed:
    
- libprotoc-dev
- [libprotobuf-c](https://github.com/protobuf-c/protobuf-c) 
- libmicrohttpd

**2. running**

1. `$ git clone https://github.com/kenwool0566/csr.git && cd csr`
2. build & run 
    - with debug profile (default)
        1. `$ make`
        2. `$ ./bin/debug/httpserver`
        3. `$ ./bin/debug/gameserver`
    - with release profile
        1. `$ make RELEASE=1`
        2. `$ ./bin/O3/httpserver`
        3. `$ ./bin/O3/gameserver`

you can find server config & hotfix config in `./include/shared/config.h`

**3. playing**

if you're using linux and understands the C ecosystem, you know how already. if you don't then look up dawn winery discord.

## todos:
1. modularize handlers
2. make code better idk lmao
