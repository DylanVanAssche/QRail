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

DATE=2019-11-28T10:00:00.000Z # 11h00 Belgian time
cd .. # Binary 1 level higher

echo "Creating directories..."
mkdir -p transfers/0/nonrush/pushing
mkdir -p transfers/1/nonrush/pushing
mkdir -p transfers/2/nonrush/pushing

echo "Hasselt -> Sint-Truiden"
./qrail-cli http://irail.be/stations/NMBS/008831005 http://irail.be/stations/NMBS/008831807 $DATE 4 --sse > transfers/0/nonrush/pushing/hasselt-sintruiden.txt 2>&1 & # Hasselt -> Sint-Truiden, 0 transfers
PID=$!
run_bench "transfers/0/nonrush/pushing/hasselt-sintruiden"

echo "Leuven -> Schaarbeek"
./qrail-cli http://irail.be/stations/NMBS/008833001 http://irail.be/stations/NMBS/008811007 $DATE 4 --sse > transfers/0/nonrush/pushing/leuven-schaarbeek.txt 2>&1 & # Leuven -> Schaarbeek, 0 transfers
PID=$!
run_bench "transfers/0/nonrush/pushing/leuven-schaarbeek"

echo "Landen -> Diest"
./qrail-cli http://irail.be/stations/NMBS/008833605 http://irail.be/stations/NMBS/008831401 $DATE 4 --sse > transfers/1/nonrush/pushing/landen-diest.txt 2>&1 & # Landen -> Diest, 1 transfers
PID=$!
run_bench "transfers/1/nonrush/pushing/landen-diest"

echo "Asse -> Antwerp-Berchem"
./qrail-cli http://irail.be/stations/NMBS/008812070 http://irail.be/stations/NMBS/008821121 $DATE 4 --sse > transfers/1/nonrush/pushing/asse-antwerpberchem.txt 2>&1 & # Asse -> Antwerp-Berchem, 1 transfers
PID=$!
run_bench "transfers/1/nonrush/pushing/asse-antwerpberchem"

echo "Eppegem -> Brussels-Schuman"
./qrail-cli http://irail.be/stations/NMBS/008822269 http://irail.be/stations/NMBS/008811916 $DATE 4 --sse > transfers/2/nonrush/pushing/eppegem-brusselsschuman.txt 2>&1 & # Eppegem -> Brussels-Schuman, 2 transfers
PID=$!
run_bench "transfers/2/nonrush/pushing/eppegem-brusselsschuman"

echo "Antwerp-Central -> Alken"
./qrail-cli http://irail.be/stations/NMBS/008821006 http://irail.be/stations/NMBS/008831039 $DATE 4 --sse > transfers/2/nonrush/pushing/antwerpcentral-alken.txt 2>&1 & # Antwerp-Central -> Alken, 2 transfers
PID=$!
run_bench "transfers/2/nonrush/pushing/antwerpcentral-alken"

echo "Benchmark ended: `date`"
