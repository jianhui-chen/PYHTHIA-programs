{
    //import file
    TFile *com1 = TFile::Open("multiplicity_default.root");
    TFile *com2 = TFile::Open("multiplicity_all_off.root");
    TFile *com3 = TFile::Open("multiplicity_MPI_off.root");
    TFile *com4 = TFile::Open("multiplicity_ISR_off.root");
    TFile *com5 = TFile::Open("multiplicity_Had_off.root");

    //use the data from "com" and graph one 1D graph labled "multiplicity_default"
    TH1D *multiplicity_default = (TH1D *)com1->Get("multiplicity_default");
    TH1D *multiplicity_all_off = (TH1D *)com2->Get("multiplicity_all_off");
    TH1D *multiplicity_MPI_off = (TH1D *)com3->Get("multiplicity_MPI_off");
    TH1D *multiplicity_ISR_off = (TH1D *)com4->Get("multiplicity_ISR_off");
    TH1D *multiplicity_Had_off = (TH1D *)com5->Get("multiplicity_Had_off");

    //create a canvas
    TCanvas *p = new TCanvas("p", "Multiplicity Comparison", 600, 500);
    // remove all other information from the histogram
    gStyle->SetOptStat(0);
    //set ticks for both x and y SetAxis
    gPad->SetTickx();
    gPad->SetTicky();

    //set the marker style as well as the color for the graph
    multiplicity_default->SetLineColor(kRed);
    multiplicity_default->SetLineStyle(1);
    multiplicity_default->SetLineWidth(3);
    //Label the x and y axis
    multiplicity_default->SetXTitle("Event Multiplicity");
    multiplicity_default->SetYTitle("Event Count");
    //set the title of the histogram as blank
    multiplicity_default->SetTitle(" ");
    //draw the histogram
    multiplicity_default->Draw("");

    // draw the second histogram
    multiplicity_all_off->SetLineColor(kBlue);
    multiplicity_all_off->SetLineStyle(1);
    multiplicity_all_off->SetLineWidth(3);
    multiplicity_all_off->Draw("SAME");

    // draw the third histogram
    multiplicity_MPI_off->SetLineColor(kGreen);
    multiplicity_MPI_off->SetLineStyle(1);
    multiplicity_MPI_off->SetLineWidth(3);
    multiplicity_MPI_off->Draw("SAME");

    // draw the fourth histogram
    //multiplicity_ISR_off->SetLineColor(6);
    multiplicity_ISR_off->SetLineStyle(4);
    multiplicity_ISR_off->SetLineWidth(3);
    multiplicity_ISR_off->Draw("SAME");

    // draw the fourth histogram
    multiplicity_Had_off->SetLineColor(7);
    multiplicity_Had_off->SetLineStyle(1);
    multiplicity_Had_off->SetLineWidth(3);
    multiplicity_Had_off->Draw("SAME");

    //create a legend for the histogram
    TLegend *leg = new TLegend(0.5, 0.6, 0.8, 0.8);
    leg->AddEntry(multiplicity_default, "default setting", "pl");
    leg->AddEntry(multiplicity_all_off, "all settings off", "pl");
    leg->AddEntry(multiplicity_MPI_off, "MPI off", "pl");
    leg->AddEntry(multiplicity_ISR_off, "ISR off", "pl");
    leg->AddEntry(multiplicity_Had_off, "Hadronization off", "pl");

    leg->SetTextSize(0.04);
    leg->Draw();

    //put a text on the graph
    TLatex *tex = new TLatex(0.15, 0.95, "Event multiplicity with different generator settings");
    tex->SetTextSize(0.04);
    tex->SetLineWidth(2);
    tex->SetNDC();
    tex->Draw();
}