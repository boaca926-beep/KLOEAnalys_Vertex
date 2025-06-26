void Proceed(const TString input_str = "Analys_class(\"../path_small/sig_tmp\",\"../../sig\")"){

  cout << "READING INPUT FILES !" << endl;
  //gROOT -> ProcessLine( "gErrorIgnoreLevel = 1001;")
  gROOT -> ProcessLine(".L ../run/MyClass.C");
  gROOT -> ProcessLine(".L ../run/Analys_class.C");
  gROOT -> ProcessLine(input_str);
}
