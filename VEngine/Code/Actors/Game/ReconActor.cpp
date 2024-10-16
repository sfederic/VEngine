#include "vpch.h"
#include "ReconActor.h"
#include "Localisation/Localisation.h"

void ReconActor::Create()
{
	__super::Create();

	SetReconText();
}

Properties ReconActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Recon Text Key", &reconTextKey).change = [&](Property&) { SetReconText(); };
	return props;
}

void ReconActor::SetReconText()
{
	reconText = Localise::GetString(reconTextKey, LocalisationFiles::reconActorLines);
}
