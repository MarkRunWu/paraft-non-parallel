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
        char file_hdr[20],file_dat[20];
        sprintf( file_hdr , "feature%d.hdr" , index );
        sprintf( file_dat , "feature%d.dat" , index );



        fout.open( file_dat , fstream::out | fstream::binary );

        Vector3i dim = pDataManager->GetBlockDimension();

        FILE* check_hdr = fopen( file_hdr , "w" );
        fprintf( check_hdr , "%d %d %d BYTE" , dim.x , dim.y , dim.z );
        fclose( check_hdr );

        unsigned char* feature_volume = new unsigned char[dim.x * dim.y * dim.z];
        memset( feature_volume , 0 , dim.x*dim.y*dim.z );
        vector<Feature>* pFeature = pFeatureTracker->GetFeatureVectorPointer(index);
        for( size_t i = 0 ; i != (*pFeature).size() ; i++ ){
            list<Vector3i> voxels = (*pFeature)[i].InnerPoints;

            int value = (*pFeature)[i].ID;
            for( list<Vector3i>::iterator it = voxels.begin() ; it != voxels.end() ; it++ )
                feature_volume[ it->z * dim.x*dim.y + it->y * dim.x + it->x] = value + 100;
        }
        //check value
        int count = 0;
        for( size_t i = 0 ; i != dim.x*dim.y*dim.z ; i++ ){
            if( feature_volume[i] == 0 )++count;
        }
        cout << "empty: " << count << endl;
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
