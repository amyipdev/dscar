#!/usr/bin/env python3

import socket
import os

HOST = "0.0.0.0"
PORT = 8080

# DCP (DSCar Control Protocol)
# Odd 3codes are server codes. Evens are client codes.
# 100 Hello - 101 Salam
# 200    - 201 Okay
# 202 Kill  - 203 Dead
# 500 DNE   - 501 Bad Request
# DNE DNE   - 503 Illegal Instruction

# TODO: determine why it doesn't allow for an easy restart on the pi
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    while True:
        conn, addr = s.accept()
        with conn:
            while True:
                data = conn.recv(1024)
                if len(data) == 0:
                    break
                if "DSCAR_DEBUG" not in os.environ:
                    print(data)
                try:
                    sc = int(data[:3])
                except:
                    conn.send(b"501 Bad Request")
                    continue
                match sc:
                    case 100:
                        conn.send(b"101 Salam")
                    case 102:
                        conn.send(b"103 Goodnight")
                    case 200:
                        # TODO: Car control logic dispatcher
                        conn.send(b"201 Okay")
                        print(data.split(b"\n")[-1])
                    case 202:
                        # TODO: Kill switch
                        conn.send(b"203 Dead")
                    case _:
                        conn.send(b"503 Illegal Instruction")
            # kill if connection drops
