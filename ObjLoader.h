#pragma once

class ObjGroup;
class MtlTex;

class ObjLoader
{
	map<string, MtlTex*> m_mapMtlTex;
public:
	ObjLoader();
	~ObjLoader();

	void Load(IN char* szFilename, IN char* FilePath, OUT vector<ObjGroup>& vecGroup);

private:
	
	void LoadMtlLib(char* szFilename, char* FilePath);
};

