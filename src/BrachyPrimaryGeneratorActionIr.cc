//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// --------------------------------------------------------------
//                 GEANT 4 - Brachytherapy example
// --------------------------------------------------------------
//
// Code developed by:
// S. Agostinelli, F. Foppiano, S. Garelli , M. Tropeano, S.Guatelli
//
//    ********************************************
//    *                                          *
//    *    BrachyPrimaryGeneratorActionIr.cc     *
//    *                                          *
//    ********************************************
//
// $Id$
//
#include "BrachyPrimaryGeneratorActionIr.hh"
#include "globals.hh"
#include "Randomize.hh"  
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"

BrachyPrimaryGeneratorActionIr::BrachyPrimaryGeneratorActionIr()
{
  G4int NumParticles = 1;
  particleGun = new G4ParticleGun(NumParticles); 
}

BrachyPrimaryGeneratorActionIr::~BrachyPrimaryGeneratorActionIr()
{
  if(particleGun)
    delete particleGun;
}

void BrachyPrimaryGeneratorActionIr::GeneratePrimaries(G4Event* anEvent)
{

  // Define primary particle type
  G4ParticleTable* pParticleTable = G4ParticleTable::GetParticleTable();
  G4String ParticleName = "gamma";
  G4ParticleDefinition* pParticle = pParticleTable -> FindParticle(ParticleName);
  particleGun -> SetParticleDefinition(pParticle);
 
  //  Random generation of gamma source point inside the Iridium core
  G4double x,y,z;
  G4double radius = 0.30*mm;
  do{
    x = (G4UniformRand()-0.5)*(radius)/0.5;
    y = (G4UniformRand()-0.5)*(radius)/0.5;
  }while(x*x+y*y > radius*radius);
 
  z = (G4UniformRand()-0.5)*1.75*mm/0.5 -1.975*mm  ;

  G4ThreeVector position(x,y,z);
  particleGun -> SetParticlePosition(position);

  // Random generation of the impulse direction
  G4double a,b,c;
  G4double n;
  do{
    a = (G4UniformRand()-0.5)/0.5;
    b = (G4UniformRand()-0.5)/0.5; 
    c = (G4UniformRand()-0.5)/0.5;
    n = a*a+b*b+c*c;
  }while(n > 1 || n == 0.0);
  n = std::sqrt(n);
  a /= n;
  b /= n;
  c /= n;

  G4ThreeVector direction(a,b,c);
  particleGun->SetParticleMomentumDirection(direction);

  // Primary particle energy
  primaryParticleEnergy = 356.*keV;//a kind of weighted mean between 316kev (83%) and 478keV (48%) and 2 other!..see nuclideExposure
  //ALIAS SAMPLE!!!in the future!!
  //we are interested in gamas because they can travel long distances inside body!!
  //beta have low energies and most of them are absorbed in capsule wall!
  particleGun->SetParticleEnergy(primaryParticleEnergy);
 

  // Generate a primary particle
  particleGun -> GeneratePrimaryVertex(anEvent);
}

