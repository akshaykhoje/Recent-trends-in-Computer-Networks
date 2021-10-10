#Create a simulator object
set ns [new Simulator]

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red

#Open the NAM trace file
set nf [open tahoe_out.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
        global ns nf
        $ns flush-trace
        #Close the NAM trace file
        close $nf
        #Execute NAM on the trace file
        exec nam tahoe_out.nam &
        exit 0
}

#Create three nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

#Create links between the nodes
$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

#Set Queue Size of link (n1-n2) to 5
$ns queue-limit $n1 $n2 5

#Give node position (for NAM)
$ns duplex-link-op $n0 $n1 orient right-up   
$ns duplex-link-op $n1 $n2 orient right-down 

#Monitor the queue for link (n2-n3). (for NAM)
$ns duplex-link-op $n1 $n2 queuePos 0.5

#Setup a TCP-Reno connection
#Using Agent/TCP creates a Tahoe connection
set tcp_1 [new Agent/TCP/Reno]
$tcp_1 set class_ 2
#Set the source node
$ns attach-agent $n0 $tcp_1

#Create the sink for the connection
set sink_1 [new Agent/TCPSink]
$ns attach-agent $n2 $sink_1
#Establish the connection
$ns connect $tcp_1 $sink_1

#Set other properties
#default packet-size is 1000
$tcp_1 set fid_ 1
$tcp_1 set window_ 20
$tcp_1 set packetSize_ 1200  

#Setup a FTP over TCP connection
set ftp_1 [new Application/FTP]
$ftp_1 attach-agent $tcp_1
$ftp_1 set type_ FTP

#Schedule events for the TCP agent
$ns at 0.5 "$ftp_1 start"
$ns at 5.0 "$ftp_1 stop"

#Call the finish procedure after 5.5 seconds of simulation time
$ns at 5.5 "finish"

#Run the simulation
$ns run

#NOTE: TCP CANNOT be simulated as such. Requires a dummy-FTP Application
#protocol to simulate and schedule
