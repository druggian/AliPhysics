/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 * Edited by Taiga Kawaguchi                                              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskWHMult
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliPIDResponse.h"
#include "AliMultSelection.h"
#include "AliAnalysisTaskWHMult.h"
#include "AliAODMCParticle.h"
#include "AliAODMCHeader.h"
#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "TFile.h"
#include "TRandom.h"
#include "TGrid.h"

class AliAnalysisTaskWHMult;	// your analysis class

using namespace std;		// std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskWHMult)	// classimp: necessary for root

AliAnalysisTaskWHMult::AliAnalysisTaskWHMult() : AliAnalysisTaskSE(), 
  fAOD(0),
  fTracks_tender(0),
  fCaloClusters_tender(0),
  fVevent(0),
  fPIDResponse(0),
  fMCTrackpart(0),
  fMCarray(0),
  fMCpart(0),
  fMCparticle(0),
  fOutputList(0),
  tree(0),
  NITSClust(2),
  NCrossedRows(100),
  NTPCClust(80),
  NSigmaMin(-1),
  NSigmaMax(3),
  M02Min(0.1),
  M02Max(0.3),
  EoPMin(0.9),
  EoPMax(1.3),
  EisoMax(0.05),
  NtrkMax(3),
  fNevents(0),
  pVertex_all(0),
  pVertex(0),
  EtavsPhi(0),
  TPCSig(0),
  fzvtx_Ntrkl(0),
  fzvtx_Ntrkl_cal(0),
  fHistNsigmaP(0),
  fPtEoverPE(0),
  fPtEoverPH(0),
  fREisolation(0),
  fdPhi_trkW_Pt(0),
  fdPhi_trkHF_Pt(0),
  fHistPt_We(0),
  fHistPt_HFe(0),
  fNtrkl_PtOfMaxTrk_W_m(0),
  fNtrkl_PtOfMaxTrk_W70_m(0),
  fNtrkl_PtOfMaxTrk_W80_m(0),
  fNtrkl_PtOfMaxTrk_W85_m(0),
  fNtrkl_PtOfMaxTrk_W90_m(0),
  fNtrkl_PtOfMaxTrk_W93_m(0),
  fNtrkl_PtOfMaxTrk_W95_m(0),
  fNtrkl_PtOfTrks_W_m(0),
  fNtrkl_PtOfTrks_W70_m(0),
  fNtrkl_PtOfTrks_W80_m(0),
  fNtrkl_PtOfTrks_W85_m(0),
  fNtrkl_PtOfTrks_W90_m(0),
  fNtrkl_PtOfTrks_W93_m(0),
  fNtrkl_PtOfTrks_W95_m(0),
  fNtrkl_PtOfMaxTrk_HF_m(0),
  fNtrkl_PtOfMaxTrk_HF70_m(0),
  fNtrkl_PtOfMaxTrk_HF80_m(0),
  fNtrkl_PtOfMaxTrk_HF85_m(0),
  fNtrkl_PtOfMaxTrk_HF90_m(0),
  fNtrkl_PtOfMaxTrk_HF93_m(0),
  fNtrkl_PtOfMaxTrk_HF95_m(0),
  fNtrkl_PtOfTrks_HF_m(0),
  fNtrkl_PtOfTrks_HF70_m(0),
  fNtrkl_PtOfTrks_HF80_m(0),
  fNtrkl_PtOfTrks_HF85_m(0),
  fNtrkl_PtOfTrks_HF90_m(0),
  fNtrkl_PtOfTrks_HF93_m(0),
  fNtrkl_PtOfTrks_HF95_m(0),
  fNtrkl_PtOfMaxTrk_H_m(0),
  fNtrkl_PtOfMaxTrk_H70_m(0),
  fNtrkl_PtOfMaxTrk_H80_m(0),
  fNtrkl_PtOfMaxTrk_H85_m(0),
  fNtrkl_PtOfMaxTrk_H90_m(0),
  fNtrkl_PtOfMaxTrk_H93_m(0),
  fNtrkl_PtOfMaxTrk_H95_m(0),
  fNtrkl_PtOfTrks_H_m(0),
  fNtrkl_PtOfTrks_H70_m(0),
  fNtrkl_PtOfTrks_H80_m(0),
  fNtrkl_PtOfTrks_H85_m(0),
  fNtrkl_PtOfTrks_H90_m(0),
  fNtrkl_PtOfTrks_H93_m(0),
  fNtrkl_PtOfTrks_H95_m(0),
  fNtrkl_PtOfMaxTrk_W_w(0),
  fNtrkl_PtOfMaxTrk_W70_w(0),
  fNtrkl_PtOfMaxTrk_W80_w(0),
  fNtrkl_PtOfMaxTrk_W85_w(0),
  fNtrkl_PtOfMaxTrk_W90_w(0),
  fNtrkl_PtOfMaxTrk_W93_w(0),
  fNtrkl_PtOfMaxTrk_W95_w(0),
  fNtrkl_PtOfTrks_W_w(0),
  fNtrkl_PtOfTrks_W70_w(0),
  fNtrkl_PtOfTrks_W80_w(0),
  fNtrkl_PtOfTrks_W85_w(0),
  fNtrkl_PtOfTrks_W90_w(0),
  fNtrkl_PtOfTrks_W93_w(0),
  fNtrkl_PtOfTrks_W95_w(0),
  fNtrkl_PtOfMaxTrk_HF_w(0),
  fNtrkl_PtOfMaxTrk_HF70_w(0),
  fNtrkl_PtOfMaxTrk_HF80_w(0),
  fNtrkl_PtOfMaxTrk_HF85_w(0),
  fNtrkl_PtOfMaxTrk_HF90_w(0),
  fNtrkl_PtOfMaxTrk_HF93_w(0),
  fNtrkl_PtOfMaxTrk_HF95_w(0),
  fNtrkl_PtOfTrks_HF_w(0),
  fNtrkl_PtOfTrks_HF70_w(0),
  fNtrkl_PtOfTrks_HF80_w(0),
  fNtrkl_PtOfTrks_HF85_w(0),
  fNtrkl_PtOfTrks_HF90_w(0),
  fNtrkl_PtOfTrks_HF93_w(0),
  fNtrkl_PtOfTrks_HF95_w(0),
  fNtrkl_PtOfMaxTrk_H_w(0),
  fNtrkl_PtOfMaxTrk_H70_w(0),
  fNtrkl_PtOfMaxTrk_H80_w(0),
  fNtrkl_PtOfMaxTrk_H85_w(0),
  fNtrkl_PtOfMaxTrk_H90_w(0),
  fNtrkl_PtOfMaxTrk_H93_w(0),
  fNtrkl_PtOfMaxTrk_H95_w(0),
  fNtrkl_PtOfTrks_H_w(0),
  fNtrkl_PtOfTrks_H70_w(0),
  fNtrkl_PtOfTrks_H80_w(0),
  fNtrkl_PtOfTrks_H85_w(0),
  fNtrkl_PtOfTrks_H90_w(0),
  fNtrkl_PtOfTrks_H93_w(0),
  fNtrkl_PtOfTrks_H95_w(0),
  fHistPt_We_Ntrkl(0),
  fNtrkl_ClustE(0),
  TPCSigForE(0),
  fNsigmaPtForE(0),
  fHistUEmult(0),
  fNtrkl_LeadHadPt_m(0),
  fNtrkl_LeadHadPt_w(0),
  fEMCEG1(kFALSE)
{
  // default constructor, don't allocate memory here!
  // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskWHMult::AliAnalysisTaskWHMult(const char* name) : AliAnalysisTaskSE(name),
  fAOD(0),
  fTracks_tender(0),
  fCaloClusters_tender(0),
  fVevent(0),
  fPIDResponse(0),
  fMCTrackpart(0),
  fMCarray(0),
  fMCpart(0),
  fMCparticle(0),
  fOutputList(0),
  tree(0),
  NITSClust(2),
  NCrossedRows(100),
  NTPCClust(80),
  NSigmaMin(-1),
  NSigmaMax(3),
  M02Min(0.1),
  M02Max(0.3),
  EoPMin(0.9),
  EoPMax(1.3),
  EisoMax(0.05),
  NtrkMax(3),
  fNevents(0),
  pVertex_all(0),
  pVertex(0),
  EtavsPhi(0),
  TPCSig(0),
  fzvtx_Ntrkl(0),
  fzvtx_Ntrkl_cal(0),
  fHistNsigmaP(0),
  fPtEoverPE(0),
  fPtEoverPH(0),
  fREisolation(0),
  fdPhi_trkW_Pt(0),
  fdPhi_trkHF_Pt(0),
  fHistPt_We(0),
  fHistPt_HFe(0),
  fNtrkl_PtOfMaxTrk_W_m(0),
  fNtrkl_PtOfMaxTrk_W70_m(0),
  fNtrkl_PtOfMaxTrk_W80_m(0),
  fNtrkl_PtOfMaxTrk_W85_m(0),
  fNtrkl_PtOfMaxTrk_W90_m(0),
  fNtrkl_PtOfMaxTrk_W93_m(0),
  fNtrkl_PtOfMaxTrk_W95_m(0),
  fNtrkl_PtOfTrks_W_m(0),
  fNtrkl_PtOfTrks_W70_m(0),
  fNtrkl_PtOfTrks_W80_m(0),
  fNtrkl_PtOfTrks_W85_m(0),
  fNtrkl_PtOfTrks_W90_m(0),
  fNtrkl_PtOfTrks_W93_m(0),
  fNtrkl_PtOfTrks_W95_m(0),
  fNtrkl_PtOfMaxTrk_HF_m(0),
  fNtrkl_PtOfMaxTrk_HF70_m(0),
  fNtrkl_PtOfMaxTrk_HF80_m(0),
  fNtrkl_PtOfMaxTrk_HF85_m(0),
  fNtrkl_PtOfMaxTrk_HF90_m(0),
  fNtrkl_PtOfMaxTrk_HF93_m(0),
  fNtrkl_PtOfMaxTrk_HF95_m(0),
  fNtrkl_PtOfTrks_HF_m(0),
  fNtrkl_PtOfTrks_HF70_m(0),
  fNtrkl_PtOfTrks_HF80_m(0),
  fNtrkl_PtOfTrks_HF85_m(0),
  fNtrkl_PtOfTrks_HF90_m(0),
  fNtrkl_PtOfTrks_HF93_m(0),
  fNtrkl_PtOfTrks_HF95_m(0),
  fNtrkl_PtOfMaxTrk_H_m(0),
  fNtrkl_PtOfMaxTrk_H70_m(0),
  fNtrkl_PtOfMaxTrk_H80_m(0),
  fNtrkl_PtOfMaxTrk_H85_m(0),
  fNtrkl_PtOfMaxTrk_H90_m(0),
  fNtrkl_PtOfMaxTrk_H93_m(0),
  fNtrkl_PtOfMaxTrk_H95_m(0),
  fNtrkl_PtOfTrks_H_m(0),
  fNtrkl_PtOfTrks_H70_m(0),
  fNtrkl_PtOfTrks_H80_m(0),
  fNtrkl_PtOfTrks_H85_m(0),
  fNtrkl_PtOfTrks_H90_m(0),
  fNtrkl_PtOfTrks_H93_m(0),
  fNtrkl_PtOfTrks_H95_m(0),
  fNtrkl_PtOfMaxTrk_W_w(0),
  fNtrkl_PtOfMaxTrk_W70_w(0),
  fNtrkl_PtOfMaxTrk_W80_w(0),
  fNtrkl_PtOfMaxTrk_W85_w(0),
  fNtrkl_PtOfMaxTrk_W90_w(0),
  fNtrkl_PtOfMaxTrk_W93_w(0),
  fNtrkl_PtOfMaxTrk_W95_w(0),
  fNtrkl_PtOfTrks_W_w(0),
  fNtrkl_PtOfTrks_W70_w(0),
  fNtrkl_PtOfTrks_W80_w(0),
  fNtrkl_PtOfTrks_W85_w(0),
  fNtrkl_PtOfTrks_W90_w(0),
  fNtrkl_PtOfTrks_W93_w(0),
  fNtrkl_PtOfTrks_W95_w(0),
  fNtrkl_PtOfMaxTrk_HF_w(0),
  fNtrkl_PtOfMaxTrk_HF70_w(0),
  fNtrkl_PtOfMaxTrk_HF80_w(0),
  fNtrkl_PtOfMaxTrk_HF85_w(0),
  fNtrkl_PtOfMaxTrk_HF90_w(0),
  fNtrkl_PtOfMaxTrk_HF93_w(0),
  fNtrkl_PtOfMaxTrk_HF95_w(0),
  fNtrkl_PtOfTrks_HF_w(0),
  fNtrkl_PtOfTrks_HF70_w(0),
  fNtrkl_PtOfTrks_HF80_w(0),
  fNtrkl_PtOfTrks_HF85_w(0),
  fNtrkl_PtOfTrks_HF90_w(0),
  fNtrkl_PtOfTrks_HF93_w(0),
  fNtrkl_PtOfTrks_HF95_w(0),
  fNtrkl_PtOfMaxTrk_H_w(0),
  fNtrkl_PtOfMaxTrk_H70_w(0),
  fNtrkl_PtOfMaxTrk_H80_w(0),
  fNtrkl_PtOfMaxTrk_H85_w(0),
  fNtrkl_PtOfMaxTrk_H90_w(0),
  fNtrkl_PtOfMaxTrk_H93_w(0),
  fNtrkl_PtOfMaxTrk_H95_w(0),
  fNtrkl_PtOfTrks_H_w(0),
  fNtrkl_PtOfTrks_H70_w(0),
  fNtrkl_PtOfTrks_H80_w(0),
  fNtrkl_PtOfTrks_H85_w(0),
  fNtrkl_PtOfTrks_H90_w(0),
  fNtrkl_PtOfTrks_H93_w(0),
  fNtrkl_PtOfTrks_H95_w(0),
  fHistPt_We_Ntrkl(0),
  fNtrkl_ClustE(0),
  TPCSigForE(0),
  fNsigmaPtForE(0),
  fHistUEmult(0),
  fNtrkl_LeadHadPt_m(0),
  fNtrkl_LeadHadPt_w(0),
  fEMCEG1(kFALSE)
{
  // constructor
  DefineInput(0, TChain::Class());

  DefineOutput(1, TList::Class()); 
}
//_____________________________________________________________________________
AliAnalysisTaskWHMult::~AliAnalysisTaskWHMult()
{
  // destructor
  if(fOutputList) {
    delete fOutputList;
  }
}
//_____________________________________________________________________________
void AliAnalysisTaskWHMult::UserCreateOutputObjects()
{
  fOutputList = new TList();
  fOutputList->SetOwner(kTRUE);

  fNevents = new TH1F("fNevents","Number of events",8,-0.5,7.5);

  pVertex_all = new TH1F("pVertex_all", "pVertex_all", 100, -20, 20);
  pVertex_all->GetXaxis()->SetTitle("Z vertex (collision point on z-axis)");
  pVertex_all->GetYaxis()->SetTitle("events");

  pVertex = new TH1F("pVertex", "pVertex", 100, -20, 20);
  pVertex->GetXaxis()->SetTitle("Z vertex (collision point on z-axis)");
  pVertex->GetYaxis()->SetTitle("events");

  EtavsPhi = new TH2F("EtavsPhi","#eta vs #phi", 500, -1.5, 1.5, 500, -1, 7);
  EtavsPhi->GetXaxis()->SetTitle("#eta");
  EtavsPhi->GetYaxis()->SetTitle("#phi (rad)");

  TPCSig = new TH2F("TPCSig","TPC signal", 5000, 0.1, 50, 10000, 0, 10000);
  TPCSig->GetXaxis()->SetTitle("p (GeV/c)");
  TPCSig->GetYaxis()->SetTitle("dE/dx");
  TPCSig->SetMarkerStyle(7);

  fzvtx_Ntrkl = new TH2F("fzvtx_Ntrkl","vertexZ vs Number of Tracklets; vertex Z (cm); Number of Tracklets",100,-10,10,300,0,300);
  fzvtx_Ntrkl_cal=new TH2F("fzvtx_Ntrkl_cal","vertexZ vs Number of Tracklets after calibration;vertex Z (cm);Number of Tracklets",100,-10,10,300,0,300);

  fHistNsigmaP = new TH2F("fHistNsigmaP","n#sigma vs p ;p (GeV/c) ;n#sigma",800,0,80,200,-10,10);

  fPtEoverPE = new TH2F("fPtEoverPE","p_{T} vs E/p (-1 < n#sigma < 3) (0.1 < M02 < 0.3) ;p_{T} (GeV/c) ;E/p",1000,0,100,100,0,3);
  fPtEoverPH = new TH2F("fPtvsEoverPH","p_{T} vs E/p (n#sigma < -3) (0.1 < M02 < 0.3) ;p_{T} (GeV/c) ;E/p",1000,0,100,100,0,3);

  fREisolation = new TH1F("fREisolation","Eiso in R<0.3",100,0,1);
  fREisolation->GetXaxis()->SetTitle("E_{iso}");
  fREisolation->GetYaxis()->SetTitle("Entries");

  fdPhi_trkW_Pt = new TH2F("fdPhi_trkW_Pt","",200,-TMath::Pi()/3.,5.*TMath::Pi()/3.,1000,0,100);
  fdPhi_trkW_Pt->GetXaxis()->SetTitle("#Delta #phi (rad)");
  fdPhi_trkW_Pt->GetYaxis()->SetTitle("p_{T,trk}(GeV/c)");
  fdPhi_trkHF_Pt = new TH2F("fdPhi_trkHF_Pt","",200,-TMath::Pi()/3.,5.*TMath::Pi()/3.,1000,0,100);
  fdPhi_trkHF_Pt->GetXaxis()->SetTitle("#Delta #phi (rad)");
  fdPhi_trkHF_Pt->GetYaxis()->SetTitle("p_{T,trk}(GeV/c)");

  fHistPt_We = new TH1F("fHistPt_We","",200,0,200);
  fHistPt_We->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPt_We->GetYaxis()->SetTitle("Counts");
  fHistPt_HFe = new TH1F("fHistPt_HFe","",200,0,200);
  fHistPt_HFe->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPt_HFe->GetYaxis()->SetTitle("Counts");

  fNtrkl_PtOfMaxTrk_W_m = new TH2F("fNtrkl_PtOfMaxTrk_W_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W70_m = new TH2F("fNtrkl_PtOfMaxTrk_W70_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W70_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W70_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W80_m = new TH2F("fNtrkl_PtOfMaxTrk_W80_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W80_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W80_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W85_m = new TH2F("fNtrkl_PtOfMaxTrk_W85_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W85_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W85_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W90_m = new TH2F("fNtrkl_PtOfMaxTrk_W90_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W90_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W90_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W93_m = new TH2F("fNtrkl_PtOfMaxTrk_W93_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W93_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W93_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W95_m = new TH2F("fNtrkl_PtOfMaxTrk_W95_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W95_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W95_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W_m = new TH2F("fNtrkl_PtOfTrks_W_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W70_m = new TH2F("fNtrkl_PtOfTrks_W70_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W70_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W70_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W80_m = new TH2F("fNtrkl_PtOfTrks_W80_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W80_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W80_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W85_m = new TH2F("fNtrkl_PtOfTrks_W85_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W85_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W85_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W90_m = new TH2F("fNtrkl_PtOfTrks_W90_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W90_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W90_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W93_m = new TH2F("fNtrkl_PtOfTrks_W93_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W93_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W93_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W95_m = new TH2F("fNtrkl_PtOfTrks_W95_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W95_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W95_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");

  fNtrkl_PtOfMaxTrk_HF_m = new TH2F("fNtrkl_PtOfMaxTrk_HF_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF70_m = new TH2F("fNtrkl_PtOfMaxTrk_HF70_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF70_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF70_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF80_m = new TH2F("fNtrkl_PtOfMaxTrk_HF80_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF80_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF80_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF85_m = new TH2F("fNtrkl_PtOfMaxTrk_HF85_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF85_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF85_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF90_m = new TH2F("fNtrkl_PtOfMaxTrk_HF90_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF90_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF90_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF93_m = new TH2F("fNtrkl_PtOfMaxTrk_HF93_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF93_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF93_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF95_m = new TH2F("fNtrkl_PtOfMaxTrk_HF95_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF95_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF95_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF_m = new TH2F("fNtrkl_PtOfTrks_HF_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF70_m = new TH2F("fNtrkl_PtOfTrks_HF70_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF70_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF70_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF80_m = new TH2F("fNtrkl_PtOfTrks_HF80_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF80_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF80_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF85_m = new TH2F("fNtrkl_PtOfTrks_HF85_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF85_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF85_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF90_m = new TH2F("fNtrkl_PtOfTrks_HF90_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF90_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF90_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF93_m = new TH2F("fNtrkl_PtOfTrks_HF93_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF93_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF93_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF95_m = new TH2F("fNtrkl_PtOfTrks_HF95_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF95_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF95_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");

  fNtrkl_PtOfMaxTrk_H_m = new TH2F("fNtrkl_PtOfMaxTrk_H_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H70_m = new TH2F("fNtrkl_PtOfMaxTrk_H70_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H70_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H70_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H80_m = new TH2F("fNtrkl_PtOfMaxTrk_H80_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H80_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H80_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H85_m = new TH2F("fNtrkl_PtOfMaxTrk_H85_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H85_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H85_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H90_m = new TH2F("fNtrkl_PtOfMaxTrk_H90_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H90_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H90_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H93_m = new TH2F("fNtrkl_PtOfMaxTrk_H93_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H93_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H93_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H95_m = new TH2F("fNtrkl_PtOfMaxTrk_H95_m","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H95_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H95_m->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H_m = new TH2F("fNtrkl_PtOfTrks_H_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H70_m = new TH2F("fNtrkl_PtOfTrks_H70_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H70_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H70_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H80_m = new TH2F("fNtrkl_PtOfTrks_H80_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H80_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H80_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H85_m = new TH2F("fNtrkl_PtOfTrks_H85_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H85_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H85_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H90_m = new TH2F("fNtrkl_PtOfTrks_H90_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H90_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H90_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H93_m = new TH2F("fNtrkl_PtOfTrks_H93_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H93_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H93_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H95_m = new TH2F("fNtrkl_PtOfTrks_H95_m","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H95_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H95_m->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");

  fNtrkl_PtOfMaxTrk_W_w = new TH2F("fNtrkl_PtOfMaxTrk_W_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W70_w = new TH2F("fNtrkl_PtOfMaxTrk_W70_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W70_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W70_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W80_w = new TH2F("fNtrkl_PtOfMaxTrk_W80_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W80_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W80_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W85_w = new TH2F("fNtrkl_PtOfMaxTrk_W85_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W85_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W85_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W90_w = new TH2F("fNtrkl_PtOfMaxTrk_W90_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W90_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W90_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W93_w = new TH2F("fNtrkl_PtOfMaxTrk_W93_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W93_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W93_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_W95_w = new TH2F("fNtrkl_PtOfMaxTrk_W95_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_W95_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_W95_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W_w = new TH2F("fNtrkl_PtOfTrks_W_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W70_w = new TH2F("fNtrkl_PtOfTrks_W70_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W70_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W70_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W80_w = new TH2F("fNtrkl_PtOfTrks_W80_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W80_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W80_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W85_w = new TH2F("fNtrkl_PtOfTrks_W85_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W85_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W85_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W90_w = new TH2F("fNtrkl_PtOfTrks_W90_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W90_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W90_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W93_w = new TH2F("fNtrkl_PtOfTrks_W93_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W93_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W93_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_W95_w = new TH2F("fNtrkl_PtOfTrks_W95_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_W95_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_W95_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");

  fNtrkl_PtOfMaxTrk_HF_w = new TH2F("fNtrkl_PtOfMaxTrk_HF_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF70_w = new TH2F("fNtrkl_PtOfMaxTrk_HF70_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF70_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF70_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF80_w = new TH2F("fNtrkl_PtOfMaxTrk_HF80_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF80_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF80_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF85_w = new TH2F("fNtrkl_PtOfMaxTrk_HF85_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF85_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF85_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF90_w = new TH2F("fNtrkl_PtOfMaxTrk_HF90_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF90_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF90_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF93_w = new TH2F("fNtrkl_PtOfMaxTrk_HF93_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF93_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF93_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_HF95_w = new TH2F("fNtrkl_PtOfMaxTrk_HF95_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_HF95_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_HF95_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF_w = new TH2F("fNtrkl_PtOfTrks_HF_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF70_w = new TH2F("fNtrkl_PtOfTrks_HF70_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF70_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF70_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF80_w = new TH2F("fNtrkl_PtOfTrks_HF80_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF80_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF80_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF85_w = new TH2F("fNtrkl_PtOfTrks_HF85_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF85_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF85_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF90_w = new TH2F("fNtrkl_PtOfTrks_HF90_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF90_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF90_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF93_w = new TH2F("fNtrkl_PtOfTrks_HF93_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF93_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF93_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_HF95_w = new TH2F("fNtrkl_PtOfTrks_HF95_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_HF95_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_HF95_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");

  fNtrkl_PtOfMaxTrk_H_w = new TH2F("fNtrkl_PtOfMaxTrk_H_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H70_w = new TH2F("fNtrkl_PtOfMaxTrk_H70_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H70_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H70_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H80_w = new TH2F("fNtrkl_PtOfMaxTrk_H80_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H80_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H80_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H85_w = new TH2F("fNtrkl_PtOfMaxTrk_H85_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H85_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H85_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H90_w = new TH2F("fNtrkl_PtOfMaxTrk_H90_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H90_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H90_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H93_w = new TH2F("fNtrkl_PtOfMaxTrk_H93_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H93_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H93_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfMaxTrk_H95_w = new TH2F("fNtrkl_PtOfMaxTrk_H95_w","",200,0,200,200,0,2);
  fNtrkl_PtOfMaxTrk_H95_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfMaxTrk_H95_w->GetYaxis()->SetTitle("p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H_w = new TH2F("fNtrkl_PtOfTrks_H_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H70_w = new TH2F("fNtrkl_PtOfTrks_H70_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H70_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H70_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H80_w = new TH2F("fNtrkl_PtOfTrks_H80_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H80_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H80_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H85_w = new TH2F("fNtrkl_PtOfTrks_H85_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H85_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H85_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H90_w = new TH2F("fNtrkl_PtOfTrks_H90_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H90_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H90_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H93_w = new TH2F("fNtrkl_PtOfTrks_H93_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H93_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H93_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");
  fNtrkl_PtOfTrks_H95_w = new TH2F("fNtrkl_PtOfTrks_H95_w","",200,0,200,200,0,2);
  fNtrkl_PtOfTrks_H95_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_PtOfTrks_H95_w->GetYaxis()->SetTitle("sum p_{T,trk}/p_{T,can}");

  fHistPt_We_Ntrkl = new TH2F("fHistPt_We_Ntrkl","",200,0,200,200,0,200);
  fHistPt_We_Ntrkl->GetXaxis()->SetTitle("p_{T,can} (GeV/c)");
  fHistPt_We_Ntrkl->GetYaxis()->SetTitle("N_{tracklets}");

  fNtrkl_ClustE = new TH2F("fNtrkl_ClustE","N_{tracklets} vs cluster energy; N_{tracklets}; E (GeV)",200,0,200,100,0,50);
  TPCSigForE = new TH2F("TPCSigForE","TPC signal for electron EMCal cut", 5000, 0.1, 50, 10000, 0, 10000);
  TPCSigForE->GetXaxis()->SetTitle("p (GeV/c)");
  TPCSigForE->GetYaxis()->SetTitle("dE/dx");
  TPCSigForE->SetMarkerStyle(7);
  fNsigmaPtForE = new TH2F("fNsigmaPtForE","n#sigma vs p_{T} for electron EMCal cut ;p_{T} (GeV/c) ;n#sigma",800,0,80,200,-10,10);

  fHistUEmult = new TH2F("fHistUEmult","Mult vs p_{T,asso} (#pi/4 < #Delta#phi < 3#pi/4)",200,0,200,200,0,20);
  fHistUEmult->GetXaxis()->SetTitle("N_{tracklets}");
  fHistUEmult->GetYaxis()->SetTitle("p_{T,asso} (GeV/c)");

  fNtrkl_LeadHadPt_m = new TH2F("fNtrkl_LeadHadPt_m","",200,0,200,200,0,200);
  fNtrkl_LeadHadPt_m->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_LeadHadPt_m->GetYaxis()->SetTitle("p_{T,lead}");
  fNtrkl_LeadHadPt_w = new TH2F("fNtrkl_LeadHadPt_w","",200,0,200,200,0,200);
  fNtrkl_LeadHadPt_w->GetXaxis()->SetTitle("N_{tracklets}");
  fNtrkl_LeadHadPt_w->GetYaxis()->SetTitle("p_{T,lead}");


  fOutputList->Add(fNevents);
  fOutputList->Add(pVertex_all);
  fOutputList->Add(pVertex);
  fOutputList->Add(EtavsPhi);
  fOutputList->Add(TPCSig);
  fOutputList->Add(fzvtx_Ntrkl);
  fOutputList->Add(fzvtx_Ntrkl_cal);
  fOutputList->Add(fHistNsigmaP);
  fOutputList->Add(fPtEoverPE);
  fOutputList->Add(fPtEoverPH);
  fOutputList->Add(fREisolation);
  fOutputList->Add(fdPhi_trkW_Pt);
  fOutputList->Add(fdPhi_trkHF_Pt);
  fOutputList->Add(fHistPt_We);
  fOutputList->Add(fHistPt_HFe);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W70_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W80_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W85_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W90_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W93_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W95_m);
  fOutputList->Add(fNtrkl_PtOfTrks_W_m);
  fOutputList->Add(fNtrkl_PtOfTrks_W70_m);
  fOutputList->Add(fNtrkl_PtOfTrks_W80_m);
  fOutputList->Add(fNtrkl_PtOfTrks_W85_m);
  fOutputList->Add(fNtrkl_PtOfTrks_W90_m);
  fOutputList->Add(fNtrkl_PtOfTrks_W93_m);
  fOutputList->Add(fNtrkl_PtOfTrks_W95_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF70_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF80_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF85_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF90_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF93_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF95_m);
  fOutputList->Add(fNtrkl_PtOfTrks_HF_m);
  fOutputList->Add(fNtrkl_PtOfTrks_HF70_m);
  fOutputList->Add(fNtrkl_PtOfTrks_HF80_m);
  fOutputList->Add(fNtrkl_PtOfTrks_HF85_m);
  fOutputList->Add(fNtrkl_PtOfTrks_HF90_m);
  fOutputList->Add(fNtrkl_PtOfTrks_HF93_m);
  fOutputList->Add(fNtrkl_PtOfTrks_HF95_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H70_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H80_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H85_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H90_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H93_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H95_m);
  fOutputList->Add(fNtrkl_PtOfTrks_H_m);
  fOutputList->Add(fNtrkl_PtOfTrks_H70_m);
  fOutputList->Add(fNtrkl_PtOfTrks_H80_m);
  fOutputList->Add(fNtrkl_PtOfTrks_H85_m);
  fOutputList->Add(fNtrkl_PtOfTrks_H90_m);
  fOutputList->Add(fNtrkl_PtOfTrks_H93_m);
  fOutputList->Add(fNtrkl_PtOfTrks_H95_m);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W70_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W80_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W85_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W90_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W93_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_W95_w);
  fOutputList->Add(fNtrkl_PtOfTrks_W_w);
  fOutputList->Add(fNtrkl_PtOfTrks_W70_w);
  fOutputList->Add(fNtrkl_PtOfTrks_W80_w);
  fOutputList->Add(fNtrkl_PtOfTrks_W85_w);
  fOutputList->Add(fNtrkl_PtOfTrks_W90_w);
  fOutputList->Add(fNtrkl_PtOfTrks_W93_w);
  fOutputList->Add(fNtrkl_PtOfTrks_W95_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF70_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF80_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF85_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF90_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF93_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_HF95_w);
  fOutputList->Add(fNtrkl_PtOfTrks_HF_w);
  fOutputList->Add(fNtrkl_PtOfTrks_HF70_w);
  fOutputList->Add(fNtrkl_PtOfTrks_HF80_w);
  fOutputList->Add(fNtrkl_PtOfTrks_HF85_w);
  fOutputList->Add(fNtrkl_PtOfTrks_HF90_w);
  fOutputList->Add(fNtrkl_PtOfTrks_HF93_w);
  fOutputList->Add(fNtrkl_PtOfTrks_HF95_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H70_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H85_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H90_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H93_w);
  fOutputList->Add(fNtrkl_PtOfMaxTrk_H95_w);
  fOutputList->Add(fNtrkl_PtOfTrks_H_w);
  fOutputList->Add(fNtrkl_PtOfTrks_H70_w);
  fOutputList->Add(fNtrkl_PtOfTrks_H80_w);
  fOutputList->Add(fNtrkl_PtOfTrks_H85_w);
  fOutputList->Add(fNtrkl_PtOfTrks_H90_w);
  fOutputList->Add(fNtrkl_PtOfTrks_H93_w);
  fOutputList->Add(fNtrkl_PtOfTrks_H95_w);
  fOutputList->Add(fHistPt_We_Ntrkl);
  fOutputList->Add(fNtrkl_ClustE);
  fOutputList->Add(TPCSigForE);
  fOutputList->Add(fNsigmaPtForE);
  fOutputList->Add(fHistUEmult);
  fOutputList->Add(fNtrkl_LeadHadPt_m);
  fOutputList->Add(fNtrkl_LeadHadPt_w);

  PostData(1, fOutputList);
}
//_____________________________________________________________________________
void AliAnalysisTaskWHMult::UserExec(Option_t *)
{
  //!!!!!!!!!!!//
  //this function is called once for each event
  //!!!!!!!!!!!//

  fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
  if(!fAOD) return;

  if(fAOD)fMCarray = dynamic_cast<TClonesArray*>(fAOD->FindListObject(AliAODMCParticle::StdBranchName()));

  fTracks_tender = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject("tracks"));			//get updated tracks
  fCaloClusters_tender = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject("caloClusters"));	//get updated clusters

  fVevent = dynamic_cast<AliVEvent*>(InputEvent());
  if (!fVevent) {
	  printf("ERROR: fVEvent not available\n");
	  return;
  }   

  AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
  if (man) {
    AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());
    if (inputHandler)   fPIDResponse = inputHandler->GetPIDResponse();
  }

  if(!gGrid){
    cout << "no Grid connection, connecting to the Grid ..." << endl;
    TGrid::Connect("alien//");
  }

  //==== systematic parameters =====
  Int_t CutITSCls = NITSClust;
  Int_t CutTPCCrossedRows = NCrossedRows;
  Int_t CutTPCCls = NTPCClust;
  Double_t CutNsigma[2] = {NSigmaMin,NSigmaMax};
  Double_t CutM02[2] = {M02Min,M02Max};
  Double_t CutEoP[2] = {EoPMin,EoPMax};
  Double_t IsolationCut_E = EisoMax;
  Int_t IsolationCut_Trk = NtrkMax;

  //==== trigger selection ======
  TString firedTrigger;
  TString TriggerEG1("EG1");

  fVevent->GetFiredTriggerClasses();
  if(fAOD) firedTrigger = fAOD->GetFiredTriggerClasses();
  if(fEMCEG1){if(!firedTrigger.Contains(TriggerEG1))return;}

  //=====reference objects file=====
  if(!tree)
  {
   cout << "no tree for correcting mult" << endl; 
   cout << "get from Grid" << endl; 
   TFile* RefData=TFile::Open("alien:///alice/cern.ch/user/t/takawagu/Data.root");
   tree=(TTree*)RefData->Get("tree");
   //TTree* treee=(TTree*)RefData->Get("treee");
  }
 
  //Int_t iTracks(fAOD->GetNumberOfTracks());		//original
  Int_t iTracks(fTracks_tender->GetEntries());		//updated

  //Int_t Nclust = fVevent->GetNumberOfCaloClusters();
  Int_t Nclust = fCaloClusters_tender->GetEntries();

  //===== Global Vtx =====
  const AliVVertex *pVtx = fVevent->GetPrimaryVertex();
  Double_t NcontV = pVtx->GetNContributors();
  Double_t Zvertex = pVtx->GetZ();
  pVertex_all->Fill(Zvertex);
  //===== SPD Vtx =====
  const AliVVertex *pVtxSPD = fVevent->GetPrimaryVertexSPD();
  Double_t ZvertexSPD = pVtxSPD->GetZ();
  Double_t NcontVSPD = pVtxSPD->GetNContributors();
  Double_t cov[6] = {0};
  pVtxSPD->GetCovarianceMatrix(cov);

  Float_t centrality(0);
  AliMultSelection *multSelection =static_cast<AliMultSelection*>(fAOD->FindListObject("MultSelection"));
  if(multSelection) centrality = multSelection->GetMultiplicityPercentile("V0M");	//get centrality

  ////////////////////////
  //  Event Selection   //
  ////////////////////////
  fNevents->Fill(0);
  // ===== 1. remove pile up events =====
  if (fVevent->IsPileupFromSPDInMultBins()) return;
  fNevents->Fill(1);
  // ===== 2. Global contributors cut =====
  if (NcontV < 2) return;
  fNevents->Fill(2);
  // ===== 3. SPD contributors cut =====
  if (NcontVSPD < 1) return;
  fNevents->Fill(3);
  // ===== 4. select events where SPD and primary vertex match =====
  if (TMath::Abs(ZvertexSPD - Zvertex) > 0.5) return;
  fNevents->Fill(4);
  // ===== 5. SPD vertex resolution cut =====
  if (TMath::Sqrt(cov[5]) > 0.25) return;
  fNevents->Fill(5);
  // ===== 6. Z vertex position cut =====
  if (TMath::Abs(Zvertex) > 10.) return;
  fNevents->Fill(6);
  pVertex->Fill(Zvertex);

//---------------SPD tracklets---------------
  Int_t nTracklets = 0;
  Double_t nAcc = 0;		//number of tracklets in etarange
  Double_t corr_nAcc = 0;	//corrected with vertexZ
  Double_t etaRange = 1.0;

  AliAODTracklets* tracklets = static_cast<const AliAODEvent*>(fAOD)->GetTracklets();
  nTracklets = tracklets->GetNumberOfTracklets();
  for (Int_t nn=0;nn<nTracklets;nn++) {
    Double_t trkltheta = tracklets->GetTheta(nn);
    Double_t trkleta = -TMath::Log(TMath::Tan(trkltheta/2.0));
    if (TMath::Abs(trkleta)<etaRange) nAcc++;
  }
  fzvtx_Ntrkl->Fill(Zvertex,nAcc);

  //=====vertex Z correction=====
  Int_t BinOfvertexZ = fzvtx_Ntrkl->GetXaxis()->FindBin(Zvertex);
  corr_nAcc = GetCorrectedNtrackletsD(tree, nAcc, BinOfvertexZ, fAOD);
  fzvtx_Ntrkl_cal->Fill(Zvertex,corr_nAcc);

  Int_t Nch = 0;
  if(fMCarray){
    Nch = CountNch();
  }

  //===== detector effect correction =====
  Double_t PtCorr_had_ele = 1.00/1.02;		//hadron/electron detctor->particle

  //===== Self UE contribution estimate via mult =====
  Double_t fAssoHadPtLow70 = 0.;
  Double_t fAssoHadPtLow80 = 0.;
  Double_t fAssoHadPtLow85 = 0.;
  Double_t fAssoHadPtLow90 = 0.;
  Double_t fAssoHadPtLow93 = 0.;
  Double_t fAssoHadPtLow95 = 0.;
  if (corr_nAcc >= 0 && corr_nAcc <= 150) {
    if (corr_nAcc > 0 && corr_nAcc < 11) {
      fAssoHadPtLow70 = 0.85;
      fAssoHadPtLow80 = 1.03;
      fAssoHadPtLow85 = 1.21;
      fAssoHadPtLow90 = 1.44;
      fAssoHadPtLow93 = 1.74;
      fAssoHadPtLow95 = 2.04;
    }
    if (corr_nAcc > 10 && corr_nAcc < 21) {
      fAssoHadPtLow70 = 0.75;
      fAssoHadPtLow80 = 0.98;
      fAssoHadPtLow85 = 1.16;
      fAssoHadPtLow90 = 1.42;
      fAssoHadPtLow93 = 1.66;
      fAssoHadPtLow95 = 1.96;
    }
    if (corr_nAcc > 20 && corr_nAcc < 31) {
      fAssoHadPtLow70 = 0.83;
      fAssoHadPtLow80 = 1.11;
      fAssoHadPtLow85 = 1.30;
      fAssoHadPtLow90 = 1.63;
      fAssoHadPtLow93 = 1.94;
      fAssoHadPtLow95 = 2.21;
    }
    if (corr_nAcc > 30 && corr_nAcc < 41) {
      fAssoHadPtLow70 = 0.84;
      fAssoHadPtLow80 = 1.09;
      fAssoHadPtLow85 = 1.30;
      fAssoHadPtLow90 = 1.72;
      fAssoHadPtLow93 = 2.04;
      fAssoHadPtLow95 = 2.38;
    }
    if (corr_nAcc > 40 && corr_nAcc < 51) {
      fAssoHadPtLow70 = 0.88;
      fAssoHadPtLow80 = 1.14;
      fAssoHadPtLow85 = 1.35;
      fAssoHadPtLow90 = 1.73;
      fAssoHadPtLow93 = 2.02;
      fAssoHadPtLow95 = 2.36;
    }
    if (corr_nAcc > 50 && corr_nAcc < 66) {
      fAssoHadPtLow70 = 0.93;
      fAssoHadPtLow80 = 1.22;
      fAssoHadPtLow85 = 1.43;
      fAssoHadPtLow90 = 1.80;
      fAssoHadPtLow93 = 2.26;
      fAssoHadPtLow95 = 2.93;
    }
    if (corr_nAcc > 65 && corr_nAcc < 150) {
      fAssoHadPtLow70 = 1.03;
      fAssoHadPtLow80 = 1.42;
      fAssoHadPtLow85 = 1.54;
      fAssoHadPtLow90 = 1.94;
      fAssoHadPtLow93 = 2.93;
      fAssoHadPtLow95 = 3.53;
    }
  }

//---------------track loop---------------
  for(Int_t i(0); i < iTracks; i++) {
    //AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));	//original
    AliAODTrack* track = dynamic_cast<AliAODTrack*>(fTracks_tender->At(i));	//updated
    if (!track) continue;

    ////////////////////////////
    // Set cutting parameters //
    ////////////////////////////
    Int_t EMCalIndex = -1;				//Has EMCal ID
    Bool_t ClsTypeEMCal = kFALSE;			//within EMCal
    Bool_t HasEMCalInfo = kFALSE;			//TrackMatching
    Double_t clustLongE = 0;				//M02
    Double_t EoverP = 0;				//E/p
    Double_t fTPCnSigma = 999;				//nsigma
    Double_t Eiso = 0;					//isolation energy
    Double_t clustEmatch = 0;				//cluster energy
    Int_t isoNtrk = 0;					//number of tracks in R
    Double_t Eiso_BG = 0;				//isolation energy for BG hadron
    Int_t isoNtrk_BG = 0;				//number of tracks in R for BG hadron

//---------------FilterBit 1---------------
    if(!track || !track->TestFilterBit(1)) continue;

  // Comparison with TPC analysis
    Double_t DCA[2] = {-999.,-999.}, covar[3];
    /////////////////////////
    //      track cut      //
    /////////////////////////
  //===== 1. TPC and ITS refit cut =====
    if (!(track->GetStatus()&AliAODTrack::kITSrefit) || !(track->GetStatus()&AliAODTrack::kTPCrefit)) continue;
  //===== 2. AOD filter bit required =====
    if (!track->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) continue;		//minimum cut
  //===== 3. TPC cluster cut =====
    if (track->GetTPCNcls() < CutTPCCls) continue;
  //===== 4. ITS cluster cut =====
    if (track->GetITSNcls() < CutITSCls) continue;
  //===== 5. SPD hit cut =====
    if (!(track->HasPointOnITSLayer(0) || track->HasPointOnITSLayer(1))) continue;
  //===== 6. Eta cut =====
    if (track->Eta() > 0.9 || track->Eta() < -0.9) continue;
  //===== 7. DCA cut =====
    if (track -> PropagateToDCA(pVtx,fAOD -> GetMagneticField(),20.,DCA,covar))
    {
      if (TMath::Abs(DCA[0]) > 2.4 || TMath::Abs(DCA[1]) > 3.2) continue;
    }
  //===== 8. chi2 cut =====
    Double_t TPCchi2NDF = track->GetTPCchi2perNDF();
    Double_t ITSchi2 = track->GetITSchi2();
    if ((ITSchi2 >= 25) || (TPCchi2NDF >= 4)) continue;
  //===== 9. NCrossedRow cut =====
    if (track->GetTPCCrossedRows() < CutTPCCrossedRows) continue;


    double eta = -log(TMath::Tan((track->Theta())/2.));
    EtavsPhi->Fill(eta,track->Phi());

    TPCSig->Fill(track->P(), track->GetTPCsignal());

    fTPCnSigma = fPIDResponse->NumberOfSigmasTPC(track, AliPID::kElectron);
    fHistNsigmaP->Fill(track->P(),fTPCnSigma);

//---------------track matching---------------
    EMCalIndex = track->GetEMCALcluster();
    if (track->Eta() > 0.6 || track->Eta() < -0.6) continue;
    AliVCluster*clustMatch = 0x0;
    if (EMCalIndex >= 0)
    {
      //clustMatch = (AliVCluster*)fVevent->GetCaloCluster(EMCalIndex);			//original
      clustMatch = dynamic_cast<AliVCluster*>(fCaloClusters_tender->At(EMCalIndex));	//updated

      Float_t showerx[3];
      clustMatch->GetPosition(showerx);
      TVector3 Corepos(showerx[0],showerx[1],showerx[2]);
      Double_t Corephi = Corepos.Phi();
      Double_t Coreeta = Corepos.Eta();
      if (Corephi < 0.) Corephi = Corephi + (2.*TMath::Pi());
      if (Corephi > 1.39 && Corephi < 3.265) ClsTypeEMCal = kTRUE;
      if (!ClsTypeEMCal) continue;

      if (TMath::Abs(clustMatch->GetTrackDx()) > 0.05 || TMath::Abs(clustMatch->GetTrackDz()) > 0.05) continue;
      HasEMCalInfo = kTRUE;

      clustEmatch = clustMatch->E();
      fNtrkl_ClustE->Fill(corr_nAcc,clustEmatch);

      clustLongE = clustMatch->GetM02();
      EoverP = clustEmatch/track->P();

//---------------nsigma cut (electron)---------------
      if (fTPCnSigma > CutNsigma[0] && fTPCnSigma < CutNsigma[1])
      {

//---------------M02 cut (electron)---------------
        if (clustLongE > CutM02[0] && clustLongE < CutM02[1])
        {
          fPtEoverPE->Fill(track->Pt(),EoverP);

//---------------EoverP cut (electron)---------------
          //////////////
          // ELECTRON //
          //////////////
          if (EoverP > CutEoP[0] && EoverP < CutEoP[1])
          {
//---------------track loop for counts in R---------------
            for(Int_t k(0); k < iTracks; k++) {
              if (k == i) continue;
              //AliAODTrack* track2 = static_cast<AliAODTrack*>(fAOD->GetTrack(k));
              AliAODTrack* track2 = dynamic_cast<AliAODTrack*>(fTracks_tender->At(k));
              if(!track2 || !track2->TestFilterBit(1)) continue;

              // Comparison with TPC analysis
              Double_t DCA2[2] = {-999.,-999.}, covar2[3];
              /////////////////////////
              //      track cut      //
              /////////////////////////
            //===== 1. TPC and ITS refit cut =====
              if (!(track2->GetStatus()&AliAODTrack::kITSrefit) || !(track2->GetStatus()&AliAODTrack::kTPCrefit)) continue;
            //===== 2. AOD filter bit required =====
              if (!track2->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) continue;
            //===== 3. TPC cluster cut =====
              if (track2->GetTPCNcls() < CutTPCCls) continue;
            //===== 4. ITS cluster cut =====
              if (track2->GetITSNcls() < CutITSCls) continue;
            //===== 5. SPD hit cut =====
              if (!(track2->HasPointOnITSLayer(0) || track2->HasPointOnITSLayer(1))) continue;
            //===== 6. Eta cut =====
              if (track2->Eta() > 0.9 || track2->Eta() < -0.9) continue;
            //===== 7. DCA cut =====
              if (track2 -> PropagateToDCA(pVtx,fAOD -> GetMagneticField(),20.,DCA2,covar2))
              {
                if (TMath::Abs(DCA2[0]) > 2.4 || TMath::Abs(DCA2[1]) > 3.2) continue;
              }
            //===== 8. chi2 cut =====
              Double_t TPCchi2NDF2 = track2->GetTPCchi2perNDF();
              Double_t ITSchi2_2 = track2->GetITSchi2();
              if ((ITSchi2_2 >= 25) || (TPCchi2NDF2 >= 4)) continue;
            //===== 9. NCrossedRow cut =====
              if (track2->GetTPCCrossedRows() < CutTPCCrossedRows) continue;

              Double_t eta2 = track2->Eta();
              Double_t phi2 = track2->Phi();
              if (phi2 < 0.0) phi2 = phi2 + (2.*TMath::Pi());
              Double_t Rtrk = sqrt(pow(track->Phi() - phi2,2.0)+pow(track->Eta() - eta2,2.0));
              if (Rtrk < 0.3) isoNtrk += 1;
            }

//---------------EMCal Cluster loop for Isolation cut start---------------
            Double_t RsumE = 0;
            for(Int_t icl=0;icl<Nclust;icl++){
              AliVCluster*clust = 0x0;
              //clust = (AliVCluster*)fVevent->GetCaloCluster(icl);			//original
              clust = dynamic_cast<AliVCluster*>(fCaloClusters_tender->At(icl));	//updated
              if(clust && clust->IsEMCAL()){
                Double_t clustE = clust->E();
                Float_t aroclsx[3];
                clust->GetPosition(aroclsx);
                TVector3 aroClsPos(aroclsx[0],aroclsx[1],aroclsx[2]);
                Double_t aroClsphi = aroClsPos.Phi();
                if (aroClsphi<0.0) aroClsphi = aroClsphi + (2.0*TMath::Pi());  // added s.s.
                Double_t aroClseta = aroClsPos.Eta();
                Double_t R = sqrt(pow(Corephi - aroClsphi,2.0)+pow(Coreeta - aroClseta,2.0));
                if (R < 0.3) RsumE = RsumE + clustE;
              }
            }
            Eiso = (RsumE - clustEmatch)/clustEmatch;
            if (track->Pt() > 30.0) fREisolation->Fill(Eiso);
            if (Eiso >= 0.1 && Eiso <= 0.50 && isoNtrk < IsolationCut_Trk) fHistPt_HFe->Fill(track->Pt());
            if (Eiso >= 0.0 && Eiso <= IsolationCut_E && isoNtrk < IsolationCut_Trk) fHistPt_We->Fill(track->Pt());

//---------------Pt cut---------------
            if (track->Pt() > 30. && track->Pt() < 60.)
            {
              Double_t sumPt_OppTrks_m = 0;
              Double_t sumPt_OppTrks70_m = 0;
              Double_t sumPt_OppTrks80_m = 0;
              Double_t sumPt_OppTrks85_m = 0;
              Double_t sumPt_OppTrks90_m = 0;
              Double_t sumPt_OppTrks93_m = 0;
              Double_t sumPt_OppTrks95_m = 0;
//---------------another track loop start---------------
              Int_t MaxPtTrackNum_m = 0;
              for(Int_t j(0); j < iTracks; j++) {
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma < -1.5) {
                  MaxPtTrackNum_m = j;
                  break;
                }
              }
              for(Int_t j(0); j < iTracks; j++) {
                //AliAODTrack* anotrack = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                if(!anotrack || !anotrack->TestFilterBit(1)) continue;
                if (j == i) continue;     //reject e<-W candidate

                /////////////////////////
                //      track cut      //
                /////////////////////////
                //===== 1. TPC and ITS refit cut =====
                if (!(anotrack->GetStatus()&AliAODTrack::kITSrefit) || !(anotrack->GetStatus()&AliAODTrack::kTPCrefit)) continue;
                //===== 2. AOD filter bit required =====
                if (!anotrack->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) continue;
                //===== 3. TPC cluster cut =====
                if (anotrack->GetTPCNcls() < CutTPCCls) continue;
                //===== 4. ITS cluster cut =====
                if (anotrack->GetITSNcls() < CutITSCls) continue;
                //===== 5. SPD hit cut =====
                if (!(anotrack->HasPointOnITSLayer(0) || anotrack->HasPointOnITSLayer(1))) continue;
                //===== 6. Eta cut =====
                if (anotrack->Eta() > 0.9 || anotrack->Eta() < -0.9) continue;
                //===== 7. DCA cut =====
                Double_t anoDCA[2] = {-999.,-999.}, anocovar[3];
                if (anotrack -> PropagateToDCA(pVtx,fAOD -> GetMagneticField(),20.,anoDCA,anocovar))
                {
                  if (TMath::Abs(anoDCA[0]) > 2.4 || TMath::Abs(anoDCA[1]) > 3.2) continue;
                }
                //===== 8. chi2 cut =====
                Double_t anoITSchi2 = anotrack->GetITSchi2();
                Double_t anoTPCchi2NDF = anotrack->GetTPCchi2perNDF();
                if ((anoITSchi2 >= 25) || (anoTPCchi2NDF >= 4)) continue;
                //===== 9. NCrossedRow cut =====
                if (anotrack->GetTPCCrossedRows() < CutTPCCrossedRows) continue;

                //===== electron reject =====
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma >= -1.5) continue;

                //Double_t anoeta = -log(TMath::Tan((anotrack->Theta())/2.));
                Double_t dPhi = anotrack->Phi() - track->Phi();

                //=== change dPhi range ===
                if (dPhi < -1.*TMath::Pi()/3.) dPhi = dPhi + 2.*TMath::Pi();
                if (dPhi > 5.*TMath::Pi()/3.) dPhi = dPhi - 2.*TMath::Pi();

                if (Eiso >= 0.0 && Eiso <= IsolationCut_E && isoNtrk < IsolationCut_Trk) {
                  fdPhi_trkW_Pt->Fill(dPhi,anotrack->Pt());
                  if (dPhi >= 1.*TMath::Pi()/4. && dPhi <= 3.*TMath::Pi()/4.) fHistUEmult->Fill(corr_nAcc,anotrack->Pt());
                }
                if (Eiso >= 0.1 && Eiso <= 0.50 && isoNtrk < IsolationCut_Trk) {
                  fdPhi_trkHF_Pt->Fill(dPhi,anotrack->Pt());
                }

                //===== Highest Pt track selection =====
                if (dPhi >= TMath::Pi()-0.3 && dPhi <= TMath::Pi()+0.3) {
                  //AliAODTrack* MaxTrk = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum_m));
                  AliAODTrack* MaxTrk = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_m));
                  Double_t dPhiMaxTrk = (MaxTrk->Phi()) - (track->Phi());
                  //=== change dPhi range ===
                  if (dPhiMaxTrk < -1.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk + 2.*TMath::Pi();
                  if (dPhiMaxTrk > 5.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk - 2.*TMath::Pi();
                  //=== select back-to-back track ===
                  if (dPhiMaxTrk < TMath::Pi()-0.3 || dPhiMaxTrk > TMath::Pi()+0.3) MaxPtTrackNum_m = j;
                  //=== higher Pt track ===
                  if (anotrack->Pt() >= MaxTrk->Pt()) MaxPtTrackNum_m = j;
                  //=== Pt sum ===
                  if (anotrack->Pt() > 0.15) sumPt_OppTrks_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow70) sumPt_OppTrks70_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow80) sumPt_OppTrks80_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow85) sumPt_OppTrks85_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow90) sumPt_OppTrks90_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow93) sumPt_OppTrks93_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow95) sumPt_OppTrks95_m += anotrack->Pt();
                }
              }   //other track loop end

              //===== Highest Pt Track / e<-W Pt Track =====
              //AliAODTrack* MaxPtTrk_m = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum_m));
              AliAODTrack* MaxPtTrk_m = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_m));
              Double_t leadPt_m = MaxPtTrk_m->Pt();

//---------------e<-W isolation cut---------------
              //////////////////////
              // e <- W candidate //
              //////////////////////
              if (Eiso >= 0.0 && Eiso <= IsolationCut_E && isoNtrk < IsolationCut_Trk) {
                fNtrkl_LeadHadPt_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m);
                if (leadPt_m > 0.15) fNtrkl_PtOfMaxTrk_W_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow70) fNtrkl_PtOfMaxTrk_W70_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow80) fNtrkl_PtOfMaxTrk_W80_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow85) fNtrkl_PtOfMaxTrk_W85_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow90) fNtrkl_PtOfMaxTrk_W90_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow93) fNtrkl_PtOfMaxTrk_W93_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow95) fNtrkl_PtOfMaxTrk_W95_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (sumPt_OppTrks_m > 0.) fNtrkl_PtOfTrks_W_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks_m/track->Pt());
                if (sumPt_OppTrks70_m > 0.) fNtrkl_PtOfTrks_W70_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks70_m/track->Pt());
                if (sumPt_OppTrks80_m > 0.) fNtrkl_PtOfTrks_W80_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks80_m/track->Pt());
                if (sumPt_OppTrks85_m > 0.) fNtrkl_PtOfTrks_W85_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks85_m/track->Pt());
                if (sumPt_OppTrks90_m > 0.) fNtrkl_PtOfTrks_W90_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks90_m/track->Pt());
                if (sumPt_OppTrks93_m > 0.) fNtrkl_PtOfTrks_W93_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks93_m/track->Pt());
                if (sumPt_OppTrks95_m > 0.) fNtrkl_PtOfTrks_W95_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks95_m/track->Pt());
                fHistPt_We_Ntrkl->Fill(track->Pt(),corr_nAcc);
              }       //for W isolation cut end
              if (Eiso >= 0.1 && Eiso <= 0.50 && isoNtrk < IsolationCut_Trk) {
                if (leadPt_m > 0.15) fNtrkl_PtOfMaxTrk_HF_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow70) fNtrkl_PtOfMaxTrk_HF70_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow80) fNtrkl_PtOfMaxTrk_HF80_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow85) fNtrkl_PtOfMaxTrk_HF85_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow90) fNtrkl_PtOfMaxTrk_HF90_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow93) fNtrkl_PtOfMaxTrk_HF93_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (leadPt_m > fAssoHadPtLow95) fNtrkl_PtOfMaxTrk_HF95_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
                if (sumPt_OppTrks_m > 0.) fNtrkl_PtOfTrks_HF_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks_m/track->Pt());
                if (sumPt_OppTrks70_m > 0.) fNtrkl_PtOfTrks_HF70_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks70_m/track->Pt());
                if (sumPt_OppTrks80_m > 0.) fNtrkl_PtOfTrks_HF80_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks80_m/track->Pt());
                if (sumPt_OppTrks85_m > 0.) fNtrkl_PtOfTrks_HF85_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks85_m/track->Pt());
                if (sumPt_OppTrks90_m > 0.) fNtrkl_PtOfTrks_HF90_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks90_m/track->Pt());
                if (sumPt_OppTrks93_m > 0.) fNtrkl_PtOfTrks_HF93_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks93_m/track->Pt());
                if (sumPt_OppTrks95_m > 0.) fNtrkl_PtOfTrks_HF95_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks95_m/track->Pt());
              }       //for HF isolation cut end

              Double_t sumPt_OppTrks_w = 0;
              Double_t sumPt_OppTrks70_w = 0;
              Double_t sumPt_OppTrks80_w = 0;
              Double_t sumPt_OppTrks85_w = 0;
              Double_t sumPt_OppTrks90_w = 0;
              Double_t sumPt_OppTrks93_w = 0;
              Double_t sumPt_OppTrks95_w = 0;
//---------------another track loop start---------------
              Int_t MaxPtTrackNum_w = 0;
                for(Int_t j(0); j < iTracks; j++) {
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma < -1.5) {
                  MaxPtTrackNum_w = j;
                  break;
                }
              }
              for(Int_t j(0); j < iTracks; j++) {
                //AliAODTrack* anotrack = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                if(!anotrack || !anotrack->TestFilterBit(1)) continue;
                if (j == i) continue;     //reject e<-W candidate

                /////////////////////////
                //      track cut      //
                /////////////////////////
                //===== 1. TPC and ITS refit cut =====
                if (!(anotrack->GetStatus()&AliAODTrack::kITSrefit) || !(anotrack->GetStatus()&AliAODTrack::kTPCrefit)) continue;
                //===== 2. AOD filter bit required =====
                if (!anotrack->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) continue;
                //===== 3. TPC cluster cut =====
                if (anotrack->GetTPCNcls() < CutTPCCls) continue;
                //===== 4. ITS cluster cut =====
                if (anotrack->GetITSNcls() < CutITSCls) continue;
                //===== 5. SPD hit cut =====
                if (!(anotrack->HasPointOnITSLayer(0) || anotrack->HasPointOnITSLayer(1))) continue;
                //===== 6. Eta cut =====
                if (anotrack->Eta() > 0.9 || anotrack->Eta() < -0.9) continue;
                //===== 7. DCA cut =====
                Double_t anoDCA[2] = {-999.,-999.}, anocovar[3];
                if (anotrack -> PropagateToDCA(pVtx,fAOD -> GetMagneticField(),20.,anoDCA,anocovar))
                {
                  if (TMath::Abs(anoDCA[0]) > 2.4 || TMath::Abs(anoDCA[1]) > 3.2) continue;
                }
                //===== 8. chi2 cut =====
                Double_t anoITSchi2 = anotrack->GetITSchi2();
                Double_t anoTPCchi2NDF = anotrack->GetTPCchi2perNDF();
                if ((anoITSchi2 >= 25) || (anoTPCchi2NDF >= 4)) continue;
                //===== 9. NCrossedRow cut =====
                if (anotrack->GetTPCCrossedRows() < CutTPCCrossedRows) continue;

                //===== electron reject =====
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma >= -1.5) continue;

                //Double_t anoeta = -log(TMath::Tan((anotrack->Theta())/2.));
                Double_t dPhi = anotrack->Phi() - track->Phi();

                //=== change dPhi range ===
                if (dPhi < -1.*TMath::Pi()/3.) dPhi = dPhi + 2.*TMath::Pi();
                if (dPhi > 5.*TMath::Pi()/3.) dPhi = dPhi - 2.*TMath::Pi();

                //===== Highest Pt track selection =====
                if (dPhi >= TMath::Pi()-0.5 && dPhi <= TMath::Pi()+0.5) {
                  //AliAODTrack* MaxTrk = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum_w));
                  AliAODTrack* MaxTrk = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_w));
                  Double_t dPhiMaxTrk = (MaxTrk->Phi()) - (track->Phi());
                  //=== change dPhi range ===
                  if (dPhiMaxTrk < -1.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk + 2.*TMath::Pi();
                  if (dPhiMaxTrk > 5.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk - 2.*TMath::Pi();
                  //=== select back-to-back track ===
                  if (dPhiMaxTrk < TMath::Pi()-0.5 || dPhiMaxTrk > TMath::Pi()+0.5) MaxPtTrackNum_w = j;
                  //=== higher Pt track ===
                  if (anotrack->Pt() >= MaxTrk->Pt()) MaxPtTrackNum_w = j;
                  //=== Pt sum ===
                  if (anotrack->Pt() > 0.15) sumPt_OppTrks_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow70) sumPt_OppTrks70_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow80) sumPt_OppTrks80_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow85) sumPt_OppTrks85_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow90) sumPt_OppTrks90_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow93) sumPt_OppTrks93_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow95) sumPt_OppTrks95_w += anotrack->Pt();
                }
              }   //other track loop end

              //===== Highest Pt Track / e<-W Pt Track =====
              //AliAODTrack* MaxPtTrk = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum));
              AliAODTrack* MaxPtTrk_w = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_w));
              Double_t leadPt_w = MaxPtTrk_w->Pt();

//---------------e<-W isolation cut---------------
              //////////////////////
              // e <- W candidate //
              //////////////////////
              if (Eiso >= 0.0 && Eiso <= IsolationCut_E && isoNtrk < IsolationCut_Trk) {
                if (leadPt_w > 0.15) fNtrkl_PtOfMaxTrk_W_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow70) fNtrkl_PtOfMaxTrk_W70_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow80) fNtrkl_PtOfMaxTrk_W80_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow85) fNtrkl_PtOfMaxTrk_W85_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow90) fNtrkl_PtOfMaxTrk_W90_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow93) fNtrkl_PtOfMaxTrk_W93_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow95) fNtrkl_PtOfMaxTrk_W95_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (sumPt_OppTrks_w > 0.) fNtrkl_PtOfTrks_W_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks_w/track->Pt());
                if (sumPt_OppTrks70_w > 0.) fNtrkl_PtOfTrks_W70_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks70_w/track->Pt());
                if (sumPt_OppTrks80_w > 0.) fNtrkl_PtOfTrks_W80_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks80_w/track->Pt());
                if (sumPt_OppTrks85_w > 0.) fNtrkl_PtOfTrks_W85_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks85_w/track->Pt());
                if (sumPt_OppTrks90_w > 0.) fNtrkl_PtOfTrks_W90_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks90_w/track->Pt());
                if (sumPt_OppTrks93_w > 0.) fNtrkl_PtOfTrks_W93_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks93_w/track->Pt());
                if (sumPt_OppTrks95_w > 0.) fNtrkl_PtOfTrks_W95_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks95_w/track->Pt());
              }       //for W isolation cut end
              if (Eiso >= 0.1 && Eiso <= 0.50 && isoNtrk < IsolationCut_Trk) {
                fNtrkl_LeadHadPt_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w);
                if (leadPt_w > 0.15) fNtrkl_PtOfMaxTrk_HF_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow70) fNtrkl_PtOfMaxTrk_HF70_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow80) fNtrkl_PtOfMaxTrk_HF80_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow85) fNtrkl_PtOfMaxTrk_HF85_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow90) fNtrkl_PtOfMaxTrk_HF90_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow93) fNtrkl_PtOfMaxTrk_HF93_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (leadPt_w > fAssoHadPtLow95) fNtrkl_PtOfMaxTrk_HF95_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
                if (sumPt_OppTrks_w > 0.) fNtrkl_PtOfTrks_HF_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks_w/track->Pt());
                if (sumPt_OppTrks70_w > 0.) fNtrkl_PtOfTrks_HF70_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks70_w/track->Pt());
                if (sumPt_OppTrks80_w > 0.) fNtrkl_PtOfTrks_HF80_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks80_w/track->Pt());
                if (sumPt_OppTrks85_w > 0.) fNtrkl_PtOfTrks_HF85_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks85_w/track->Pt());
                if (sumPt_OppTrks90_w > 0.) fNtrkl_PtOfTrks_HF90_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks90_w/track->Pt());
                if (sumPt_OppTrks93_w > 0.) fNtrkl_PtOfTrks_HF93_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks93_w/track->Pt());
                if (sumPt_OppTrks95_w > 0.) fNtrkl_PtOfTrks_HF95_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks95_w/track->Pt());
              }		//for HF isolation cut end
            }		//Pt cut end
          }		//M02 cut
        }		//EoverP cut
      }			//nsigma cut

//---------------nsigma cut (hadron)---------------
      if (fTPCnSigma < -3. && HasEMCalInfo && clustLongE > CutM02[0] && clustLongE < CutM02[1])
      {
        fPtEoverPH->Fill(track->Pt(),EoverP);

//---------------E/p cut---------------
        if (EoverP > CutEoP[0] && EoverP < CutEoP[1]) {

//---------------Isolation cut same as W---------------
          for(Int_t k(0); k < iTracks; k++) {
            if (k == i) continue;
            //AliAODTrack* track2 = static_cast<AliAODTrack*>(fAOD->GetTrack(k));
            AliAODTrack* track2 = dynamic_cast<AliAODTrack*>(fTracks_tender->At(k));
            if(!track2 || !track2->TestFilterBit(1)) continue;

            // Comparison with TPC analysis
            Double_t DCA2[2] = {-999.,-999.}, covar2[3];
            /////////////////////////
            //      track cut      //
            /////////////////////////
          //===== 1. TPC and ITS refit cut =====
            if (!(track2->GetStatus()&AliAODTrack::kITSrefit) || !(track2->GetStatus()&AliAODTrack::kTPCrefit)) continue;
          //===== 2. AOD filter bit required =====
            if (!track2->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) continue;
          //===== 3. TPC cluster cut =====
            if (track2->GetTPCNcls() < CutTPCCls) continue;
          //===== 4. ITS cluster cut =====
            if (track2->GetITSNcls() < CutITSCls) continue;
          //===== 5. SPD hit cut =====
            if (!(track2->HasPointOnITSLayer(0) || track2->HasPointOnITSLayer(1))) continue;
          //===== 6. Eta cut =====
            if (track2->Eta() > 0.9 || track2->Eta() < -0.9) continue;
          //===== 7. DCA cut =====
            if (track2 -> PropagateToDCA(pVtx,fAOD -> GetMagneticField(),20.,DCA2,covar2))
            {
              if (TMath::Abs(DCA2[0]) > 2.4 || TMath::Abs(DCA2[1]) > 3.2) continue;
            }
          //===== 8. chi2 cut =====
            Double_t TPCchi2NDF2 = track2->GetTPCchi2perNDF();
            Double_t ITSchi2_2 = track2->GetITSchi2();
            if ((ITSchi2_2 >= 25) || (TPCchi2NDF2 >= 4)) continue;
          //===== 9. NCrossedRow cut =====
            if (track2->GetTPCCrossedRows() < CutTPCCrossedRows) continue;

            Double_t eta2 = track2->Eta();
            Double_t phi2 = track2->Phi();
            if (phi2 < 0.0) phi2 = phi2 + (2.*TMath::Pi());
            Double_t Rtrk = sqrt(pow(track->Phi() - phi2,2.0)+pow(track->Eta() - eta2,2.0));
            if (Rtrk < 0.3) isoNtrk_BG += 1;
          }

          Double_t RsumE = 0;
          for(Int_t icl=0;icl<Nclust;icl++){
            AliVCluster*clust = 0x0;
            //clust = (AliVCluster*)fVevent->GetCaloCluster(icl);			//original
            clust = dynamic_cast<AliVCluster*>(fCaloClusters_tender->At(icl));	//updated
            if(clust && clust->IsEMCAL()){
              Double_t clustE = clust->E();
              Float_t aroclsx[3];
              clust->GetPosition(aroclsx);
              TVector3 aroClsPos(aroclsx[0],aroclsx[1],aroclsx[2]);
              Double_t aroClsphi = aroClsPos.Phi();
              if (aroClsphi<0.0) aroClsphi = aroClsphi + (2.0*TMath::Pi());  // added s.s.
              Double_t aroClseta = aroClsPos.Eta();
              Double_t R = sqrt(pow(Corephi - aroClsphi,2.0)+pow(Coreeta - aroClseta,2.0));
              if (R < 0.3) RsumE = RsumE + clustE;
            }
          }
          Eiso_BG = (RsumE - clustEmatch)/clustEmatch;
          if (Eiso_BG >= 0.0 && Eiso_BG <= IsolationCut_E && isoNtrk_BG < IsolationCut_Trk) {

//---------------Pt cut---------------
            if (track->Pt() > 30. && track->Pt() < 60.)
            {
              Double_t sumPt_OppTrks_m = 0;
              Double_t sumPt_OppTrks70_m = 0;
              Double_t sumPt_OppTrks80_m = 0;
              Double_t sumPt_OppTrks85_m = 0;
              Double_t sumPt_OppTrks90_m = 0;
              Double_t sumPt_OppTrks93_m = 0;
              Double_t sumPt_OppTrks95_m = 0;
//---------------another track loop start---------------
              Int_t MaxPtTrackNum_m = 0;
              for(Int_t j(0); j < iTracks; j++) {
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma < -1.5) {
                  MaxPtTrackNum_m = j;
                  break;
                }
              }
              for(Int_t j(0); j < iTracks; j++) {
                //AliAODTrack* anotrack = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                if(!anotrack || !anotrack->TestFilterBit(1)) continue;
                if (j == i) continue;     //reject e<-W candidate
    
                /////////////////////////
                //      track cut      //
                /////////////////////////
                //===== 1. TPC and ITS refit cut =====
                if (!(anotrack->GetStatus()&AliAODTrack::kITSrefit) || !(anotrack->GetStatus()&AliAODTrack::kTPCrefit)) continue;
                //===== 2. AOD filter bit required =====
                if (!anotrack->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) continue;
                //===== 3. TPC cluster cut =====
                if (anotrack->GetTPCNcls() < CutTPCCls) continue;
                //===== 4. ITS cluster cut =====
                if (anotrack->GetITSNcls() < CutITSCls) continue;
                //===== 5. SPD hit cut =====
                if (!(anotrack->HasPointOnITSLayer(0) || anotrack->HasPointOnITSLayer(1))) continue;
                //===== 6. Eta cut =====
                if (anotrack->Eta() > 0.9 || anotrack->Eta() < -0.9) continue;
                //===== 7. DCA cut =====
                Double_t anoDCA[2] = {-999.,-999.}, anocovar[3];
                if (anotrack -> PropagateToDCA(pVtx,fAOD -> GetMagneticField(),20.,anoDCA,anocovar))
                {
                  if (TMath::Abs(anoDCA[0]) > 2.4 || TMath::Abs(anoDCA[1]) > 3.2) continue;
                }
                //===== 8. chi2 cut =====
                Double_t anoITSchi2 = anotrack->GetITSchi2();
                Double_t anoTPCchi2NDF = anotrack->GetTPCchi2perNDF();
                if ((anoITSchi2 >= 25) || (anoTPCchi2NDF >= 4)) continue;
                //===== 9. NCrossedRow cut =====
                if (anotrack->GetTPCCrossedRows() < CutTPCCrossedRows) continue;
      
                //===== electron reject =====
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma >= -1.5) continue;
      
                //Double_t anoeta = -log(TMath::Tan((anotrack->Theta())/2.));
                Double_t dPhi = anotrack->Phi() - track->Phi();
      
                //=== change dPhi range ===
                if (dPhi < -1.*TMath::Pi()/3.) dPhi = dPhi + 2.*TMath::Pi();
                if (dPhi > 5.*TMath::Pi()/3.) dPhi = dPhi - 2.*TMath::Pi();
      
                //===== Highest Pt track selection =====
                if (dPhi >= TMath::Pi()-0.3 && dPhi <= TMath::Pi()+0.3) {
                  //AliAODTrack* MaxTrk = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum_m));
                  AliAODTrack* MaxTrk = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_m));
                  Double_t dPhiMaxTrk = (MaxTrk->Phi()) - (track->Phi());
                  //=== change dPhi range ===
                  if (dPhiMaxTrk < -1.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk + 2.*TMath::Pi();
                  if (dPhiMaxTrk > 5.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk - 2.*TMath::Pi();
                  //=== select back-to-back track ===
                  if (dPhiMaxTrk < TMath::Pi()-0.3 || dPhiMaxTrk > TMath::Pi()+0.3) MaxPtTrackNum_m = j;
                  //=== higher Pt track ===
                  if (anotrack->Pt() >= MaxTrk->Pt()) MaxPtTrackNum_m = j;
                  //=== Pt sum ===
                  if (anotrack->Pt() > 0.15) sumPt_OppTrks_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow70) sumPt_OppTrks70_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow80) sumPt_OppTrks80_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow85) sumPt_OppTrks85_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow90) sumPt_OppTrks90_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow93) sumPt_OppTrks93_m += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow95) sumPt_OppTrks95_m += anotrack->Pt();
                }
              }   //other track loop end
    
              //AliAODTrack* MaxPtTrk_m = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum_m));
              AliAODTrack* MaxPtTrk_m = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_m));
              Double_t leadPt_m = MaxPtTrk_m->Pt();
              if (leadPt_m > 0.15) fNtrkl_PtOfMaxTrk_H_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
              if (leadPt_m > fAssoHadPtLow70) fNtrkl_PtOfMaxTrk_H70_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
              if (leadPt_m > fAssoHadPtLow80) fNtrkl_PtOfMaxTrk_H80_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
              if (leadPt_m > fAssoHadPtLow85) fNtrkl_PtOfMaxTrk_H85_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
              if (leadPt_m > fAssoHadPtLow90) fNtrkl_PtOfMaxTrk_H90_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
              if (leadPt_m > fAssoHadPtLow93) fNtrkl_PtOfMaxTrk_H93_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
              if (leadPt_m > fAssoHadPtLow95) fNtrkl_PtOfMaxTrk_H95_m->Fill(corr_nAcc,PtCorr_had_ele*leadPt_m/track->Pt());
              if (sumPt_OppTrks_m > 0.) fNtrkl_PtOfTrks_H_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks_m/track->Pt());
              if (sumPt_OppTrks70_m > 0.) fNtrkl_PtOfTrks_H70_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks70_m/track->Pt());
              if (sumPt_OppTrks80_m > 0.) fNtrkl_PtOfTrks_H80_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks80_m/track->Pt());
              if (sumPt_OppTrks85_m > 0.) fNtrkl_PtOfTrks_H85_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks85_m/track->Pt());
              if (sumPt_OppTrks90_m > 0.) fNtrkl_PtOfTrks_H90_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks90_m/track->Pt());
              if (sumPt_OppTrks93_m > 0.) fNtrkl_PtOfTrks_H93_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks93_m/track->Pt());
              if (sumPt_OppTrks95_m > 0.) fNtrkl_PtOfTrks_H95_m->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks95_m/track->Pt());

              Double_t sumPt_OppTrks_w = 0;
              Double_t sumPt_OppTrks70_w = 0;
              Double_t sumPt_OppTrks80_w = 0;
              Double_t sumPt_OppTrks85_w = 0;
              Double_t sumPt_OppTrks90_w = 0;
              Double_t sumPt_OppTrks93_w = 0;
              Double_t sumPt_OppTrks95_w = 0;
//---------------another track loop start---------------
              Int_t MaxPtTrackNum_w = 0;
                for(Int_t j(0); j < iTracks; j++) {
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma < -1.5) {
                  MaxPtTrackNum_w = j;
                  break;
                }
              }
              for(Int_t j(0); j < iTracks; j++) {
                //AliAODTrack* anotrack = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
                AliAODTrack* anotrack = dynamic_cast<AliAODTrack*>(fTracks_tender->At(j));
                if(!anotrack || !anotrack->TestFilterBit(1)) continue;
                if (j == i) continue;     //reject e<-W candidate
    
                /////////////////////////
                //      track cut      //
                /////////////////////////
                //===== 1. TPC and ITS refit cut =====
                if (!(anotrack->GetStatus()&AliAODTrack::kITSrefit) || !(anotrack->GetStatus()&AliAODTrack::kTPCrefit)) continue;
                //===== 2. AOD filter bit required =====
                if (!anotrack->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) continue;
                //===== 3. TPC cluster cut =====
                if (anotrack->GetTPCNcls() < CutTPCCls) continue;
                //===== 4. ITS cluster cut =====
                if (anotrack->GetITSNcls() < CutITSCls) continue;
                //===== 5. SPD hit cut =====
                if (!(anotrack->HasPointOnITSLayer(0) || anotrack->HasPointOnITSLayer(1))) continue;
                //===== 6. Eta cut =====
                if (anotrack->Eta() > 0.9 || anotrack->Eta() < -0.9) continue;
                //===== 7. DCA cut =====
                Double_t anoDCA[2] = {-999.,-999.}, anocovar[3];
                if (anotrack -> PropagateToDCA(pVtx,fAOD -> GetMagneticField(),20.,anoDCA,anocovar))
                {
                  if (TMath::Abs(anoDCA[0]) > 2.4 || TMath::Abs(anoDCA[1]) > 3.2) continue;
                }
                //===== 8. chi2 cut =====
                Double_t anoITSchi2 = anotrack->GetITSchi2();
                Double_t anoTPCchi2NDF = anotrack->GetTPCchi2perNDF();
                if ((anoITSchi2 >= 25) || (anoTPCchi2NDF >= 4)) continue;
                //===== 9. NCrossedRow cut =====
                if (anotrack->GetTPCCrossedRows() < CutTPCCrossedRows) continue;
    
                //===== electron reject =====
                Double_t assoTPCnSigma = 999;
                assoTPCnSigma = fPIDResponse->NumberOfSigmasTPC(anotrack, AliPID::kElectron);
                if (assoTPCnSigma >= -1.5) continue;
    
                //Double_t anoeta = -log(TMath::Tan((anotrack->Theta())/2.));
                Double_t dPhi = anotrack->Phi() - track->Phi();
    
                //=== change dPhi range ===
                if (dPhi < -1.*TMath::Pi()/3.) dPhi = dPhi + 2.*TMath::Pi();
                if (dPhi > 5.*TMath::Pi()/3.) dPhi = dPhi - 2.*TMath::Pi();
    
                //===== Highest Pt track selection =====
                if (dPhi >= TMath::Pi()-0.5 && dPhi <= TMath::Pi()+0.5) {
                  //AliAODTrack* MaxTrk = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum_w));
                  AliAODTrack* MaxTrk = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_w));
                  Double_t dPhiMaxTrk = (MaxTrk->Phi()) - (track->Phi());
                  //=== change dPhi range ===
                  if (dPhiMaxTrk < -1.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk + 2.*TMath::Pi();
                  if (dPhiMaxTrk > 5.*TMath::Pi()/3.) dPhiMaxTrk = dPhiMaxTrk - 2.*TMath::Pi();
                  //=== select back-to-back track ===
                  if (dPhiMaxTrk < TMath::Pi()-0.5 || dPhiMaxTrk > TMath::Pi()+0.5) MaxPtTrackNum_w = j;
                  //=== higher Pt track ===
                  if (anotrack->Pt() >= MaxTrk->Pt()) MaxPtTrackNum_w = j;
                  //=== Pt sum ===
                  if (anotrack->Pt() > 0.15) sumPt_OppTrks_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow70) sumPt_OppTrks70_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow80) sumPt_OppTrks80_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow85) sumPt_OppTrks85_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow90) sumPt_OppTrks90_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow93) sumPt_OppTrks93_w += anotrack->Pt();
                  if (anotrack->Pt() > fAssoHadPtLow95) sumPt_OppTrks95_w += anotrack->Pt();
                }
              }   //other track loop end
    
              //===== Highest Pt Track / e<-W Pt Track =====
              //AliAODTrack* MaxPtTrk = static_cast<AliAODTrack*>(fAOD->GetTrack(MaxPtTrackNum));
              AliAODTrack* MaxPtTrk_w = dynamic_cast<AliAODTrack*>(fTracks_tender->At(MaxPtTrackNum_w));
              Double_t leadPt_w = MaxPtTrk_w->Pt();
    
              if (leadPt_w > 0.15) fNtrkl_PtOfMaxTrk_H_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
              if (leadPt_w > fAssoHadPtLow70) fNtrkl_PtOfMaxTrk_H70_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
              if (leadPt_w > fAssoHadPtLow80) fNtrkl_PtOfMaxTrk_H80_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
              if (leadPt_w > fAssoHadPtLow85) fNtrkl_PtOfMaxTrk_H85_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
              if (leadPt_w > fAssoHadPtLow90) fNtrkl_PtOfMaxTrk_H90_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
              if (leadPt_w > fAssoHadPtLow93) fNtrkl_PtOfMaxTrk_H93_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
              if (leadPt_w > fAssoHadPtLow95) fNtrkl_PtOfMaxTrk_H95_w->Fill(corr_nAcc,PtCorr_had_ele*leadPt_w/track->Pt());
              if (sumPt_OppTrks_w > 0.) fNtrkl_PtOfTrks_H_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks_w/track->Pt());
              if (sumPt_OppTrks70_w > 0.) fNtrkl_PtOfTrks_H70_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks70_w/track->Pt());
              if (sumPt_OppTrks80_w > 0.) fNtrkl_PtOfTrks_H80_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks80_w/track->Pt());
              if (sumPt_OppTrks85_w > 0.) fNtrkl_PtOfTrks_H85_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks85_w/track->Pt());
              if (sumPt_OppTrks90_w > 0.) fNtrkl_PtOfTrks_H90_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks90_w/track->Pt());
              if (sumPt_OppTrks93_w > 0.) fNtrkl_PtOfTrks_H93_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks93_w/track->Pt());
              if (sumPt_OppTrks95_w > 0.) fNtrkl_PtOfTrks_H95_w->Fill(corr_nAcc,PtCorr_had_ele*sumPt_OppTrks95_w/track->Pt());
            }		//Pt cut end
          }		//isolation cut end
        }		//E/p cut
      }			//hadron filter end
    }			//track match end
    if (HasEMCalInfo && clustLongE > 0.1 && clustLongE < 0.3 && EoverP > 0.9 && EoverP < 1.3) {
      TPCSigForE->Fill(track->P(),track->GetTPCsignal());
      fNsigmaPtForE->Fill(track->Pt(),fTPCnSigma);
    }
  }		//track loop end

  PostData(1, fOutputList);

}
//----------MC Mother's Particle Information----------
void AliAnalysisTaskWHMult::FindMother(AliAODMCParticle* part, int &label, int &pid, double &ptmom)
{
  if (part->GetMother() > -1) {
    label = part->GetMother();
    AliAODMCParticle *partM = (AliAODMCParticle*)fMCarray->At(label);
    //part = (AliAODMCParticle*)fMCarray->At(label);
    pid = partM->GetPdgCode();
    ptmom = partM->Pt();
  }
  //cout << "Find Mother : label = " << label << " ; pid" << pid << endl;
}
//----------MC Mother's W Information. Father is Origin Mather----------
void AliAnalysisTaskWHMult::FindFather(AliAODMCParticle* part, int &label, int &pid, double &ptdad)
{
  while (part->GetMother() > 0) {
    label = part->GetMother();
    //AliAODMCParticle *partM = (AliAODMCParticle*)fMCarray->At(label);
    part = (AliAODMCParticle*)fMCarray->At(label);
    pid = part->GetPdgCode();
    ptdad = part->Pt();
  }
  //cout << "Find Mother : label = " << label << " ; pid" << pid << endl;
}
//----------vertexZ correction for the number of tracklets----------
Double_t AliAnalysisTaskWHMult::GetCorrectedNtrackletsD(TTree* tree, Double_t uncorrectedNacc, Int_t BinOfvertexZ, AliAODEvent* fAOD)
{
  //if(BinOfvertexZ<1 || BinOfvertexZ > 100) return uncorrectedNacc;
  if(!tree) return uncorrectedNacc;

  Int_t TreeEntry = BinOfvertexZ - 1;
  Double_t corr_ZvNtl = 0.;
  Double_t corr_ZvNtl_data_a = 0.;
  Double_t corr_ZvNtl_data_b = 0.;
  Double_t corr_ZvNtl_data_c = 0.;
  Double_t corr_ZvNtl_MC_a = 0.;
  Double_t corr_ZvNtl_MC_b = 0.;
  Double_t corr_ZvNtl_MC_c = 0.;

  Int_t runNo = fAOD->GetRunNumber();

  if (!fMCarray) {
    //===== data 2016 =====
    if (runNo >= 256941 && runNo <= 264347) tree->SetBranchAddress("vertexZ_Ntrkl_data_a",&corr_ZvNtl_data_a);  //2016
    //===== data 2017 =====
    if (runNo >= 270581 && runNo <= 282704) tree->SetBranchAddress("vertexZ_Ntrkl_data_b",&corr_ZvNtl_data_b);  //2017
    //===== data 2018 =====
    if (runNo >= 285008 && runNo <= 294925) tree->SetBranchAddress("vertexZ_Ntrkl_data_c",&corr_ZvNtl_data_c);  //2018
  }
  if (fMCarray) {
    //===== MC 2016 =====
    if (runNo >= 256941 && runNo <= 264347) tree->SetBranchAddress("vertexZ_Ntrkl_MC_a",&corr_ZvNtl_MC_a);      //2016
    //===== MC 2017 =====
    if (runNo >= 270581 && runNo <= 282704) tree->SetBranchAddress("vertexZ_Ntrkl_MC_b",&corr_ZvNtl_MC_b);      //2017
    //===== MC 2018 =====
    if (runNo >= 285008 && runNo <= 294925) tree->SetBranchAddress("vertexZ_Ntrkl_MC_c",&corr_ZvNtl_MC_c);      //2018
  }

  tree->GetEntry(TreeEntry);

  if (!fMCarray) {
    //===== data 2016 =====
    if (runNo >= 256941 && runNo <= 264347) corr_ZvNtl = corr_ZvNtl_data_a;
    //===== data 2017 =====
    if (runNo >= 270581 && runNo <= 282704) corr_ZvNtl = corr_ZvNtl_data_b;
    //===== data 2018 =====
    if (runNo >= 285008 && runNo <= 294925) corr_ZvNtl = corr_ZvNtl_data_c;
  }
  if (fMCarray) {
    //===== MC 2016 =====
    if (runNo >= 256941 && runNo <= 264347) corr_ZvNtl = corr_ZvNtl_MC_a;
    //===== MC 2017 =====
    if (runNo >= 270581 && runNo <= 282704) corr_ZvNtl = corr_ZvNtl_MC_b;
    //===== MC 2018 =====
    if (runNo >= 285008 && runNo <= 294925) corr_ZvNtl = corr_ZvNtl_MC_c;
  }

  Double_t deltaM = 0;
  deltaM = uncorrectedNacc*(corr_ZvNtl - 1.);

  Double_t correctedNacc = uncorrectedNacc + (deltaM>0 ? 1 : -1) * gRandom->PoissonD(TMath::Abs(deltaM));
  if (correctedNacc<0) correctedNacc = 0;

  return correctedNacc;
}
//----------count number of charged particles generated MC----------
Int_t AliAnalysisTaskWHMult::CountNch()
{
  Int_t Nch = 0;
  for (int iMCpart = 0;iMCpart < fMCarray->GetEntriesFast();iMCpart++) {
    fMCparticle = (AliAODMCParticle*)fMCarray->At(iMCpart);		//call all generated particles in fMCarray of each event
    Int_t chargetrue = fMCparticle->Charge();				//get charge
    Double_t pdgEta = fMCparticle->Eta();				//acceptance
    Bool_t PrimaryParticle = fMCparticle->IsPhysicalPrimary();		//choose primary particle
    if(chargetrue!=0){
      if(TMath::Abs(pdgEta)<1.0){
        if(PrimaryParticle){
          Nch++;
        }
      }
    }
  }
  return Nch;
}

void AliAnalysisTaskWHMult::Terminate(Option_t *)
{
  // terminate
}
