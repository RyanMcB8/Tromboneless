#!/bin/bash

set -e 
cd Documentation || { echo "Documentation folder not found"; exit 1; }


echo "Cleaning old LaTeX auxiliary files..."
rm -f refman.aux refman.log refman.out refman.toc

echo "Running Doxygen..."
doxygen doxygen_config &  
DOXY_PID=$!
# Wait for Doxygen to finish completely
echo "Waiting for Doxygen to finish..."
wait $DOXY_PID


INDEX_FILE="latex/index.tex"
echo "Checking that $INDEX_FILE is fully written..."
while true; do
    if [[ -f "$INDEX_FILE" ]]; then
        SIZE1=$(stat -c%s "$INDEX_FILE")
        sleep 2
        SIZE2=$(stat -c%s "$INDEX_FILE")
        if [[ $SIZE1 -eq $SIZE2 ]]; then
            echo "$INDEX_FILE is stable."
            break
        fi
    else
        sleep 1
    fi
done


cd latex || { echo "Latex folder not found"; exit 1; }
echo "Compiling refman.tex..."
pdflatex refman.tex
pdflatex refman.tex
echo "Build complete!"