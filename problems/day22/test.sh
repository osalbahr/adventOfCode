#!/bin/bash
    diff <(./day22 < 1.IN) 1.OUT \
&&  diff <(./day22 < 2.IN) 2.OUT
