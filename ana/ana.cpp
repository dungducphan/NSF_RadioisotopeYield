#include <TH1.h>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TMath.h>
#include <TGraph.h>
#include <TString.h>

#include <vector>
#include <tuple>
#include <string>
#include <iostream>

TH1D* BuildSpectrumFromFile(const int& energyInMeV) {
    auto file = TFile::Open(Form("DeuteronData_With_%02i_MeV_Deuterons.root", energyInMeV), "READ");
    auto tree = (TTree*) file->Get("DeuteronNTuple");
    TH1D* spectrum = new TH1D(Form("DeuteronSpectrum_With_%02i_MeV", energyInMeV), ";Energy (MeV); Unnormalized Count (per MeV)", 50, 0, 50);

    // Loop over the tree and fill the spectrum
    double energy;
    double weight;
    tree->SetBranchAddress("Energy", &energy);
    tree->SetBranchAddress("Weight", &weight);
    for (int i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);
        spectrum->Fill(energy, weight);
    }

    return spectrum;
}

TH1D* BuildSpectrumFromFileWithoutWeights(const int& energyInMeV) {
    auto file = TFile::Open(Form("DeuteronData_With_%02i_MeV_Deuterons.root", energyInMeV), "READ");
    auto tree = (TTree*) file->Get("DeuteronNTuple");
    TH1D* spectrum = new TH1D(Form("DeuteronSpectrum_With_%02i_MeV", energyInMeV), ";Energy (MeV); Unnormalized Count (per MeV)", 50, 0, 50);

    // Loop over the tree and fill the spectrum
    double energy;
    tree->SetBranchAddress("Energy", &energy);
    for (int i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);
        spectrum->Fill(energy);
    }

    return spectrum;
}

int main(int argc, char** argv) {
    TF1* Spectrum = nullptr;
    TString OutputName = "";
    if (argv[1] == TString("Exponential")) {
        OutputName = "DoubleWeightedSpectrum_Exponential";
        Spectrum = new TF1("ExponentialSpectrum", "0.23 * exp(-0.23 * x)", 0, 50);
    } else if (argv[1] == TString("Maxwellian")) {
        OutputName = "DoubleWeightedSpectrum_Maxwellian";
        Spectrum = new TF1("MaxwellianSpectrum", "TMath::Sqrt(2./TMath::Pi()) * (1./([0]*[0]*[0])) * x * x * exp((-1./([0]*[0])) * x * x)", 0, 50);
        Spectrum->SetParameter(0, 3.0);
    } else {
        std::cout << "Usage: " << argv[0] << " <Exponential/Maxwellian>" << std::endl;
        return 0;
    }

    std::vector<std::tuple<TH1D*, double, double>> RawSpectra;
    for (int i = 6; i < 51; i++) {
        TH1D* spectrum = nullptr;
        double scale_input = 0;
        if (i < 12) {
            spectrum = BuildSpectrumFromFile(i);
            scale_input = 1. / 100.;
        }  else {
            spectrum = BuildSpectrumFromFileWithoutWeights(i);
            scale_input = 1.;
        }
        double scale_output = Spectrum->Integral(-0.5 + (double) i, +0.5 + (double) i);
        RawSpectra.emplace_back(spectrum, scale_input, scale_output);
    }

    TH1D* DoubleWeightedSpectrum = new TH1D(Form("DoubleWeightedSpectrum"), ";Energy (MeV); Normalized Count (per MeV)", 50, 0, 50);
    for (auto& spectrum : RawSpectra) {
        auto hist = std::get<0>(spectrum);
        double scale_input = std::get<1>(spectrum);
        double scale_output = std::get<2>(spectrum);
        hist->Scale(scale_input);
        DoubleWeightedSpectrum->Add(hist, scale_output);
    }

    auto c = new TCanvas("c", "c", 900, 900);
    DoubleWeightedSpectrum->SetLineColor(kRed);
    DoubleWeightedSpectrum->SetLineWidth(2);
    DoubleWeightedSpectrum->Draw("hist");
    c->SaveAs(Form("%s.pdf", OutputName.Data()));
    c->SaveAs(Form("%s.png", OutputName.Data()));

    return 0;
}

