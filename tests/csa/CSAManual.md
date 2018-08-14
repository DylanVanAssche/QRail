# CSA Manual

Debugging the Connection Scan Algorithm (CSA) is hard, to make it easier we
ran the algorithm manually for a trip between *Vilvoorde* and *Brugge*.

The reasons why we choose those 2 specific stations are simple:

- At least 1 transfer is included
- Transferring can be happening in several stations, it's not limited to a specific station
- This test covers all the CSA code with just one test

:bulb: The result of this manual CSA calculation is used in the unit test for the algorithm.

## Parameters

|      Parameter    | Value                       |
| ----------------- | --------------------------- |
| Departure time    | 02/08/2018 13:00:00.000 UTC |
| Arrival time      | N/A                         |
| Max transfers     | 4                           |
| Departure station | Vilvoorde (008811189)       |
| Arrival station   | Brugge (008891009)          |
| Filtered vehicles | S11788, IC539, IC540        |

## Linked Connections fragments

We list the Linked Connections fragments for the filtered vehicles below.
They are sorted by departure time in *descending* order (18:00:00 :arrow_right: 13:00:00) as requested by the Connection Scan Algorithm.

:warning: To keep everything compact we merged the pages into one big array already. The complete pages are available as JSON files: `connectionsDATE.json`.

```json

[
	{
      "@id": "http://irail.be/connections/8891009/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008891009",
      "arrivalStop": "http://irail.be/stations/NMBS/008891702",
      "departureTime": "2018-08-02T18:02:00.000Z",
      "arrivalTime": "2018-08-02T18:16:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8892007/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008892007",
      "arrivalStop": "http://irail.be/stations/NMBS/008891009",
      "departureTime": "2018-08-02T17:38:00.000Z",
      "arrivalTime": "2018-08-02T18:00:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814001/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814001",
      "arrivalStop": "http://irail.be/stations/NMBS/008892007",
      "departureTime": "2018-08-02T17:06:00.000Z",
      "arrivalTime": "2018-08-02T17:35:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8891009/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008891009",
      "arrivalStop": "http://irail.be/stations/NMBS/008891702",
      "departureTime": "2018-08-02T17:04:00.000Z",
      "arrivalTime": "2018-08-02T17:18:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8813003/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008813003",
      "arrivalStop": "http://irail.be/stations/NMBS/008814001",
      "departureTime": "2018-08-02T16:59:00.000Z",
      "arrivalTime": "2018-08-02T17:03:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8812005/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008812005",
      "arrivalStop": "http://irail.be/stations/NMBS/008813003",
      "departureTime": "2018-08-02T16:55:00.000Z",
      "arrivalTime": "2018-08-02T16:58:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8892007/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008892007",
      "arrivalStop": "http://irail.be/stations/NMBS/008891009",
      "departureTime": "2018-08-02T16:40:00.000Z",
      "arrivalTime": "2018-08-02T17:02:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8833001/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008833001",
      "arrivalStop": "http://irail.be/stations/NMBS/008812005",
      "departureTime": "2018-08-02T16:34:00.000Z",
      "arrivalTime": "2018-08-02T16:53:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814241/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814241",
      "arrivalStop": "http://irail.be/stations/NMBS/008814209",
      "departureTime": "2018-08-02T16:27:00.000Z",
      "arrivalTime": "2018-08-02T16:34:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814258/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814258",
      "arrivalStop": "http://irail.be/stations/NMBS/008814241",
      "departureTime": "2018-08-02T16:23:00.000Z",
      "arrivalTime": "2018-08-02T16:27:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814266/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814266",
      "arrivalStop": "http://irail.be/stations/NMBS/008814258",
      "departureTime": "2018-08-02T16:19:00.000Z",
      "arrivalTime": "2018-08-02T16:22:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814167/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814167",
      "arrivalStop": "http://irail.be/stations/NMBS/008814266",
      "departureTime": "2018-08-02T16:15:00.000Z",
      "arrivalTime": "2018-08-02T16:18:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814142/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814142",
      "arrivalStop": "http://irail.be/stations/NMBS/008814167",
      "departureTime": "2018-08-02T16:11:00.000Z",
      "arrivalTime": "2018-08-02T16:14:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814134/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814134",
      "arrivalStop": "http://irail.be/stations/NMBS/008814142",
      "departureTime": "2018-08-02T16:08:00.000Z",
      "arrivalTime": "2018-08-02T16:11:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814001/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814001",
      "arrivalStop": "http://irail.be/stations/NMBS/008892007",
      "departureTime": "2018-08-02T16:06:00.000Z",
      "arrivalTime": "2018-08-02T16:35:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814118/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814118",
      "arrivalStop": "http://irail.be/stations/NMBS/008814134",
      "departureTime": "2018-08-02T16:04:00.000Z",
      "arrivalTime": "2018-08-02T16:07:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8841004/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008841004",
      "arrivalStop": "http://irail.be/stations/NMBS/008833001",
      "departureTime": "2018-08-02T16:02:00.000Z",
      "arrivalTime": "2018-08-02T16:32:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8814001/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008814001",
      "arrivalStop": "http://irail.be/stations/NMBS/008814118",
      "departureTime": "2018-08-02T16:00:00.000Z",
      "arrivalTime": "2018-08-02T16:04:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8813003/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008813003",
      "arrivalStop": "http://irail.be/stations/NMBS/008814001",
      "departureTime": "2018-08-02T15:59:00.000Z",
      "arrivalTime": "2018-08-02T16:03:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8812005/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008812005",
      "arrivalStop": "http://irail.be/stations/NMBS/008813003",
      "departureTime": "2018-08-02T15:55:00.000Z",
      "arrivalTime": "2018-08-02T15:58:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8813037/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008813037",
      "arrivalStop": "http://irail.be/stations/NMBS/008814001",
      "departureTime": "2018-08-02T15:54:00.000Z",
      "arrivalTime": "2018-08-02T15:57:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8813003/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008813003",
      "arrivalStop": "http://irail.be/stations/NMBS/008813037",
      "departureTime": "2018-08-02T15:52:00.000Z",
      "arrivalTime": "2018-08-02T15:54:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8813045/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008813045",
      "arrivalStop": "http://irail.be/stations/NMBS/008813003",
      "departureTime": "2018-08-02T15:49:00.000Z",
      "arrivalTime": "2018-08-02T15:51:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8812005/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008812005",
      "arrivalStop": "http://irail.be/stations/NMBS/008813045",
      "departureTime": "2018-08-02T15:47:00.000Z",
      "arrivalTime": "2018-08-02T15:49:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8811007/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008811007",
      "arrivalStop": "http://irail.be/stations/NMBS/008812005",
      "departureTime": "2018-08-02T15:41:00.000Z",
      "arrivalTime": "2018-08-02T15:45:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8844008/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008844008",
      "arrivalStop": "http://irail.be/stations/NMBS/008841004",
      "departureTime": "2018-08-02T15:39:00.000Z",
      "arrivalTime": "2018-08-02T15:57:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8811148/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008811148",
      "arrivalStop": "http://irail.be/stations/NMBS/008811007",
      "departureTime": "2018-08-02T15:36:00.000Z",
      "arrivalTime": "2018-08-02T15:40:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8833001/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008833001",
      "arrivalStop": "http://irail.be/stations/NMBS/008812005",
      "departureTime": "2018-08-02T15:34:00.000Z",
      "arrivalTime": "2018-08-02T15:53:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8811189/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008811189",
      "arrivalStop": "http://irail.be/stations/NMBS/008811148",
      "departureTime": "2018-08-02T15:32:00.000Z",
      "arrivalTime": "2018-08-02T15:36:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8822269/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008822269",
      "arrivalStop": "http://irail.be/stations/NMBS/008811189",
      "departureTime": "2018-08-02T15:27:00.000Z",
      "arrivalTime": "2018-08-02T15:31:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8844503/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008844503",
      "arrivalStop": "http://irail.be/stations/NMBS/008844008",
      "departureTime": "2018-08-02T15:26:00.000Z",
      "arrivalTime": "2018-08-02T15:38:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8822251/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008822251",
      "arrivalStop": "http://irail.be/stations/NMBS/008822269",
      "departureTime": "2018-08-02T15:24:00.000Z",
      "arrivalTime": "2018-08-02T15:27:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8822004/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008822004",
      "arrivalStop": "http://irail.be/stations/NMBS/008822251",
      "departureTime": "2018-08-02T15:19:00.000Z",
      "arrivalTime": "2018-08-02T15:24:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8844628/20180802/IC540",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008844628",
      "arrivalStop": "http://irail.be/stations/NMBS/008844503",
      "departureTime": "2018-08-02T15:18:00.000Z",
      "arrivalTime": "2018-08-02T15:25:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC540/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC540",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8822343/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008822343",
      "arrivalStop": "http://irail.be/stations/NMBS/008822004",
      "departureTime": "2018-08-02T15:06:00.000Z",
      "arrivalTime": "2018-08-02T15:09:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8841004/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008841004",
      "arrivalStop": "http://irail.be/stations/NMBS/008833001",
      "departureTime": "2018-08-02T15:02:00.000Z",
      "arrivalTime": "2018-08-02T15:32:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8822228/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008822228",
      "arrivalStop": "http://irail.be/stations/NMBS/008822343",
      "departureTime": "2018-08-02T15:01:00.000Z",
      "arrivalTime": "2018-08-02T15:05:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8822210/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008822210",
      "arrivalStop": "http://irail.be/stations/NMBS/008822228",
      "departureTime": "2018-08-02T14:58:00.000Z",
      "arrivalTime": "2018-08-02T15:01:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8821311/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008821311",
      "arrivalStop": "http://irail.be/stations/NMBS/008822210",
      "departureTime": "2018-08-02T14:53:00.000Z",
      "arrivalTime": "2018-08-02T14:57:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8821337/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008821337",
      "arrivalStop": "http://irail.be/stations/NMBS/008821311",
      "departureTime": "2018-08-02T14:50:00.000Z",
      "arrivalTime": "2018-08-02T14:52:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8821246/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008821246",
      "arrivalStop": "http://irail.be/stations/NMBS/008821337",
      "departureTime": "2018-08-02T14:47:00.000Z",
      "arrivalTime": "2018-08-02T14:50:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8821147/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008821147",
      "arrivalStop": "http://irail.be/stations/NMBS/008821246",
      "departureTime": "2018-08-02T14:43:00.000Z",
      "arrivalTime": "2018-08-02T14:47:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8821121/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008821121",
      "arrivalStop": "http://irail.be/stations/NMBS/008821147",
      "departureTime": "2018-08-02T14:40:00.000Z",
      "arrivalTime": "2018-08-02T14:43:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8844008/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008844008",
      "arrivalStop": "http://irail.be/stations/NMBS/008841004",
      "departureTime": "2018-08-02T14:39:00.000Z",
      "arrivalTime": "2018-08-02T14:57:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8821006/20180802/S11788",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008821006",
      "arrivalStop": "http://irail.be/stations/NMBS/008821121",
      "departureTime": "2018-08-02T14:35:00.000Z",
      "arrivalTime": "2018-08-02T14:39:00.000Z",
      "direction": "Nivelles",
      "gtfs:trip": "http://irail.be/vehicle/S11788/20180802",
      "gtfs:route": "http://irail.be/vehicle/S11788",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8844503/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008844503",
      "arrivalStop": "http://irail.be/stations/NMBS/008844008",
      "departureTime": "2018-08-02T14:26:00.000Z",
      "arrivalTime": "2018-08-02T14:38:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    },
		{
      "@id": "http://irail.be/connections/8844628/20180802/IC539",
      "@type": "Connection",
      "departureStop": "http://irail.be/stations/NMBS/008844628",
      "arrivalStop": "http://irail.be/stations/NMBS/008844503",
      "departureTime": "2018-08-02T14:18:00.000Z",
      "arrivalTime": "2018-08-02T14:25:00.000Z",
      "direction": "Ostende",
      "gtfs:trip": "http://irail.be/vehicle/IC539/20180802",
      "gtfs:route": "http://irail.be/vehicle/IC539",
      "gtfs:pickupType": "gtfs:Regular",
      "gtfs:dropOffType": "gtfs:Regular"
    }
]
```
