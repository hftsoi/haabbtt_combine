submitDir="submit"
mkdir -p "${submitDir}"

inputDir=inputfiles

cat > "job.sub" << EOF
executable = rundryrun.sh
log = ${submitDir}/log.log
output = ${submitDir}/output.txt
error = ${submitDir}/error.txt
transfer_input_files = ${inputDir}
requestdisk = 1G
requestmemory = 1G
queue
EOF

condor_submit job.sub
