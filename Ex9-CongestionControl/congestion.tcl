#Create a simulator object
set ns [new Simulator]

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red

#Open the NAM trace file
set nf [open congestion_out.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
        global ns nf
        $ns flush-trace
        #Close the NAM trace file
        close $nf
        #Execute NAM on the trace file
        exec nam out.nam &
        exit 0
}

#Create three nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

#Create links between the nodes
$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

#Give node position (for NAM)
$ns duplex-link-op $n0 $n1 orient right-up   # (+45deg)
$ns duplex-link-op $n1 $n2 orient right-down # (-45deg)

#Setup a TCP-Tahoe connection
set tcp_1 [new Agent/TCP -a]
$tcp_1 set class_ 2
$ns attach-agent $n0 $tcp_1

set sink_1 [new Agent/TCPSink]
$ns attach-agent $n1 $sink_1
$ns connect $tcp_1 $sink_1
$tcp_1 set fid_ 1

#Setup a TCP-Tahoe connection
set tcp_2 [new Agent/TCP -a]
$tcp_2 set class_ 2
$ns attach-agent $n0 $tcp_2

set sink_2 [new Agent/TCPSink]
$ns attach-agent $n1 $sink_2
$ns connect $tcp_2 $sink_2
$tcp_2 set fid_ 2
