#include "vehicles.h"

using namespace std;

int main() {
	Cotxe cotxes[5];
	Avio avions[3];
	int res, tipus, num, km, h;

	cotxes[0].nom = "Cotxe 1";
	cotxes[0].passatgers = 0;
	cotxes[0].maxPassatgers = 2;
	cotxes[0].marca = "Chevrolet";
	cotxes[0].model = "Camaro";
	cotxes[0].maxVelocitat = 300;
	cotxes[0].comptaKm = 0;


	avions[0].nom = "Avio 1";
	avions[0].passatgers = 0;
	avions[0].maxPassatgers = 200;
	avions[0].fabricant = "Boeing";
	avions[0].model = "747";
	avions[0].maxVelocitat = 1200;
	avions[0].horesVol = 0;
	avions[0].distancia = 0;
	avions[0].pilot = "Michael Jackson";
	

	do {
		cout << "Menú principal" << endl;
		cout << "1. Trajecte nou\n" << "2. Consulta\n" << "3. Sortir" << endl;
		cin >> res;

		if (res == 1) {
			cout << "TRAJECTE NOU\n" << endl;
			cout << "Tipus de vehicle" << endl;
			cout << "1. Cotxe\n" << "2. Avio" << endl;
			cin >> tipus;
			if (tipus == 1) {
				cout << "Selecciona cotxe (1..5): ";
				cin >> num;
				cout << "Distancia a recorrer (en Km): ";
				cin >> km;
				cotxes[num - 1].trajecte(km);
			}
			else if (tipus == 2) {
				cout << "Selecciona avio (1..3): ";
				cin >> num;
				cout << "Distancia del trajecte(en Km): ";
				cin >> km;
				cout << "Temps del trajecte (en hores): ";
				cin >> h;
				avions[num - 1].trajecte(h, km);
			}
			else { cout << "Tipus de vehicle no identificat\n"; }
		}
		else if (res == 2) {
			cout << "CONSULTAR ESTAT\n" << endl;
			cout << "Tipus de vehicle" << endl;
			cout << "1. Cotxe\n" << "2. Avio" << endl;
			cin >> tipus;
			if (tipus == 1) {
				cout << "Selecciona cotxe (1..5): ";
				cin >> num;
				cotxes[num - 1].printDetails();
			}
			else if (tipus == 2) { // Consultar avió
				cout << "Selecciona avio (1..3): ";
				cin >> num;
				avions[num - 1].printDetails();
			}
		}
		cout << endl;
	} while (res > 0 && res < 3);


	system("pause");
	return 0;
}