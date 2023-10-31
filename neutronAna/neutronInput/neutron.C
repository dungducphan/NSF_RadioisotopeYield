void neutron() {
//    auto file = TFile::Open(Form("Neutron.root"), "READ");
//    auto tree = (TTree*) file->Get("NeutronNTuple");
//
//
//    TH1D* ShieldedNeutrons = new TH1D(Form("NeutronSpectrum"), ";Energy (MeV); Unnormalized Count (per 100 keV)", 50, 0, 5);
//    TH1D* ShieldedGammas = new TH1D(Form("GammaSpectrum"), ";Energy (MeV); Unnormalized Count (per 100 keV)", 50, 0, 5);
//
//    double pdgCode;
//    double energy;
//    double weight;
//    tree->SetBranchAddress("PDGCode", &pdgCode);
//    tree->SetBranchAddress("Energy", &energy);
//    tree->SetBranchAddress("Weight", &weight);
//    for (int i = 0; i < tree->GetEntries(); ++i) {
//        tree->GetEntry(i);
//        if (pdgCode == 2112) {
//            ShieldedNeutrons->Fill(energy, weight);
//        } else if (pdgCode == 22) {
//            ShieldedGammas->Fill(energy, weight);
//        } else {
//            continue;
//        }
//    }

    TFile* file = TFile::Open(Form("Out.root"), "READ");
    TH1D* ShieldedNeutrons = (TH1D*) file->Get("NeutronSpectrum");
    TH1D* ShieldedGammas = (TH1D*) file->Get("GammaSpectrum");

    gStyle->SetOptStat(0);

    auto c = new TCanvas( "c", "c", 900, 900);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);

    ShieldedNeutrons->Scale(1. / 8E6);
    ShieldedNeutrons->GetYaxis()->SetTitle("Counts (per 100 keV)");
    ShieldedGammas->Scale(1. / 8E6);
    ShieldedGammas->GetYaxis()->SetTitle("Counts (per 100 keV)");

    ShieldedNeutrons->SetLineColor(kRed);
    ShieldedNeutrons->SetLineWidth(3);
    ShieldedGammas->SetLineColor(kBlue);
    ShieldedGammas->SetLineWidth(3);

    ShieldedNeutrons->GetXaxis()->CenterTitle();
    ShieldedNeutrons->GetYaxis()->CenterTitle();
    ShieldedNeutrons->SetLabelOffset(0.01, "XY");
    ShieldedNeutrons->SetLabelSize(0.04, "XY");
    ShieldedNeutrons->SetTitleOffset(1.25, "X");
    ShieldedNeutrons->SetTitleOffset(1.50, "Y");
    ShieldedNeutrons->SetTitleSize(0.04, "XY");
    ShieldedNeutrons->SetNdivisions(505, "XY");
    ShieldedNeutrons->GetYaxis()->SetRangeUser(0, 0.4);
    ShieldedNeutrons->GetXaxis()->SetMaxDigits(2);
    ShieldedNeutrons->GetYaxis()->SetMaxDigits(3);

    ShieldedNeutrons->Draw("HIST");
    ShieldedGammas->Draw("HIST SAME");

    auto legend = new TLegend(0.65, 0.7, 0.95, 0.9);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.04);
    legend->AddEntry(ShieldedGammas, "Gamma", "l");
    legend->AddEntry(ShieldedNeutrons, "Neutron", "l");
    legend->Draw();

    c->SaveAs(Form("ShieldedSpectrum.pdf"));
    c->SaveAs(Form("ShieldedSpectrum.png"));

    ShieldedNeutrons->GetYaxis()->SetRangeUser(1E-7, 1);
    c->SetLogy();
    c->SaveAs(Form("ShieldedSpectrumLogy.pdf"));
    c->SaveAs(Form("ShieldedSpectrumLogy.png"));

    std::cout << ShieldedNeutrons->Integral() << std::endl;
    std::cout << ShieldedGammas->Integral() << std::endl;
}
