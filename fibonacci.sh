#!/bin/bash

# Number of terms
N=10

# Starting values
a=0
b=1

echo "Fibonacci sequence up to $N terms:"
for (( i=0; i<N; i++ ))
do
    echo -n "$a "
    # Calculate next term
    fn=$((a + b))
    a=$b
    b=$fn
done

echo  # to move to the next line
