#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <Windows.h>

using namespace std;

void allVectorsToTxt(vector<tuple<double, double, double>> positions,
	vector<tuple<double, double, double>> speeds,
	vector<tuple<double, double, double>> rotations,
	vector<tuple<double, double, double>> accelerations) {

	if (positions.size() != speeds.size()
		or speeds.size() != rotations.size()
		or rotations.size() != accelerations.size()
		or accelerations.size() != positions.size()) {
		cerr << "Lists size aren't the same size" << endl;
		return;
	}

	string directoryPath = "D:/TableTennis/output/simulations/";

	if (!filesystem::exists(directoryPath)) {
		if (filesystem::create_directories(directoryPath)) {
			cerr << "Failed to create the directory" << endl;
		}
	}
	ofstream file;
	file.open(directoryPath + "transfer.txt");
	if (!file.is_open()) {
		cerr << "Cannot save tracking data" << endl;
	}
	for (int i = 0; i < positions.size(); i++) {
		
		file 
			<< get<0>(positions[i]) << ";" << get<1>(positions[i]) 
			<< ";" << get<2>(positions[i]) << ";;"
			<< get<0>(speeds[i]) << ";" << get<1>(speeds[i]) << ";" 
			<< get<2>(speeds[i]) << ";;"
			<< get<0>(rotations[i]) << ";" << get<1>(rotations[i]) << ";"
			<< get<2>(rotations[i]) << ";;"
			<< get<0>(accelerations[i]) << ";" << get<1>(accelerations[i]) 
			<< ";" << get<2>(accelerations[i]) << ";;" << endl;

	}
	file.close();
	system("py D:/TableTennis/visualisation/simulation/classic_graphs.py");

}

auto txtToAllVectors(){
	// https://learn.microsoft.com/fr-fr/windows/win32/dlgbox/using-common-dialog-boxes?redirectedfrom=MSDN#open_file

	wchar_t fileName[MAX_PATH] = { 0 };
	OPENFILENAMEW ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; // The parent window handle (or NULL if you don't have one).
	ofn.lpstrFilter = L"txt Files (*.txt)\0*.txt\0"; // Filter for txt files only.
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST; // Flags for dialog behavior.

	if (!GetOpenFileNameW(&ofn)) {
		// The user canceled the dialog or an error occurred.
		// You can handle this case accordingly (e.g., return an empty string).
		cerr << "Can't find the file" << endl;
	}
	wstring wideFileName(fileName);
	string directoryPath = string(wideFileName.begin(), wideFileName.end());


	if (!filesystem::exists(directoryPath)) {
		cerr << "File doesn't exist" << endl;
	}
	ifstream file;
	file.open(directoryPath);
	if (!file.is_open()) {
		cerr << "Cannot open tracking data" << endl;
	}
	
	auto read_size = size_t(4096);
	auto stream = ifstream(directoryPath.data());
	auto out = string();
	auto buf = string(read_size, '\0');
	while (stream.read(&buf[0], read_size)) {
		out.append(buf, 0, stream.gcount());
	}
	out.append(buf, 0, stream.gcount());
	
	cout << out << endl;
}