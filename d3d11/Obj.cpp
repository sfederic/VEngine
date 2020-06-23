#include "Obj.h"
#include <Windows.h>

bool loadOBJFile(const char* filename, OBJData& data)
{
	std::vector<XMFLOAT3> v; //Vertex posisitons
	std::vector<XMFLOAT2> vt; //Texcoords
	std::vector<XMFLOAT3> vn; //Normals
	std::vector<uint16_t> vf; //Faces
	std::vector<uint16_t> vtf;
	std::vector<uint16_t> vnf; 

	FILE* file = nullptr;
	fopen_s(&file, filename, "r");
	if (!file)
	{
		char errmsg[512];
		snprintf(errmsg, sizeof(errmsg), "%s not found.", filename);
		MessageBox(0, errmsg, "loadOBJFile", 0);
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
			v.push_back(XMFLOAT3(v1, v2, v3));
		}
		else if (strcmp(line, "vt") == 0) //Texcoords
		{
			float v1, v2;
			fscanf_s(file, "%f %f\n", &v1, &v2);
			vt.push_back(XMFLOAT2(v1, v2));
		}
		else if (strcmp(line, "vn") == 0) //Normals
		{
			float v1, v2, v3;
			fscanf_s(file, "%f %f %f\n", &v1, &v2, &v3);
			vn.push_back(XMFLOAT3(v1, v2, v3));
		}
		else if (strcmp(line, "f") == 0) //Faces
		{
			uint16_t f[10] = {0}; //There was a stack corruption before when size was 9. Figure out.
			fscanf_s(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
				&f[0], &f[1], &f[2],
				&f[3], &f[4], &f[5],
				&f[6], &f[7], &f[8]);

			vf.push_back(f[0]); vtf.push_back(f[1]); vnf.push_back(f[2]);
			vf.push_back(f[3]); vtf.push_back(f[4]); vnf.push_back(f[5]);
			vf.push_back(f[6]); vtf.push_back(f[7]); vnf.push_back(f[8]);
		}
	}

	int reserveSize = vf.size();
	if (data.verts.size() > 0) { data.verts.reserve(reserveSize); }

	for (int i = 0; i < vf.size(); i++)
	{
		Vertex vertex = {};

		int vf_index = vf[i] - 1;
		assert(vf_index < v.size());
		vertex.pos = v[vf_index];

		int vtf_index = vtf[i] - 1;
		assert(vtf_index < vt.size());
		vertex.uv = vt[vtf_index];

		int vnf_index = vnf[i] - 1;
		assert(vnf_index < vn.size());
		vertex.normal = vn[vnf_index];

		data.verts.push_back(vertex);
	}

	fclose(file);
	return true;
}

//TODO: Feel out if it's even worth optimizing before FBX implementation
/*bool loadOBJFileRealloc(const char* filename, OBJDataRealloc* data)
{
	XMFLOAT3* v = (XMFLOAT3*)malloc(sizeof(XMFLOAT3));
	XMFLOAT2* vt = (XMFLOAT2*)malloc(sizeof(XMFLOAT2));
	XMFLOAT3* vn = (XMFLOAT3*)malloc(sizeof(XMFLOAT3));
	uint16_t* vf = (uint16_t*)malloc(sizeof(uint16_t) * 3);
	uint16_t* vtf = (uint16_t*)malloc(sizeof(uint16_t) * 3);
	uint16_t* vnf = (uint16_t*)malloc(sizeof(uint16_t) * 3);

	int vi = 0;
	int vti = 0;
	int vni = 0;
	int fi = 0;

	FILE* file = nullptr;
	fopen_s(&file, filename, "r");
	if (!file)
	{
		char errmsg[512];
		snprintf(errmsg, sizeof(errmsg), "%s not found.", filename);
		MessageBox(0, errmsg, "loadOBJFile", 0);
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
			fscanf_s(file, "%f %f %f\n", &v[vi].x, &v[vi].y, &v[vi].z);
			vi += 3;
			v = (XMFLOAT3*)realloc(v, sizeof(XMFLOAT3) * vi);
		}
		else if (strcmp(line, "vt") == 0) //Texcoords
		{
			fscanf_s(file, "%f %f\n", &vt[vti].x, &vt[vti].y);
			vti += 2;
			vt = (XMFLOAT2*)realloc(vt, sizeof(XMFLOAT2) * vti);
		}
		else if (strcmp(line, "vn") == 0) //Normals
		{
			fscanf_s(file, "%f %f %f\n", &vn[vni].x, &vn[vni].y, &vn[vni].z);
			vni += 3;
			vn = (XMFLOAT3*)realloc(vn, sizeof(XMFLOAT3) * vni);
		}
		else if (strcmp(line, "f") == 0) //Faces
		{
			fscanf_s(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
				&vf[fi], &vtf[fi+1], &vnf[fi+2],
				&vf[fi+3], &vtf[fi+4], &vnf[fi+5],
				&vf[fi+6], &vtf[fi+7], &vnf[fi+8]);

			fi += 9;
			vf = (uint16_t*)realloc(vf, (sizeof(uint16_t) * 3) * fi);
			vtf = (uint16_t*)realloc(vtf, (sizeof(uint16_t) * 3) * fi);
			vnf = (uint16_t*)realloc(vnf, (sizeof(uint16_t) * 3) * fi);
		}
	}

	XMFLOAT3* verts = (XMFLOAT3*)malloc(sizeof(XMFLOAT3) * vi);
	XMFLOAT2* uvs = (XMFLOAT2*)malloc(sizeof(XMFLOAT2) * vti);
	XMFLOAT3* normals = (XMFLOAT3*)malloc(sizeof(XMFLOAT3) * vni);

	for (int i = 0; i < vi; i++)
	{
		verts[i] = v[vf[i] - 1];
	}

	memcpy(&data->verts->pos, verts, sizeof(XMFLOAT3) * vi);

	for (int i = 0; i < vti; i++)
	{
		uvs[i] = vt[vtf[i] - 1];
	}

	memcpy(&data->verts->uv, uvs, sizeof(XMFLOAT2) * vti);

	for (int i = 0; i < vni; i++)
	{
		normals[i] = vn[vnf[i] - 1];
	}

	memcpy(&data->verts->normal, normals, sizeof(XMFLOAT3) * vti);

	fclose(file);
	return true;
}*/