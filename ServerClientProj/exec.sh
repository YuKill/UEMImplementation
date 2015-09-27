#! /bin/bash


cd server
./serv1 9990 &
./serv2 9991 &
./serv3 9992 &
cd ..
./inter/InterMachineStart &
./client/ClientStart
