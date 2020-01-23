#include "systemc.h"

SC_MODULE(Processor1) {
	sc_fifo_out<int> sliders;
	sc_fifo_out<int> state;
	sc_out<bool> flag_ERROR;
	sc_out<bool> LED1;
	sc_out<bool> LED2;
	sc_out<bool> LED4;
	sc_out<bool> LED5;
	sc_out<bool> LED3;
	sc_out<bool> LED6;

	int SW = 0;
	bool checkSW[7] = { false };
	bool Confirmer;
	bool flagERROR;

	void task1() {

		//sprawdza czy nie nastapil error oraz zajmuje sie wjazdem na pierwsze skrzyzowanie 

		while (1) {
			cin >> SW;
			if (SW > 0 || SW < 8) {
				Confirmer = false;
				checkSW[SW - 1] = !checkSW[SW - 1];
				flagERROR = false;
				int error = 0;
				for (int i = 0; i < 5; i++) {
					if (checkSW[i] == true)
						++error;
					if (error > 1)
						flagERROR = true;
				}
				if (checkSW[SW - 1] == false)
					SW = -SW;

				flag_ERROR.write(flagERROR);
				sliders.write(SW);
				state.write(SW);
				wait(50, SC_NS);
			}
			if (SW == 1) {
				wait(30, SC_NS);
				cout << "Wjazd na skrzyzowanie 1" << endl;
			}
			if (SW == -1) {

				LED5.write(false);
				LED2.write(false);
				LED3.write(false);
				wait(30, SC_NS);
				cout << "Wyjazd ze skrzyzowania 1" << endl;
			}

		}
	}

	void task2() {

		//zajmuje sie wjazdem na drugie skrzyzowanie 

		while (1) {
			wait(3, SC_NS);
			if (SW == 2 && !Confirmer) {
				Confirmer = true;

				wait(30, SC_NS);
				cout << "Wjazd na skrzyzowanie 2" << endl;
			}
			else if (SW == -2 && !Confirmer) {
				Confirmer = true;
				LED5.write(false);
				LED2.write(false);
				LED3.write(false);
				wait(30, SC_NS);
				cout << "Wyjazd ze skrzyzowania 2" << endl;
			}
		}
	}

	


	void task3() {

		//zajmuje sie wjazdem na trzecie skrzyzowanie 

		while (1) {
			wait(3, SC_NS);
			if (SW == 3 && !Confirmer) {
				Confirmer = true;

				wait(30, SC_NS);
				cout << "Wjazd na skrzyzowanie 3" << endl;
			}
			else if (SW == -3 && !Confirmer) {
				Confirmer = true;
				LED5.write(false);
				LED2.write(false);
				LED3.write(false);
				wait(30, SC_NS);
				cout << "Wyjazd ze skrzyzowania 3" << endl;
			}
		}
	}

	void task4() {

		//zajmuje sie wjazdem na czwarte skrzyzowanie 

		while (1) {
			wait(3, SC_NS);
			if (SW == 4 && !Confirmer) {
				Confirmer = true;

				wait(30, SC_NS);
				cout << "Wjazd na skrzyzowanie 4" << endl;
			}
			else if (SW == -4 && !Confirmer) {
				Confirmer = true;
				LED5.write(false);
				LED2.write(false);
				LED3.write(false);
				wait(30, SC_NS);
				cout << "Wyjazd ze skrzyzowania 4" << endl;
			}
		}
	}


	void taskSwiatla() {

		//zajmuje sie obsluga swiatel na skrzyzowaniu 

		while (1) {
			wait(3, SC_NS);
			if (SW == 6 && !Confirmer) {
				//swiatla zielone dla strony jeden, czerwwone dla drugiej
				Confirmer = true;
				LED1.write(true);
				LED6.write(true);
				wait(30, SC_NS);
				cout << "Swiatla Zielone" << endl;
			}
			else if ((SW == -6) && (!Confirmer)) {
				//swiatla pomaranczowe dla strony jeden
				Confirmer = true;
				LED1.write(false);
				LED2.write(true);
				LED5.write(true);
				wait(30, SC_NS);
				cout << "Swiatla pomaranczowe" << endl;

			}
			else if (SW == 7 && !Confirmer) {
				//swiatla czerwone dla peirwszej strony 
				Confirmer = true;
				LED2.write(false);
				LED5.write(false);
				LED6.write(false);
				LED3.write(true);
				LED4.write(true);
				wait(30, SC_NS);
				cout << "Swiatla Czerwone" << endl;

			}
			else if ((SW == -7) && (!Confirmer)) {
				//swiatla pomaranczoo czerwone dla pierwszej strony 
				Confirmer = true;
				LED4.write(false);
				LED2.write(true);
				LED5.write(true);
				wait(30, SC_NS);
				cout << "Swiatla Pomaranczowo-Czerwone" << endl;
			}
		}
	}

	SC_CTOR(Processor1) {
		SC_THREAD(taskSwiatla);
		SC_THREAD(task1);
		SC_THREAD(task2);
		SC_THREAD(task3);
		SC_THREAD(task4);
	}
};

SC_MODULE(Processor2) {
	sc_fifo_in<int> sliders;
	sc_in<bool> flag_ERROR;
	sc_out<bool> LED6;

	int lastState = 0;
	int state = 0;

	void task6() {

		//zajmuje sie wjazdem na skrzyzowanie 5

		while (1) {
			wait(3, SC_NS);
			state = sliders.read();
			if (lastState != state) {
				if (state == 5) {
					wait(30, SC_NS);
					cout << "Wjazd na skrzyzowanie 5" << endl;
				}
				if (state == -5) {
					wait(30, SC_NS);
					cout << "Wyjazd ze skrzyzowania 5" << endl;
				}
				lastState = state;
			}
		}
	}

	SC_CTOR(Processor2) {
		SC_THREAD(task6);
	}
};

SC_MODULE(Module) {
	sc_fifo_in<int> sliders;
	sc_in<bool> flag_ERROR;
	sc_out<bool> LED3;
	sc_out<bool> ledError;

	int state = 0;
	int lastState = 0;

	//Modul sprzeotowy, zajmuje sie wyswietlanie komunikatu dla error

	void taskModule() {
		while (1) {
			wait(3, SC_NS);
			state = sliders.read();
			if (lastState != state) {
				if (flag_ERROR) {
					wait(10, SC_NS);
					cout << "Err" << endl;
				}
				else {
					wait(10, SC_NS);
					cout << "  " << endl;
				}
				lastState = state;
			}
		}
	}

	SC_CTOR(Module) {
		SC_THREAD(taskModule);
	}
};

SC_MODULE(Bus) {

	//szyna
	sc_fifo<int> sliders;
	sc_fifo<int> state;
	sc_signal<bool> flag_ERROR;
	sc_signal<bool> LED1;
	sc_signal<bool> LED2;
	sc_signal<bool> LED3;
	sc_signal<bool> LED4;
	sc_signal<bool> LED5;
	sc_signal<bool> LED6;
	sc_signal<bool> ledError;

	SC_CTOR(Bus) {

	}
};

SC_MODULE(Monitor) {
	sc_in<bool> LED1;
	sc_in<bool> LED2;
	sc_in<bool> LED3;
	sc_in<bool> LED4;
	sc_in<bool> LED5;
	sc_in<bool> LED6;
	sc_in<bool> ledError;

	void show() {
		//wyswietla przy zmianie LEDsow
		system("cls");
		cout << "LED1 2LED 3LED 4LED 5LED LED6  ERROR" << endl;
		cout << LED1 << "    " << LED2 << "    " << LED3 << "    " << LED4 << "    " << LED5 << "    " << LED6 << "    " << ledError << endl;
	}

	SC_CTOR(Monitor) {
		SC_METHOD(show);
		sensitive << LED1;
		sensitive << LED2;
		sensitive << LED3;
		sensitive << LED4;
		sensitive << LED5;
		sensitive << LED6;
		sensitive << ledError;
	}
};

int sc_main(int argc, char** argv)
{
	Monitor m("monitor");
	Bus bus("b");
	Processor1 p1("proc1");
	Processor2 p2("proc2");
	Module mod("module");


	p1.LED1(bus.LED1);
	p1.LED2(bus.LED2);
	p1.LED4(bus.LED4);
	p1.LED5(bus.LED5);
	p1.LED3(bus.LED3);
	p1.LED6(bus.LED6);
	mod.LED3(bus.LED3);
	mod.ledError(bus.ledError);
	p2.LED6(bus.LED6);
	m.LED1(bus.LED1);
	m.LED2(bus.LED2);
	m.LED4(bus.LED4);
	m.LED5(bus.LED5);
	m.ledError(bus.ledError);
	m.LED3(bus.LED3);
	m.LED6(bus.LED6);
	p1.sliders(bus.sliders);
	p1.state(bus.state);
	p2.sliders(bus.sliders);
	mod.sliders(bus.state);
	p1.flag_ERROR(bus.flag_ERROR);
	p2.flag_ERROR(bus.flag_ERROR);
	mod.flag_ERROR(bus.flag_ERROR);

	sc_start();
	return 0;
}