#!/bin/bash
make \
&& diff <(./day22 < 1.IN) 1.OUT \
&& diff <(./day22 < 2.IN) 2.OUT
