#!/bin/bash

target=$(( RANDOM % 51 ))
echo "guess number game(between 0 and 50)"

while true
do
	read -p "please enter the number you guess:" guess
	
	if ! [[ "$guess" =~ ^[0-9]+$ ]]; then
		echo "invalid input, please enter the number again!"
		continue
	fi

	if [ "$guess" -gt "$target" ]; then
		echo "too large!"
	elif [ "$guess" -lt "$target" ]; then
		echo "too small!"
	else
		echo "Congratulations, you guessed it right!"
		break	
	fi
done

