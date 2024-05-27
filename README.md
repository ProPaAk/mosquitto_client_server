## Mosquitto client-server

The project implements the logic of the client-server operation through the mosquito broker and needs mosquitto broker to running in order to work.

To setup broker on Linux system for this project you can obtain and install mosquitto package via your package manager:
 e.g -  `sudo yum install mosquitto` for RHEL based systems
 and run:
`mosquitto` command.
This starts localhost broker in your system. After that you can run project.

On Debian-based distributions probably you need to install additional libraries:
`sudo apt install xsltproc docbook-xsl`

Was tested on Centos 9 stream.
For more infomation about mosquitto you can check [this](https://mosquitto.org/documentation/) documentation page.
