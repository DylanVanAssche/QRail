#!/bin/bash
echo "Creating directories..."
mkdir -p transfers/0/nonrush/reference
mkdir -p transfers/1/nonrush/reference
mkdir -p transfers/2/nonrush/reference

echo "Starting benchmark `date`"
function run_bench() {
    echo "Waiting for 10 min..."
    sleep 600 # sleep 10 min
    pkill -f qrail-cli
}
DATE=2019-11-28T09:30:00.000Z # 10h30 Belgian time

./qrail-cli http://irail.be/stations/NMBS/008831005 http://irail.be/stations/NMBS/008831807 $DATE 4 > transfers/0/nonrush/polling/hasselt-sintruiden.txt 2>&1 & # Hasselt -> Sint-Truiden, 0 transfers
run_bench
./qrail-cli http://irail.be/stations/NMBS/008833001 http://irail.be/stations/NMBS/008811007 $DATE 4 > transfers/0/nonrush/polling/leuven-schaarbeek.txt 2>&1 & # Leuven -> Schaarbeek, 0 transfers
run_bench
./qrail-cli http://irail.be/stations/NMBS/008833605 http://irail.be/stations/NMBS/008831401 $DATE 4 > transfers/1/nonrush/polling/landen-diest.txt 2>&1 & # Landen -> Diest, 1 transfers
run_bench
./qrail-cli http://irail.be/stations/NMBS/008812070 http://irail.be/stations/NMBS/008821121 $DATE 4 > transfers/1/nonrush/polling/asse-antwerpberchem.txt 2>&1 & # Asse -> Antwerp-Berchem, 1 transfers
run_bench
./qrail-cli http://irail.be/stations/NMBS/008822269 http://irail.be/stations/NMBS/008811916 $DATE 4 > transfers/2/nonrush/polling/eppegem-brusselsschuman.txt 2>&1 & # Eppegem -> Brussels-Schuman, 2 transfers
run_bench
./qrail-cli http://irail.be/stations/NMBS/008821006 http://irail.be/stations/NMBS/008831039 $DATE 4 > transfers/2/nonrush/polling/antwerpcentral-alken.txt 2>&1 & # Antwerp-Central -> Alken, 2 transfers
run_bench
echo "Benchmark ended: `date`"
