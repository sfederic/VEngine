#include "Obj.h"
#include <Windows.h>

bool loadOBJFile(const char* filename, OBJData& data)
{
	std::vector<float> v; //Vertices
	std::vector<float> vt; //Texcoords
	std::vector<float> vn; //Normals
	std::vector<uint16_t> vf; //Faces
	std::vector<uint16_t> vtf;
	std::vector<uint16_t> vnf; 

	FILE* file = nullptr;
	fopen_s(&file, filename, "r");
	if (!file)
	{
		char errmsg[512];
		snprintf(errmsg, sizeof(errmsg), "%s not found.", filename);
		OutputDebugString(errmsg);
		return false;
	}

	while (1)
	{
		char line[256];
		int res = fscanf_s(file, "%s", line, _countof(line));

		if (res == EOF) //EOF 
		{
			break;
		}

		if (strcmp(line, "v") == 0) //Vertices
		{
			float v1, v2, v3;
			fscanf_s(file, "%f %f %f\n", &v1, &v2, &v3);
			v.push_back(v1);
			v.push_back(v2);
			v.push_back(v3);
		}
		else if (strcmp(line, "vt") == 0) //Texcoords
		{
			float v1, v2;
			fscanf_s(file, "%f %f\n", &v1, &v2);
			vt.push_back(v1);
			vt.push_back(v2);
		}
		else if (strcmp(line, "vn") == 0) //Normals
		{
			float v1, v2, v3;
			fscanf_s(file, "%f %f %f\n", &v1, &v2, &v3);
			vn.push_back(v1);
			vn.push_back(v2);
			vn.push_back(v3);
		}
		else if (strcmp(line, "f") == 0) //Faces
		{
			uint16_t f[10] = {0}; //There was a stack corruption before when size was 9. Figure out.
			fscanf_s(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
				&f[0], &f[1], &f[2],
				&f[3], &f[4], &f[5],
				&f[6], &f[7], &f[8]);

			vf.push_back(f[0]); vf.push_back(f[1]); vf.push_back(f[2]);
			vtf.push_back(f[3]); vtf.push_back(f[4]); vtf.push_back(f[5]);
			vnf.push_back(f[6]); vnf.push_back(f[7]); vnf.push_back(f[8]);
		}
	}

	int reserveSize = vf.size();

	if (v.size() > 0) { data.verts.reserve(reserveSize); }
	if (vt.size() > 0) { data.uvs.reserve(reserveSize); }
	if (vn.size() > 0) { data.normals.reserve(reserveSize); }

	for (int i = 0; i < reserveSize; i++)
	{
		data.verts.push_back(v[vf[i] - 1]);
		data.uvs.push_back(vt[vtf[i] - 1]);
		data.normals.push_back(vn[vnf[i] - 1]);
	}

	fclose(file);
	return true;
}
