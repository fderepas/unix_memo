Available space in volume group (VG)
```
vgs
```
Available space on the specific physical volume to use:
```
pvs
```
Size of current logical volume:
```
lvs
```

lvextend -l+100%FREE /dev/ubuntu-vg/ubuntu-lv 
lvextend -l+100%FREE  /dev/mapper/ubuntu--vg-ubuntu--lv 
