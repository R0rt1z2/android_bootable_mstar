HOST_PROCESSOR_NUMBER=`cat /proc/cpuinfo | grep processor | wc -l`
HOST_PROCESSOR_NUMBER=$(($HOST_PROCESSOR_NUMBER+0))
if [ $HOST_PROCESSOR_NUMBER -gt 1 ] ; then
	HOST_PROCESSOR_NUMBER=1
fi
echo "HOST_PROCESSOR_NUMBER: " $HOST_PROCESSOR_NUMBER
make -s -j$HOST_PROCESSOR_NUMBER
