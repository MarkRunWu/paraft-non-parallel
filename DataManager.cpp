#include "DataManager.h"

DataManager::DataManager() {
    tfResolution = -1;
    pMaskVolume = NULL;
    dataSequence.clear();
}

DataManager::~DataManager() {
    if (!dataSequence.empty()) {
        for (DataSequence::iterator it = dataSequence.begin(); it != dataSequence.end(); it++) {
            delete [] it->second;
        }   // unload data
    }

    if (pMaskVolume != NULL) {
        delete [] pMaskVolume;
    }
}

void DataManager::CreateNewMaskVolume() {
    pMaskVolume = new float[volumeSize];
    std::fill(pMaskVolume, pMaskVolume+volumeSize, 0);
}

void DataManager::InitTFSettings(string filename) {
    ifstream inf(filename.c_str(), ios::binary);
    if (!inf) { cout << "cannot read tf setting: " + filename << endl; exit(1); }

    float tfResF = 0.0f;
    inf.read(reinterpret_cast<char*>(&tfResF), sizeof(float));
    if (tfResF < 1) { cout << "tfResolution = " << tfResF << endl; exit(2); }

    tfResolution = (int)tfResF;
    pTFOpacityMap = new float[tfResolution];
    inf.read(reinterpret_cast<char*>(pTFOpacityMap), tfResolution*sizeof(float));
    for(int i = 0 ; i != tfResolution ; i++)
        cout << pTFOpacityMap[i] << endl;
    inf.close();
}

void DataManager::LoadDataSequence(const Metadata &meta, const int timestep) {
    blockDim = meta.volumeDim;
    volumeSize = blockDim.Product();
#ifdef _DEBUG
    cout << "delete data not within t-2 ~ t+2" << endl;
#endif
    // delete if data is not within [t-2, t+2] of current timestep t
    for (DataSequence::iterator it = dataSequence.begin(); it != dataSequence.end(); ) {
        if (it->first < timestep-2 || it->first > timestep+2) {
            delete [] it->second;
            it = dataSequence.erase(it);
        }else it++;
    }
#ifdef _DEBUG
    cout << "finish delete data" << endl;
#endif
    for (int t = timestep-2; t <= timestep+2; t++) {
        if (t < meta.start || t > meta.end || dataSequence[t] != NULL) {
            continue;
        }
        cout << "Load data " << timestep << " from Disk" << endl;
        char timestamp[21];  // up to 64-bit #
        sprintf_s(timestamp, "%02d", t);
        string fpath = meta.path + "/" + meta.prefix + timestamp + "." + meta.surfix;

        ifstream inf(fpath.c_str(), ios::binary);
        if (!inf) { cout << "cannot read file: " + fpath << endl; exit(1); }

        dataSequence[t] = new float[volumeSize];
        inf.read(reinterpret_cast<char*>(dataSequence[t]), volumeSize*sizeof(float));
        inf.close();

        //nomalize(dataSequence[t])
    }
}

void DataManager::nomalize(float *pData) {
    float min = pData[0], max = pData[0];
    for (int i = 1; i < volumeSize; i++) {
        min = min < pData[i] ? min : pData[i];
        max = max > pData[i] ? max : pData[i];
    }
    cout << "Min: " << min << "Max: " << max << endl;
    for (int i = 0; i < volumeSize; i++) {
        pData[i] = (pData[i] - min) / (max - min);
    }
}
