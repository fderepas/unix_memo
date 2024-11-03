To prevent my ubuntu system from going to sleep:
```
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target
```
To re-enable sleep:
```
sudo systemctl unmask sleep.target suspend.target hibernate.target hybrid-sleep.target
```
