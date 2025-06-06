#include <windows.h>
#include <iostream>
#include "Source.h"
#include <string>

using namespace std;

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	char buffer[MAX_PATH];
	DWORD length = GetCurrentDirectoryA(MAX_PATH, buffer);

	if (length < 0 || length > MAX_PATH) {
		MessageBoxA(NULL, "No se pudo encontrar el directorio actual", "ERROR", NULL);
		return 1;
	}

	cout << "[+] Directorio actual: " << buffer << endl;

	strcat_s(buffer, "\\AppData");
	SetEnvironmentVariableA("APPDATA", buffer);

	char filePath[MAX_PATH];
	DWORD filePathSize = GetModuleFileNameA(NULL, filePath, MAX_PATH);
	if (filePathSize == 0 || filePathSize >= MAX_PATH) {
		MessageBoxA(NULL, "No se pudo obtener el nombre del archivo", "ERROR", NULL);
		return 1;
	}

	cout << "[+] Verificando nombre del archivo" << endl;

	string fileName = filePath;
	size_t lastSlash = fileName.find_last_of("\\/");
	if (lastSlash != string::npos) {
		fileName = fileName.substr(lastSlash + 1);
	}

	size_t launcherPos = fileName.find("_launcher");
	if (launcherPos != string::npos) {
		fileName = fileName.substr(0, launcherPos);
	}
	else {
		MessageBoxA(NULL, "El nombre del archivo no contiene '_launcher'", "ERROR", NULL);
		return 1;
	}

	fileName += ".exe";

	cout << "[+] Intentando iniciar: " << fileName << endl;

	STARTUPINFOA si = { sizeof(si)};
	PROCESS_INFORMATION pi;

	int processCreated = CreateProcessA(
		fileName.c_str(),
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);

	if (processCreated == 0) {
		MessageBoxA(NULL, "No se pudo iniciar el proceso", "ERROR", NULL);
		return 1;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}
