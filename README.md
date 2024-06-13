# Low Latency Stream Service Client Example
Example application for using Hi-Techniques' DAQ Low Latency Data Stream Service


This is an example application showing how to use Hi-Technique's Low Latency
Data Stream Service available from select Hi-Techniques DAQ devices.  The code
contained within this application is example code only and should not be
considered "production" code in any sense of that word.  As the license states,
this code carries no warranty of any kind.


Development system: Ubuntu 20.04 LTS

Other linux development systems may work but have not been tried.
Windows development environments have not been tried.


Dependencies:

    Normal linux development tools
    Qt5:            Qt 5.1
    Boost:          1.72 or later
    Nlohmann/JSON:  3.0 or later


Install boost via:

    sudo apt-get install libboost-all-dev

Install Nlohmann/JSON via:

    sudo apt-get install nlohmann-json3-dev

Install Qt5 via:

    sudo apt-get install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools


How to build on linux development machine:

    1. Goto the directory into which the repository was cloned
    2. Type 'qmake'
    3. When qmake finishes, type 'make'

Enjoy
