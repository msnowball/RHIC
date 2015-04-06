#ifndef PHG3toG4SvxHit_h
#define PHG3toG4SvxHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// Drift chamber hit
///
/// It records:
/// - the particle ID
/// - the track ID
/// - the layer ID
/// - the station ID
/// - the detector ID
/// - the particle time
/// - the particle energy loss
/// - the particle local and global in/out/avg positions
/// - the particle momentum vector



class PHG3toG4SvxHit : public G4VHit
{
public:
    PHG3toG4SvxHit();
    PHG3toG4SvxHit(const PHG3toG4SvxHit &right);
    virtual ~PHG3toG4SvxHit();

    const PHG3toG4SvxHit& operator=(const PHG3toG4SvxHit &right);
    const PHG3toG4SvxHit& operator+=(const PHG3toG4SvxHit &right);
    int operator==(const PHG3toG4SvxHit &right) const;

    G4bool isTheSameHit(PHG3toG4SvxHit *right);
    
    void *operator new(size_t);
    void operator delete(void *aHit);
    
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    void SetParticleID(G4int id) { fParticleID = id; }
    G4int GetParticleID() const { return fParticleID; }

    void SetTrackID(G4int id) { fTrackID = id; }
    G4int GetTrackID() const { return fTrackID; }

    void SetLayerID(G4int z) { fLayerID = z; }
    G4int GetLayerID() const { return fLayerID; }

    void SetStationID(G4int sta) { fStationID = sta; }
    G4int GetStationID() const { return fStationID; }

    void SetDetID(G4int d) { fDetID = d; }
    G4int GetDetID() const { return fDetID; }

    void SetArmID(G4int a) { fArmID = a; }
    G4int GetArmID() const { return fArmID; }

    void SetVolumeName(G4String s) { fVolumeName = s; }
    G4String GetVolumeName() const { return fVolumeName; }

    void SetMotherVolumeName(G4String s) { fMotherVolumeName = s; }
    G4String GetMotherVolumeName() const { return fMotherVolumeName; }

    void SetTimeIn(G4double t) { fTimeIn = t; }
    G4double GetTimeIn() const { return fTimeIn; }

    void SetTimeOut(G4double t) { fTimeOut = t; }
    G4double GetTimeOut() const { return fTimeOut; }

    void SetdE(G4double e) { fdE = e; }
    G4double GetdE() const { return fdE; }

    void SetLocalPosIn(G4ThreeVector xyz) { fLocalPosIn = xyz; }
    G4ThreeVector GetLocalPosIn() const { return fLocalPosIn; }

    void SetLocalPosOut(G4ThreeVector xyz) { fLocalPosOut = xyz; }
    G4ThreeVector GetLocalPosOut() const { return fLocalPosOut; }

    void SetWorldPosIn(G4ThreeVector xyz) { fWorldPosIn = xyz; }
    G4ThreeVector GetWorldPosIn() const { return fWorldPosIn; }

    void SetWorldPosOut(G4ThreeVector xyz) { fWorldPosOut = xyz; }
    G4ThreeVector GetWorldPosOut() const { return fWorldPosOut; }

    void SetWorldPosAvg(G4ThreeVector xyz) { fWorldPosAvg = xyz; }
    G4ThreeVector GetWorldPosAvg() const { return fWorldPosAvg; }

    void SetMomentumIn(G4ThreeVector p) { fMomentumIn = p; }
    G4ThreeVector GetMomentumIn() const { return fMomentumIn; }

    void SetMomentumOut(G4ThreeVector p) { fMomentumOut = p; }
    G4ThreeVector GetMomentumOut() const { return fMomentumOut; }

    void SetIsFirstHit(G4bool h) { fIsFirstHit = h; }
    G4bool IsFirstHit() const { return fIsFirstHit; }

    void SetIsLastHit(G4bool h) { fIsLastHit = h; }
    G4bool IsLastHit() const { return fIsLastHit; }

    void SetHitVolume(G4int i, G4int hv) { fHitVolume[i] = hv; }
    G4int GetHitVolume(G4int i) const  { return fHitVolume[i]; }
    

    
private:
    G4int fParticleID;
    G4int fTrackID;
    G4int fLayerID;
    G4int fStationID;
    G4int fDetID;
    G4int fArmID;
    G4String fVolumeName;
    G4String fMotherVolumeName;
    G4double fTimeIn;
    G4double fTimeOut;
    G4double fdE;
    G4bool fIsFirstHit;
    G4bool fIsLastHit;
    G4ThreeVector fLocalPosIn;
    G4ThreeVector fLocalPosOut;
    G4ThreeVector fWorldPosIn;
    G4ThreeVector fWorldPosOut;
    G4ThreeVector fWorldPosAvg;
    G4ThreeVector fMomentumIn;
    G4ThreeVector fMomentumOut;
    
    G4int fHitVolume[9];


};

typedef G4THitsCollection<PHG3toG4SvxHit> PHG3toG4SvxHitsCollection;

extern G4ThreadLocal G4Allocator<PHG3toG4SvxHit>* PHG3toG4SvxHitAllocator;


#endif
