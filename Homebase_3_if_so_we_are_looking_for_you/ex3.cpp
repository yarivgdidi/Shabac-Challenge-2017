
#include "stdafx.h"
#include "iostream"
#include "iomanip"
#include "time.h"  
#include "fstream"
#include <stdlib.h>  
#include <direct.h>


#define TRESHOLD 0.6
#define CHALLENGE 3
#define MFILE "./EncryptedMessage.bin"
#define LENGTH 54

// use challenge 2 message to test decrypt function
//#define CHALLENGE 2
//#define MFILE "./EncryptedMessageEX2.bin"
//#define LENGTH 157

struct key {
	uint8_t operation;
	uint8_t parameter;
	uint32_t iterations;
};


class Decrypt {
	key static_k[8] = { { 0, 45, 87 },{ 1, 172, 40 },{ 2 ,7, 40 },{ 1,24,48 },{ 1,75,100 },{ 0,82,82 },{ 2,2,64 },{ 0,55,33 } };
	key random_k[3] = { { 0,1,1 },{ 1,18,49 },{ 2,247,81 } };//, { 2 ,128,148 }, { 2,106,96 }, { 0,0,0 }};
	key random_keys[6][3] = { {{0,1,1},{1,12,10},{2,207,42} },{{0,0,0}, { 2,0,0 }, { 1,0,0 }}, { {1,0,0},{0,0,0},{2,0,0} }, { {1,0,0},{2,0,0},{0,0,0} }, { {2,0,0},{1,0,0},{0,0,0}, }, { {2,0,0},{0,0,0},{1,0,0} } };
	key k;

	int key_size;
	char message[LENGTH + 1 ];
	char orig_message[LENGTH + 1];
	int index, direction;

	public:
		 Decript::Decript() {
			index = 1;
			direction = 1;
			key_size = 8;
			read_message();
			copy_message();
		}

		int apply_operator(uint8_t target, uint8_t operation, uint8_t parameter) {
			if (operation == 0)
				return target ^ parameter;
			else if (operation == 1)
				return (target + parameter) & 255;
			else if (operation == 2)
				return (target - parameter) & 255;
			else
				return target;
		 }

		void decript_random_k() {
			for (int i = 0; i < 3; i++) {
				decript(random_k[i]);
			}
		}

		void decrypt(key k) {
			using std::cout;

			for (uint32_t n = 0; n < k.iterations; n++) {
				message[index] = apply_operator(message[index], k.operation, k.parameter);
				if ((direction == 1 && index == (LENGTH - 1)) || (direction == -1 && index == 0)) {
					direction *= -1;
				}
				index += direction;
			}
			
		}

		bool test_message() {
			using std::cout;
			int good = 0;
			int bad = 0;
			char c;

			for (int i = 0; i < LENGTH; i++) {
				c = message[i];

				if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 32 || c == 63 || c == 46 || c == 44) {
					good++;
					bad = 0;
				}
				else {
					bad++;
				//	if (bad > 25)
				//		return false;
				}

			}
			int length = int(LENGTH);
			float score = float(good) / float(length);
			if (score > TRESHOLD) {
				print_random_key();
				cout << "score" << score << ": \t" << " key size \t" << key_size <<  " message: \t" << message << "\n";
				return true;
			}

			else
				return false;



		}

		void print_random_key() {
			using std::cout;
			for (int i = 0; i < 3; i++) {
				cout << "i: " << i << "o: " << (uint32_t)random_k[i].operation << " p: " << (uint32_t)random_k[i].parameter << " n: " << (uint32_t)random_k[i].iterations << "\n";
			}
				
		

		}

		void save_random_key(int i) {
			key new_key;
			new_key.operation = k.operation;
			new_key.parameter = k.parameter;
			new_key.iterations = k.iterations;
			random_k[i] = new_key;

		}

		void generate_key(int i, int p0, int i0, int p1, int i1, int p2, int i2) {

			random_k[0].operation = random_keys[i][0].operation;
			random_k[1].operation = random_keys[i][1].operation;
			random_k[2].operation = random_keys[i][2].operation;

			random_k[0].parameter = p0;
			random_k[1].parameter = p1;
			random_k[2].parameter = p2;

			random_k[0].iterations = i0;
			random_k[1].iterations = i1;
			random_k[2].iterations = i2;
			
		}

		void generate_key(int key_index) {
			using std::cout;
			if (3 == CHALENGE) {
				//min + (rand() % (int)(max - min + 1))
				k.operation = rand() % (int)(3);
				k.parameter = rand() % (int)(254);
				k.iterations = rand() % (uint32_t)(254); //2147483647
			}
			else {
				k.operation = static_k[key_index].operation;
				k.parameter = static_k[key_index].parameter;
				k.iterations = static_k[key_index].iterations;
			}
			
			
		}

		void set_random_k(int i, int o, int p, int n) {
			random_k[i].operation = o;
			random_k[i].parameter = p;
			random_k[i].iterations = n;
		}

		void read_message() {
			using std::ifstream;
		
			ifstream source(MFILE);
			int i = 0;
			while (i < LENGTH) {
				source.get(message[i]);
				i++;
			}
			message[i] = NULL;
			
		}
		void print_message() {
			using std::cout;
			cout <<  message << '\n';
		}
		void copy_message() {
			for (int i=0;  i<= LENGTH; i++) //include string terminator
				orig_message[i] = message[i];
		}
		void reset() {
			copy_message();
			index = 1;
			direction = 1;
	
		}

		void set_key_size(int size) {
			key_size = size;
		}
		int get_key_size() {
			return key_size;
		}
};


int main()
{
	using std::cout;
	srand(time(NULL));
	Decrypt D;

//	void generate_key(int i, int p0, int i0, int p1, int i1, int p2, int i2) {
	
	for (int i = 0; i < 6; i++) {
		for (int p0 = 1; p0 < 255; p0++) {
			for (int i0 = 1; i0 < 255; i0++) {
				for (int p1 = 1; p1 < 255; p1++) {
					for (int i1 = 1; i1 < 255; i1++) {
						for (int p2 = 1; p2 < 255; p2++) {
							for (int i2 = 1; i2 < 255; i2++) {
								D.generate_key(i, p0, i0, p1, i1, p2, i2);
								D.decrypt_random_k();
								D.test_message();
								D.reset();
							}
						}
					}
				}
			}
		}

	}
	return 0;
}
