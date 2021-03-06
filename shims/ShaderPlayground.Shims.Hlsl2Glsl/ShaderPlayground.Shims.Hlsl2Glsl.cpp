#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Source/include/hlsl2glsl.h"

static std::vector<char> ReadBinaryFile(const char* fileName)
{
	std::ifstream ifs(fileName, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	return result;
}

int main(int argc, const char* argv[])
{
	auto language = (EShLanguage)std::stoi(argv[2]);
	auto targetVersion = (ETargetVersion)std::stoi(argv[3]);
	auto entryPoint = argv[4];

	auto shaderBytes = ReadBinaryFile(argv[1]);

	Hlsl2Glsl_Initialize();

	auto compilerHandle = Hlsl2Glsl_ConstructCompiler(language);

	if (Hlsl2Glsl_Parse(
		compilerHandle,
		shaderBytes.data(),
		targetVersion,
		NULL,
		0))
	{
		if (Hlsl2Glsl_Translate(
			compilerHandle,
			entryPoint,
			targetVersion,
			0))
		{
			auto glslSource = Hlsl2Glsl_GetShader(compilerHandle);

			std::ofstream out(argv[5], std::ios::out);
			out << glslSource;
			out.close();
		}
	}

	auto errorLog = Hlsl2Glsl_GetInfoLog(compilerHandle);

	std::ofstream out(argv[6], std::ios::out);
	out << errorLog;
	out.close();

	Hlsl2Glsl_DestructCompiler(compilerHandle);

	Hlsl2Glsl_Shutdown();

	return 0;
}