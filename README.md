# CPP 2025 tutorials

- This repository will contain codes used in tutorials for the Computational Particle Physics course in IIT Hyderabad, Spring 2025 semester.
- `tutorial1.cc` : a simple code to understand Pythia8 and FastJet. 

## FastJet installation

- Open a terminal and run:
```bash
curl -O https://fastjet.fr/repo/fastjet-3.4.3.tar.gz
tar zxvf fastjet-3.4.3.tar.gz
cd fastjet-3.4.3/
```
- then install in a separate installation directory:
```bash
./configure --prefix=$PWD/../fastjet-install
make
make check
make install
```
## Configuring FastJet with Pythia8

- Go to your Pythia8 directory and run:
```bash
./configure --with-fastjet3=/path/to/fastjet-install
make
```