void Proceed(const TString input_str = "Analys_class(\"input_path\",\"sig\")"){

  cout << "READING INPUT FILES !" << endl;
  //gROOT -> ProcessLine( "gErrorIgnoreLevel = 1001;")
  gROOT -> ProcessLine(".L MyClass.C");
  gROOT -> ProcessLine(".L Analys_class.C");
  gROOT -> ProcessLine(input_str);
}
