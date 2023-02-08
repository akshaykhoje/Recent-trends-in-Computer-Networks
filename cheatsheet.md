# CHEATSHEET FOR NS2 

***NS2 is an event-driven Network Simulator. Event-driven means, we have to put events on the simulation timeline and move along the timeline and actions are taken while confronting an event.***
```
Event -> what happens
Handler -> what to do
An event is associated with a handler.
```
**Dispatching time**
```
Time when the event is scheduled to occur (firing time, executing time) / stored in the variable "time_" of each event.

Each simulation has exactly one scheduler!
```
---
---

## create a simulator object / setting a variable
---
```
set x 20         
set ns [new Simulator]
```

## unset a variable
---
```
unset x
```

## use the variable value
---
`b=x`
```
set b $x
```

## basic arithmetic operations
---
```
set x 20
set y 32
expr $x + $y   // x + y = 52
```

## conditional statement
---
```
if  [ expr  condition ]
{
	if – body
}
```

## while loop
---
```
while
{
	condition
}
{
	while-body
}
```

## for loop
---
```
for  {  initialization }  {  condition }  { increment/decrement }
{
	Loop body
}
```

## functions 
---
- **used to combine multiple commands into a new command**
```
proc   < procedure name >
{ }
{
	procedure body
}
```
```
proc factorial fact {
 
    if {$fact <= 1} {
 
        return 1
    }
expr $fact * [factorial [expr $fact-1]]
}
```

## setting colours
---
```
$ns color "#FF0000"
$ns color "#FFFFFF"
```

## file handling in TCL
---
```
set fp [open filename r]
set data [read $fp]
close $fp
set fp1 [open filename w]
puts $fp1 "writing to a file"
close $fp1
```

## call a subprocess within another process (exec)
---
```
exec rm $fp
```

## create a trace file
---
- **A trace file is used to store the coverage info or overall network info**
- **To generate a trace file, first create a trace file using <mark>OTcl Script</mark>**
```
set tracefile [open wired.tr w]
$ns trace-all $tracefile
```

## create a animation info or nam file
---
- **Nam stands for Network Animator.**
- **It supports packet transmission, topology, data inspection, etc**
- **During the inspection, it supports animation**
```
set namfile [open wired.nam w]
$ns namtrace-all $namfile
```

## create nodes
---
```
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
```


## create link
---
**droptail means dropping the tail**
```
$ns duplex-link $n0 $n1 5Mb 2ms DropTail
$ns duplex-link $n1 $n2 10Mb 6ms DropTail
$ns duplex-link $n1 $n4 3Mb 2ms DropTail
$ns duplex-link $n4 $n3 100Mb 2ms DropTail
$ns duplex-link $n4 $n5 4Mb 10ms DropTail
```

## setting Queue limit
---
$ns queue-limit $n1 $n4 2

**creation of agents**
```
set udp [new Agent/UDP]
set null [new Agent/Null]
$ns attach-agent $n0 $udp
$ns attach-agent $n3 $null
$ns connect $udp $null
```

## creation of TCP agent
---
set tcp [new Agent/TCP]

set sink [new Agent/TCPSink]
```
$ns attach-agent $n2 $tcp
$ns attach-agent $n5 $sink
$ns connect $tcp $sink
```
## creation of Application CBR, FTP
---
```
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
set ftp [new Application/FTP]
$ftp attach-agent $tcp
```

## start Traffic
---
```
$ns at 1.0 "$cbr start"    //schedule an event
$ns at 2.0 "$ftp start"

$ns at 10.0 "finish"   
proc finish {} {
	global ns tracefile namfile
	$ns flush-trace
        #exec xgraph out0.tr out1.tr out2.tr -geometry 800x400 &
	close $tracefile
	close $namfile
	exit 0
}

puts "Simulation is starting"
$ns run   // start the scheduler
```