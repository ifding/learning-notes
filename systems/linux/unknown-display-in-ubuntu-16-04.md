## Unknown display in Ubuntu 16.04

In my case I needed the 1360x768 resolution (it only gave me the option for chosing 1024x768 or 800x600)

so here is how I got the right resolution:
```sh
$ xrandr
Screen 0: minimum 8 x 8, current 1024 x 768, maximum 32767 x 32767
HDMI1 disconnected (normal left inverted right x axis y axis)
VGA1 connected primary 1024x768+0+0 (normal left inverted right x axis y axis) 0mm x 0mm
   1024x768      60.00* 
   800x600       60.32    56.25  
   848x480       60.00  
   640x480       59.94 
```

VGA1 is my device, then I created the resolution I wanted

Created a new mode for my resolution:
```sh
$ xrandr --newmode "1360x768_60.00"   84.75  1360 1432 1568 1776  768 771 781 798 -hsync +vsync
```

Finally added the new mode to my VGA1 device:
```sh
$ sudo xrandr --addmode VGA1 1360x768_60.00
```

Then I went to Displays and was able to select the 1360x768 resolution.

```sh
$ xrandr --output VGA1 --mode 1360x768_60.00
```

## Reference
- [Unknown display in Ubuntu 16.04](https://askubuntu.com/questions/860735/unknown-display-in-ubuntu-16-04/927635)
- [Comment 3 for bug 1517046](https://bugs.launchpad.net/ubuntu/+source/xorg-server/+bug/1517046/comments/3)