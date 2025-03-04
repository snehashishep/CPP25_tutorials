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

## ROOT installation

- For linux, it is preferable to not use the latest ROOT version. So, download the ROOT binaries corresponding to your linux version from [here, for version 6.30.](https://root.cern/releases/release-63008/)
- Untar it in your Projects directory and source it:
```bash
tar -zxvf root.XYZ.tar.gz
source root/bin/thisroot.sh
```

- Then, open your `.bashrc` profile:
```bash
vi ~/.bashrc
```

- Add these paths there and save it.
```bash
source /path/to/Projects/root/bin/thisroot.sh
export ROOTSYS=/path/to/Projects/root
export PATH=$PATH:$ROOTSYS/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib
```

- After closing the file, do a check of whether ROOT is working or not.
```bash
source ~/.bashrc
root
```

- If it is working, you should get an output like this:
```
   ------------------------------------------------------------------
  | Welcome to ROOT 6.26/10                        https://root.cern |
  | (c) 1995-2021, The ROOT Team; conception: R. Brun, F. Rademakers |
  | Built for linuxx8664gcc on Nov 16 2022, 10:42:54                 |
  | From tags/v6-26-10@v6-26-10                                      |
  | With c++ (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0                   |
  | Try '.help', '.demo', '.license', '.credits', '.quit'/'.q'       |
   ------------------------------------------------------------------

root [0]
```

## Delphes installation

- Get the Delphes code and make it:
```bash
wget http://cp3.irmp.ucl.ac.be/downloads/Delphes-3.5.0.tar.gz
tar -zxf Delphes-3.5.0.tar.gz
cd Delphes-3.5.0
make
```

- But we don't just want the vanilla Delphes, we need to configure it with Pythia8. So open your `.bashrc` profile again and add the following lines (replace 8xxx with your version, and the correct path):
```bash
export PYTHIA8=/path/to/pythia8xxx
export PYTHIA8DATA=$PYTHIA8/share/Pythia8/xmldoc
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PYTHIA8/lib
export PATH=$PATH:$PYTHIA8/bin
```
- Then go to the `Delphes-3.5.0` directory and make again with the flag:
```bash
make HAS_PYTHIA8=true
```

- Check if you get the `DelphesPythia8` executable in the directory after this. 
- We would also like to have the `EventDisplay` feature of Delphes. So, run this:
```bash
make -j display
```