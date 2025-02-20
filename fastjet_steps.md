# A simple analogy for FastJet clustering with Pythia8

Jet clustering steps using FastJet in Pythia8 is very similar to the job of a florist. Assume you are a florist with some orders to create specific bouquets. Let us see how.

- First, you need a plan to make the bouquets and decide on a size, and a huge bag to keep the flowers you pick from the garden. Only then you enter the garden.
```cpp
fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, R);  // jet algo and radius
std::vector <fastjet::PseudoJet> fjInputs; // temporary vector to cluster jets
```

- After entering the garden, you first ensure your bag is empty.
```cpp
fjInputs.resize(0);
```

- Now you start searching through the flowers with some specific criteria, like their age, their brightness, their size. You also want only roses and daisies in your bouquets.
```cpp
for (int ifj=0; ifj<event.size(); ++ifj){
    	if (event[ifj].isFinal() && abs(event[ifj].eta()) < etaMax){
    		if (event[ifj].isHadron() || event[ifj].idAbs()==22){ // only hadrons and photons
```

- Everytime you see a flower that matches these criteria, you put it in a plastic wrapper, and throw it in the bag.
```cpp
PseudoJet particleTemp = event[ifj]; // throwing the 4-vector of the particle at 'ifj' into a temporary vector
fjInputs.push_back( particleTemp ); //throwing the temporary particle into the fjInput bag

    		}

    	}
    }

```

- Now you have enough flowers to start creating the bouquets. First, define two boxes to seggregate the bouquets.
```cpp
vector <PseudoJet> inclusiveJets, sortedJets;
```

- Then, remember the plan you had to create the bouquet. Also remember the sizes of the bouquets. Start clustering them accordingly.
```cpp
ClusterSequence clustSeq(fjInputs, jetDef); //sequentially cluster the objects from the fjInput bag using jetDef
```

- Now, you decide to keep only those bouquets that carry at least 20 flowers. You put them in one box.
```cpp
inclusiveJets = clustSeq.inclusive_jets(pTMin); // keep only those jets that pass the pTmin
```

- And then, you want to impress your client by arranging them in the order of how many flowers they contain. So you put them in the new box accordingly.
```cpp
sortedJets    = sorted_by_pt(inclusiveJets); // arrange those inclusive jets by their pT
```

- Congratulations, you are a jet florist now.