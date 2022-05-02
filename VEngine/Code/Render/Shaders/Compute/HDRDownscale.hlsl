#include "DownScale.hlsli"

float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
    float avgLum = 0.0;
    
    // Skip out of bound pixels
    if (CurPixel.y < Res.y)
    {
        // Sum a group of 4x4 pixels
        int3 nFullResPos = int3(CurPixel * 4, 0);
        float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
        
        [unroll]
        for (int i = 0; i < 4; i++)
        {
            [unroll]
            for (int j = 0; j < 4; j++)
            {
                downScaled += HDRTex.Load(nFullResPos, int2(j, i));
            }
        }
        downScaled /= 16.0;
        // Calculate the lumenace value for this pixel
        avgLum = dot(downScaled, LUM_FACTOR);
        // Write the result to the shared memory
        SharedPositions[groupThreadId] = avgLum;
    }
    
    // Synchronize before next step
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}

float DownScale1024to4(uint dispatchThreadId, uint groupThreadId, float avgLum)
{
    // Expend the downscale code from a loop
    [unroll]
    for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3; groupSize < 1024;
        groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4)
    {
        // Skip out of bound pixels
        if (groupThreadId % groupSize == 0)
        {
            // Calculate the luminance sum for this step
            float stepAvgLum = avgLum;
            stepAvgLum += dispatchThreadId + step1 < Domain ?
                SharedPositions[groupThreadId + step1] : 
                avgLum;
            
            stepAvgLum += dispatchThreadId + step2 < Domain ?
                SharedPositions[groupThreadId + step2] :
                avgLum;
            
            stepAvgLum += dispatchThreadId + step3 < Domain ?
                SharedPositions[groupThreadId + step3] :
                avgLum;

            // Store the results
            avgLum = stepAvgLum;
            SharedPositions[groupThreadId] = stepAvgLum;
        }
        
        // Synchronize before next step
        GroupMemoryBarrierWithGroupSync();
    }
    
    return avgLum;
}

void DownScale4to1(uint dispatchThreadId, uint groupThreadId, uint groupId, float avgLum)
{
    if (groupThreadId == 0)
    {
        // Calculate the average lumenance for this thread group
        float fFinalAvgLum = avgLum;
        fFinalAvgLum += dispatchThreadId + 256 < Domain ?
            SharedPositions[groupThreadId + 256] : avgLum;
        fFinalAvgLum += dispatchThreadId + 512 < Domain ?
            SharedPositions[groupThreadId + 512] : avgLum;
        fFinalAvgLum += dispatchThreadId + 768 < Domain ?
            SharedPositions[groupThreadId + 768] : avgLum;
        fFinalAvgLum /= 1024.0;
        // Write the final value into the 1D UAV which
        // will be used on the next step
        AverageLum[groupId] = fFinalAvgLum;
    }
}

[numthreads(1024, 1, 1)]
void CSMain(uint3 groupId : SV_GroupID,
    uint3 dispatchThreadId : SV_DispatchThreadID,
    uint3 groupThreadId : SV_GroupThreadID)
{
    uint2 CurPixel = uint2(dispatchThreadId.x % Res.x, dispatchThreadId.x / Res.x);
    
    // Reduce a group of 16 pixels to a single pixel and store in the shared memory
    float avgLum = DownScale4x4(CurPixel, groupThreadId.x);
    
    // Down scale from 1024 to 4
    avgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x, avgLum);
    // Downscale from 4 to 1
    
    DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x, avgLum);
}
