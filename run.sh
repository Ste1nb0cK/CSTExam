set -euo pipefail

process_line() {
    # Parse the tokens in the current line into variables
    local tokens=("$@")
    local beta="${tokens[0]:-}"
    local n="${tokens[1]:-}"
    local delta="${tokens[2]:-}"
    local directoryname="beta${beta}n${n}delta${delta}"
    local outputname="${directoryname}/rawoutput.out" # save all the outputs
    mkdir -p $directoryname
    ./h2.x ${beta} ${n} ${delta}  > $outputname
    # Print columns separately
    awk '{print $1}' $outputname > $directoryname/r.out
    awk '{print $2}' $outputname > $directoryname/epot.out
    awk '{print $3}' $outputname > $directoryname/ekin.out
    awk '{print $4}' $outputname > $directoryname/etot.out
    awk '{print $5}' $outputname > $directoryname/p.out
}

# Read from arguments if provided, otherwise from stdin
if [ "$#" -gt 0 ]; then
    # Case 1: arguments supplied
    inputs=("$@")
else
    # Case 2: no arguments, read from stdin
    # Read all lines from stdin 
    while IFS= read -r line || [[ -n "$line" ]]; do
        read -r -a tokens <<< "$line"
        process_line "${tokens[@]}"
    done
fi

#
# Extract the  numbers (average and statistical error)
# read avg err < <(
#     awk '{print $4}' "$outroute" | \
#     ./statfor.x | \
#     grep -E 'average|statistical error' | \
#     awk '/average/ {a=$NF} /statistical error/ {b=$NF} END {print a, b}'
# )
# Move all the outfiles to the directory
# mv -f blocking.out corr.out histo.out $directoryname 
# Print beta average statistical error
# echo $beta $avg $err 
