#ifndef DATABLOCKCONTROLLER_H
#define DATABLOCKCONTROLLER_H

#include "Utils.h"
#include "DataManager.h"
#include "FeatureTracker.h"

class BlockController {

public:
    BlockController();
    ~BlockController();

    void InitParameters(const Metadata &meta);
    void TrackForward(const Metadata &meta);
    void ExtractAllFeatures();
    void SetCurrentTimestep(int t) { currentTimestep = t; }

    void exportFeature(int index){
        fstream fout;
        string f_name = "feature_" + index + ".dat";
        fout.open( f_name , fstream::out | fstream::binary );
        Vector3i dim = pDataManager->GetBlockDimension();
        unsigned char* feature_volume = new unsigned char[dim.x * dim.y * dim.z];

        vector<Feature>* pFeature = pFeatureTracker->GetFeatureVectorPointer(index);
        for( size_t i = 0 ; i != (*pFeature).size() ; i++ ){
            list<Vector3i> voxels = (*pFeature)[i].InnerPoints;
            int value = (*pFeature)[i].ID;
            for( size_t v = 0 ; v != voxels.size() ; v++ )
                feature_volume[v] = value;
        }
        fout.write( (char*)feature_volume , dim.x * dim.y * dim.z );
        fout.close();
        delete [] feature_volume;
    }
private:
    DataManager             *pDataManager;
    FeatureTracker          *pFeatureTracker;
    FeatureVectorSequence    featureSequence;
    int                      currentTimestep;
};

#endif // DATABLOCKCONTROLLER_H
