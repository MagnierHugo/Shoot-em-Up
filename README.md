# Renard Etoilé
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
