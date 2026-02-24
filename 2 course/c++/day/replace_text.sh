#!/bin/bash
search="$1"
replace="$2"
target="$3"
recursive=false
if [ $# -ge 4 ] && [ "$4" == "-r" ];
then
	recursive=true
fi
replace_in_file() {
	local file="$1"
	local count=0

	if [ -f "$file" ]; then
		temp_file=$(mktemp)
		count=$(sed -n "s/$search/$replace/gp" "$file" | wc -l)
		sed "s/$search/$replace/g" "$file" > "$temp_file"
		if [ $count -gt 0 ]; then
			mv "$temp_file" "$file"
			echo "Replace $count entries in $file"
		else
			rm "$temp_file"
		fi
	fi
}

if [ -f "$target" ]; then
	replace_in_file "$target"
elif [ -d "target" ]; then
	if [ "$recursive" = true ]; then
		while ips=read -r -d '' file; do
			replace_in_file "$file"
		done < <(find "$target" -type f -print0)
	else
		for file in "$target"/*; do
			if [ -f "$file" ]; then
				replace_in_file "$file"
			fi
		done
	fi
else
	echo "Error: $target is not a valid file or directory"
	exit 1
fi
