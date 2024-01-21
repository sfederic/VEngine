#include "../Include/TransformOut.hlsli"

//Ref:https://www.docswell.com/s/EpicGamesJapan/5DEVPV-2023-12-01-082936#p24
VS_OUT main(VS_IN i)
{
    VS_OUT o = TransformOutOutline(i);
    return o;
}
