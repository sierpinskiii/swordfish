#!/bin/bash

targetbin=$1

pipe=/tmp/swordfish.fifo

exec $targetbin > $pipe &

./swordfish
