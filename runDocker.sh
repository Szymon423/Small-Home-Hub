#!/bin/bash
docker run -v $PWD:/Small-Home-Hub -w /Small-Home-Hub -p 11111:11111 -i -t --rm szymon432/arm64-compiler-image:latest /bin/bash