#!/bin/bash
BENCHMARK_TIME=900 # seconds
INTERFACE="enp1s0f1" #network interface filtering

function run_bench() {
    echo "Benchmark started at `date` for $PID ($1)"
    mkdir -p $1
    pidstat -p $PID 1 | ts '[%Y-%m-%d %H:%M:%S]' > "$1/cpu.txt" &
    pidstat -r -p $PID 1 | ts '[%Y-%m-%d %H:%M:%S]' > "$1/mem.txt" &
    echo "Press CTRL+C to end the benchmark"
    i=0
    until [ $i -gt $BENCHMARK_TIME ]
    do
        i=$((i+1))
        ifconfig -a $INTERFACE | ts '[%Y-%m-%d %H:%M:%S]' >> "$1/network.txt"
        sleep 1
    done
    pkill -f qrail-cli
    pkill -f pidstat
    echo "Benchmark completed"
}

DATE=2019-11-28T07:00:00.000Z # 08h00 Belgian time
cd .. # Binary 1 level higher

echo "Creating directories..."
mkdir -p connections/0/nonrush/reference
mkdir -p connections/1/nonrush/reference
mkdir -p connections/2/nonrush/reference

echo "Leuven -> Diest"
./qrail-cli http://irail.be/stations/NMBS/008833001 http://irail.be/stations/NMBS/008831401 $DATE 4 > connections/0/nonrush/reference/leuven-diest.txt 2>&1 & # Leuven -> Diest, 2 connections
PID=$!
run_bench "connections/0/nonrush/reference/leuven-diest"

echo "Mechelen -> Brussels-Congres"
./qrail-cli http://irail.be/stations/NMBS/008822004 http://irail.be/stations/NMBS/008813045 $DATE 4 > connections/1/nonrush/reference/mechelen-brusselscongres.txt 2>&1 & # Mechelen -> Brussels-Congres, 6 connections
PID=$!
run_bench "connections/1/nonrush/reference/mechelen-brusselscongres"

echo "Antwerp-Central -> Lommel"
./qrail-cli http://irail.be/stations/NMBS/008821006 http://irail.be/stations/NMBS/008832565 $DATE 4 > connections/1/nonrush/reference/antwerpcentral-lommel.txt 2>&1 & # Antwerp-Central -> Lommel, 7 connections
PID=$!
run_bench "connections/1/nonrush/reference/antwerpcentral-lommel"

echo "Charleroi-Sud -> Ronet"
./qrail-cli http://irail.be/stations/NMBS/008872009 http://irail.be/stations/NMBS/008861119 $DATE 4 > connections/2/nonrush/reference/charleroisud-ronet.txt 2>&1 & # Charleroi-Sud -> Ronet, 13 connections
PID=$!
run_bench "connections/2/nonrush/reference/charleroisud-ronet"

echo "Luttre -> Brussels-Congres"
./qrail-cli http://irail.be/stations/NMBS/008871308 http://irail.be/stations/NMBS/008813045 $DATE 4 > connections/2/nonrush/reference/luttre-brusselscongres.txt 2>&1 & # Lutte -> Brussels-Congres, 15 connections
PID=$!
run_bench "connections/2/nonrush/reference/luttre-brusselscongres"

echo "Asse -> Antwerp-Berchem"
./qrail-cli http://irail.be/stations/NMBS/008812070 http://irail.be/stations/NMBS/008821121 $DATE 4 > connections/0/nonrush/reference/asse-antwerpberchem.txt 2>&1 & # Asse -> Antwerp-Berchem, 17 connections
PID=$!
run_bench "connections/0/nonrush/reference/brusselssouth-linkebeek"


echo "Benchmark ended: `date`"
