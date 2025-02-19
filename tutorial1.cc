#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/FastJet3.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include <iostream>
#include <sstream> 
#include <fstream>
#include <cmath>
#include <numeric>

// A simple code to understand the basics of a Pythia8 analysis without using Delphes etc.
// Snehashis Parashar, 2025.

using namespace Pythia8;
using namespace std;
using namespace fastjet;

int main(){ 

// initial parameters

int nEvent=1000; // number of events to generate
double lumi=1000.; // the luminosity at which a final state will be counted

// kinematic parameters
  
double R       = 0.4;    // Jet size
double pTMin   = 20.0;    // Min jet and lepton pT
double etaMax  = 4.5; 	// HCal eta
double etaMaxLep = 2.5;	 // ECal eta

// initialization commands

Pythia pythia;

Event& event = pythia.event;

// collider setup

pythia.readString("Beams:frameType=1"); // CM frame
pythia.readString("Beams:idA = 2212"); // proton beams
pythia.readString("Beams:idB = 2212");
pythia.readString("Beams:eCM = 14000."); // 14 TeV ECM. 

// process selection

// pythia.readString("HiggsSM:gg2H=on"); // gg -> h production

pythia.readString("Top:gg2ttbar=on");
pythia.readString("Top:qqbar2ttbar=on"); // turn on if you are doing pp -> ttbar

// hadronization and radiation

pythia.readString("PartonLevel:FSR = on");
pythia.readString("PartonLevel:ISR = on");
pythia.readString("PartonLevel:MPI = off");

pythia.readString("HadronLevel:Hadronize = on"); // Hadronisation
pythia.readString("HadronLevel:Decay = on"); // Decay

// renorm and factorization scales

pythia.readString("SigmaProcess:renormScale2 = 4"); // Q^2 renormalization scale for 2to 2 processes: m_{invariant}^{2} of the system
pythia.readString("SigmaProcess:factorScale2 = 4"); // Q^2 factorization scale for 2to 2 processes: m_{invariant}^{2} of the system


// instead for LHE reading, use these:

// pythia.readString("Beams:frameType=4");
// pythia.readString("Beams:LHEF = /path/to/file.lhe");
// pythia.readString("LesHouches:setLifetime =2"); // read lifetimes from LHE file
// pythia.readString("SLHA:minMassSM = 200."); // anything above this mass is BSM. reduce it if you have lighter BSM particles.



pythia.readString("Next:numberCount = 100 "); // event counter in terminal

// jet definitions

fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, R);  // jet algo and radius

std::vector <fastjet::PseudoJet> fjInputs; // temporary vector to cluster jets

pythia.init();

// book histograms

Hist jetmul("jet multiplicity", 20,0.,10.);
Hist lepmul("lepton multiplicity", 20, 0., 10.);

Hist leppt1("lep 1 pt", 200, 0.,2000.);
Hist leppt2("lep 2 pt", 200, 0.,2000.);

Hist jetpt1("jet 1 pt", 200, 0.,2000.);
Hist jetpt2("jet 2 pt", 200, 0.,2000.);

Hist ptmiss("missing pt", 300,0.,3000.);

Hist Mjj("dijet invariant mass", 300, 0., 3000.);

Hist MET("missing pT", 300, 0., 3000.);

// start event loop

for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
	pythia.next();

	if (iEvent < 10) {
		pythia.process.list();
		event.list();
	} // print first 10 events

	// list loop for leptons

	int nlep = 0; // counter for leptons per event
	double lep_pt[10] = {0.}; // array for lepton pT
	// you can add similar arrays for lepton eta, phi, etc.
	double lep_px[10] = {0.};
	double lep_py[10] = {0.};
	double lep_pz[10] = {0.};
	double lep_e[10] = {0.};
	

	for (int il=0; il<event.size(); ++il){
     	if (event[il].isFinal() && abs(event[il].eta()) < etaMaxLep) {
	   		if (event[il].idAbs() == 11 || event[il].idAbs() == 13){
	   			if (event[il].pT() > 20.0){

	   				lep_pt[nlep]=event[il].pT(); // fill the pT for each lepton you encounter
	   				lep_px[nlep]=event[il].px();
	   				lep_py[nlep]=event[il].py();
	   				lep_pz[nlep]=event[il].pz();
	   				lep_e[nlep]=event[il].e();


	   				++nlep; // increment the lepton counter

	   			}
	   		}
	   	}
	} // end of lepton list loop

	lepmul.fill(nlep); // fill lep multiplicity histogram

	if (nlep >= 1){
		leppt1.fill(lep_pt[0]);
		leppt2.fill(lep_pt[1]);
	} // fill lepton pT histograms if there is at least 1 lepton

	// Jet clustering: initialization

	int njet = 0;
	fjInputs.resize(0);

	// passing particles to fastjet
	for (int ifj=0; ifj<event.size(); ++ifj){
    	if (event[ifj].isFinal() && abs(event[ifj].eta()) < etaMax){
    		if (event[ifj].isHadron() || event[ifj].idAbs()==22){ // only hadrons and photons (q: why photons?)
    		// if (event[ifj].isHadron()){ // you can try with only hadrons
    			PseudoJet particleTemp = event[ifj]; // throwing the 4-vector of the particle at 'ifj' into a temporary vector
    			// PseudoJet particleTemp = (event[ifj].px(), event[ifj].py(), event[ifj].pz(), event[ifj].E()); // you can explicitly do it like this.
    			fjInputs.push_back( particleTemp ); //throwing the temporary particle into the fjInput bag

    		}

    	}
    }

    // calling the jet clustering

    vector <PseudoJet> inclusiveJets, sortedJets; // define new "bozes" to keep jets

    ClusterSequence clustSeq(fjInputs, jetDef); //sequentially cluster the objects from the fjInput bag using jetDef

    inclusiveJets = clustSeq.inclusive_jets(pTMin); // keep only those jets that pass the pTmin

    sortedJets    = sorted_by_pt(inclusiveJets); // arrange those inclusive jets by their pT

    njet = sortedJets.size(); // jet multiplicity

    jetmul.fill(njet);

    // filling the pT

    // if (njet >= 1) {
    // 	jetpt1.fill(sortedJets[0].pt());
    // 	jetpt2.fill(sortedJets[1].pt());
    // } // this approach uses the things in the pseudojet class

    // you can also write a list loop (will be useful later)

    double j_pt[20] = {0.};
    double j_px[20]={0.};
	double j_py[20]={0.};
	double j_pz[20]={0.};
	double j_e[20]={0.};

    for (int ij=0; ij<njet; ++ij){
    	j_pt[ij] = sortedJets[ij].pt();
    	j_px[ij]=sortedJets[ij].px();
    	j_py[ij]=sortedJets[ij].py();
    	j_pz[ij]=sortedJets[ij].pz();
    	j_e[ij]=sortedJets[ij].e();
    }

    if (njet >= 1) {
    	jetpt1.fill(j_pt[0]);
    	jetpt2.fill(j_pt[1]);
    }

    // dijet invariant mass

    double pxsum=0.;
    double pysum=0.;
    double pzsum=0.;
    double esum=0.;

    if (njet >=2){ // try with this first
    // if (njet >=2 && j_pt[0] >= 40. && j_pt[1] >= 40.){ //ensure only hard jets come up

    	for (int i=0; i<njet; ++i){
    		for (int j=i+1; j<njet; ++j){
    			pxsum = j_px[i] + j_px[j];
    			pysum = j_py[i] + j_py[j];
    			pzsum = j_pz[i] + j_pz[j];
    			esum = j_e[i] + j_e[j];

    			double mjj = sqrt(pow2(esum)-pow2(pxsum)-pow2(pysum)-pow2(pzsum));

    			// double mjj = (sortedJets[i]+sortedJets[j]).m(); // you can simply do this, not recommended

    			Mjj.fill(mjj);
    		}
    	}
	}

	// missing pT or MET = sqrt(pxvis^2 + pyvis^2)

	double pxvis = 0.;
	double pyvis = 0.;
	double ptmiss = 0.;

	for (int i=0; i<nlep; ++i){
		pxvis += lep_px[i];
		pyvis += lep_py[i];
	}

	for (int i=0; i<njet; ++i){
		pxvis += j_px[i];
		pyvis += j_py[i];
	}

	ptmiss = sqrt(pow2(pxvis)+pow2(pyvis));
	MET.fill(ptmiss);





} // end of event loop

HistPlot hpl("tutorial1plot");

hpl.frame("lep_mul", "lepton multiplicity", "$n_l$");
hpl.add(lepmul);
hpl.plot(true);

hpl.frame("lep_pt", "lepton pT", "$p_T$");
hpl.add(leppt1);
hpl.add(leppt2);
hpl.plot(true);

hpl.frame("jet_mul", "jet multiplicity", "$n_j$");
hpl.add(jetmul);
hpl.plot(true);

hpl.frame("jet_pt", "jet pT", "$p_T$");
hpl.add(jetpt1);
hpl.add(jetpt2);
hpl.plot(true);

hpl.frame("jet_mjj", "dijet invariant mass", "$M_{jj}$");
hpl.add(Mjj);
hpl.plot(true);

hpl.frame("met", "MET", "MET");
hpl.add(MET);
hpl.plot(true);

pythia.stat();

return 0;

} // end of main