#!/usr/bin/env python3

import traceback
import socket
import subprocess
import time


#subprocess.run(["killall", "cserver" ])
#proc = subprocess.Popen(
#        ["./cserver"],
#        stdin = subprocess.PIPE,
#        stdout = subprocess.PIPE,
#        )
#time.sleep(2)

error = 0

endpoint = ("localhost", 1982)

def get_socket():
    return socket.socket(socket.AF_INET,socket.SOCK_STREAM)

def test_connect():
    #setup
    sock = get_socket()
    #test

    try:
        sock.connect(endpoint)
    except Exception:
        print("\n\033[91m!!TEST_CONNECT FAILED!!\033[0m\n")
        print(traceback.format_exc());
        global error
        error = 1
        return
    #teardown
    print("\033[92mtest connect passed\033[0m")
    sock.shutdown(socket.SHUT_RDWR)
    sock.close()

def test_message():
    #setup
    sock = get_socket()
    #test
    
    try:
        sock.connect(endpoint)
        sock.sendall(b"Hello\n")
        data = sock.recv(1024)
        assert data == b"anon | Hello\n"
    except Exception:
        print("\n\033[91m!!TEST_MESSAGE FAILED!!\033[0m\n")
        print(traceback.format_exc());
        global error
        error = 1
        return

    #teardown
    print("\033[92mtest message passed\033[0m")
    sock.shutdown(socket.SHUT_RDWR)
    sock.close()

#will fail if MAX_CLIENTS is lower than 3
def test_multiple_people():
    #setup
    num_socks = 3
    sockets = []
    for i in range(num_socks):
        sockets.append(get_socket())

    
    
    try:
        for sock in sockets:
            sock.connect(endpoint)
        
        for sock in sockets:
            sock.settimeout(5.0)

        for sock in sockets:
            sock.sendall(b"hello\n")
            for s in sockets:
                resp = s.recv(256)
                assert resp == b"anon | hello\n"
    
    except Exception:
        print("\n\033[91m!!TEST_PEOPLE FAILED!!\033[0m\n")
        print(traceback.format_exc());
        global error
        error = 1
        return

    #teardown
    print("\033[92mtest people passed\033[0m")
    for sock in sockets:
        sock.shutdown(socket.SHUT_RDWR)
        sock.close()

def test_nick():
    #setup
    dave = get_socket()
    timo = get_socket()
    trif = get_socket()

    dave.settimeout(5.0)
    timo.settimeout(5.0)
    trif.settimeout(5.0)
    #test
    try:
        dave.connect(endpoint)
        timo.connect(endpoint)
        trif.connect(endpoint)

        dave.settimeout(5.0)
        timo.settimeout(5.0)
        trif.settimeout(5.0)

        timo.sendall(b"hello!\n")
        reply1 = timo.recv(1024)
        reply2 = dave.recv(1024)
        reply3 = trif.recv(1024)

        assert reply1 == b"anon | hello!\n"
        assert reply2 == b"anon | hello!\n"
        assert reply3 == b"anon | hello!\n"

        timo.sendall(b"/nick timo\n")

        timo.recv(1024)
        dave.recv(1024)
        trif.recv(1024)

        timo.sendall(b"im timo")

        reply1 = timo.recv(1025)
        reply2 = dave.recv(1025)
        reply3 = trif.recv(1025)

        assert reply1 == b"timo | im timo\n"
        assert reply2 == b"timo | im timo\n"
        assert reply3 == b"timo | im timo\n"

        dave.sendall(b"/nick timo")
        dave.recv(1024)
        dave.sendall(b"im gay")

        reply1 = timo.recv(1025)
        reply2 = dave.recv(1025)
        reply3 = trif.recv(1025)

        assert reply1 == b"anon | im gay\n"
        assert reply2 == b"anon | im gay\n"
        assert reply3 == b"anon | im gay\n"

    except Exception:
        print("\n\033[91m!!TEST_NICK FAILED!!\033[0m\n")
        print(traceback.format_exc());
        global error
        error = 1
        return;
    #teardown
    print("\033[92mtest nick passed\033[0m")
    dave.shutdown(socket.SHUT_RDWR)
    timo.shutdown(socket.SHUT_RDWR)
    trif.shutdown(socket.SHUT_RDWR)
    dave.close()
    timo.close()
    trif.close()
print("")

test_connect()
test_message()
test_multiple_people()
test_nick()
exit(error)
#proc.stdin.write(b"stop\n")
