#!/usr/local/bin/zsh

init=100000000
ops=1000
spots=100

totalops=$(echo "${ops} * ${spots}" | bc)
./seq -i ${init} -o ${totalops}

for ((p = 2; p <= 16; p += 2)); do
		for dsworkers in 1 2; do
		    ratio=$(echo "scale=8; (${dsworkers} / ${p}) + 0.0001" | bc)
		    echo "${p},${ratio},${init},${ops}"
		    ./skiplist --nproc ${p} --dsratio ${ratio} --batchspots ${spots} -i ${init} -o ${ops}
		done
    ratio=0.5
		echo "${p},${ratio},${init},${ops}"
		./skiplist --nproc ${p} --dsratio ${ratio} --batchspots ${spots} -i ${init} -o ${ops}
done

output="init=${init}\nops=${ops}\nspots=${spots}\n\n"
output+=$(cat 05.02.log)
echo ${output} | mail -s "Experiment done" robertu13@gmail.com
