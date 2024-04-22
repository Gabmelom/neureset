# Neureset

## Instructions for Ubuntu 22.04 LTS
1. Open a terminal in the Neureset folder
2. Install Qt Charts with `sudo apt install libqt5charts5-dev`
    - If you run into any issues here, try running `sudo apt update`
3. Open the neureset.pro file with Qt Creator
4. Build and run the project
    - If you run into issues, try running qmake
5. Have fun

## Dear TA
Filenames and folders should hopefully be descriptive enough of the diagram or files they represent.

The 'Source code' folder has the source code files for the project. Follow the instructions above to test the project. You probably know what to do, it should run in the course VM with no problems.

## Team 30 members:
- Josiah Mak (101220579)
- Briana Lee (101226914)
- Gabriel Melo Martins (101141430)
- Spencer Bennett (101148567)
    
## What we each did:
The outline below is a brief overview of our tasks per group member. For a more detailed view click on [this link](https://github.com/Gabmelom/neureset/commits/main/) and filter per user. You can also follow [this link](https://github.com/Gabmelom/neureset/graphs/contributors) for fun contribution metrics.

### Spencer Bennett
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

### Gabriel Melo
- Sequence diagrams: headset connection lost, low battery response
- UML class diagram draft (with group) + final draft
- Use case 1,2,3 draft (with Spencer) + final draft
- Traceability matrix final draft
- Design and implement UI for all components
- MainWindow implementations for all UI related tasks
- Bug fixes everywhere
- Videos
- Final submission cleanup

### Josiah Mak
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
- Tracability Matrix (with Briana)

### Briana Lee
- Traceability Matrix
- Use Cases #3 and #4 (with Josiah)
- UML Class diagram (group effort)
- Creation of device and headset classes
- Modified device and headset classes to use QTimer instead of QThreads (with Josiah)
- Implemented session pause/resume functionality
- Implemented session stop functionality
- Implemented headset losing/re-establishing connection during session
- Implemented session timeout after pausing or losing headset connection for too long
- Implemented light functionalities during session (blue, green, red lights)
- Implemented starting session only once device is turned on, headset is connected, and device has enough battery

## Submission contents
```
Neureset
│   README.md
│   
├───Documentation
│       Sequence Diagram - Headset connection lost.png
│       Sequence Diagram - Low battery response.png
│       Sequence Diagram - Main use case (VP).png
│       Sequence Diagram - Therapy history viewing with PC.drawio.png
│       Traceability Matrix.pdf
│       UML class diagram.png
│       UML State Diagram.drawio.png
│       Use case diagram.png
│       Use cases and design explanation.pdf
│       
├───Source code
│   │   Device.cpp
│   │   Device.h
│   │   Headset.cpp
│   │   Headset.h
│   │   icons.qrc
│   │   main.cpp
│   │   mainwindow.cpp
│   │   mainwindow.h
│   │   mainwindow.ui
│   │   neureset.pro
│   │   PC.cpp
│   │   PC.h
│   │   resources.qrc
│   │   SessionLog.cpp
│   │   SessionLog.h
│   │   
│   └───icons
│           arrow-down.svg
│           arrow-up.svg
│           battery-exclamation.svg
│           battery-full.svg
│           battery-half.svg
│           battery-low.svg
│           battery-three-quarters.svg
│           house.svg
│           left-from-bracket.svg
│           pause.svg
│           play-pause.svg
│           play.svg
│           power-off.svg
│           stop.svg
│           
└───Videos
        Neureset - Date and time.mkv
        Neureset - Low battery response.mkv
        Neureset - Main use case.mkv
        Neureset - Therapy history with PC.mkv
        

```
