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
#include <TSystem.h>

#include <vector>
#include <tuple>
#include <string>
#include <iostream>

TH1D* BuildSpectrumFromFile(const int& energyInkeV, const int& pdgCodeSelector = 2112) {
    auto file = TFile::Open(Form("Data/DeuteronData_With_%05i_keV_Deuterons.root", energyInkeV), "READ");
    auto tree = (TTree*) file->Get("DeuteronNTuple");
    TH1D* spectrum = new TH1D(Form("DeuteronSpectrum_With_%05i_keV", energyInkeV), ";Energy (MeV); Unnormalized Count (per 500 keV)", 60, 0, 30);

    // Loop over the tree and fill the spectrum
    double pdgCode;
    double energy;
    double weight;
    tree->SetBranchAddress("PDGCode", &pdgCode);
    tree->SetBranchAddress("Energy", &energy);
    tree->SetBranchAddress("Weight", &weight);
    for (int i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);
        if (pdgCode != pdgCodeSelector) continue;
        spectrum->Fill(energy, weight);
    }

    return spectrum;
}

TH1D* GetShieldedSpectrum(TF1* InputSpectrum, const int& pdgCode) {
    TString ShieldedParticleName = "";
    if (pdgCode == 2112) {
        ShieldedParticleName = "Neutron";
    } else if (pdgCode == 22) {
        ShieldedParticleName = "Gamma";
    } else {
        std::cout << "Invalid PDG code: " << pdgCode << std::endl;
        return nullptr;
    }

    std::vector<std::tuple<TH1D*, double>> RawSpectra;
    double Normalization = InputSpectrum->Integral(0, 30);

    for (int i = 150; i < 5000; i += 100) {
        TH1D* spectrum = BuildSpectrumFromFile(i, pdgCode);
        double scale_output = InputSpectrum->Integral((double) ((i - 50)/1000), (double) ((i + 50)/1000)) / Normalization;
        RawSpectra.emplace_back(spectrum, scale_output);
    }

    for (int i = 5100; i < 10000; i += 200) {
        TH1D* spectrum = BuildSpectrumFromFile(i, pdgCode);
        double scale_output = InputSpectrum->Integral((double) ((i - 100)/1000), (double) ((i + 100)/1000)) / Normalization;
        RawSpectra.emplace_back(spectrum, scale_output);
    }

    for (int i = 10500; i < 30000; i += 1000) {
        TH1D* spectrum = BuildSpectrumFromFile(i, pdgCode);
        double scale_output = InputSpectrum->Integral((double) ((i - 500)/1000), (double) ((i + 500)/1000)) / Normalization;
        RawSpectra.emplace_back(spectrum, scale_output);
    }

    TH1D* DoubleWeightedSpectrum = new TH1D(Form("%sSpectrum", ShieldedParticleName.Data()), ";Energy (MeV); Normalized Count (per MeV)", 60, 0, 30);
    for (auto& spectrum : RawSpectra) {
        auto hist = std::get<0>(spectrum);
        double scale = std::get<1>(spectrum);
        DoubleWeightedSpectrum->Add(hist, scale);
    }

    std::cout << pdgCode << " : " << DoubleWeightedSpectrum->Integral() << std::endl;
    return DoubleWeightedSpectrum;
}

void Plotting(TH1D* ShieldedNeutrons, TH1D* ShieldedGammas) {
    gStyle->SetOptStat(0);

    auto c = new TCanvas( "c", "c", 900, 900);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);

    ShieldedNeutrons->SetLineColor(kRed);
    ShieldedNeutrons->SetLineWidth(3);
    ShieldedGammas->SetLineColor(kBlue);
    ShieldedGammas->SetLineWidth(3);

    ShieldedGammas->GetXaxis()->CenterTitle();
    ShieldedGammas->GetYaxis()->CenterTitle();
    ShieldedGammas->SetLabelOffset(0.01, "XY");
    ShieldedGammas->SetLabelSize(0.04, "XY");
    ShieldedGammas->SetTitleOffset(1.25, "X");
    ShieldedGammas->SetTitleOffset(1.50, "Y");
    ShieldedGammas->SetTitleSize(0.04, "XY");
    ShieldedGammas->SetNdivisions(505, "XY");
    ShieldedGammas->GetYaxis()->SetRangeUser(0, 500);
    ShieldedGammas->GetXaxis()->SetMaxDigits(2);
    ShieldedGammas->GetYaxis()->SetMaxDigits(2);

    ShieldedGammas->Draw("HIST");
    ShieldedNeutrons->Draw("HIST SAME");

    auto legend = new TLegend(0.65, 0.7, 0.95, 0.9);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.04);
    legend->AddEntry(ShieldedGammas, "Gamma", "l");
    legend->AddEntry(ShieldedNeutrons, "Neutron", "l");
    legend->Draw();

    c->SaveAs(Form("ShieldedSpectrum.pdf"));
    c->SaveAs(Form("ShieldedSpectrum.png"));

    ShieldedGammas->GetYaxis()->SetRangeUser(1E-4, 1000);
    c->SetLogy();
    c->SaveAs(Form("ShieldedSpectrumLogy.pdf"));
    c->SaveAs(Form("ShieldedSpectrumLogy.png"));
}

int main(int argc, char** argv) {
    TF1* Spectrum = nullptr;
    std::string SpectrumName = "";
    if (argv[1] == TString("Exponential")) {
        SpectrumName = "ShieldedSpectra_Exponential.root";
        Spectrum = new TF1("ExponentialSpectrum", "0.23 * exp(-0.23 * x)", 0, 30);
    } else if (argv[1] == TString("Maxwellian")) {
        SpectrumName = "ShieldedSpectra_Maxwellian.root";
        Spectrum = new TF1("MaxwellianSpectrum", "TMath::Sqrt(2./TMath::Pi()) * (1./([0]*[0]*[0])) * x * x * exp((-1./([0]*[0])) * x * x)", 0, 30);
        Spectrum->SetParameter(0, 2.0);
    } else {
        std::cout << "Usage: " << argv[0] << " <Exponential/Maxwellian>" << std::endl;
        return 0;
    }

    TH1D* ShieldedNeutrons = nullptr;
    TH1D* ShieldedGammas = nullptr;
    if (gSystem->AccessPathName(SpectrumName.c_str())) {
        ShieldedNeutrons = GetShieldedSpectrum(Spectrum, 2112);
        ShieldedGammas = GetShieldedSpectrum(Spectrum, 22);
        if (ShieldedNeutrons == nullptr || ShieldedGammas == nullptr) {
            std::cout << "Error: Shielded spectrum is null" << std::endl;
            return 0;
        }
        TFile *outfile = new TFile(SpectrumName.c_str(), "RECREATE");
        outfile->cd();
        ShieldedNeutrons->Write();
        ShieldedGammas->Write();
        outfile->Write();
        outfile->Close();
    } else {
        TFile *infile = new TFile(SpectrumName.c_str(), "READ");
        infile->cd();
        ShieldedNeutrons = (TH1D*) infile->Get("NeutronSpectrum");
        ShieldedGammas = (TH1D*) infile->Get("GammaSpectrum");
    }

    Plotting(ShieldedNeutrons, ShieldedGammas);

    return 0;
}

