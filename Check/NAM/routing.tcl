set ns [new Simulator]
$ns rtproto DV 

$ns color 1 Red 
$ns color 2 Blue 

set nf_1 [open routing_op.tr w]
$ns trace-all $nf_1

set nf_2 [open routing_op.nam w]
$ns namtrace-all $nf_2

proc finish {} {
    global ns nf_1 nf_2 
    $ns flush-trace
    close $nf_1
    close $nf_2
    exec nam routing_op.nam &
    exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

$ns duplex-link $n0 $n1 1Mb 10ms DropTail 
$ns duplex-link $n1 $n2 1Mb 10ms DropTail 
$ns duplex-link $n2 $n3 1Mb 10ms DropTail 
$ns duplex-link $n3 $n0 1Mb 10ms DropTail 

$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient down
$ns duplex-link-op $n2 $n3 orient left 
$ns duplex-link-op $n3 $n0 orient up 

set tcp [new Agent/TCP]
$tcp set window_ 20
$tcp set packetSize 1000
$tcp set class_ 1
$tcp set fid_ 1
$ns attach-agent $n0 $tcp 

set tcp_sink [new Agent/TCPSink]
$ns attach-agent $n2 $tcp_sink 

$ns connect $tcp $tcp_sink

set ftp [new Application/FTP]
$ftp set type_ FTP 
$ftp attach-agent $tcp

$ns at 1.0 "$ftp start"
$ns rtmodel-at 1.5 down $n0 $n1
$ns rtmodel-at 2.5 up $n0 $n1
$ns at 4.0 "$ftp stop"

$ns at 4.5 "finish"
$ns run
