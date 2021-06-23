{
    //import file
    TFile *com1 = TFile::Open("highest_pt_and_mult_no_cut.root");
    TFile *com2 = TFile::Open("highest_pt_and_mult_5.root");
    TFile *com3 = TFile::Open("highest_pt_and_mult_10.root");
    TFile *com4 = TFile::Open("highest_pt_and_mult_15.root");
    TFile *com5 = TFile::Open("highest_pt_and_mult_20.root");
    TFile *com6 = TFile::Open("highest_pt_and_mult_25.root");

    //use the data from "com" and graph one 1D graph labled "multiplicity_default"
    TH1D *highest_pt_no_cut = (TH1D *)com1->Get("highest_pt_no_cut");
    TH1D *highest_pt_5 = (TH1D *)com2->Get("highest_pt_5");
    TH1D *highest_pt_10 = (TH1D *)com3->Get("highest_pt_10");
    TH1D *highest_pt_15 = (TH1D *)com4->Get("highest_pt_15");
    TH1D *highest_pt_20 = (TH1D *)com5->Get("highest_pt_20");
    TH1D *highest_pt_25 = (TH1D *)com6->Get("highest_pt_25");

    //create a canvas
    TCanvas *p = new TCanvas("p", "Highest jet pT comparison", 600, 500);
    // remove all other information from the histogram
    gStyle->SetOptStat(0);
    //set ticks for both x and y SetAxis
    gPad->SetTickx();
    gPad->SetTicky();

    //set the marker style as well as the color for the graph
    //highest_pt_no_cut->SetLineColor(kRed);
    highest_pt_no_cut->SetLineStyle(1);
    highest_pt_no_cut->SetLineWidth(3);
    //Label the x and y axis
    highest_pt_no_cut->SetXTitle("Highest jet pT per event");
    highest_pt_no_cut->SetYTitle("Jet Count");
    //set the title of the histogram as blank
    highest_pt_no_cut->SetTitle(" ");
    //draw the histogram
    highest_pt_no_cut->Draw("");

    // draw the second histogram
    //multiplicity_all_off->SetLineColor(kBlue);
    highest_pt_5->SetLineStyle(2);
    highest_pt_5->SetLineWidth(3);
    highest_pt_5->Draw("SAME");

    // draw the third histogram
    highest_pt_10->SetLineColor(kGreen);
    highest_pt_10->SetLineStyle(1);
    highest_pt_10->SetLineWidth(3);
    highest_pt_10->Draw("SAME");

    // draw the fourth histogram
    highest_pt_15->SetLineColor(kRed);
    highest_pt_15->SetLineStyle(1);
    highest_pt_15->SetLineWidth(3);
    highest_pt_15->Draw("SAME");

    // draw the fifth histogram
    //multiplicity_Had_off->SetLineColor(7);
    highest_pt_20->SetLineStyle(10);
    highest_pt_20->SetLineWidth(3);
    highest_pt_20->Draw("SAME");

    // draw the sixth histogram
    highest_pt_25->SetLineColor(9);
    highest_pt_25->SetLineStyle(1);
    highest_pt_25->SetLineWidth(3);
    highest_pt_25->Draw("SAME");

    //create a legend for the histogram
    TLegend *leg = new TLegend(0.5, 0.6, 0.8, 0.8);
    leg->AddEntry(highest_pt_no_cut, "no cut", "pl");
    leg->AddEntry(highest_pt_5, "pT Hat Min cut of 5 GeV", "pl");
    leg->AddEntry(highest_pt_10, "pT Hat Min cut of 10 GeV", "pl");
    leg->AddEntry(highest_pt_15, "pT Hat Min cut of 15 GeV", "pl");
    leg->AddEntry(highest_pt_20, "pT Hat Min cut of 20 GeV", "pl");
    leg->AddEntry(highest_pt_25, "pT Hat Min cut of 25 GeV", "pl");

    leg->SetTextSize(0.03);
    leg->Draw();

    //put a text on the graph
    TLatex *tex = new TLatex(0.25, 0.95, "Highest jet pT with different pT Hat Min Cuts");
    tex->SetTextSize(0.04);
    tex->SetLineWidth(2);
    tex->SetNDC();
    tex->Draw();
}