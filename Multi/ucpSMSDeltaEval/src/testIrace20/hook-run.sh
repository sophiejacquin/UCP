#!/bin/bash
###############################################################################
# This hook is the command that is executed every run.
# Check the examples in examples/
#
# This hook is run in the execution directory (execDir, --exec-dir),
# the same directory where hook-evaluate is executed. Hence, you may
# need to copy extra files needed by the executable to this directory.
#
#
# PARAMETERS:
# $1 is the instance name
# $2 is the candidate number
# The rest ($* after `shift 2') are parameters to the run
#
# RETURN VALUE:
# This hook should print one numerical value: the cost that must be minimized.
# Exit with 0 if no error, with 1 in case of error
###############################################################################
EXE=./run.sh # Here is the path of your algorithm to tune
FIXED_PARAMS=""     # Put here what you always give to your algorithm, for all candidates
INSTANCE=$1         # Don't touch this
CANDIDATE=$2        # Don't touch this
shift 2 || exit 1   # Don't touch this
CAND_PARAMS=$*      # Don't touch this...

STDOUT="stdout.txt"
STDERR="stderr.txt"

# Call the program and catch the standard output in a variable 'result'
$EXE ${CAND_PARAMS} 1> $STDOUT 2> $STDERR
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)

echo "$COST"


