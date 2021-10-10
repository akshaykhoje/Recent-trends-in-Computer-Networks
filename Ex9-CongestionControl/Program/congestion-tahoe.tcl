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

#Give node position (for NAM)
$ns duplex-link-op $n0 $n1 orient right-up   # (+45deg)
$ns duplex-link-op $n1 $n2 orient right-down # (-45deg)

#Setup a TCP-Tahoe connection
# Using Agent/TCP creates a Tahoe connection
set tcp_1 [new Agent/TCP -a]
$tcp_1 set class_ 2
# Set the source node
$ns attach-agent $n0 $tcp_1

# Create the sink for the connection
set sink_1 [new Agent/TCPSink]
$ns attach-agent $n1 $sink_1
# Establish the connection
$ns connect $tcp_1 $sink_1

# Set other properties
$tcp_1 set fid_ 1
$tcp_1 set window_ 6000
# default packet-size is 1000
$tcp_1 set packetSize_ 1200  

#Setup a TCP-Tahoe connection
set tcp_2 [new Agent/TCP -a]
$tcp_2 set class_ 2
#Set the source node
$ns attach-agent $n0 $tcp_2

# Create the sink for the connection
set sink_2 [new Agent/TCPSink]
# Set the sink node
$ns attach-agent $n1 $sink_2
# Establish the connection
$ns connect $tcp_2 $sink_2

# Set other properties
$tcp_2 set fid_ 2
$tcp_2 set window_ 6000
$tcp_2 set packetSize_ 900
