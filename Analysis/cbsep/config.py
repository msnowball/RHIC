#!/usr/bin/python




kTreeName = "T"
kNormalize = True
#kTreeFiles = ["../mc_data_production/pdst_bbbar_noEmbed.root","../mc_data_production/pdst_ccbar_noEmbed.root","../mc_data_production/data.txt"]
kTreeFiles = ["../mc_data_production/pdst_bbbar_sigOnly.root","../mc_data_production/pdst_ccbar_sigOnly.root", "../mc_data_production/test.root" ]
#kTreeFiles = ["/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/RECO/Step3_Embedding/test/output/test_0_pdst.root"]
#kTreeFiles = ["../mc_data_production/pdst_bbbar_noEmbed.root","../mc_data_production/pdst_ccbar_noEmbed.root","../mc_data_production/pdst_bbbar_Embed.root","../mc_data_production/pdst_ccbar_Embed.root"]
#kSampleNames = ["B", "D","B_embed","D_embed"]
kSampleNames = ["B","D","B_{emb}","C_{emb}"]
kSampleSigmas = [1, 1, 1, 1]
kSampleTypes = ["MC", "MC", "MC", "MC"]
kSampleColors = [4,3,6,7]
kSampleWidths = [2,2,2,2]
kNpTbins = 9
kpTMin = 0.75
kpTMax = 5.25
kNpbins = 40
kpMin = 0
kpMax = 20
kNDCAbins = 100
kDCAMin = -.5
kDCAMax = 0.5
kNDCAResbins = 100
kDCAResMin = -1.0
kDCAResMax = 1.0

kLumi = 1

kpTVarName = "sqrt(SingleMuons.px*SingleMuons.px+SingleMuons.py*SingleMuons.py)"
kpVarName = "sqrt(SingleMuons.px*SingleMuons.px+SingleMuons.py*SingleMuons.py+SingleMuons.pz*SingleMuons.pz)"
kpzVarName = "SingleMuons.pz"
kDrawOpt = "HIST"

kLegX1 = 0.8
kLegX2 = 0.95
kLegY1 = 0.75
kLegY2 = 0.92


