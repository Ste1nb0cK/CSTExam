beta=$1 # read from stdin
mkdir -p "beta$beta"
outroute="beta$beta/beta$beta.out"
datafile="h2.vmc"
./h2.x "$beta" > $outroute
# Extract numbers (average and statistical error)
read avg err < <(
    awk '{print $2}' "$outroute" | \
    ./statfor.x | \
    grep -E 'average|statistical error' | \
    awk '/average/ {a=$NF} /statistical error/ {b=$NF} END {print a, b}'
)
# Move all the outfiles to the directory
mv -f blocking.out corr.out histo.out "beta$beta"
# Print beta average statistical error
echo $beta $avg $err 
