for i in {1..100000};
do
	echo $i
#	sleep 1
	./test_server.py
done
