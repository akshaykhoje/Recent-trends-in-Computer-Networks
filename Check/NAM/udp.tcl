set ns [new Simulator]
$ns rtproto LS

$ns color 1 Red 
$ns color 2 Blue 

set fd_1 [open "UDP_op.tr" w]
$ns trace-all $fd_1

set fd_2 [open "UDP_op.nam" w]
$ns namtrace-all $fd_2 

proc finish {} {
    global ns fd_1 fd_2 
    $ns flush-trace 
    close $fd_1
    close $fd_2
    exec nam "UDP_op.nam" &
    exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 1Mb 10ms DropTail 
$ns duplex-link $n1 $n2 2Mb 10ms DropTail 

$ns queue-limit $n0 $n1 20

$ns duplex-link-op $n0 $n1 orient right 
$ns duplex-link-op $n1 $n2 orient down

set udp [new Agent/UDP]
$udp set class_ 1
$udp set fid_ 1
$ns attach-agent $n0 $udp 

set null [new Agent/Null]
$ns attach-agent $n2 $null 

$ns connect $udp $null 

set CBR [new Application/Traffic/CBR]
$CBR set type_ CBR 
$CBR set packet_size_ 1000
$CBR set rate_ 0.2
$CBR set random_ false
$CBR attach-agent $udp

$ns at 0.5 "$CBR start"
$ns rtmodel-at 1.2 down $n0 $n1
$ns rtmodel-at 1.7 up $n0 $n1
$ns at 2.0 "$CBR stop"

$ns at 2.5 "finish"
$ns run



