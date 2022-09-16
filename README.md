# UCS1511-ComputerNetworks-Labwork

Programs, reports, documentation and screenshots implemented and designed for the laboratory coursework in `UCS1511: Computer Networks` 

## Experiments

All experiment reports can be found [here](./Reports).

### Networking Functions using System Calls in C

1. [Theoretical Study](./Ex1-TheoreticStudy)
    - Study the basic network commands in GNU systems.
    - Study the common system calls in the C language.
    - [Network Commands in *nix](./Ex1-TheoreticStudy/A_NetworkCommands.pdf)
    - [System Calls in C](./Ex1-TheoreticStudy/B_SystemCalls.pdf)
    <br>
    
2. [TCP Echo Server and Chat Server](./Ex2-TCP)
    - Implement an Echo Server using C and the TCP protocol
    - Implement a File Server using C and the TCP protocol
    - [Implementations and Reports](./Ex2-TCP)
      - **A**: Echo server
      - **B**: File server
    <br>
    
 3. [UDP Chat Server - Single and Multi client](./Ex3-UDP)
    - Implement a single-client chat server using C and the UDP protocol
    - Implement a multi-client chat server using C and the UDP protocol
    - [Implementations and Reports](./Ex3-UDP)
      - **A**: Single-Client
      - **B**: Multi-Client
    <br>
    
4. [ARP Simulation](./Ex4-ARPSimulation)
  - Simulate the working of ARP using socket programming in the C language.
  - Use a TCP-based multi-client server to exchange messages.
  - [Sample Output](./Ex4-ARPSimulation/Documentation/Output.png)
  - [Report](./Ex4-ARPSimulation/Documentation/Report.pdf)
  <br>

5. [DNS Simulation](./Ex5-DNSSimulation)
  - Simulate the working of Domain Name Server using socket programming in the C
language.
  - Use a UDP-based multi-client server to exchange messages.
  - [Sample Output](./Ex5-DNSSimulation/Documentation/Outputs)
  - [Report](./Ex5-DNSSimulation/Documentation/Report.pdf)
  <br>

6. [Hamming Code for Error Correction](./Ex6-HammingCode)
  - Implement Hamming Code for Single Error Correction using C socket program.
  - Use a TCP-based transmission server to exchange messages.
  - [Sample Output](./Ex6-HammingCode/Documentation/Outputs)
  - [Report](./Ex6-HammingCode/Documentation/Report.pdf)
  <br>


7. [HTTP Web Client](./Ex7-WebpageDownload)
  - Implement a HTTP web client program using C socket program.
  - Construct a TCP client to connect with the web server and download a file.
  - [Sample Output](./Ex7-WebpageDownload/Documentation/Outputs)
  - [Report](./Ex7-WebpageDownload/Documentation/Report.pdf)
  <br>
  
### Network Simulations and Analyses using NS2 and NAM
  
8. [Simple Network Simulation using NS2 and NAM](./Ex8-NetworkSimulator)
  - Study the Network Simulator (NS2) using a simple tcl-script generated network schedule
comprising of FTP traffic using TCP packets.
  - Additionally, construct a constant bitrate UDP packet traffic directed
to the same node 
  - [Sample Output](./Ex8-NetworkSimulator/Outputs)
  - [Report](./Ex8-NetworkSimulator/Documentation/Report.pdf)
  <br>
  
9. [Congestion Control](./Ex9-CongestionControl)
  - Analyze the difference between the congestion control mechanism offered by Tahoe and Reno
TCP agents using NS2.
  - Use a network schedule comprising of FTP traffic using TCP packets for the analysis.
  - [Sample Output](./Ex9-CongestionControl/Documentation/Outputs)
  - [Report](./Ex9-CongestionControl/Documentation/Report.pdf)
  <br>
  
10. [Routing Protocols](./Ex10-RoutingProtocols)
  - Analyze the difference between the the distance-vector and link-state routing protocols when
executing two UDP connections in a network using NS2.
  - Use a network schedule comprising of FTP traffic using UDP datagrams for the analysis.
  - [Sample Output](./Ex10-RoutingProtocols/Documentation/Outputs)
  - [Report](./Ex10-RoutingProtocols/Documentation/Report.pdf)
  <br>
  
11. [Comparison and TCP and UDP Protocols](./Ex11-TCP-UDP)
  - Analyze the difference between a TCP and UDP connection sharing a bottle-neck link using NS2.
  - Use a network schedule comprising of FTP and CBR traffic for the TCP and UDP networks, respectively.
  - [Sample Output](./Ex11-TCP-UDP/Documentation/Outputs)
  - [Report](./Ex11-TCP-UDP/Documentation/Report.pdf)
  <br>
