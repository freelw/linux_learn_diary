for i in `seq 0 31`;
do
let "value=1<<$i";
printf "echo %X > /sys/class/net/eth0/queues/tx-%d/xps_cpus"  $value $i;
echo 
done