# Tromboneless
An open source device which is capable of measuring the uses oral input to synthesis the output a brass trombone would using the MIDI protocol.


## Steps to install:
1) Install Raspberry Pi OS onto your Raspberry Pi.<br>
   If you need a guide on how to flash a Pi with the OS, please follow this guide: <br>
   https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system <br>
   <br>
2) clone this git repository using the command:<br>
 ```  git clone https://github.com/RyanMcB8/Tromboneless.git ```<br>

## SSH Steps

* SSH pair created on Raspberry with Located in /home/tromboneless/.ssh
* 
1) When generating own pair, make ssh of ed25519 format: ```ssh-keygen -t ed25519```
2) Add your public key to /home/yourusername/.ssh/authorized_keys. If connected to the same network as the pi, you can do this automatically with ```ssh-copy-id yourusername@IP_Address```

## If on the same network as pi:
ssh in with eg ```ssh name@IP_Address```

## If on a different network:

1.) You need to set up your own personal tailscale account. (Note - I think student emails won't work unless via github.)
   Invitations to share access to the pi are made on the owner account, then you need to be logged in to your personal account when you follow the invitation link.
   After this is set up, you work from your personal account.
   
2.) Make sure your public ssh key is on the pi

3.) ssh in with ip address associated with the pi in tailscale (not the same as the default device one seen above): ```ssh name@IP_Address```.
Recommend working within vscode, as you can open a remote window from there which will have a proper GUI hosted by your local machine, but controlling the hardware on the pi.

4.) Once in the pi, make sure you're working within your git branch. Compiling from here will use the actual pi hardware so you can make sure it's working properly.

