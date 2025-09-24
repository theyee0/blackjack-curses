# blackjack-curses
An implementation of Blackjack with an ncurses GUI

## Demo (non-linux users)
There is a video included in the releases, also here for your convenience. It showcases a bit of the gameplay and interface.


https://github.com/user-attachments/assets/c8370f0d-4f44-4e82-a578-d1a5c8d625f9



## Getting the program
If you are an x86-86 linux user, you can go to releases and download the binary. Assuming you have libncurses installed, it should run without a hitch. If you just want a preview, there are also screenshots in the releases section.

### Building from source
If you do not have the correct architecture, you can run the Makefile script to build it specifically for your system:
```
make release
```

Release is recommended because it will apply the most optimization. However, if you wish to debug, or if you are having issues with the optimized build, you can run
```
make debug
```
or just
```
make
```
respectively.
