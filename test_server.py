#!/usr/bin/env python3

import traceback
import socket

def get_socket():
    return socket.socket(socket.AF_INET,socket.SOCK_STREAM)

def test_connect():
    #setup
    sock = get_socket()

    #test

    try:
        sock.connect(("localhost",1982))
    except Exception:
        print("\n\033[91m!!TEST_CONNECT FAILED!!\033[0m\n")
        print(traceback.format_exc());
        return
    #teardown
    print("\033[92mtest connect passed\033[0m")
    sock.close()

def test_message():
    #setup
    sock = get_socket()

    #test

    try:
        sock.connect(("localhost", 1982))
        sock.sendall(b"Hello\n")
        data = sock.recv(1024)
        assert data == b"anon | Hello\n"
    except Exception:
        print("\n\033[91m!!TEST_MESSAGE FAILED!!\033[0m\n")
        print(traceback.format_exc());
        return

    #teardown
    print("\033[92mtest message passed\033[0m")
    sock.close()

#will fail if MAX_CLIENTS is lower than 3
def test_multiple_people():
    #setup
    c1 = get_socket()
    c2 = get_socket()
    c3 = get_socket()

    try:
        c1.connect(("localhost", 1982))
        c2.connect(("localhost", 1982))
        c3.connect(("localhost", 1982))

        resps = []

        c1.sendall(b"hello\n")
        resps.append(c1.recv(256))
        resps.append(c2.recv(256))
        resps.append(c3.recv(256))

        c2.sendall(b"hello\n")
        resps.append(c1.recv(256))
        resps.append(c2.recv(256))
        resps.append(c3.recv(256))

        c3.sendall(b"hello\n")
        resps.append(c1.recv(256))
        resps.append(c2.recv(256))
        resps.append(c3.recv(256))

        for x in resps:
            assert x == b"anon | hello\n"

    except Exception:
        print("\n\033[91m!!TEST_PEOPLE FAILED!!\033[0m\n")
        print(traceback.format_exc());
        return

    #teardown
    print("\033[92mtest people passed\033[0m")
    c1.close()
    c2.close()
    c3.close()

print("")

test_connect()
test_message()
test_multiple_people()
