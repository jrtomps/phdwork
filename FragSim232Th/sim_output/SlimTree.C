
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include <string>
#include <iostream>

void SlimTree(TTree* T)
{
	TBranch * b;
	const Int_t nbr = 10;
	std::string bnames[nbr] = {"SSDNonIonizingDepE","SSDMass", 
				"SSDCharge", "PreStepE", 					"SurfCurrent","TarNonIonizingDepE",
				"TarDeltaCharge","day",
				"month","year"};

	std::cout << "BEGIN SLIMMING of TTree : " << T->GetName();
	for (UInt_t i=0; i<nbr; i++)
	{
		std::cout << "\n\tRemoving branch " << bnames[i];
		b = T->GetBranch(bnames[i].data());
		if (b!=0)
		{
			T->GetListOfBranches()->Remove(b);
			std::cout << " ... success";
			TLeaf* l = T->GetLeaf(bnames[i].data());
			T->GetListOfLeaves()->Remove(l);
			T->Write();
			
		}
		else
			std::cout << " ... failure";
		
	}
	std::cout << std::endl;
	std::cout << "SLIMMING COMPLETE" << std::endl;

}
