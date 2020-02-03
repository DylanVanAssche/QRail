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
mkdir -p traveltime/0/rush/pushing
mkdir -p traveltime/1/rush/pushing
mkdir -p traveltime/2/rush/pushing
mkdir -p traveltime/3/rush/pushing
mkdir -p traveltime/4/rush/pushing

echo "Brussels-Central -> Brussels-South"
./qrail-cli http://irail.be/stations/NMBS/008813003 http://irail.be/stations/NMBS/008814001 $DATE 4 --sse > traveltime/0/rush/pushing/brusselscentral-brusselssouth.txt 2>&1 & # Brussels-Central -> Brussels-South, 0 - 10 min travel time
PID=$!
run_bench "traveltime/0/rush/pushing/brusselscentral-brusselssouth"

echo "Leuven -> Zichem"
./qrail-cli http://irail.be/stations/NMBS/008833001 http://irail.be/stations/NMBS/008833274 $DATE 4 --sse > traveltime/0/rush/pushing/leuven-zichem.txt 2>&1 & # Leuven -> Zichem, 20 - 30 min travel time
PID=$!
run_bench "traveltime/0/rush/pushing/leuven-zichem"

echo "Dendermonde -> Brussels-Central"
./qrail-cli http://irail.be/stations/NMBS/008893401 http://irail.be/stations/NMBS/008813003 $DATE 4 --sse > traveltime/1/rush/pushing/dendermonde-brusselscentral.txt 2>&1 & # Dendermonde -> Brussels-Central, 30 - 40 min travel time
PID=$!
run_bench "traveltime/1/rush/pushing/dendermonde-brusselscentral"

echo "Landen -> Diest"
./qrail-cli http://irail.be/stations/NMBS/008833605 http://irail.be/stations/NMBS/008831401 $DATE 4 --sse > traveltime/1/rush/pushing/landen-diest.txt 2>&1 & # Landen -> Diest, 50 - 60 min travel time
PID=$!
run_bench "traveltime/1/rush/pushing/landen-diest"

echo "Leopoldsburg -> Antwerp-Berchem"
./qrail-cli http://irail.be/stations/NMBS/008832003 http://irail.be/stations/NMBS/008821121 $DATE 4 --sse > traveltime/2/rush/pushing/leopoldsburg-antwerpberchem.txt 2>&1 & # Leopoldsburg -> Antwerp-Berchem, 60 - 70 min travel time
PID=$!
run_bench "traveltime/2/rush/pushing/leopoldsburg-antwerpberchem"

echo "Puurs -> Brussels-Central"
./qrail-cli http://irail.be/stations/NMBS/008822715 http://irail.be/stations/NMBS/008813003 $DATE 4 --sse > traveltime/2/rush/pushing/puurs-brusselscentral.txt 2>&1 & # Puurs -> Brussels-Central, 80 - 90min travel time
PID=$!
run_bench "traveltime/2/rush/pushing/puurs-brusselscentral"

echo "Kortrijk -> Brussels-Central"
./qrail-cli http://irail.be/stations/NMBS/008896008 http://irail.be/stations/NMBS/008813003 $DATE 4 --sse > traveltime/3/rush/pushing/kortrijk-brusselscentral.txt 2>&1 & # Kortrijk -> Brussels-Central, 90 - 100min travel time
PID=$!
run_bench "traveltime/3/rush/pushing/kortrijk-brusselscentral"

echo "Charleroi-Sud -> Antwerp-Central"
./qrail-cli http://irail.be/stations/NMBS/008872009 http://irail.be/stations/NMBS/008821006 $DATE 4 --sse > traveltime/3/rush/pushing/charleroisud-antwerpcentral.txt 2>&1 & # Charleroi-Sud -> Antwerp-Central, 110 - 120min travel time
PID=$!
run_bench "traveltime/3/rush/pushing/charleroisud-antwerpcentral"

echo "Liege-Carre -> Brussels-North"
./qrail-cli http://irail.be/stations/NMBS/008841558 http://irail.be/stations/NMBS/008812005 $DATE 4 --sse > traveltime/4/rush/pushing/liegecarre-brusselsnorth.txt 2>&1 & # Liege-Carre -> Brussels-North, 120 - 130min travel time
PID=$!
run_bench "traveltime/4/rush/pushing/liegecarre-brusselsnorth"

echo "Benchmark ended: `date`"
