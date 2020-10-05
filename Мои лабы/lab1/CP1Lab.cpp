// CP1Lab.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

using namespace std;

void remove_symbols(char* ftext, char* wtext);
void count_freq_monogr(char* wtext, float* cont,int mode);
void count_freq_bigr(char* wtext, float** cont,int mode);
float entrophy_for_monogr(float* cont);
float entrophy_for_bigr(float** cont);


int main()
{   
	setlocale(LC_ALL, "rus");

	char *filetext = new char[10000000];
	char *worktext = new char[10000000];
	float *freq_mono = new float[32];
	float **freq_bi = new float*[32];

	for (int i = 0; i < 32; i++)
	{
		freq_mono[i] = 0.0;
	}
	for (int k = 0; k < 32; k++)
	{
		freq_bi[k] = new float[32];
		for (int i = 0; i < 32; i++)
		{
			freq_bi[k][i] = 0.0;
		}
	}

	ifstream fin("text.txt");

	fin.read(filetext, 10000000);
	fin.close();
	remove_symbols(filetext, worktext);
	count_freq_monogr(worktext,freq_mono,1);
	count_freq_bigr(worktext, freq_bi,1);
	ofstream fout1("monograms_with_spaces.txt");
	for (int i = 0; i < 32; i++)
	{ 
		if ((char)('а' + i) == 'ъ')
		{
			fout1 << ' ' << '=';
			fout1 << freq_mono[i] << endl;
		}
		else {
			fout1 << (char)('а' + i) << '=';
			fout1 << freq_mono[i] << endl;
		}
		
	}
	fout1.close();
	ofstream fout2("bigrams_with_spaces.txt");
	for (int k = 0; k < 32; k++)
	{
		for (int j = 0; j < 32; j++)
		{   
			if (freq_bi[k][j]!=0) {
				if ((char)('а' + k) == 'ъ')
					fout2 << ' ';
				else
					fout2 << (char)('а' + k);
				if ((char)('а' + j) == 'ъ')
				{
					fout2 << ' ' << '=';
					fout2 << freq_bi[k][j] << endl;
				}
				else {
					fout2 << (char)('а' + j) << '=';
					fout2 << freq_bi[k][j] << endl;
				}
			}
		}
	}
	fout2.close();
	float entr1 = entrophy_for_monogr(freq_mono);
	float entr2 = entrophy_for_bigr(freq_bi);
	count_freq_monogr(worktext, freq_mono, 0);
	count_freq_bigr(worktext, freq_bi, 0);
	ofstream fout3("monograms_without_spaces.txt");
	for (int i = 0; i < 32; i++)
	{
		if ((char)('а' + i) == 'ъ')
		{
			fout3 << ' ' << '=';
			fout3 << freq_mono[i] << endl;
		}
		else {
			fout3 << (char)('а' + i) << '=';
			fout3 << freq_mono[i] << endl;
		}

	}
	fout3.close();
	ofstream fout4("bigrams_without_spaces.txt");
	for (int k = 0; k < 32; k++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (freq_bi[k][j] != 0) {
				if ((char)('а' + k) == 'ъ')
					fout4 << ' ';
				else
					fout4 << (char)('а' + k);
				if ((char)('а' + j) == 'ъ')
				{
					fout4 << ' ' << '=';
					fout4 << freq_bi[k][j] << endl;
				}
				else {
					fout4 << (char)('а' + j) << '=';
					fout4 << freq_bi[k][j] << endl;
				}
			}
		}
	}
	fout4.close();
	float entr3 = entrophy_for_monogr(freq_mono);
	float entr4 = entrophy_for_bigr(freq_bi);
	ofstream fout5("entrophy.txt");
	fout5 << "H1 с пробелами = " << entr1<< endl;
	fout5 << "H2 с пробелами = " << entr2 << endl;
	fout5 << "H1 без пробелов = " << entr3 << endl;
	fout5 << "H2 без пробелов  = " << entr4 << endl;
	fout5.close();
}

void remove_symbols(char* ftext,char* wtext)
{   
	int64_t i = 0;
	int64_t j = 0;
	while (ftext[i] != 'Z')
	{   
		if (ftext[i] == 'ё') ftext[i] = 'е';
		if (ftext[i] == 'ъ') ftext[i] = 'ь';
		if (ftext[i] >= 'а' && ftext[i] <= 'я') 
		wtext[j++] = ftext[i];
	
		else if (ftext[i] >= 'А' && ftext[i] <= 'Я') 
			wtext[j++] = ftext[i] + 32;		
		
		else if ((ftext[i] == ' ' || ftext[i] == '\t' || ftext[i] == '\n' || ftext[i] == '\v') && wtext[j - 1] != ' ')
			wtext[j++] = ' ';
		i++;
	}
	wtext[j] = '\0';
}

void count_freq_monogr(char* wtext, float* cont, int mode) {
	for (int a = 0; a < 32; a++)
	{
		cont[a] = 0.0;
	}
	int i = 0;
	int text_size = 0;
	while (wtext[i] != '\0')
	{
		if (mode == 1) {
			if (wtext[i] == ' ') cont[26] += 1.0;
			else  cont[(int)wtext[i] + 32] += 1.0;
			
			text_size++;
		} 
		else if (wtext[i] != ' ') {
			cont[(int)wtext[i] + 32] += 1.0;
			text_size++;
		}	
		i++;
	}
	for (int j = 0; j < 32; j++)
	{
		cont[j] = cont[j]/text_size;
	}
}

void count_freq_bigr(char* wtext, float** cont , int mode) {
	for (int y = 0; y < 32; y++)
	{
		for (int b = 0; b < 32; b++)
		{
			cont[y][b] = 0.0;
		}
	}
	int i = 0;
	int text_size = 0;
	while (wtext[i+1] != '\0')
	{
		if (mode == 1) {
			if (wtext[i] == ' ') cont[26][(int)wtext[i + 1] + 32] += 1.0;
			else if (wtext[i + 1] == ' ') cont[(int)wtext[i] + 32][26] += 1.0;
			else  cont[(int)wtext[i] + 32][(int)wtext[i + 1] + 32] += 1.0;
			
			text_size++;
		}
		else if (wtext[i] != ' ')
		{
			cont[(int)wtext[i] + 32][(int)wtext[i + 1] + 32] += 1.0;
			text_size++;
		}
		i++;
	}
	int z = 0;
	while (wtext[z + 2] != '\0')
	{   
		if (mode == 1) {
			if (wtext[z] == ' ') cont[26][(int)wtext[z + 2] + 32] += 1.0;
			else if (wtext[z + 2] == ' ') cont[(int)wtext[z] + 32][26] += 1.0;
			else  cont[(int)wtext[z] + 32][(int)wtext[z + 2] + 32] += 1.0;
			
			text_size++;
		}
		else if (wtext[z] != ' ')
		{
			cont[(int)wtext[z] + 32][(int)wtext[z + 2] + 32] += 1.0;
			
			text_size++;
		}
		z++;
		
	}
	for (int k = 0; k < 32; k++)
	{
		for (int j = 0; j < 32; j++)
		{
			cont[k][j] = cont[k][j] / text_size;
		}
	}
}

float entrophy_for_monogr(float* cont)
{
	float count = 0.0;
	for (int i = 0; i < 32; i++)
	{  
		if(cont[i]!=0)
		count += cont[i] * log2f(cont[i]);
	}
	return -count;
}
float entrophy_for_bigr(float** cont)
{
	float count = 0.0;
	for (int k = 0; k < 32; k++)
	{
		for (int i = 0; i < 32; i++)
		{
			if (cont[k][i] != 0)
			count += cont[k][i]*log2f(cont[k][i]);
		}
	}
	return -count;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
