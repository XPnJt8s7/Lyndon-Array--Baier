#!/bin/bash

./LA $1 > res_${1} && less -r -S res_${1}
