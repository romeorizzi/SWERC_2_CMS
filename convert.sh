#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "This command requires one argument."
    echo "Usage: $0  <source_original_SWERC_problem_directory>"
    exit 1
fi
if [ ! -d $1 ]; then
    echo "source_directory not found!"
    echo "Usage: $0  <source_original_SWERC_problem_directory>"
    exit 1
fi

cd $1;

prefix="SWERC"
name=$(cat problem.yaml | grep name | cut -d: -f2 | tr -d '\n\f' | tr [:upper:] [:lower:] | tr -s [:space:] '_')
prefixed_name="$prefix$name"
time_limit=$(cat problem.yaml | grep validation_time | cut -d: -f2)

outdir="../$prefixed_name"

mkdir -p $outdir/testo/
cp problem.pdf $outdir/testo/testo.pdf

mkdir -p $outdir/sol/
cp -v submissions/accepted/* $outdir/sol/

mkdir -p $outdir/gen/
cp -v input_format_validators/* $outdir/gen/
#if [ -f $outdir/gen/validate.py ]; then
#    mv $outdir/gen/validate.py $outdir/gen/valida.py
#else
#    echo '#!/usr/bin/env python2' > $outdir/gen/valida.py
#    chmod a+x $outdir/gen/valida.py
#fi
echo '#!/usr/bin/env python2' > $outdir/gen/valida.py
chmod a+x $outdir/gen/valida.py

cat <<EOF >> $outdir/gen/GEN
# File che elenca gli input e ne specifica la generazione.
# Le parti della riga dopo un # vengono ignorate.
# Le righe non banali (ovvero contenenti qualcosa prima del primo #)
# vengono passate al generatore e generano un input ciascuna.
# Se una riga inizia con "#ST:" (senza le virgolette) delimita un subtask.
# "#ST:" va seguito con il punteggio assegnato a quel subtask.
# Se sono presenti i subtask, la somma dei punti dei subtask DEVE essere 100.

#ST: 0
EOF

mkdir -p $outdir/input/
mkdir -p $outdir/SWERC_instances/sample/
mkdir -p $outdir/SWERC_instances/secret/
mkdir -p $outdir/att/
i=0
for fin in $(ls data/sample/*.in); do 
    cp -v $fin $outdir/input/input$i.txt
    cp -v $fin $outdir/att/input$i.txt
#    cp -v $fin $outdir/SWERC_instances/sample/input$i.txt
#    echo "#COPY: SWERC_instances/sample/$(basename $fin)"  >> $outdir/gen/GEN
    gzip -c $fin > $outdir/SWERC_instances/sample/$(basename $fin).gz
    echo "ZCOPY SWERC_instances/sample/$(basename $fin).gz"  >> $outdir/gen/GEN
    ((i++))
done;

echo >> $outdir/gen/GEN
echo '#ST: 100' >> $outdir/gen/GEN

for fin in $(ls data/secret/*.in); do 
    cp -v $fin $outdir/input/input$i.txt
#    cp -v $fin $outdir/SWERC_instances/secret/input$i.txt
#    echo "#COPY: SWERC_instances/secret/$(basename $fin)"  >> $outdir/gen/GEN
    gzip -c $fin > $outdir/SWERC_instances/secret/$(basename $fin).gz
    echo "ZCOPY SWERC_instances/secret/$(basename $fin).gz"  >> $outdir/gen/GEN
    ((i++))
done;

i=0
mkdir -p $outdir/output/
for fout in $(ls data/sample/*.ans) $(ls data/secret/*.ans); do 
    cp -v $fout $outdir/output/output$i.txt
    ((i++))
done;

echo "# CMS-problem-packet obtained from the following original problem:" > $outdir/task.yaml
cat problem.yaml | while read line
do
    echo -n "# " >> $outdir/task.yaml
    echo $line  >> $outdir/task.yaml
done
echo "# -----------------------------------" >> $outdir/task.yaml
total_num_inputs=$i

echo Input number = $total_num_inputs
echo Problem name = $prefixed_name 
echo Time limit = $time_limit

cat <<EOF >> $outdir/task.yaml
memory_limit: 256
name: $prefixed_name
title: $prefixed_name
time_limit: $time_limit.0
token_mode: infinite
infile: ""
outfile: ""
n_input: $total_num_inputs
EOF

# collochiamo infine un generatore base che si limita a ricopiare la stringa nella riga passatagli da GEN
cat <<EOF >> $outdir/gen/generatore.py
#!/usr/bin/env python2
from sys import argv, exit
import os, string

if len(argv) < 2:
    exit(1)

if argv[1] == "COPY":
    if len(argv) < 3:
        exit(1)
    os.system("cat " + argv[2])
elif argv[1] == "ZCOPY":
    if len(argv) < 3:
        exit(1)
    os.system("zcat " + argv[2])
else:
    os.system("echo " + ' '.join(argv[1:]))    

exit(0)
EOF

chmod a+x $outdir/gen/generatore.py
echo
echo "Ricordati di rinominare una qualche soluzione come soluzione.cpp o suluzione.py se vuoi poter lanciare cmsMake"
echo "Inoltre, sempre nella cartella sol, non puoi avere due file con lo stesso nome come pippo.cpp e pippo.py"
