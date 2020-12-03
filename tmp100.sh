#! /bin/bash

# In order to use the script you have to have the module inserted
# Then just call the script "./tmp100.sh <value>" where the "value is the sensor you want to read from"

for i in {1..100}
do
	sudo cat /dev/tmp100_$1
done
