# Some material for references

<p>This may help you in your assignments and/or practical understanding of concepts which is usually what our faculty expects 👌.</p>

<br>

**Be ready for application-based learning in Networking!**

---

## SDN topology using mininet
```
https://medium.com/@click4abhishekagarwal/getting-started-with-sdn-597663e5caef
http://mininet.org/walkthrough/
```

Simple code for testing

```
from mininet.topo import Topo
from mininet.net import Mininet

class SimpleTopology(Topo):
    def init(self):
        Topo.init(self)

        switch = self.addSwitch('s1')
        host1 = self.addHost('h1')
        host2 = self.addHost('h2')
        
        self.addLink(host1, switch)
        self.addLink(host2, switch)

topo = SimpleTopology()
net = Mininet(topo=topo)
net.start()
net.pingAll()
net.stop()
```

---

## SAN and NAS
```
https://www.sannastechie.tk/2018/11/details-explanation-of-san-storage-architecture.html
https://www.backblaze.com/blog/whats-the-diff-nas-vs-san/
```

## Web-proxy
This one's pretty interesting and simple. Have a good read!
```
https://github.com/feamster/COS461-Public/blob/master/assignments/assignment5/README.md
```


