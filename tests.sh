#!/bin/zsh

ITER=5

WORKMIN=0.05
WORKMAX=1.25
WORKINTER=0.15

OPMIN=100000
OPMAX=100000
OPINTER=100000

cd prob/testbed && make clean && make param
if [ $? -ne 0 ]; then
		echo "Compilation failed. Exiting."
		exit
fi
cd ../..

for ((NPROC = 8; NPROC <= 8; NPROC += 8))
do
		echo "WORKTIME,REALTIME,WORK,OPS,NPROC,ITER,LOCKS,BATCH,RATIO" > test.$NPROC.0.0.s.log
		echo "WORKTIME,REALTIME,WORK,OPS,NPROC,ITER,LOCKS,BATCH,RATIO" > test.$NPROC.0.0.p.log
		echo "WORKTIME,REALTIME,WORK,OPS,NPROC,ITER,LOCKS,BATCH,RATIO" > test.$NPROC.0.100.s.log
		echo "WORKTIME,REALTIME,WORK,OPS,NPROC,ITER,LOCKS,BATCH,RATIO" > test.$NPROC.50.100.p.log
		echo "WORKTIME,REALTIME,WORK,OPS,NPROC,ITER,LOCKS,BATCH,RATIO" > test.$NPROC.50.100.s.log
		for ((WORK = $WORKMIN;WORK <= $WORKMAX;WORK += $WORKINTER))
		do
				for ((OPS = $OPMIN;OPS <= $OPMAX;OPS += $OPINTER))
				do
						file=test.$NPROC.0.0.s.log
						echo prob/testbed/param --nproc $NPROC --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER --sequential
						prob/testbed/param --nproc $NPROC --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER --sequential >> $file

						file=test.$NPROC.0.0.p.log
						echo prob/testbed/param --nproc $NPROC --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER
						prob/testbed/param --nproc $NPROC --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER  >> $file

						file=test.$NPROC.0.100.s.log
						echo prob/testbed/param --nproc $NPROC --dsprob 0 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential
						prob/testbed/param --nproc $NPROC --dsprob 0 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential >> $file

						file=test.$NPROC.50.100.p.log
						echo prob/testbed/param --nproc $NPROC --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER
						prob/testbed/param --nproc $NPROC --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER  >> $file

						file=test.$NPROC.50.100.s.log
						echo prob/testbed/param --nproc $NPROC --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential
						prob/testbed/param --nproc $NPROC --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential >> $file
				done
		done
done
