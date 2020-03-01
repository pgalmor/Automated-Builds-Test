#ifndef __VEHICLES_H__
#define __VEHICLES_H__

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

class Vehicles {
protected:
public:
	string nom;

	Vehicles() {};
	Vehicles(string n) {
		nom = n;
	}
	string getNom() {
		return nom;
	}
};

class Cotxe : public Vehicles {
public:
	int passatgers, maxPassatgers;
	string marca, model;
	int maxVelocitat, comptaKm;

	Cotxe() :Vehicles("") {
		passatgers = 0;
		maxPassatgers = 0;
		marca = "";
		model = "";
		maxVelocitat = 0;
		comptaKm = 0;
	}
	Cotxe(int pas, int maxpas, string marc, string mod, int maxvel) {
		passatgers = pas;
		maxPassatgers = maxpas;
		marca = marc;
		model = mod;
		maxVelocitat = maxvel;
		comptaKm = 0;
	}
	void printDetails() {
		cout << "Passatgers: " << passatgers << endl;
		cout << "Maxim de passatgers: " << maxPassatgers << endl;
		cout << "Marca: " << marca << endl;
		cout << "Model: " << model << endl;
		cout << "Velocitat maxima: " << maxVelocitat << endl;
		cout << "Quilometres recorreguts: " << comptaKm << endl;
	}
	void trajecte(int &km) {
		comptaKm += km;
	}
};

class Avio : public Vehicles {
public:
	int passatgers, maxPassatgers;
	string fabricant, model;
	int maxVelocitat, horesVol, distancia;
	string pilot;

	Avio() :Vehicles("") {
		passatgers = 0;
		maxPassatgers = 0;
		fabricant = "";
		model = "";
		maxVelocitat = 0;
		horesVol = 0;
		distancia = 0;
		pilot = "";
	}
	Avio(int pas, int maxpas, string fabr, string mod, int maxvel, string pil) {
		passatgers = pas;
		maxPassatgers = maxpas;
		fabricant = fabr;
		model = mod;
		maxVelocitat = maxvel;
		horesVol = 0;
		distancia = 0;
		pilot = pil;
	}
	void printDetails() {
		cout << "Passatgers: " << passatgers << endl;
		cout << "Maxim de passatgers: " << maxPassatgers << endl;
		cout << "Fabricant: " << fabricant << endl;
		cout << "Model: " << model << endl;
		cout << "Velocitat maxima: " << maxVelocitat << endl;
		cout << "Hores de vol: " << horesVol << endl;
		cout << "Distancia recorreguda: " << distancia << endl;
		cout << "Pilot: " << pilot << endl;
	}
	void trajecte(int temps, int dist) {
		horesVol += temps;
		distancia += dist;
	}
};

#endif