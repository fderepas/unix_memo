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
                         │        Raspberry Pi         │
                         │  - Wi-Fi uplink (wlan0)     │
                         │  - NAT + DHCP on eth0       │
                         │  - eth0 = 10.42.0.1/24      │
                         └──────────────┬──────────────┘
                                        │ eth0
                                        │ (UTP cable)
                          (WAN)         │
                         ┌──────────────┴──────────────┐
                         │    Router (for isntance)    │
                         │  WAN: DHCP -> 10.42.0.2     │
                         │  LAN: 192.168.1.1/24        │
                         └──────────────┬──────────────┘
                                        │ (LAN switch)
                     ┌──────────────────┼──────────────────┐
                     │                  │                  │
                192.168.1.10       192.168.1.20       192.168.1.30
                  (PC)                (TV)               (IoT)
```

With the above architecture the path of an outgoing packet will be:
```
192.168.1.10 ->
    Router (NAT to 10.42.0.2) ->
        Pi eth0 10.42.0.1 (NAT) ->
            wlan0 -> Internet
```

# Instructions
Here are the instructions to have the Raspberry Pi to share its conection over the Ethernet port.

To get the list of wifi networks from the command line:
```
sudo nmcli dev wifi list
```
To connect to a wifi network from the command line:
```
sudo nmcli dev wifi connect "YOUR_SSID" password "YOUR_WIFI_PASSWORD"
```

List current connection per network interface:
```
sudo nmcli con show
```
Share network with "Wired connection 1":
```
sudo nmcli con mod "Wired connection 1" ipv4.method shared ipv6.method ignore connection.autoconnect yes
sudo nmcli con down "Wired connection 1" || true
```
