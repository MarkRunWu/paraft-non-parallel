#include "BlockController.h"

int main (int argc, char** argv) {
    Metadata meta; {
        meta.start      = 1;
        meta.end        = 4;
        meta.prefix     = "QCLOUDf";
        meta.surfix     = "little";
        meta.path       = "D:/cloud_data";
        meta.tfPath     = "hurricane.tfe";
        meta.volumeDim  = Vector3i(500, 500, 100);
    }

    int currentTimestep = meta.start;

    BlockController *pBlockController = new BlockController();
    pBlockController->SetCurrentTimestep(currentTimestep);
    pBlockController->InitParameters(meta);
#ifdef _DEBUG
    cout << "init loading" << endl;
#endif
    while (currentTimestep++ < meta.end) {
        pBlockController->SetCurrentTimestep(currentTimestep);
        pBlockController->TrackForward(meta);
        cout << currentTimestep << " done." << endl;
    }
    for( int i = meta.start ; i <= meta.end ; i++ ){
        pBlockController->exportFeature(i);
    }
    delete pBlockController;
    return 0;
}
