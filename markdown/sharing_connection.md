This page explains how to configure a Raspberry Pi—or any computer with both Wi-Fi and Ethernet—to share its Wi-Fi connection over the Ethernet port.

# Architecture

In the example below the Rasberry Pi is connected with a Router:
```
                         ┌─────────────────────────────┐
                         │           Internet          │
                         └──────────────▲──────────────┘
                                        │
                                  Wi-Fi (client)
                                        │ wlan0
                         ┌──────────────┴──────────────┐
                         │        Raspberry Pi 5       │
                         │  - Wi-Fi uplink (wlan0)     │
                         │  - NAT + DHCP on eth0       │
                         │  - eth0 = 10.42.0.1/24      │
                         └──────────────┬──────────────┘
                                        │ eth0
                                        │ (UTP cable)
                          (WAN)         │
                         ┌──────────────┴──────────────┐
                         │           Router            │
                         │  WAN: DHCP -> 10.42.0.2     │
                         │  LAN: 192.168.1.1/24        │
                         └──────────────┬──────────────┘
                                        │ (LAN switch)
                     ┌──────────────────┼──────────────────┐
                     │                  │                  │
                192.168.1.10       192.168.1.20       192.168.1.30
                  (PC)                (TV)               (IoT)
```

With the above architecture the path of an outgoing will be:
```
192.168.1.10 ->
    Router (NAT to 10.42.0.2) ->
        Pi eth0 10.42.0.1 (NAT) ->
            wlan0 -> Internet
```

# Instructions
To get the list of wifi networks from the command line:
```
nmcli dev wifi list
```
To connect to a wifi network from the command line:
```
nmcli dev wifi connect "YOUR_SSID" password "YOUR_WIFI_PASSWORD"
```

List current connection per network interface:
```
nmcli con show
```
Share network with "Wired connection 1":
```
nmcli con mod "Wired connection 1" ipv4.method shared ipv6.method ignore connection.autoconnect yes
nmcli con down "Wired connection 1" || true
nmcli con up "Wired connection 1"
```