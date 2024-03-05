#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>

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
