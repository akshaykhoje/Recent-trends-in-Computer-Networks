#Create a simulator object
set ns [new Simulator]
$ns rtproto DV

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red

# Open the trace file
set nf [open distvec_out.tr w]
$ns trace-all $nf

#Open the NAM trace file
set nf [open distvec_out.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
	global ns nf
	$ns flush-trace
	#Close the NAM trace file
	close $nf
	# Extract graph points and plot them
	exec awk -f extract_rtpkts.awk distvec_out.tr > dv_graph.tr &
	exec xgraph dv_graph.tr &
	#Execute NAM on the trace file
	exec nam distvec_out.nam &
	exit 0
}

#Create twelve nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]

#Create links between the nodes
$ns duplex-link $n0 $n8 1Mb 10ms DropTail
$ns duplex-link $n8 $n7 1Mb 10ms DropTail
$ns duplex-link $n7 $n6 1Mb 10ms DropTail
$ns duplex-link $n6 $n2 1Mb 10ms DropTail
$ns duplex-link $n2 $n1 1Mb 10ms DropTail
$ns duplex-link $n1 $n10 1Mb 10ms DropTail
$ns duplex-link $n10 $n11 1Mb 10ms DropTail
$ns duplex-link $n11 $n5 1Mb 10ms DropTail
$ns duplex-link $n5 $n4 1Mb 10ms DropTail
$ns duplex-link $n4 $n3 1Mb 10ms DropTail
$ns duplex-link $n3 $n9 1Mb 10ms DropTail
$ns duplex-link $n9 $n0 1Mb 10ms DropTail
$ns duplex-link $n9 $n11 1Mb 10ms DropTail
#Other links to complete the network

# Set queue-limit on node-11
$ns queue-limit $n10 $n11 50
$ns queue-limit $n5 $n11 50
# Set queue-limit on node-9
$ns queue-limit $n0 $n9 50
$ns queue-limit $n3 $n9 50

#Give node position (for NAM)
#Upper-Ring 
$ns duplex-link-op $n0 $n8 orient right-up
$ns duplex-link-op $n8 $n7 orient right
$ns duplex-link-op $n7 $n6 orient right
$ns duplex-link-op $n6 $n2 orient right
$ns duplex-link-op $n2 $n1 orient right-down
$ns duplex-link-op $n1 $n10 orient left-down
$ns duplex-link-op $n10 $n11 orient left
#Lower-Ring 
$ns duplex-link-op $n11 $n5 orient down
$ns duplex-link-op $n5 $n4 orient left
$ns duplex-link-op $n4 $n3 orient left-up
$ns duplex-link-op $n3 $n9 orient right-up
$ns duplex-link-op $n9 $n0 orient left-up
$ns duplex-link-op $n9 $n11 orient right

#Monitor the queue for link (n11). (for NAM)
$ns duplex-link-op $n10 $n11 queuePos 0.5
$ns duplex-link-op $n5 $n11 queuePos 0.5
#Monitor the queue for link (n9). (for NAM)
$ns duplex-link-op $n9 $n3 queuePos -0.5
$ns duplex-link-op $n9 $n0 queuePos -0.5

#Setup a UDP connection
set udp_1 [new Agent/UDP]
$udp_1 set class_ 1
#Set the source node
$ns attach-agent $n0 $udp_1
# Set the sink node (null)
set null_1 [new Agent/Null]
$ns attach-agent $n5 $null_1
# Create a logical connection between the two agents
# Done by assigning respective IPs mutually
$ns connect $udp_1 $null_1
$udp_1 set fid_ 1
#Setup a CBR over UDP connection
set cbr_1 [new Application/Traffic/CBR]
$cbr_1 attach-agent $udp_1
$cbr_1 set type_ CBR
$cbr_1 set packet_size_ 1000
$cbr_1 set rate_ 0.5mb
$cbr_1 set random_ false

#Setup a UDP connection
set udp_2 [new Agent/UDP]
$udp_2 set class_ 2
#Set the source node
$ns attach-agent $n1 $udp_2
# Set the sink node (null)
set null_2 [new Agent/Null]
$ns attach-agent $n5 $null_2
# Create a logical connection between the two agents
# Done by assigning respective IPs mutually
$ns connect $udp_2 $null_2
$udp_2 set fid_ 2
#Setup a CBR over UDP connection
set cbr_2 [new Application/Traffic/CBR]
$cbr_2 attach-agent $udp_2
$cbr_2 set type_ CBR
$cbr_2 set packet_size_ 1000
$cbr_2 set rate_ 0.5mb
$cbr_2 set random_ false

#Schedule events for the UDP agents
$ns at 0.5 "$cbr_1 start"
$ns at 1.0 "$cbr_2 start"
$ns rtmodel-at 2.0 down $n11 $n5
$ns rtmodel-at 2.5 down $n7 $n6
$ns rtmodel-at 3.0 up $n11 $n5
$ns rtmodel-at 3.5 up $n7 $n6
$ns at 4.0 "$cbr_1 stop"
$ns at 4.5 "$cbr_2 stop"

#Call the finish procedure after 5.5 seconds of simulation time
$ns at 5.5 "finish"

#Run the simulation
$ns run
