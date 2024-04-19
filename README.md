# Neureset

## Quicklink for the worthy
https://drive.google.com/drive/folders/1KfQnQHPvCLlNU4ujRyucGFC7S9DHbaLv?usp=sharing

## Team 30 members:
- Josiah Mak
- Briana Lee
- Gabriel Melo Martins
- Spencer Bennett
    
## What we each did:
Spencer:
- Main use case sequence diagram
- Device state diagram
- Use case 1,2,3 (with gabriel)
- helped with the class diagram (group effort)
- contributions to main:
    - implemented the admin controls (#59)
    - implemented the date on the UI
        - display the date
        - change the date
    - implemented the first version of the main use case
        - initial implementation of the device, headset, and log class (#26, 27, 29)
        - dominant frequency calculation function
    - implemented battery depletion during treatment
    - implemented the safety senarios
        - handling low power in the treatment session
        - handling (some) of the headset connecting/ disconnecting during the session        
- Initial implementation of sessionLog, device, and headset classes

Gabriel Melo:
- Don't be lazy, look at the commit history, filter by contributor

Josiah Mak:
- Sequence diagram for view session history
- Use case 3, 4 (with Briana)
- Class diagram (group effort)
- Implemented storing the baseline to session log
- Implemented connection to PC
- Implemented sending session log to PC
- Implemented displaying the battery levels
- Implemented showing the progress of the session
- Fixed bugs on main session after big merge conflicts
- Modify to use QTimer instead of threads (with Briana)
- Initial implmentation for PC

## Submission contents
```
root
│   .gitignore
│   README.md
│   tree.txt
│   
└───neureset
    │   .gitignore
    │   Device.cpp
    │   Device.h
    │   Headset.cpp
    │   Headset.h
    │   icons.qrc
    │   main.cpp
    │   mainwindow.cpp
    │   mainwindow.h
    │   mainwindow.ui
    │   neureset.pro
    │   PC.cpp
    │   PC.h
    │   resources.qrc
    │   SessionLog.cpp
    │   SessionLog.h
    │   
    ├───docs
    │   │   Design Desision.pdf
    │   │   eeg_connection_lost.png
    │   │   low_battery_response.png
    │   │   main_use_case.png
    │   │   Sequence Diagram (therapy history viewing with PC).drawio.png
    │   │   State Diagram.drawio.png
    │   │   Traceability Matrix.pdf
    │   │   Use Cases.pdf
    │   │   
    │   └───src
    │           eeg_connection_lost.txt
    │           low_battery_response.txt
    │           
    └───icons
            arrow-down.svg
            arrow-up.svg
            battery-exclamation.svg
            battery-full.svg
            battery-half.svg
            battery-low.svg
            battery-three-quarters.svg
            house.svg
            left-from-bracket.svg
            pause.svg
            play-pause.svg
            play.svg
            power-off.svg
            stop.svg
```
