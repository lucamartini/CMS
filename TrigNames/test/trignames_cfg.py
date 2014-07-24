# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       #'file:/gpfs/gpfsddn/cms/user/martini/data/mc/winter10/reco/JPsiToMuMu.root'
       #'/store/user/vesentin/X3872_NoRho_Direct_Feb2011/Xdirect_NORho-Onia2MuMu-v8NewRECO/9ca182053f2fc5a2331d0f4f823cb8c2/onia2MuMuPAT_40_2_oWU.root',
                                      '/store/data/Run2011A/MuOnia/AOD/05Aug2011-v1/0000/D2A14695-FDC0-E011-B0F7-00248C0BE01E.root',
    )
)

process.demo = cms.EDAnalyzer('TrigNames',

    triggerEventLabel    = cms.InputTag("hltTriggerSummaryAOD::HLT"),
    HLTriggerResults     = cms.InputTag("TriggerResults","","HLT")

)


process.p = cms.Path(process.demo)

