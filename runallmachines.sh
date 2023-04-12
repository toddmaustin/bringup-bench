#!/bin/bash
declare user=taustin
declare workdir=tools/dev/WorkBench-1.0


# machines available
declare farm_hosts=(beta zeta skellam coxian cartman donald)

# simulator parameters to use in each machine
# the first parameter will be used for the first machine
# in the list above, the second parameter for the second machine, etc.


declare simparam=(
    'MOPTS=ev6:EV6:2048:1:0:16384'
    'MOPTS=steely:STEELY:512:1:0:4:1000000000'
    'MOPTS=hesson:HESSON:64:2:14:1000000000'
    'MOPTS=moshovos:MOSHOVOS:1024:1:0:1000000000'
    'MOPTS=blind:BLIND'
    'MOPTS=oracle:ORACLE'
)

# echo ${simparam[*]}
# echo ${farm_hosts[*]}

declare -i count=${#farm_hosts[*]}
declare -i i=0


if [ $count -ne  ${#simparam[*]} ] 
 then
    echo the numbers of hosts and simulation requests do no match
    echo farm_hosts: ${#farm_hosts[*]}
    echo simparam:  ${#simparam[*]}
    exit 1
fi

echo Preparing to run $count remote jobs...


while [ $i -lt $count ]
do
    declare machine=${farm_hosts[$i]}
    declare scriptfile=run$machine.sh
    declare params=${simparam[$i]}
    echo Starting jobs on $machine with parameters:\'$params\'
    echo "cd $workdir" > $scriptfile
    echo "./runexps.sh all run-tests $params > $machine.out 2> $machine.err" >> $scriptfile
    echo "touch ./$machine.done" >> $scriptfile  
    chmod +x $scriptfile
    ssh -f $user@$machine $workdir/$scriptfile 
    i=$[$i+1]
done

