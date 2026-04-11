# Tromboneless
An open source device which is capable of measuring the users oral input to synthesise the output a brass trombone would using the MIDI protocol.

Please refer to our [wiki](https://github.com/RyanMcB8/Tromboneless/wiki) to see componenent, sequence and state diagrams for the MIDI implementation.

## Steps to install:
1) Install Raspberry Pi OS onto your Raspberry Pi.<br>
   If you need a guide on how to flash a Pi with the OS, please follow [this guide](https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system) <br>
   <br>

### Option 1
2) clone this git repository and its submodule dependencies using one line one the command line:<br>
 ```  git clone --recursive https://github.com/RyanMcB8/Tromboneless.git ```<br>

### Option 2
2) clone this git repository using the command:<br>
 ```  git clone https://github.com/RyanMcB8/Tromboneless.git ```<br>

3) To add the necessary submodules into the workspace, you will need to use the following command line commands:<br>
``` git submodules init```<br>
``` git submodules update```<br>


## Dependencies
The app side of this project uses the juce framework to create all the widgets necessary.
 - Juce <br>

## Documentation
If you wish to create your very own Tromboneless, the hardware used to create the protoype may be seen in the [Hardware](https://github.com/RyanMcB8/Tromboneless/tree/main/Documentation/Hardware) directory under Documentation. This includes a full circuit diagram.<br>

For documentation of the code used, please refer to the [Documentation](https://github.com/RyanMcB8/Tromboneless/tree/main/Documentation) directory where there are both LaTex and HTML versions of the documentation for this project.<br>

## Social Media
Feel free to follow our social media accounts for frequent updates on the progress off the project including showcase videos and demonstrations:<br>
[Reddit](https://www.reddit.com/user/Forward_Vehicle4096/)<br>
[Instagram](https://www.instagram.com/tromboneless.tech/)<br>

## SSH Steps
The following steps are to allow for a remote access connection into your Raspberry Pi if you wish to use another computer as your input terminal.<br>

* SSH pair created on Raspberry with Located in /home/tromboneless/.ssh

1) When generating own pair, make ssh of ed25519 format: ```ssh-keygen -t ed25519```
2) Add your public key to /home/yourusername/.ssh/authorized_keys. If connected to the same network as the pi, you can do this automatically with ```ssh-copy-id yourusername@IP_Address```

### If on the same network as pi:
ssh in with eg ```ssh name@IP_Address```

### If on a different network:

1.) You need to set up your own personal tailscale account.
   Invitations to share access to the pi are made on the owner account, then you need to be logged in to your personal account when you follow the invitation link.
   After this is set up, you work from your personal account.
   
2.) Make sure your public ssh key is on the pi

3.) ssh in with ip address associated with the pi in tailscale (not the same as the default device one seen above): ```ssh name@IP_Address```.
We recommend working within vscode, as you can open a remote window from there which will have a proper GUI hosted by your local machine, but controlling the hardware on the pi.
