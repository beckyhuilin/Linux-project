#!/bin/bash

# define keyword
keyword="TODO"

# define result file
result_file="result.log"

> "$result_file"

#find .text files and save their paths to a file
find . -type f -name '*.txt' > txt_list.txt

# initialize file counter
num=0

while read filepath
do
	if grep -q "$keyword" "$filepath"; then
		echo "$filepath" >> "$result_file"
		chmod 444 "$filepath"
		((num++))
	fi
done < txt_list.txt

# output
echo "completed."
echo "number of processed files: $num"


