# Mafia Shootout
Top-down Shoot'em Up Game built in C++ using the SFML

## Installation

### Requirements

- xmake

### Building

```
xmake
xmake run
```
That's it....

### Generate Visual Studio solution

```
xmake project -k vsxmake -m "debug,release"
```

## Known Problems

### Crash Problems

- Empty Entities of unknown origins cause the dereferencing of nullptr and crashes the game despite a nullptr check.
- Unable to Destroy an object at the end of the program despite the object being destroyed without problem anywhere else during execution.

### Some other little problems

- Score not being properly passed from the Lvls to the Win and GameOver scenes (will always be 0)
- The UI coordinates doesn't scale with the window, so if the window is resized the interraction with the UI elements will be messed-up.

### Note

To make the repo public I had to delete the main music due to copyright reasons, so this version of the game doesn't have music.

## Authors

### Devs

- Antoine RIVIALE (ariviale@gaming.tech)
- Amélie HEINRICH (aheinrich@gaming.tech)
- Ivania TAVARES SILVA (itavaressilva@gaming.tech)
- Tristan VAN LAECKE (tvanlaecke@gaming.tech)
- Hugo MAGNIER (hmagnier@gaming.tech)
- Mathias Rivierez (mrivierez@gaming.tech)

### Artists

- Maximilien HAMMELET (mhammelet@gaming.art)
- Vincenzo LUPO (vlupo@gaming.art)
- Sofiane BEJAOUI (sbejaoui@gaming.art)
- Sacha LAUDE (slaude@gaming.art)


## Screenshots

<img width="642" alt="{AD00A233-BE83-43D9-857E-2E8FD7899287}" src="https://github.com/user-attachments/assets/8d893449-3093-466f-bbda-bf09bbb0ea8d" />
<img width="642" alt="{F9D8629F-BF2C-487D-8647-6CF9FB81B844}" src="https://github.com/user-attachments/assets/ab973159-3653-4ff1-b17c-4ff42104ca88" />
<img width="642" alt="{8908283A-1193-49E0-B1EB-E49A756E4E18}" src="https://github.com/user-attachments/assets/033926bf-df68-48a0-9801-3a631438114f" />
<img width="642" alt="{B56B8751-1DBB-4333-B374-995CAFC1EBCB}" src="https://github.com/user-attachments/assets/e734a48c-1d41-4eaf-ae3d-1539d0805a1b" />
<img width="642" alt="{EF87D3EC-52C5-4869-9516-0DB41DA60705}" src="https://github.com/user-attachments/assets/0a014832-4820-48a4-9bda-98b5e3210a27" />
<img width="642" alt="{6E032962-FC53-468D-8153-9F667AA5C180}" src="https://github.com/user-attachments/assets/89dcf750-f163-477f-88d6-519188100a8e" />
<img width="642" alt="{83EE188D-5CE9-4383-A8B0-07C87E8575AB}" src="https://github.com/user-attachments/assets/d67f8461-45ff-46c4-826f-4fc7637ddf8c" />
<img width="642" alt="{9A7778D5-2271-4FA4-B95F-7BEF7EA1980D}" src="https://github.com/user-attachments/assets/107e01f1-8a7b-4f67-ad91-da3003c4115d" />





