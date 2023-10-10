#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <num_subscribers>"
    exit 1
fi

num_subscribers=$1

# Compile publisher and subscriber programs
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

# Start subscribers
for ((i=1; i<=$num_subscribers; i++)); do
    gnome-terminal -- ./subscriber $i &
done

# Start the publisher
./publisher $num_subscribers

# Cleanup
for ((i=1; i<=$num_subscribers; i++)); do
    rm -f "/tmp/ex1/s$i"
done

# Clean up compiled programs
rm -f publisher subscriber

