for i in {1..10000};
do
	echo $i
#	sleep 1
	./test_server.py
	if [ $? -ne 0 ]
	then
		break
	fi
done
