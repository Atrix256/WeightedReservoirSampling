#include <direct.h>
#include <vector>
#include <random>

#define DETERMINISTIC() true

static const size_t c_numTests = 100000;
static const size_t c_numItems = 100;

std::mt19937 GetRNG()
{
    #if DETERMINISTIC()
    std::mt19937 mtl;
    #else
    std::random_device rd;
    std::mt19937 mtl(rd());
    #endif

    return mtl;
}

void UniformReservoirSampling()
{
    printf(__FUNCTION__ "\n");

    std::mt19937 mtl = GetRNG();
    std::uniform_real_distribution<float> dist;

    int lastPercent = -1;
    std::vector<size_t> histogram(c_numItems, 0);
    for (size_t testIndex = 0; testIndex < c_numTests; ++testIndex)
    {
        int percent = int(100.0f * float(testIndex) / float(c_numTests));
        if (lastPercent != percent)
        {
            lastPercent = percent;
            printf("\r%i%%", percent);
        }

        int chosenItemIndex = -1;
        for (int itemIndex = 0; itemIndex < c_numItems; ++itemIndex)
        {
            float chance = 1.0f / float(itemIndex + 1);
            if (dist(mtl) < chance)
                chosenItemIndex = itemIndex;
        }

        histogram[chosenItemIndex]++;
    }
    printf("\r100%%\n");

    FILE* file = nullptr;
    fopen_s(&file, "out/" __FUNCTION__ ".csv", "wb");
    fprintf(file, "\"Value\",\"Count\"\n");
    for (size_t itemIndex = 0; itemIndex < c_numItems; ++itemIndex)
        fprintf(file, "\"%zu\",\"%zu\"\n", itemIndex, histogram[itemIndex]);
    fclose(file);
}

void WeightedReservoirSampling()
{
    printf(__FUNCTION__ "\n");

    std::mt19937 mtl = GetRNG();
    std::uniform_real_distribution<float> dist;

    // PDF is y = x^2
    // Map the items to (0,1] and use the PDF as the PMF
    std::vector<float> PMFs(c_numItems);
    float pmfSum = 0.0f;
    for (size_t i = 0; i < c_numItems; ++i)
    {
        float x = float(i+1) / float(c_numItems);
        PMFs[i] = x * x;
        pmfSum += PMFs[i];
    }
    // normalize PMFs
    for (float& f : PMFs)
        f /= pmfSum;

    int lastPercent = -1;
    std::vector<size_t> histogram(c_numItems, 0);
    for (size_t testIndex = 0; testIndex < c_numTests; ++testIndex)
    {
        int percent = int(100.0f * float(testIndex) / float(c_numTests));
        if (lastPercent != percent)
        {
            lastPercent = percent;
            printf("\r%i%%", percent);
        }

        float weightSum = 0.0f;
        int chosenItemIndex = -1;
        for (int itemIndex = 0; itemIndex < c_numItems; ++itemIndex)
        {
            weightSum += PMFs[itemIndex];
            float chance = PMFs[itemIndex] / weightSum;
            if (dist(mtl) < chance)
                chosenItemIndex = itemIndex;
        }

        histogram[chosenItemIndex]++;
    }
    printf("\r100%%\n");

    FILE* file = nullptr;
    fopen_s(&file, "out/" __FUNCTION__ ".csv", "wb");
    fprintf(file, "\"Value\",\"Expected Count\",\"Actual Count\"\n");
    for (size_t itemIndex = 0; itemIndex < c_numItems; ++itemIndex)
        fprintf(file, "\"%zu\",\"%zu\",\"%zu\"\n", itemIndex, size_t(PMFs[itemIndex] * float(c_numTests)), histogram[itemIndex]);
    fclose(file);
}

int main(int argc, char** argv)
{
    _mkdir("out");
    UniformReservoirSampling();
    WeightedReservoirSampling();
    //SubsetReservoirSampling();
    return 0;
}

/*
TODO:
random subset reservoir sampling
golden ratio?

Links:
* reservoir sampling with hats video: https://www.youtube.com/watch?v=A1iwzSew5QY
* restirs paper: https://research.nvidia.com/sites/default/files/pubs/2020-07_Spatiotemporal-reservoir-resampling/ReSTIR.pdf

*/